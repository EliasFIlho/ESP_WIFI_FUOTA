#include "filesystem.h"




void init_file_system(){
    const char *TAG = "SPIFFS [+]";
    esp_vfs_spiffs_conf_t config ={
        .base_path = "/spiffs",
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false
    };
    ESP_ERROR_CHECK_WITHOUT_ABORT(esp_vfs_spiffs_register(&config));
    size_t total = 0;
    size_t used = 0;

    esp_spiffs_info(config.partition_label,&total,&used);
    ESP_LOGI(TAG,"Total size: %d - Used: %d",total,used);

    FILE *p = fopen("/spiffs/teste.txt","r");
    if(p != NULL){
        char data[64];
        fgets(data,sizeof(data),p);
        ESP_LOGI(TAG,"DATA: %s ",data);
        
    }
    fclose(p);

}