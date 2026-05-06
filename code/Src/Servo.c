#include "Servo.h"
#include "Headfile.h"

void Servo_Init(void)
{
    HAL_TIM_Base_Start(&htim4);
    //回到中间位置
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    Servo_SetAngle(90);
}

void Servo_SetAngle(uint16_t angle)
{
    if (angle > 180) angle = 180;
    uint16_t compare = 500 + angle * 2000 / 180;
    __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, compare);
}
