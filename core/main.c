/**************************************************************************************************/
/** @file     main.c
 *  @brief    x
 *  @details  x
 *
 *  @author   Justin Reina, Firmware Engineer
 *  @created  5/30/25
 *  @last rev 5/30/25 'v0'
 *
 *  @section    Opens
 *		unknown argument false report (Espressif-IDE)
 *
 *  @section    Legal Disclaimer
 *      SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 *      SPDX-License-Identifier: CC0-1.0
 */
/**************************************************************************************************/

//************************************************************************************************//
//                                            INCLUDES                                            //
//************************************************************************************************//

//Standard Library Includes
#include <stdio.h>
#include <inttypes.h>

//FreeRTOS Includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//SDK Includes
#include "sdkconfig.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_log.h"
#include "esp_system.h"

//Project Includes
#include "main.h"


//************************************************************************************************//
//                                            VARIABLES                                           //
//************************************************************************************************//

//Constants
static const char* TAG = "esp32_proj";


//************************************************************************************************//
//                                          PUBLIC FUNCTIONS                                      //
//************************************************************************************************//

/**************************************************************************************************/
/** @fcn        void app_main(void)
 *  @brief      FreeRTOS task for main application
 *  @details    Called by FreeRTOS scheduler when started, running on core for main thread
 *
 *  @section    Purpose
 *      Unlike normal FreeRTOS tasks, or embedded C main functions, the app_main() task is allowed 
 *      to return. If this happens, The task is cleaned up and the system will continue running 
 *      with other RTOS tasks scheduled normally. Therefore, it is possible to implement app_main 
 *      as either a function that creates other application tasks and then returns, or as a main 
 *      application task itself. app_main() has a fixed RTOS priority, one higher than the minimum
 *
 *  @pre    second stage bootloader
 *  @post   no return
 */
/**************************************************************************************************/
void app_main(void) {
	
	//Locals
	esp_err_t err;									/* Espressif status code api				  */
	esp_chip_info_t chip_info;	                    /* Print chip information                     */
	uint32_t flash_size;
	
	
	//Notify
    printf("\n\nHello world!\n");

	//Check
    esp_chip_info(&chip_info);
    
	printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN)   ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT)         ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE)        ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");
			   
    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
	
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
	
	//Read
	err = esp_flash_get_size(NULL, &flash_size);
	
	//Check
	ESP_ERROR_CHECK(err);

	//Handle
    if(err != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

	//Notify
    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

	//Demo
    utils_test_fcn();

	//Loop
    for (int i = 10; i >= 0; i--) {
		
		//Update
        printf("Restarting in %d seconds...\n", i);
        
		//Delay
		vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
	
	
	//Notify
    printf("Restarting now.\n\n\n\n");
	
	ESP_LOGI(TAG, "Demo complete");
	
	
	//Reset
    fflush(stdout);
    esp_restart();
}
