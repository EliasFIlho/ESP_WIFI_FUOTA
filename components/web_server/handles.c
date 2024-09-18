#include "handles.h"

esp_err_t root_get_handler(httpd_req_t *req)
{
    const char *TAG = "[/] Handle:";
    const char resp[] = "URI GET Response";
    ESP_LOGI(TAG, "%s", resp);
    char page[4096];
    read_file_as_str("login.html", page, sizeof(page));
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

io_set_t io_config_comp(char *data)
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

void validate_and_assing(char *dest, size_t dest_size, const char *key, cJSON *json)
{
    cJSON *data;
    data = cJSON_GetObjectItemCaseSensitive(json, key);
    if (cJSON_IsString(data) && (data->valuestring != NULL))
    {
        snprintf(dest, dest_size, data->valuestring);
    }
}

void validate_and_assing_atoi(int *dest, const char *key, cJSON *json)
{
    cJSON *data;
    data = cJSON_GetObjectItemCaseSensitive(json, key);
    if (cJSON_IsString(data) && (data->valuestring != NULL))
    {
        *dest = atoi(data->valuestring);
    }
}

void validate_and_assing_io(io_set_t *dest, const char *key, cJSON *json)
{
    cJSON *data;
    data = cJSON_GetObjectItemCaseSensitive(json, key);
    if (cJSON_IsString(data) && (data->valuestring != NULL))
    {
        *dest = io_config_comp(data->valuestring);
    }
}

esp_err_t parse_config_body(char *content, device_cfg_t *config_structure)
{
    cJSON *json = cJSON_Parse(content);
    if (json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            ESP_LOGE("PARSER", "Error: %s\n", error_ptr);
        }
        cJSON_Delete(json);
        return ESP_FAIL;
    }

    validate_and_assing(config_structure->device_name, sizeof(config_structure->device_name), "device_name", json);
    validate_and_assing(config_structure->device_ip, sizeof(config_structure->device_ip), "ip_address", json);
    validate_and_assing(config_structure->server_ip, sizeof(config_structure->server_ip), "server_ip", json);
    validate_and_assing_atoi(&config_structure->port, "port", json);
    validate_and_assing_atoi(&config_structure->sample_time, "sampling_interval", json);
    validate_and_assing_io(&config_structure->io_01,"io_pin_1",json);
    validate_and_assing_io(&config_structure->io_02,"io_pin_2",json);
    validate_and_assing_io(&config_structure->io_03,"io_pin_3",json);
    validate_and_assing_io(&config_structure->io_04,"io_pin_4",json);
    cJSON_Delete(json);

    return ESP_OK;
}

esp_err_t config_post_handler(httpd_req_t *req)
{
    char content[512];
    read_recv_data_as_string(req, content, sizeof(content));
    ESP_LOGI("RECV", "POST: %s", content);
    char page[3000];
    read_file_as_str("login.html", page, sizeof(page));
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
