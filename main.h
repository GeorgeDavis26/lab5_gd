// main.h
// George Davis
// gdavis@hmc.edu
// 10/07/25 
// Header file of the E155 lab5 main.c file 

//adapted from prof. Brakes main.h example code for the interrupt.c tutorial

#ifndef MAIN_H
#define MAIN_H

#include "STM32L432KC.h"
#include <stm32l432xx.h>

///////////////////////////////////////////////////////////////////////////////
// Custom defines
///////////////////////////////////////////////////////////////////////////////

#define GPIO_A PA1 //five volt tolerant GPIO pin
#define GPIO_B PA2 //five volt tolerant GPIO pin

#define GPIO_I PA3 //INTERRUPT FLAG PIN
#define GPIO_P PA4 //POLLING FLAG PIN

#define DELAY_TIM TIM2

#endif // MAIN_H