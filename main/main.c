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
#include "hbridge_dir_hil.h"

/* Private Defines & Macros **************************************************/

#define TASK_TAG               ("MAIN")
#define LED_GPIO_NUM           (GPIO_NUM_21)
#define PWMA_GPIO_NUM          (GPIO_NUM_22)
#define DIR_GPIO_NUM          (GPIO_NUM_23)
#define MOTOR1                 (0)
#define MOTOR2                 (1)
#define MAIN_INFO(fmt, ...)    ESP_LOGI(TASK_TAG, fmt, ##__VA_ARGS__)
#define MAIN_ERROR(fmt, ...)   ESP_LOGE(TASK_TAG, fmt, ##__VA_ARGS__)
#define MAIN_WARNING(fmt, ...) ESP_LOGW(TASK_TAG, fmt, ##__VA_ARGS__)

/* Private types definition **************************************************/

/* Private variables *********************************************************/
static brushedMotorHandle_t _motorHandle = NULL;

/* Private prototypes ********************************************************/

static void * _motorInit(uint32_t freq, void* args);

/* Private Functions *********************************************************/

static void * _motorInit(uint32_t freq, void* args)
{
    if(args == (void*)MOTOR1){
        return HBDHIL_initIos(PWMA_GPIO_NUM, DIR_GPIO_NUM, freq);
    }

    return NULL;
}

/* Public Functions **********************************************************/

void app_main()
{
    uint8_t ledHandle = 0;

    if (LEDDRV_init() != ESP_OK) {
        MAIN_ERROR("LED driver init failed");
    }

    _motorHandle = BMDRV_addMotor(3900, _motorInit, (void*)MOTOR1, (brushedMotorSetDutyCycleCbk_t)HBDHIL_setDutyCycle);
    if(_motorHandle == NULL){
        MAIN_ERROR("Motor init failed");
    }

    if (POSAPP_init() != ESP_OK) {
        MAIN_ERROR("Position app init failed");
    }

    ledHandle = LEDDRV_registerLed(LED_GPIO_NUM, LED_NO_PIN, LED_NO_PIN);

    OSUTILS_setSystemReady();

    LEDDRV_setLedBlinking(ledHandle, 0xFF0000, true, 100, 500, BLINK_CONTINIOUSLY);
    BMDRV_setSpeed(_motorHandle, 50.0);
}