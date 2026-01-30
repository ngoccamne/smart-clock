idf_component_register(SRCS "mqtt_client_tcp.c"
                       INCLUDE_DIRS "."
                       REQUIRES freertos lwip esp_wifi nvs_flash mqtt log esp_driver_gpio driver)