#pragma once

#include <stdio.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "lwip/err.h"
#include "lwip/sys.h"

void wifi_event_handle(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void wifi_ip_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void wifi_init_sta();