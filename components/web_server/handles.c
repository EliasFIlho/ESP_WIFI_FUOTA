#include "handles.h"

esp_err_t root_get_handler(httpd_req_t *req)
{
    const char *TAG = "[/] Handle:";
    const char resp[] = "URI GET Response";
    ESP_LOGI(TAG, "%s", resp);
    char page[4096];
    read_file_as_str("login.html", page, 4096);
    httpd_resp_send(req, page, strlen(page));
    return ESP_OK;
}

esp_err_t config_get_handler(httpd_req_t *req)
{
    const char *TAG = "[/config] Handle:";
    const char resp[] = "URI GET Response";
    ESP_LOGI(TAG, "%s", resp);
    char page[512];
    long pos = 0;
    esp_err_t ret;
    do
    {
        ret = read_file_as_str_chunked("config.html", page, sizeof(page), &pos);
        if (ret == ESP_ERR_NOT_FOUND)
        {
            break;
        }
        httpd_resp_send_chunk(req, page, strlen(page));
    } while (ret != ESP_OK);

    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

void parse_config_body(char *content, device_cfg_t *config_structure)
{

    char *token;
    char TOKENS[CONFIG_TOKENS_NUMBER][CONFIG_TOKENS_SIZE];
    // GET TOKENS PART
    int counter = 0;
    token = strtok(content, "&");
    size_t token_size;
    while (token != NULL)
    {
        token_size = strlen(token);
        strncpy(TOKENS[counter], token, token_size);
        TOKENS[counter][token_size] = '\0';
        counter++;
        token = strtok(NULL, "&");
    }

    char *key;
    char *data;

    for (int i = 0; i < CONFIG_TOKENS_NUMBER; i++)
    {
        key = strtok(TOKENS[i], "=");
        data = strtok(NULL, "=");
        ESP_LOGI("TOKENS ARRAY: ", "KEY[%s] - DATA[%s]\n", key, data);
        if (strcmp(key, "device_name")==0)
        {
            ESP_LOGW("TOKENS: ", "NICE");
        }
    }
}

esp_err_t config_post_handler(httpd_req_t *req)
{
    char content[512];
    read_recv_data_as_string(req, content, sizeof(content));
    ESP_LOGI("RECV", "POST: %s", content);
    char page[3000];
    read_file_as_str("login.html", page, 3000);
    esp_err_t ret = httpd_resp_send(req, page, HTTPD_RESP_USE_STRLEN);
    device_cfg_t dev;
    parse_config_body(content, &dev);

    return ret;
}

esp_err_t read_recv_data_as_string(httpd_req_t *req, char *buffer, size_t buffer_size)
{
    size_t recv_size = MIN(req->content_len, buffer_size);
    int ret = httpd_req_recv(req, buffer, recv_size);
    if (ret <= 0)
    {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {

            httpd_resp_send_408(req);
            return ESP_FAIL;
        }
    }
    else
    {
        buffer[recv_size] = '\0';
        return ESP_OK;
    }
    return ESP_FAIL;
}
esp_err_t authentication_post_handler(httpd_req_t *req)
{
    char content[100];
    read_recv_data_as_string(req, content, sizeof(content));
    ESP_LOGI("RECV", "POST: %s", content);
    char page[4096];
    read_file_as_str("page.html", page, 4096);
    return httpd_resp_send(req, page, HTTPD_RESP_USE_STRLEN);
}
