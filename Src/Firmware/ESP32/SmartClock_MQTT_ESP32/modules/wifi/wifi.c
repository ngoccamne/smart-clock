#include "wifi.h"

static EventGroupHandle_t s_wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "SCAN WIFI";
static const char *TAG_1 = "WIFI STATION";
static int s_retry_num = 0;

void wifi_scan(void)
{
    uint16_t number = 10;
    wifi_ap_record_t wifi_found[10];
    memset(wifi_found, 0, sizeof(wifi_found)); // Toan bo mang wifi_found deu bang 0
    
    //ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA)); 
    //ESP_ERROR_CHECK(esp_wifi_start());
     
    esp_wifi_scan_start(NULL, true);
    esp_wifi_scan_get_ap_records(&number, wifi_found); // Lay ket qua va giai phong bo nho

    ESP_LOGI(TAG, "AP number ap_info holds = %u", number);
    for (int i = 0; i < number; i++){
        ESP_LOGI(TAG, "SSDI \t\t%s", wifi_found[i].ssid); // Ten wifi
        ESP_LOGI(TAG, "RSSI \t\t%d", wifi_found[i].rssi); // Cuong do 
        ESP_LOGI(TAG, "Channel \t\t%d", wifi_found[i].primary); // Kenh
    }
}

static void event_wifi_sta_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    // WIFI VUA KHOI DONG XONG
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        // esp_wifi_connect();
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        if (s_retry_num < ESP_MAXIMUM_RETRY) 
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "Retry to connect to the AP");
        } 
        else 
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"Connect to the AP fail");
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void)
{
    s_wifi_event_group = xEventGroupCreate();
    ESP_LOGI(TAG_1,"Starting connect");  

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    
    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    esp_event_handler_instance_register(WIFI_EVENT,
                                        ESP_EVENT_ANY_ID,
                                        &event_wifi_sta_handler,
                                        NULL,
                                        &instance_any_id);
    esp_event_handler_instance_register(IP_EVENT,
                                        IP_EVENT_STA_GOT_IP,
                                        &event_wifi_sta_handler,
                                        NULL,
                                        &instance_got_ip);
    ESP_LOGI(TAG_1,"Error check done");

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = ESP_WIFI_SSID,
            .password = ESP_WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            
            .pmf_cfg = {
                .capable = true,
                .required = false
            }
        }
    };
    ESP_LOGI(TAG_1,"Config done");

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();
    ESP_LOGI(TAG_1, "Wifi_init_station finished.");
 
    wifi_scan(); 

    ESP_LOGI(TAG, "Scan done!");
    esp_wifi_connect();

    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE, // Xoa den sau khi doc? (FALSE la giu nguyen)
        pdFALSE, // Cho tat ca den sang hay chi mot? (FALSE la chi can mot den sang)
        portMAX_DELAY); 

    ESP_LOGI(TAG_1,"Config done 2");

   
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG_1, "Connected to ap SSID:%s password:%s",
                 ESP_WIFI_SSID, ESP_WIFI_PASS);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG_1, "Failed to connect to SSID:%s, password:%s",
                 ESP_WIFI_SSID, ESP_WIFI_PASS);
    } else {
        ESP_LOGE(TAG_1, "UNEXPECTED EVENT");
    }
}


