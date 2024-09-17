#pragma once
#include "esp_http_server.h"
#include "esp_log.h"
#include "esp_event.h"


void init_http_server();
esp_err_t get_handler(httpd_req_t *req);
httpd_handle_t start_webserver(void);
void stop_webserver(httpd_handle_t server);