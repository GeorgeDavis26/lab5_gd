/*

File    : main.c
Purpose : UPDATE!
Author  : George Davis
email.  : gdavis@hmc.edu

*/

#include <stdio.h>

#include <stdlib.h>

#include "main.h"

//global
int count = 0;

/*
    CW INDICATED BY A POSITIVE COUNTER VALUE
      GPIO 1 BEFORE GPIO 2 
    CCW INDICATED BY A NEGATIVE COUNTER VALUE
      GPIO 2 BEFORE GPIO 1
*/
void EXTI1_IRQHandler(void){
    // Check that the correct GPIO pin was what triggered our interrupt
/*
GPIO_A
*/
    if (EXTI->PR1 & (1 << gpioPinOffset(GPIO_A))){ // checks rising edge
            //how can we detect rising falling 
            // check the other 
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(GPIO_A));
        // Then perform rotary encoding logic
        if(digitalRead(GPIO_A) && digitalRead(GPIO_B))        {count--;} //A = 1 B = 1
        else if(digitalRead(GPIO_A) && ~digitalRead(GPIO_B))  {count++;} //A = 1 B = 0
        else if(~digitalRead(GPIO_A) && digitalRead(GPIO_B))  {count++;} //A = 0 B = 1
        else if(~digitalRead(GPIO_A) && ~digitalRead(GPIO_B)) {count--;} //A = 0 B = 0
    }
  }

void EXTI2_IRQHandler(void){
    // Check that the correct GPIO pin was what triggered our interrupt
/*
GPIO_B
*/
    if (EXTI->PR1 & (1 << gpioPinOffset(GPIO_B))){ // checks rising edge
            //how can we detect rising falling 
            // check the other 
        // If so, clear the interrupt (NB: Write 1 to reset.)
        EXTI->PR1 |= (1 << gpioPinOffset(GPIO_B));
        // Then perform rotary encoding logic
        if(digitalRead(GPIO_A) && digitalRead(GPIO_B))        {count++;} //A = 1 B = 1
        else if(digitalRead(GPIO_A) && ~digitalRead(GPIO_B))  {count--;} //A = 1 B = 0
        else if(~digitalRead(GPIO_A) && digitalRead(GPIO_B))  {count--;} //A = 0 B = 1
        else if(~digitalRead(GPIO_A) && ~digitalRead(GPIO_B)) {count++;} //A = 0 B = 0
    }
}


int main(void) {
/*

  main function of lab 5 functionality
  inititalizes a counter that measures a set period of time
  then reads four parameters off of the two photosensors on the rotary
  quadrature encoder. With the rising and falling edge of each sensor triggering
  a counter to increment the algorithim is awarded a 
  4x resolution comapared a signle rising edge detection.

*/

  //CONFIGURE SYS CLOCK AS PLL
//    configureFlash();
//    configureClock();

  //CONFIGURE TWO GPIOS AS INPUT
    gpioEnable(GPIO_PORT_A); 

    pinMode(GPIO_A, GPIO_INPUT); 
    pinMode(GPIO_B, GPIO_INPUT); 

    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD1, 0b10); // Set PA1 as pull-down
    GPIOA->PUPDR |= _VAL2FLD(GPIO_PUPDR_PUPD2, 0b10); // Set PA2 as pull-down


  //CONFIGURE ONE TIMER THAT CAN BE INITIALIZED BY HARDWARE
    RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
    initTIM(DELAY_TIM);

    // TODO
    // 1. Enable SYSCFG clock domain in RCC
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    // 2. Configure EXTICR for the input rotary ecoders
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI1, 0b000); // Select PA1
    SYSCFG->EXTICR[1] |= _VAL2FLD(SYSCFG_EXTICR1_EXTI2, 0b000); // Select PA2

    // Enable interrupts globally
    __enable_irq();
/*
GPIO_A
*/
    // Configure mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(GPIO_A)); // Configure the mask bit
    // Enable rising edge trigger
    EXTI->RTSR1 |= (1 << gpioPinOffset(GPIO_A));// Enable rising edge trigger
    // Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(GPIO_A));// Enable falling edge trigger
    // Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI1_IRQn);

/*
GPIO_B
*/
    // 1. Configure mask bit
    EXTI->IMR1 |= (1 << gpioPinOffset(GPIO_B)); // Configure the mask bit
    // 2. Enable rising edge trigger
    EXTI->RTSR1 |= (1 << gpioPinOffset(GPIO_B));// Enable rising edge trigger
    // 3. Enable falling edge trigger
    EXTI->FTSR1 |= (1 << gpioPinOffset(GPIO_B));// Enable falling edge trigger
    // 4. Turn on EXTI interrupt in NVIC_ISER
    NVIC->ISER[0] |= (1 << EXTI2_IRQn);

  uint32_t ms = 500;
  uint32_t ppr = 408; //Pulses Per Revolution characteristic of the rotary encoder u bsed

  while(1){
    count = 0;
    delay_millis(TIM2, ms);
    float rate = (count / (ppr * (ms * 1e-3) * 4)); // rate calculation
    if(rate > 0){
      printf("CW at Rate = ");
      printf("%f", rate);
      printf("[rev/sec] \n");
    }
    else if(rate < 0){
      printf("CCW at Rate = ");
      printf("%f", rate);
      printf("[rev/sec] \n");
    }
    else{
      printf("Not Rotating, Rate = ");
      printf("%f", rate);
      printf("[rev/sec] \n");
    }
  }
};
