#include "stm32f1xx.h"
#include <stdio.h>
#include "gpio.h"
#include "tim.h"
uint32_t Read_Weigh(void);
void user_delaynus_tim(uint32_t nus);
void weight_get();
