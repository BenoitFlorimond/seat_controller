/**
 * ****************************************************************************
 * main.c
 * ****************************************************************************
 */

/* Documentation *************************************************************/

/* Includes ******************************************************************/

#include "brushed_motor_driver.h"
#include "hbridge_dir_hil.h"
#include "leds_driver.h"
#include "os_utils.h"
#include "position_app.h"
#include <driver/gpio.h>
#include <esp_log.h>

/* Private Defines & Macros **************************************************/

#define TASK_TAG                  ("MAIN")
#define LED_GPIO_NUM              (GPIO_NUM_21)
#define MOTOR_LEFT_PWMA_GPIO_NUM  (GPIO_NUM_23)
#define MOTOR_LEFT_DIR_GPIO_NUM   (GPIO_NUM_22)
#define MOTOR_RIGHT_PWMA_GPIO_NUM (GPIO_NUM_33)
#define MOTOR_RIGHT_DIR_GPIO_NUM  (GPIO_NUM_25)
#define MOTOR1                    (0)
#define MOTOR2                    (1)
#define MAIN_INFO(fmt, ...)       ESP_LOGI(TASK_TAG, fmt, ##__VA_ARGS__)
#define MAIN_ERROR(fmt, ...)      ESP_LOGE(TASK_TAG, fmt, ##__VA_ARGS__)
#define MAIN_WARNING(fmt, ...)    ESP_LOGW(TASK_TAG, fmt, ##__VA_ARGS__)

/* Private types definition **************************************************/

/* Private variables *********************************************************/
static brushedMotorHandle_t _motorLeftHandle  = NULL;
static brushedMotorHandle_t _motorRightHandle = NULL;

/* Private prototypes ********************************************************/

static void* _motorInit(uint32_t freq, void* args);

/* Private Functions *********************************************************/

static void* _motorInit(uint32_t freq, void* args)
{
    if (args == (void*)MOTOR1) {
        return HBDHIL_initIos(MOTOR_LEFT_PWMA_GPIO_NUM, MOTOR_LEFT_DIR_GPIO_NUM, freq);
    } else if (args == (void*)MOTOR2) {
        return HBDHIL_initIos(MOTOR_RIGHT_PWMA_GPIO_NUM, MOTOR_RIGHT_DIR_GPIO_NUM, freq);
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

    _motorLeftHandle = BMDRV_addMotor(3900, _motorInit, (void*)MOTOR1, (brushedMotorSetDutyCycleCbk_t)HBDHIL_setDutyCycle);
    if (_motorLeftHandle == NULL) {
        MAIN_ERROR("Motor left init failed");
    }
    _motorRightHandle = BMDRV_addMotor(3900, _motorInit, (void*)MOTOR2, (brushedMotorSetDutyCycleCbk_t)HBDHIL_setDutyCycle);
    if (_motorRightHandle == NULL) {
        MAIN_ERROR("Motor right init failed");
    }

    if (POSAPP_init() != ESP_OK) {
        MAIN_ERROR("Position app init failed");
    }

    OSUTILS_setSystemReady();

    ledHandle = LEDDRV_registerLed(LED_GPIO_NUM, LED_NO_PIN, LED_NO_PIN);
    LEDDRV_setLedBlinking(ledHandle, 0xFF0000, true, 100, 500, BLINK_CONTINIOUSLY);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if ((BMDRV_setSpeed(_motorLeftHandle, 10.0) != ESP_OK) || (BMDRV_setSpeed(_motorRightHandle, -10.0) != ESP_OK)) {
            MAIN_ERROR("Could not set speed");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        if ((BMDRV_setSpeed(_motorLeftHandle, 50.0) != ESP_OK) || (BMDRV_setSpeed(_motorRightHandle, -50.0) != ESP_OK)) {
            MAIN_ERROR("Could not set speed");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        if ((BMDRV_setSpeed(_motorLeftHandle, 100.0) != ESP_OK) || (BMDRV_setSpeed(_motorRightHandle, -100.0) != ESP_OK)) {
            MAIN_ERROR("Could not set speed");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        if ((BMDRV_setSpeed(_motorLeftHandle, -10.0) != ESP_OK) || (BMDRV_setSpeed(_motorRightHandle, 10.0) != ESP_OK)) {
            MAIN_ERROR("Could not set speed");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        if ((BMDRV_setSpeed(_motorLeftHandle, -50.0) != ESP_OK) || (BMDRV_setSpeed(_motorRightHandle, 50.0) != ESP_OK)) {
            MAIN_ERROR("Could not set speed");
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
        if ((BMDRV_setSpeed(_motorLeftHandle, -100.0) != ESP_OK) || (BMDRV_setSpeed(_motorRightHandle, 100.0) != ESP_OK)) {
            MAIN_ERROR("Could not set speed");
        }
    }
}