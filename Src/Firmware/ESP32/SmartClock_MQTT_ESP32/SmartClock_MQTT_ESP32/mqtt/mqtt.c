#include "mqtt.h"
#include <string.h>
#include "esp_log.h"
#include "lwip/sockets.h"

static const char *TAG = "MQTT";

void mqtt_send_string(int id, const char *string)
{
    uint16_t len = htons(strlen(string)); // htons: convert Little Endian to Big Endian
    send(id, &len, 2, 0);
    send(id, str, strlen(string), 0);
}

int mqtt_connect(mqtt_client *client, const char *client_id)
{
    uint8_t id_len = strlen(client_id);
    uint8_t remain_len = 10 + (2 + id_len);

    uint8_t fixed_header[] = { MQTT_CONNECT, remain_len };
    uint8_t variable_header[] = {
        0x00, 0x04, 'M', 'Q', 'T', 'T', 
        0x04,                           
        0x02,                           
        0x00, 0x3C                   
    };

    send(client->id, fixed_header, 2, 0);
    send(client->id, variable_header, 10, 0);
    send_mqtt_string(client->id, client_id);
    
    ESP_LOGI(TAG, "Sent CONNECT");
    return 0;
}

int mqtt_publish(mqtt_client *client, const char* topic, const char* message) 
{
    uint8_t topic_len = strlen(topic);
    uint8_t msg_len = strlen(message);
    uint8_t remain_len = (2 + topic_len) + msg_len;

    uint8_t fixed_header[] = { MQTT_PUBLISH, remain_len };

    send(client->id, fixed_header, 2, 0);
    send_mqtt_string(client->id, topic);
    send(client->id, message, msg_len, 0);

    ESP_LOGI(TAG, "Sent PUBLISH to %s", topic);
    return 0;
}

int mqtt_subscribe(mqtt_client *client, const char* topic) 
{
    uint8_t topic_len = strlen(topic);
    uint8_t remain_len = 2 + (2 + topic_len) + 1; 

    uint8_t fixed_header[] = { MQTT_SUBSCRIBE, remain_len };
    uint8_t packet_id[] = { 0x00, 0x01 }; 
    uint8_t qos = 0x00;                   

    send(client->id, fixed_header, 2, 0);
    send(client->id, packet_id, 2, 0);
    send_mqtt_string(client->id, topic);
    send(client->id, &qos, 1, 0);

    ESP_LOGI(TAG, "Sent SUBSCRIBE to %s", topic);
    return 0;
}

int mqtt_min_ping(mqtt_client *client) 
{
    uint8_t ping_packet[] = { MQTT_PINGREQ, 0x00 };
    send(client->id, ping_packet, 2, 0);
    ESP_LOGI(TAG, "Sent PINGREQ");
    return 0;
}