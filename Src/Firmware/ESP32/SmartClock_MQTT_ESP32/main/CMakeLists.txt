idf_component_register(SRCS "main.c" 
                    PRIV_REQUIRES mqtt_client nvs_flash esp_netif wifi 
                                       esp_driver_uart mqtt log esp_driver_gpio
                    INCLUDE_DIRS ".")
