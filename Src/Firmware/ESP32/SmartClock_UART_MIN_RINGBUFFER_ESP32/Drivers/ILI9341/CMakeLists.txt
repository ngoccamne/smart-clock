idf_component_register(
    SRCS 
        "Adafruit_GFX_AS.cpp" 
        "Adafruit_ILI9341_fast_as.cpp"
        
        # Các file C
        "Font7s.c" 
        "Font16.c" 
        "Font32.c" 
        "Font64.c" 
        "mini-printf.c"
        
    INCLUDE_DIRS "include"
    
    REQUIRES "driver" "spi" "freertos"
)