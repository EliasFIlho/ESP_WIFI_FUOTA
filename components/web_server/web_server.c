#include <stdio.h>
#include "web_server.h"
#include "esp_log.h"
#include "filesystem.h"
#include "handles.h"

#define RESP_LEN 16
const char *TAG = "WEB SERVER [+]";

/**
 * @brief Função para iniciar o servidor web.
 * 
 */

void init_http_server()
{

    httpd_handle_t server_handle = start_webserver();
    if (server_handle == NULL)
    {
        ESP_LOGE(TAG, "SERVER ERROR TO START");
    }
    ESP_LOGI(TAG, "SERVER STARTED");
}



/**
 * @brief Estrutura de endpoint.
 * 
 * Endpoint: /
 * Fução associada: root_get_handler
 * Metodo: GET
 * 
 */

httpd_uri_t uri_get = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_get_handler,
    .user_ctx = NULL};

/**
 * @brief Estrutura de endpoint.
 * 
 * Endpoint: /authentication
 * Fução associada: authentication_post_handler
 * Metodo: POST
 * 
 */
httpd_uri_t uri_post = {
    .uri = "/authentication",
    .method = HTTP_POST,
    .handler = authentication_post_handler,
    .user_ctx = NULL};

/**
 * @brief Estrutura de endpoint.
 * 
 * Endpoint: /config
 * Fução associada: config_get_handler
 * Metodo: GET
 * 
 */

httpd_uri_t config_endpoint_get = {
    .uri = "/config",
    .method = HTTP_GET,
    .handler = config_get_handler,
    .user_ctx = NULL,};

/**
 * @brief Estrutura de endpoint.
 * 
 * Endpoint: /config
 * Fução associada: config_post_handler
 * Metodo: POST
 * 
 */

httpd_uri_t config_endpoint_post = {
    .uri = "/config",
    .method = HTTP_POST,
    .handler = config_post_handler,
    .user_ctx = NULL};



/**
 * @brief Função interna para iniciar o servidor web.
 * Associação de endpoints.
 * Configuração de task.
 * Inicialização do filesystem.
 */
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
        httpd_register_uri_handler(server, &config_endpoint_post);
    }
    return server;
}


/**
 * @brief Função interna para desligar o servidor web.
 * 
 */

void stop_webserver(httpd_handle_t server)
{
    if (server)
    {
        httpd_stop(server);
    }
}