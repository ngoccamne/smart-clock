#ifndef _WIFI_H
#define _WIFI_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "lwip/err.h"
#include "lwip/sys.h"

/*******************************************************************************************************************
 * Defines
 ******************************************************************************************************************/

#define ESP_WIFI_SSID     "PIF_CLUB"
#define ESP_WIFI_PASS     "chinsochin"
#define ESP_MAXIMUM_RETRY 5

/*******************************************************************************************************************
 * Function prototype
 ******************************************************************************************************************/

void wifi_init_sta(void);

#endif
