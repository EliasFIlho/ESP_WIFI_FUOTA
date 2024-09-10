#include <stdio.h>
#include "wifi.h"

void wifi_event_handle(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        ESP_LOGI("WIFI", "STATION STARTED");
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    case WIFI_EVENT_STA_STOP:
        ESP_LOGI("WIFI", "STATION STOPPED");
        break;
    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI("WIFI", "STATION CONNECTED");
        
        break;
    case WIFI_EVENT_STA_DISCONNECTED:
        ESP_LOGI("WIFI", "STATION DISCONNECTED");
        ESP_ERROR_CHECK(esp_wifi_connect());
        break;
    default:
        ESP_LOGI("WIFI", "DEFAULT HANDLE: %ld", event_id);
        break;
    }
}

void wifi_ip_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
{
    switch (event_id)
    {
    case IP_EVENT_STA_GOT_IP:
        ESP_LOGI("WIFI", "GOT IP");
        break;

    default:
        ESP_LOGI("WIFI", "DEFAULT IP HANDLE: %ld", event_id);
        break;
    }
}

void wifi_init_sta()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, wifi_event_handle, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, wifi_ip_event_handler, NULL));

    wifi_config_t wifi = {
        .sta = {
            .ssid = CONFIG_ESP_WIFI_SSID,
            .password = CONFIG_ESP_WIFI_PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
}
