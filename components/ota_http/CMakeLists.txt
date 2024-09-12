idf_component_register(SRCS "ota_http.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_http_client" "esp_https_ota" "esp_partition" json)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)