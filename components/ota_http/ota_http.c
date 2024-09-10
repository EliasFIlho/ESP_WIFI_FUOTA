#include <stdio.h>
#include "ota_http.h"
#include "esp_https_ota.h"

static const char *TAG = "OTA_MONITOR";
static char version_buffer[128];
static char parsed_version[32];
static int recv_len = 0;

#define VERSION "1.0.0"


bool RUNNING_OTA = false;

void init_ota_monitor(ota_http_t *http_config)
{
    xTaskCreate(ota_monitor_task, "OTA MONITOR", configMINIMAL_STACK_SIZE + 4096, (void *)http_config, tskIDLE_PRIORITY + 1, NULL);
}

bool parse_version_and_compare(const char *json_str, const char *current_version)
{

    const char *version_key = "\"version\":\"";
    char *version_start = strstr(json_str, version_key);

    if (version_start != NULL)
    {
        version_start += strlen(version_key);
        char *version_end = strchr(version_start, '"');

        if (version_end != NULL)
        {
            size_t version_len = version_end - version_start;
            strncpy(parsed_version, version_start, version_len);
            parsed_version[version_len] = '\0';
            int comp = strcmp(parsed_version, current_version);
            if (comp == 0)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            ESP_LOGE(TAG, "Failed to find end of version string");
            return false;
        }
    }
    else
    {
        ESP_LOGE(TAG, "Failed to find version key");
    }
    return false;
}

esp_err_t _http_handle(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);
        if (!esp_http_client_is_chunked_response(evt->client))
        {
            memcpy(version_buffer, evt->data, evt->data_len);
            recv_len = evt->data_len;
        }
        ESP_LOGI(TAG, "DATA: %s", version_buffer);
        if (parse_version_and_compare(version_buffer, VERSION))
        {
            ESP_LOGI(TAG, "VERSION: EQUAL");
            ESP_LOGI(TAG, "CURRENT VERSION: %s", VERSION);
            RUNNING_OTA = false;
        }
        else
        {
            ESP_LOGI(TAG, "VERSION: DIFFERENT\nRunning OTA");
            ESP_LOGI(TAG, "CURRENT VERSION: %s", VERSION);
            RUNNING_OTA = true;
        }
        break;
    default:
        ESP_LOGI(TAG, "DEFAULT EVENT HANDLE");
        break;
    }
    return ESP_OK;
}

void http_config_get_request(esp_http_client_config_t *conf, ota_http_t *http_config, bool version, bool evt)
{
    ESP_LOGI(TAG, "Host: %s, Port: %d, EndPoint Firmware: %s, EndPoint Version: %s", http_config->host, http_config->port, http_config->end_point_firmware,http_config->end_point_version);
    conf->host = http_config->host;
    conf->port = http_config->port;
    if (version)
    {

        conf->path = http_config->end_point_version;
    }
    else
    {
        conf->path = http_config->end_point_firmware;
    }
    conf->method = HTTP_METHOD_GET;
    conf->transport_type = HTTP_TRANSPORT_OVER_TCP;
    if (evt)
    {
        conf->event_handler = _http_handle;
    }
}

esp_err_t perfom_request(esp_http_client_config_t *config)
{
    
    esp_http_client_handle_t client = esp_http_client_init(config);
    if (client == NULL)
    {
        return ESP_FAIL;
    }
    else
    {
        ESP_RETURN_ON_ERROR(esp_http_client_perform(client), TAG, "Failed to perform request");
        ESP_RETURN_ON_ERROR(esp_http_client_cleanup(client), TAG, "Failed to perform cleanup");
    }
    return ESP_OK;
}

void perform_http_ota(ota_http_t *http_config)
{
    ESP_LOGI(TAG, "RUNNING OTA");
    esp_http_client_config_t config;
    memset(&config, 0, sizeof(esp_http_client_config_t));
    http_config_get_request(&config, http_config, false, false);

    esp_https_ota_config_t ota_config = {
        .http_config = &config,
    };
    esp_err_t ret = esp_https_ota(&ota_config);

    if (ret == ESP_OK)
    {
        esp_restart();
    }
    else
    {
        ESP_LOGE(TAG, "OTA FAILS");
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "OTA Failed with error code: %d", ret);
        }
    }
}

void ota_monitor_task(void *pvParameters)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);
    ota_http_t *http_config = (ota_http_t *)pvParameters;
    esp_http_client_config_t config;
    memset(&config, 0, sizeof(esp_http_client_config_t));
    http_config_get_request(&config, http_config, true,true);
    perfom_request(&config);
    while (1)
    {

        perfom_request(&config);
        if (RUNNING_OTA)
        {
            perform_http_ota(http_config);
        }
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}