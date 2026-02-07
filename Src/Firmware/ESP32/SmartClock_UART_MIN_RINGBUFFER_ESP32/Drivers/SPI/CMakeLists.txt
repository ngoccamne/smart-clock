idf_component_register(
    SRCS 
        "dma.c" 
        "hspi.c" 
        "spi.c" 
        
    INCLUDE_DIRS "include"
    
    REQUIRES "driver" "esp_system"
)