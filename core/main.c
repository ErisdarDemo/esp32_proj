/**************************************************************************************************/
/** @file     main.c
 *  @brief    Project setup & directory relocation example 
 *  @details  Serving reference for other projects
 *
 *  @author   Justin Reina, Firmware Engineer
 *  @created  3/31/25
 *  @last rev 3/31/25
 *
 *  @note   Private functions & variables are declared static
 *
 *  @section    Opens
 *		none listed
 *
 *  @section    Legal Disclaimer
 *      ©2025 Justin Reina. All rights reserved. All contents of this source file and/or any other
 *      related source files are the explicit property of Justin Reina. Do not distribute.
 *      Do not copy.
 */
/**************************************************************************************************/

//************************************************************************************************//
//                                            INCLUDES                                            //
//************************************************************************************************//

//Standard Library Includes
#include <stdio.h>
#include <inttypes.h>

//Library Includes
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

//SDK Includes
#include "sdkconfig.h"

//FreeRTOS Includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Project Includes
#include "utils.h"
#include "main.h"


//************************************************************************************************//
//                                          PUBLIC FUNCTIONS                                      //
//************************************************************************************************//

/**************************************************************************************************/
/** @fcn        void app_main(void)
 *  @brief      FreeRTOS task for main application
 *  @details    Called by FreeRTOS scheduler when started
 *
 *  @section    Purpose
 *      Unlike normal FreeRTOS tasks, or embedded C main functions, the app_main() task is allowed 
 *		to return. If this happens, The task is cleaned up and the system will continue running 
 *		with other RTOS tasks scheduled normally. Therefore, it is possible to implement app_main 
 *		as either a function that creates other application tasks and then returns, or as a main 
 *		application task itself. app_main() has a fixed RTOS priority, one higher than the minimum
 *
 *	@pre	second stage bootloader
 *	@post	esp_restart()
 */
/**************************************************************************************************/
void app_main(void) {
	
    //Locals
    unsigned major_rev;								/* Chip major revision                        */
    unsigned minor_rev;								/* Chip minor revision                        */
    uint32_t flash_size;							/* Flash memory size (Bytes) 				  */
    esp_err_t stat;									/* SDK response status code 				  */
    esp_chip_info_t chip_info;						/* Info about the chip						  */


    //---------------------------------------- Initialize ----------------------------------------//
    printf("Hello esp!\n");

	//Get Info
    esp_chip_info(&chip_info);
	
	//Retrieve
    major_rev = (chip_info.revision / 100);			/* divisor									  */
    minor_rev = (chip_info.revision % 100);			/* remainder								  */

    /* Print chip information */
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
	          CONFIG_IDF_TARGET,
	          chip_info.cores,
	          (chip_info.features & CHIP_FEATURE_WIFI_BGN)   ? "WiFi/" : "",
	          (chip_info.features & CHIP_FEATURE_BT)         ? "BT"    : "",
	          (chip_info.features & CHIP_FEATURE_BLE)        ? "BLE"   : "",
	          (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : ""
	      );
	               
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
        
    //Get Size
    stat = esp_flash_get_size(NULL, &flash_size);
    
    //Safety
    if(stat != ESP_OK) {    
        printf("Get flash size failed");    
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", 
              flash_size / (uint32_t)(1024 * 1024),
           	  (chip_info.features & CHIP_FEATURE_EMB_FLASH)  ? "embedded" : "external"
          );

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());


    //------------------------------------------ Operate -----------------------------------------//

    for (int i = 10; i >= 0; i--) {
		
        printf("Restarting in %d seconds...\n", i);
        
        utils_test_fcn();
        
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    
    
    //------------------------------------------ Restart -----------------------------------------//
    printf("Restarting now.\n");

    fflush(stdout);

    esp_restart();
}

