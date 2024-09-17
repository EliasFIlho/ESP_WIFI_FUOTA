#include "filesystem.h"

void init_file_system()
{
    const char *TAG = "SPIFFS [+]";

    esp_vfs_spiffs_conf_t config = {
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false};
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_spiffs_register(&config));
    size_t total = 0;
    size_t used = 0;

    esp_spiffs_info(config.partition_label, &total, &used);
    ESP_LOGI(TAG, "Total size: %d - Used: %d", total, used);
}

void read_file_as_str(const char *file_name, char *buffer, size_t buffer_size)
{
    const char *TAG = "SPIFFS [+]";
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s", "/spiffs/", file_name);
    ESP_LOGW(TAG, "File path: %s", file_path);

    FILE *p = fopen(file_path, "r");
    if (p != NULL)
    {
        char aux_buffer[64];
        size_t total_read = 0;

        while (fgets(aux_buffer, sizeof(aux_buffer), p) != NULL)
        {
            size_t len = strlen(aux_buffer);
            if (total_read + len >= buffer_size - 1)
            {
                ESP_LOGW(TAG, "Buffer size exceeded, truncating file content");
                break;
            }

            strncpy(buffer + total_read, aux_buffer, len);
            total_read += len;
        }

        buffer[total_read] = '\0';
    }
    else
    {
        ESP_LOGE(TAG, "Pointer NULL");
    }

    fclose(p);
}

esp_err_t write_config_file(device_cfg_t *dev)
{
    cJSON *json = cJSON_CreateObject();

    cJSON_AddStringToObject(json, "DEVICE_NAME", dev->device_name);
    cJSON_AddStringToObject(json, "DEVICE_IP", dev->device_ip);
    cJSON_AddStringToObject(json, "SERVER_IP", dev->server_ip);
    cJSON_AddNumberToObject(json, "PORT", dev->port);
    cJSON_AddNumberToObject(json, "SAMPLE_TIME", dev->sample_time);
    cJSON_AddNumberToObject(json, "IO_1_CONFIG", dev->io_01);
    cJSON_AddNumberToObject(json, "IO_2_CONFIG", dev->io_02);
    cJSON_AddNumberToObject(json, "IO_3_CONFIG", dev->io_03);
    cJSON_AddNumberToObject(json, "IO_4_CONFIG", dev->io_04);

    char *config_file = cJSON_Print(json);

    FILE *p = fopen("/spiffs/device.cfg", "w");
    if (p == NULL)
    {
        ESP_LOGE("SPIFFS", "FAIL TO OPEN");
        return ESP_FAIL;
    }

    fprintf(p, "%s", config_file);
    fclose(p);
    return ESP_OK;
}

esp_err_t read_file_as_str_chunked(const char *file_name, char *buffer, size_t buffer_size, long *position)
{
    const char *TAG = "SPIFFS [+]";
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s", "/spiffs/", file_name);
    // ESP_LOGI(TAG, "File path: %s", file_path);
    FILE *p = fopen(file_path, "r");

    if (p == NULL)
    {

        ESP_LOGE(TAG, "Pointer NULL");
        fclose(p);
        return ESP_ERR_NOT_FOUND;
    }
    fseek(p, *position, SEEK_SET);
    if (fgets(buffer, buffer_size, p) != NULL)
    {
        if (feof(p))
        {
            ESP_LOGW(TAG, "End of file");
            return ESP_OK;
        }
        // ESP_LOGI(TAG, "File content: %s", buffer);
    }
    else
    {
        ESP_LOGE(TAG, "fgets return NULL");
    }
    *position = ftell(p);
    fclose(p);
    return ESP_ERR_NOT_FINISHED;
}