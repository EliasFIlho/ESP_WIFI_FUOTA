#include <stdio.h>
#include "web_server.h"
#include "esp_log.h"
#include "filesystem.h"


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

esp_err_t get_handler(httpd_req_t *req)
{
    const char resp[] = "URI GET Response";
    ESP_LOGI(TAG, "%s", resp);
    init_file_system();

    httpd_resp_send(req, resp, RESP_LEN);
    return ESP_OK;
}

httpd_uri_t uri_get = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = get_handler,
        .user_ctx = NULL};
    


httpd_handle_t start_webserver(void)
{
    
    
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &uri_get);
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