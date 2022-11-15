/**
 * ****************************************************************************
 * main.c
 * ****************************************************************************
 */

/* Documentation *************************************************************/

/* Includes ******************************************************************/

#include "brushed_motor_driver.h"
#include "leds_driver.h"
#include "os_utils.h"
#include "position_app.h"
#include <driver/gpio.h>
#include <esp_log.h>

/* Private Defines & Macros **************************************************/

#define TASK_TAG               ("MAIN")
#define LED_GPIO_NUM           (GPIO_NUM_21)
#define PWMA_GPIO_NUM          (GPIO_NUM_22)
#define PWMB_GPIO_NUM          (GPIO_NUM_23)
#define MAIN_INFO(fmt, ...)    ESP_LOGI(TASK_TAG, fmt, ##__VA_ARGS__)
#define MAIN_ERROR(fmt, ...)   ESP_LOGE(TASK_TAG, fmt, ##__VA_ARGS__)
#define MAIN_WARNING(fmt, ...) ESP_LOGW(TASK_TAG, fmt, ##__VA_ARGS__)

/* Private types definition **************************************************/

/* Private variables *********************************************************/

/* Private prototypes ********************************************************/

/* Private Functions *********************************************************/

/* Public Functions **********************************************************/

void app_main()
{
    uint8_t ledHandle = 0;
    brushedMotorHandle_t motorHandle = NULL;

    if (LEDDRV_init() != ESP_OK) {
        MAIN_ERROR("LED driver init failed");
    }

    motorHandle = BMDRV_addMotor(PWMA_GPIO_NUM, PWMB_GPIO_NUM, 44000);
    BMDRV_setSpeed(motorHandle, 50.0);

        if (POSAPP_init() != ESP_OK)
    {
        MAIN_ERROR("Position app init failed");
    }

    OSUTILS_setSystemReady();

    ledHandle = LEDDRV_registerLed(LED_GPIO_NUM, LED_NO_PIN, LED_NO_PIN);
    LEDDRV_setLedBlinking(ledHandle, 0xFF0000, true, 100, 500, BLINK_CONTINIOUSLY);
}