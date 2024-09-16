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

        //ESP_LOGI(TAG, "DATA: %s", buffer);
    }
    else
    {
        ESP_LOGE(TAG, "Pointer NULL");
    }

    fclose(p);
}
