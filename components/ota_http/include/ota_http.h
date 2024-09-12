#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_check.h"
#include "cJSON.h"

typedef struct{
    const char *host;
    const char *end_point_version;
    const char *end_point_firmware;
    int port;
}ota_http_t;


void init_ota_monitor(ota_http_t *http_config);
void ota_monitor_task(void *pvParameters);
esp_err_t _http_handle(esp_http_client_event_t *evt);
esp_err_t get_request();
bool compare_version(const char *version_to_compare, const char *current_version);
esp_err_t parse_json(const char *json_str, char *version_buffer, size_t version_buffer_size);
void http_config_get_request(esp_http_client_config_t *conf, ota_http_t *http_config, bool version,bool evt);
esp_err_t perfom_request(esp_http_client_config_t *config);
void perform_http_ota(ota_http_t *http_config);

