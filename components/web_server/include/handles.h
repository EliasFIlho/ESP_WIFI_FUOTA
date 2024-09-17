#pragma once
#include "filesystem.h"
#include "web_server.h"
#include "esp_log.h"
#include "defines_types.h"






esp_err_t root_get_handler(httpd_req_t *req);
esp_err_t authentication_post_handler(httpd_req_t *req);
esp_err_t read_recv_data_as_string(httpd_req_t *req, char *buffer, size_t buffer_size);
esp_err_t config_get_handler(httpd_req_t *req);
esp_err_t read_file_as_str_chunked(const char *file_name, char *buffer, size_t buffer_size, long *position);
esp_err_t config_post_handler(httpd_req_t *req);
esp_err_t parse_config_body(char *content, device_cfg_t *config_structure);
io_set_t io_config_parse(char *data);


