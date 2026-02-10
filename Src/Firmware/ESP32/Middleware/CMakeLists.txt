set(MIN_LIB_PATH "../../MIN_Library") 

idf_component_register(
    SRCS 
        "MIN_Port/min_port.c"                 
        "${MIN_LIB_PATH}/target/min.c"        
    
    INCLUDE_DIRS 
        "MIN_Port"                            
        "${MIN_LIB_PATH}/target"              
        
    REQUIRES driver                         
)