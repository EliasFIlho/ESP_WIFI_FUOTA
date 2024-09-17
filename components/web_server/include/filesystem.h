#pragma once
#include "esp_spiffs.h"
#include "esp_log.h"
#include <string.h>
#include "defines_types.h"
#include "cJSON.h"



void init_file_system();
void read_file_as_str(const char *file_name, char *buffer, size_t buffer_size);
esp_err_t write_config_file(device_cfg_t *dev);