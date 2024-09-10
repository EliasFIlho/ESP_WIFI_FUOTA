#pragma once
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_http_client.h"
#include "esp_check.h"

void init_ota_monitor(void);
void ota_monitor_task(void *pvParameters);
esp_err_t _http_handle(esp_http_client_event_t *evt);
esp_err_t get_request();
bool parse_version_and_compare(const char *json_str,const char *current_version);
void http_config_get_request(esp_http_client_config_t *conf, const char *host, int port, const char *end_point, bool evt);
esp_err_t perfom_request(esp_http_client_config_t *config);
void perform_http_ota();
