#include "wifi.h"
#include "nvs_flash.h"
#include "ota_http.h"
#include "driver/gpio.h"
#include "web_server.h"

#define PORT 8000
#define END_POINT_VERSION "/ota_version"
#define END_POINT_FIRMWARE "/ota_update"
#define HOST "10.109.49.78"



/**
 * @brief Loop principal, inicializa o nvs e os gpio, logo em seguida o wifi.
 * Após a inicialização do wifi, inicializa as tarefas do servidor web e do monitoramento ota
 * 
 */

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    gpio_config_t gpio_conf = {
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1<<2),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };

    gpio_config(&gpio_conf);
    
    wifi_init_sta();

    vTaskDelay(5000 / portTICK_PERIOD_MS);

    ota_http_t teste = {
        .end_point_version = END_POINT_VERSION,
        .end_point_firmware = END_POINT_FIRMWARE,
        .host = HOST,
        .port = PORT};


    init_ota_monitor(&teste);
    init_http_server();
    

    while (1)
    {
        gpio_set_level(2,1);
        // vTaskDelay(100 / portTICK_PERIOD_MS);
        // gpio_set_level(2,0);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
