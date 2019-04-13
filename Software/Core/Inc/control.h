#ifndef __CONTROL_H
#define __CONTROL_H
#include "stm32f1xx_hal.h"
typedef struct{
    void (*init)(void);
}control_type;
extern control_type control;
#endif
