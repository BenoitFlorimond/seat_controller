/**
 * ****************************************************************************
 * position_app.c
 * ****************************************************************************
 */

/* Documentation *************************************************************/

/* Includes ******************************************************************/

#include "position_app.h"
#include "accelerometer_driver.h"
#include "freertos_includes.h"
#include "lis2dw12_hil.h"
#include "os_utils.h"
#include <esp_log.h>
#include <math.h>

/* Private Defines & Macros **************************************************/

#define TASK_TAG                 ("POS")
#define TASK_STACK_SIZE          (2048)
#define TASK_PRIO                (3)
#define POSAPP_INFO(fmt, ...)    ESP_LOGI(TASK_TAG, fmt, ##__VA_ARGS__)
#define POSAPP_ERROR(fmt, ...)   ESP_LOGE(TASK_TAG, fmt, ##__VA_ARGS__)
#define POSAPP_WARNING(fmt, ...) ESP_LOGW(TASK_TAG, fmt, ##__VA_ARGS__)
#define QUEUE_SIZE               (10)
/* Hard conf */
#define I2C_SDA_NUM              (26)
#define I2C_SCL_NUM              (27)
#define I2C_MASTER_NUM           (0)
#define I2C_FREQ_HZ              (100000)
/* Driver conf */
#define POLLING_PERIOD_MS        (100)

/* Private types definition **************************************************/

typedef struct {
    int16_t xAxis;
    int16_t yAxis;
    int16_t zAxis;
} axisValues_struct_t;

/* Private variables *********************************************************/

static QueueHandle_t _queueForPositioning  = NULL;
static SemaphoreHandle_t _orientationMutex = NULL;
static float _pitch                        = 0.0;
static float _roll                         = 0.0;

/* Private prototypes ********************************************************/

static void _process(void* priv);
static esp_err_t _initCom(void);
static void _axisValueCbk(int16_t xAxis, int16_t yAxis, int16_t zAxis);

/* Private Functions *********************************************************/

static void _process(void* priv)
{
    axisValues_struct_t axisValues = { 0 };
    float pitch, roll, xAxis, yAxis, zAxis = 0.0;

    OSUTILS_waitSystemStartup();

    for (;;) {
        xQueueReceive(_queueForPositioning, &axisValues, portMAX_DELAY);

        /* Invert x and y and convert from mg to g (float) */
        xAxis = (float)axisValues.yAxis / 1000.0;
        yAxis = (float)axisValues.xAxis / 1000.0;
        zAxis = (float)axisValues.zAxis / 1000.0;
        roll  = atan2(yAxis, zAxis) * 180.0 / M_PI;
        pitch = atan2((-xAxis), sqrt(yAxis * yAxis + zAxis * zAxis)) * 180.0 / M_PI;

        POSAPP_INFO("X = %0.2f, Y = %0.2f, Z = %0.2f, pitch = %0.2f, roll = %0.2f", xAxis, yAxis, zAxis, pitch, roll);

        xSemaphoreTake(_orientationMutex, portMAX_DELAY);
        _pitch = pitch;
        _roll  = roll;
        xSemaphoreGive(_orientationMutex);
    }
}

static esp_err_t _initCom(void)
{
    return lis2dw12_initCom(I2C_SDA_NUM, I2C_SCL_NUM, I2C_MASTER_NUM, I2C_FREQ_HZ);
}

static void _axisValueCbk(int16_t xAxis, int16_t yAxis, int16_t zAxis)
{
    axisValues_struct_t axisValues = { 0 };

    axisValues.xAxis = xAxis;
    axisValues.yAxis = yAxis;
    axisValues.zAxis = zAxis;

    xQueueSend(_queueForPositioning, &axisValues, WRITE_IN_QUEUE_DEFAULT_TIMEOUT);
}

/* Public Functions **********************************************************/

esp_err_t POSAPP_init(void)
{
    acceleroApi_struct_t acceleroApi = {
        &_initCom,
        &lis2dw12_initDevice,
        &lis2dw12_readAxis
    };

    _queueForPositioning = xQueueCreate(QUEUE_SIZE, sizeof(axisValues_struct_t));
    if (_queueForPositioning == NULL) {
        return ESP_ERR_NO_MEM;
    }

    _orientationMutex = xSemaphoreCreateMutex();
    if (_orientationMutex == NULL) {
        return ESP_ERR_NO_MEM;
    }

    if (xTaskCreate(_process, TASK_TAG, TASK_STACK_SIZE, NULL, TASK_PRIO, NULL) != pdPASS) {
        return ESP_ERR_NO_MEM;
    }

    return ACCDRV_init(&acceleroApi, true, POLLING_PERIOD_MS, _axisValueCbk);
}

void POSAPP_getOrientation(float* pitch, float* roll)
{
    xSemaphoreTake(_orientationMutex, portMAX_DELAY);
    *pitch = _pitch;
    *roll  = _roll;
    xSemaphoreGive(_orientationMutex);
}