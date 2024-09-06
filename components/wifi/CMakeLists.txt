idf_component_register(SRCS "wifi.c"
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_wifi" "ota_http")

include($ENV{IDF_PATH}/tools/cmake/project.cmake)