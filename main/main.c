#include "wifi.h"
#include "nvs_flash.h"
#include "ota_http.h"

#define PORT 8000
#define END_POINT_VERSION "/ota_version"
#define END_POINT_FIRMWARE "/ota_update"
#define HOST "10.109.49.78"

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    wifi_init_sta();

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    ota_http_t teste = {
        .end_point_version = END_POINT_VERSION,
        .end_point_firmware = END_POINT_FIRMWARE,
        .host = HOST,
        .port = PORT};


    init_ota_monitor(&teste);

    while (1)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
