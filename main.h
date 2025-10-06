// main.h
// Josh Brake
// jbrake@hmc.edu
// 10/31/22 

#ifndef MAIN_H
#define MAIN_H

#include "STM32L432KC.h"
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Custom defines
///////////////////////////////////////////////////////////////////////////////

#define GPIO_A PA1 //five volt tolerant GPIO pin
#define GPIO_B PA2 //five volt tolerant GPIO pin

#define DELAY_TIM TIM2

#endif // MAIN_H