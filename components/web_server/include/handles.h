#pragma once
#include "filesystem.h"
#include "web_server.h"
#include "esp_log.h"

esp_err_t root_get_handler(httpd_req_t *req);
esp_err_t authentication_post_handler(httpd_req_t *req);
esp_err_t read_recv_data_as_string(httpd_req_t *req, char *buffer, size_t buffer_size);