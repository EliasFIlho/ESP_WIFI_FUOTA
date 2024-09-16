#pragma once
#include "esp_spiffs.h"
#include "esp_log.h"
#include <string.h>

#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

void init_file_system();
void read_file_as_str(const char *file_name, char *buffer, size_t buffer_size);
