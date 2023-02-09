/**
 * ****************************************************************************
 * regulation_app.c
 * ****************************************************************************
 */

/* Documentation *************************************************************/

/* Includes ******************************************************************/

#include "regulation_app.h"
#include "brushed_motor_driver.h"
#include "freertos_includes.h"
#include "hbridge_dir_hil.h"
#include "os_utils.h"
#include "pid_service.h"
#include "position_app.h"
#include <driver/gpio.h>
#include <esp_log.h>

/* Private Defines & Macros **************************************************/

#define TASK_TAG                  ("REG")
#define TASK_STACK_SIZE           (2048)
#define TASK_PRIO                 (4)
#define LOG_INFO(fmt, ...)        ESP_LOGI(TASK_TAG, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)       ESP_LOGE(TASK_TAG, fmt, ##__VA_ARGS__)
#define LOG_WARNING(fmt, ...)     ESP_LOGW(TASK_TAG, fmt, ##__VA_ARGS__)
/* Hardware defines */
#define MOTOR_PWM_FREQ_HZ         (3900)
#define MOTOR_LEFT_PWMA_GPIO_NUM  (GPIO_NUM_23)
#define MOTOR_LEFT_DIR_GPIO_NUM   (GPIO_NUM_22)
#define MOTOR_RIGHT_PWMA_GPIO_NUM (GPIO_NUM_33)
#define MOTOR_RIGHT_DIR_GPIO_NUM  (GPIO_NUM_25)
/* PID defines */
#define PID_SAMPLING_MS           (100)
#define PID_PITCH_DECIMATION      (1)
#define PID_PITCH_KP              (2.0)
#define PID_PITCH_KI              (0.0)
#define PID_PITCH_KD              (0.0)
#define PID_ROLL_DECIMATION       (1)
#define PID_ROLL_KP               (2.0)
#define PID_ROLL_KI               (0.0)
#define PID_ROLL_KD               (0.0)

/* Private types definition **************************************************/

/* Private variables *********************************************************/

static brushedMotorHandle_t _motorLeftHandle  = NULL;
static brushedMotorHandle_t _motorRightHandle = NULL;
static pidHandle_t _pidPitchHandle            = NULL;
static pidHandle_t _pidRollHandle             = NULL;

/* Private prototypes ********************************************************/

static void* _motorInit(brushedMotorHandle_t handle);
static void _pidSetOutput(pidHandle_t pid, double value);
static esp_err_t _pidMeasure(pidHandle_t pid, double* value);

/* Private Functions *********************************************************/

static void* _motorInit(brushedMotorHandle_t handle)
{
    if (handle == _motorLeftHandle) {
        return HBDHIL_initIos(MOTOR_LEFT_PWMA_GPIO_NUM, MOTOR_LEFT_DIR_GPIO_NUM, MOTOR_PWM_FREQ_HZ);
    } else if (handle == _motorRightHandle) {
        return HBDHIL_initIos(MOTOR_RIGHT_PWMA_GPIO_NUM, MOTOR_RIGHT_DIR_GPIO_NUM, MOTOR_PWM_FREQ_HZ);
    }

    return NULL;
}

static void _pidSetOutput(pidHandle_t pid, double value)
{
    static double _averageSpeed = 0;
    static double _deltaSpeed   = 0;

    if (pid == _pidPitchHandle) {
        _averageSpeed = -value;
    } else if (pid == _pidRollHandle) {
        _deltaSpeed = -value;
    }

    BMDRV_setSpeed(_motorLeftHandle, (float)_averageSpeed - _deltaSpeed);
    BMDRV_setSpeed(_motorRightHandle, (float)_averageSpeed + _deltaSpeed);
}

static esp_err_t _pidMeasure(pidHandle_t pid, double* value)
{
    if (pid == _pidPitchHandle) {
        POSAPP_getOrientation(value, NULL);
        return ESP_OK;
    } else if (pid == _pidRollHandle) {
        POSAPP_getOrientation(NULL, value);
        return ESP_OK;
    }

    return ESP_FAIL;
}

/* Public Functions **********************************************************/

esp_err_t REGAPP_init(void)
{
    _motorLeftHandle = BMDRV_createMotor(_motorInit, (brushedMotorSetDutyCycleCbk_t)HBDHIL_setDutyCycle);
    if ((_motorLeftHandle == NULL) || (BMDRV_initMotor(_motorLeftHandle) != ESP_OK)) {
        LOG_ERROR("Motor left init failed");
        return ESP_FAIL;
    }

    _motorRightHandle = BMDRV_createMotor(_motorInit, (brushedMotorSetDutyCycleCbk_t)HBDHIL_setDutyCycle);
    if ((_motorRightHandle == NULL) || (BMDRV_initMotor(_motorRightHandle) != ESP_OK)) {
        LOG_ERROR("Motor right init failed");
        return ESP_FAIL;
    }

    if (PIDSVC_init(10) != ESP_OK) {
        LOG_ERROR("PID service init failed");
        return ESP_FAIL;
    }

    _pidPitchHandle = PIDSVC_createPid(PID_PITCH_DECIMATION, PID_PITCH_KP, PID_PITCH_KI, PID_PITCH_KD, 0.0, _pidSetOutput, _pidMeasure);
    _pidRollHandle  = PIDSVC_createPid(PID_ROLL_DECIMATION, PID_ROLL_KP, PID_ROLL_KI, PID_ROLL_KD, 0.0, _pidSetOutput, _pidMeasure);

    return ESP_OK;
}