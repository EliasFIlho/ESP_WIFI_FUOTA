#pragma once
#include "filesystem.h"
#include "web_server.h"
#include "esp_log.h"



#define CONFIG_TOKENS_NUMBER 9
#define CONFIG_TOKENS_SIZE 150

#define DEVICE_NAME_BUFFER 16
#define DEVICE_IP_BUFFER 16

#define SERVER_IP_BUFFER DEVICE_IP_BUFFER

#define EQUAL(X, Y) (((X) == (Y)) ? (0) : (1))

typedef enum {
    DISABLE =0,
    ON_OFF,
    PULSE_COUNTER
}io_set_t;


typedef struct {
    char device_name[DEVICE_NAME_BUFFER];
    char device_ip[DEVICE_IP_BUFFER];
    char server_ip[SERVER_IP_BUFFER];
    int port;
    int sample_time;
    io_set_t io_01;
    io_set_t io_02;
    io_set_t io_03;
    io_set_t io_04;
}device_cfg_t;

esp_err_t root_get_handler(httpd_req_t *req);
esp_err_t authentication_post_handler(httpd_req_t *req);
esp_err_t read_recv_data_as_string(httpd_req_t *req, char *buffer, size_t buffer_size);
esp_err_t config_get_handler(httpd_req_t *req);
esp_err_t read_file_as_str_chunked(const char *file_name, char *buffer, size_t buffer_size, long *position);
esp_err_t config_post_handler(httpd_req_t *req);
esp_err_t parse_config_body(char *content, device_cfg_t *config_structure);
io_set_t io_config_parse(char *data);


