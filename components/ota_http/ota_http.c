#include <stdio.h>
#include "ota_http.h"
#include "esp_https_ota.h"

static const char *TAG = "OTA_MONITOR";
static char version_buffer[128];
static char parsed_version[32];
static int recv_len = 0;

#define VERSION "1.0.0"

void init_ota_monitor(void)
{
    xTaskCreate(ota_monitor_task, "OTA MONITOR", configMINIMAL_STACK_SIZE + 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
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
        if (parse_version_and_compare(version_buffer, "1.0.0"))
        {
            ESP_LOGI(TAG, "VERSION: EQUAL");
        }
        else
        {
            ESP_LOGI(TAG, "VERSION: DIFFERENT\nRunning OTA");
            esp_http_client_config_t config = {
                .host = "10.109.49.78",
                .port = 8000,
                .path = "/ota_update",
            };
            esp_https_ota_config_t ota_config = {
                .http_config = &config,
            };
            esp_err_t ret = esp_https_ota(&ota_config);
            if (ret == ESP_OK)
            {
                esp_restart();
            }
        }
        break;
    default:
        break;
    }
    return ESP_OK;
}

esp_err_t get_request()
{
    esp_http_client_config_t config = {
        .host = "10.109.49.78",
        .port = 8000,
        .path = "/ota_version",
        .method = HTTP_METHOD_GET,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .event_handler = _http_handle};

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client != NULL)
    {
        esp_err_t ret = esp_http_client_perform(client);
        esp_http_client_cleanup(client);
        return ESP_OK;
    }
    else
    {
        ESP_LOGE(TAG, "ERROR TO INIT CLIENT");
        return ESP_FAIL;
    }
}

void ota_monitor_task(void *pvParameters)
{
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    ESP_LOGI(TAG, "Start loop");
    while (1)
    {
        ESP_LOGI(TAG, "CURRENT VERSION: %s", VERSION);
        ESP_ERROR_CHECK(get_request());

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}