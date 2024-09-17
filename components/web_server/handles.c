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

io_set_t io_config_parse(char *data)
{
    if (strcmp(data, "on_off") == 0)
    {
        return ON_OFF;
    }
    else if (strcmp(data, "pulse_counter") == 0)
    {
        return PULSE_COUNTER;
    }
    else
    {
        return DISABLE;
    }
}

esp_err_t parse_config_body(char *content, device_cfg_t *config_structure)
{

    char *token;
    char TOKENS[CONFIG_TOKENS_NUMBER][CONFIG_TOKENS_SIZE];
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

    if (counter > CONFIG_TOKENS_NUMBER)
    {
        return ESP_ERR_INVALID_SIZE;
    }
    char *key;
    char *data;
    for (int i = 0; i < counter; i++)
    {
        key = strtok(TOKENS[i], "=");
        data = strtok(NULL, "=");
        // ESP_LOGI("TOKENS ARRAY: ", "KEY[%s] - DATA[%s]\n", key, data);

        if (strcmp(key, "device_name") == 0)
        {
            int writed = snprintf(config_structure->device_name, sizeof(config_structure->device_name), "%s", data);
            int ret = EQUAL(writed, strlen(data));
            if (ret != 0)
            {
                ESP_LOGW("PARSER", "INVALIDE\nWRITED[%d]  DATA[%d]\nFunction return[%d]", writed, strlen(data), ret);
                return ESP_FAIL;
            }
        }
        else if (strcmp(key, "ip_address") == 0)
        {
            int writed = snprintf(config_structure->device_ip, sizeof(config_structure->device_ip), "%s", data);
            int ret = EQUAL(writed, strlen(data));
            if (ret != 0)
            {
                ESP_LOGW("PARSER", "INVALIDE\nWRITED[%d]  DATA[%d]\nFunction return[%d]", writed, strlen(data), ret);
                return ESP_FAIL;
            }
        }
        else if (strcmp(key, "server_ip") == 0)
        {
            int writed = snprintf(config_structure->server_ip, sizeof(config_structure->server_ip), "%s", data);
            int ret = EQUAL(writed, strlen(data));
            if (ret != 0)
            {
                ESP_LOGW("PARSER", "INVALIDE\nWRITED[%d]  DATA[%d]\nFunction return[%d]", writed, strlen(data), ret);
                return ESP_FAIL;
            }
        }
        else if (strcmp(key, "port") == 0)
        {
            config_structure->port = atoi(data);
        }
        else if (strcmp(key, "sampling_interval") == 0)
        {
            config_structure->sample_time = atoi(data);
        }
        else if (strcmp(key, "io_pin_1") == 0)
        {

            config_structure->io_01 = io_config_parse(data);
        }
        else if (strcmp(key, "io_pin_2") == 0)
        {

            config_structure->io_02 = io_config_parse(data);
        }
        else if (strcmp(key, "io_pin_3") == 0)
        {
            config_structure->io_03 = io_config_parse(data);
        }
        else if (strcmp(key, "io_pin_4") == 0)
        {
            config_structure->io_04 = io_config_parse(data);
        }
    }
    return ESP_OK;
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
    ret = parse_config_body(content, &dev);
    if (ret != ESP_OK)
    {
        return ret;
    }
    else
    {
        ESP_LOGI("CONFIG HANDLE", "NAME[%s]\nDEV_IP[%s]\nSERVER_IP[%s]\nPORT[%d]\nSAMPLE[%d]\nIO_1[%d]\nIO_2[%d]\nIO_3[%d]\nIO_4[%d]", dev.device_name, dev.device_ip, dev.server_ip, dev.port, dev.sample_time, dev.io_01, dev.io_02, dev.io_03, dev.io_04);
        ret = write_config_file(&dev);
    }

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
