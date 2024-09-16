#include <stdio.h>
#include "web_server.h"
#include "esp_log.h"
#include "filesystem.h"
#include "handles.h"

#define RESP_LEN 16
const char *TAG = "WEB SERVER [+]";

void init_http_server()
{

    httpd_handle_t server_handle = start_webserver();
    if (server_handle == NULL)
    {
        ESP_LOGE(TAG, "SERVER ERROR TO START");
    }
    ESP_LOGI(TAG, "SERVER STARTED");
}

httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_get_handler,
    .user_ctx = NULL};

httpd_uri_t uri_post = {
    .uri = "/authentication",
    .method = HTTP_POST,
    .handler = authentication_post_handler,
    .user_ctx = NULL};

httpd_uri_t config_endpoint_get = {
    .uri = "/config",
    .method = HTTP_GET,
    .handler = config_get_handler,
    .user_ctx = NULL};

httpd_handle_t start_webserver(void)
{

    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;
    config.stack_size = 8192;
    init_file_system();

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
        httpd_register_uri_handler(server, &uri_post);
        httpd_register_uri_handler(server, &config_endpoint_get);
    }
    return server;
}

void stop_webserver(httpd_handle_t server)
{
    if (server)
    {
        httpd_stop(server);
    }
}