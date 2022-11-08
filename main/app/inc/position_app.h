/**
* ****************************************************************************
* position_app.h
* ****************************************************************************
*/

#ifndef __POSITION_APP_H__
#define __POSITION_APP_H__

/* Documentation *************************************************************/

/* Includes ******************************************************************/

#include <esp_err.h>

/* Public Defines & Macros ***************************************************/

/* Public types definition ***************************************************/

/* Public prototypes *********************************************************/

esp_err_t POSAPP_init(void);

void POSAPP_getOrientation(float * pitch, float * roll);

#endif /* __POSITION_APP_H__ */