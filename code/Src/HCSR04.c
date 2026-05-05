#include "Headfile.h"
#include "HCSR04.h"

uint16_t TIM_CNT = 0;

float HCSR04_Get_Distance(void)
{
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);
    DelayUs(5);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_SET);
    DelayUs(20);
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_5, GPIO_PIN_RESET);

    __HAL_TIM_SetCounter(&htim3, 0);

    uint16_t timeout = 0;
    while(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6) == GPIO_PIN_RESET)
    {
        if(++timeout > 60000) return -1.0f;
    }

    HAL_TIM_Base_Start(&htim3);

    timeout = 0;
    while(HAL_GPIO_ReadPin(GPIOF, GPIO_PIN_6) == GPIO_PIN_SET)
    {
        if(++timeout > 60000) break;
    }

    HAL_TIM_Base_Stop(&htim3);
    TIM_CNT = __HAL_TIM_GetCounter(&htim3);

    float distance = (TIM_CNT * 0.034f) / 2.0f;

    vTaskDelay(pdMS_TO_TICKS(100));

    return distance;
}
