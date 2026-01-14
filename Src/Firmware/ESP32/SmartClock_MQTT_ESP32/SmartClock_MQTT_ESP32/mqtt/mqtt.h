#ifndef MQTT_H
#define MQTT_H

#include <stdint.h>

#define MQTT_CONNECT 0x10
#define MQTT_PUBLISH     0x30
#define MQTT_SUBSCRIBE   0x82  
#define MQTT_PINGREQ     0xC0
#define MQTT_DISCONNECT  0xE0

typedef struct {
    int id;
    const char *broker_ip;
    uint16_t port;
} mqtt_client; 

void mqtt_send_string(int id, const char *string);
int mqtt_connect(mqtt_client *client, const char *client_id);
int mqtt_publish(mqtt_client *client, const char* topic, const char* message);
int mqtt_subscribe(mqtt_client *client, const char* topic);
int mqtt_min_ping(mqtt_client *client); 
#endif