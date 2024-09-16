idf_component_register(SRCS "handles.c" "filesystem.c" "web_server.c"
                    INCLUDE_DIRS "include" "data"
                    REQUIRES "esp_http_server" "spiffs")
spiffs_create_partition_image(spiffs data FLASH_IN_PROJECT)