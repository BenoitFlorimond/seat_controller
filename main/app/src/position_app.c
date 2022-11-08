/**
 * ****************************************************************************
 * position_app.c
 * ****************************************************************************
 */

/* Documentation *************************************************************/

/* Includes ******************************************************************/

#include "position_app.h"
#include "lis2dw12_hil.h"
#include "accelerometer_driver.h"
#include "freertos_includes.h"
#include "os_utils.h"
#include <esp_log.h>

/* Private Defines & Macros **************************************************/

#define TASK_TAG                 ("POS")
#define TASK_STACK_SIZE          (2048)
#define TASK_PRIO                (3)
#define POSAPP_INFO(fmt, ...)    ESP_LOGI(TASK_TAG, fmt, ##__VA_ARGS__)
#define POSAPP_ERROR(fmt, ...)   ESP_LOGE(TASK_TAG, fmt, ##__VA_ARGS__)
#define POSAPP_WARNING(fmt, ...) ESP_LOGW(TASK_TAG, fmt, ##__VA_ARGS__)
/* Hard conf */
#define I2C_SDA_NUM              (26)
#define I2C_SCL_NUM              (27)
#define I2C_MASTER_NUM           (0)
#define I2C_FREQ_HZ              (100000)
/* Driver conf */
#define POLLING_PERIOD_MS        (100)

/* Private types definition **************************************************/

/* Private variables *********************************************************/

/* Private prototypes ********************************************************/

static void _process(void* priv);
static esp_err_t _initCom(void);
static void _axisValueCbk(int16_t xAxis, int16_t yAxis, int16_t zAxis);

/* Private Functions *********************************************************/

static void _process(void* priv)
{
    OSUTILS_waitSystemStartup();

    for (;;) {
    }
}

static esp_err_t _initCom(void)
{
    return lis2dw12_initCom(I2C_SDA_NUM, I2C_SCL_NUM, I2C_MASTER_NUM, I2C_FREQ_HZ);
}

static void _axisValueCbk(int16_t xAxis, int16_t yAxis, int16_t zAxis)
{
    POSAPP_INFO("X = %d, Y = %d, Z = %d", xAxis, yAxis, zAxis);
}

/* Public Functions **********************************************************/

esp_err_t POSAPP_init(void)
{
    acceleroApi_struct_t acceleroApi = {
        &_initCom,
        &lis2dw12_initDevice,
        &lis2dw12_readAxis
    };

    // if (xTaskCreate(_process, TASK_TAG, TASK_STACK_SIZE, NULL, TASK_PRIO, NULL) != pdPASS) {
    //     return ESP_ERR_NO_MEM;
    // }

    return ACCDRV_init(&acceleroApi, true, POLLING_PERIOD_MS, _axisValueCbk);
}