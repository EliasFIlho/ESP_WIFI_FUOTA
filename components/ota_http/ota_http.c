#include <stdio.h>
#include "ota_http.h"
static const char *TAG = "OTA_MONITOR";

void init_ota_monitor(void)
{
    xTaskCreate(ota_monitor_task, "OTA MONITOR", configMINIMAL_STACK_SIZE + 4096, NULL, tskIDLE_PRIORITY + 1, NULL);
}

esp_err_t _http_handle(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGD(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_CONNECTED");
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGD(TAG, "HTTP_EVENT_ON_DATA: %.*s", evt->data_len, (char *)evt->data);

    default:
        break;
    }
    return ESP_OK;
}

esp_err_t get_request()
{
    esp_http_client_config_t config = {
        .url = "http://10.109.49.78:8000/ota_version",
        .method = HTTP_METHOD_GET,
        .transport_type = HTTP_TRANSPORT_OVER_TCP,
        .event_handler = _http_handle};

    esp_http_client_handle_t client = esp_http_client_init(&config);
    if (client != NULL)
    {

        esp_http_client_perform(client);
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

        ESP_ERROR_CHECK(get_request());

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
}