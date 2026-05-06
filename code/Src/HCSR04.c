#include "Headfile.h"
#include "HCSR04.h"

uint16_t TIM_CNT = 0;



float HCSR04_Get_Distance(void)
{
    HAL_GPIO_WritePin(HCSR04_GPIO_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);
    DelayUs(5);
    HAL_GPIO_WritePin(HCSR04_GPIO_PORT, HCSR04_TRIG_PIN, GPIO_PIN_SET);
    DelayUs(20);
    HAL_GPIO_WritePin(HCSR04_GPIO_PORT, HCSR04_TRIG_PIN, GPIO_PIN_RESET);

    __HAL_TIM_SetCounter(&htim3, 0);

    while(HAL_GPIO_ReadPin(HCSR04_GPIO_PORT, HCSR04_ECHO_PIN) == GPIO_PIN_RESET)
    {
        if(__HAL_TIM_GetCounter(&htim3) > 30000) return -1.0f;
    }

    __HAL_TIM_SetCounter(&htim3, 0);

    while(HAL_GPIO_ReadPin(HCSR04_GPIO_PORT, HCSR04_ECHO_PIN) == GPIO_PIN_SET)
    {
        if(__HAL_TIM_GetCounter(&htim3) > 30000) break;
    }

    TIM_CNT = __HAL_TIM_GetCounter(&htim3);

    float distance = (TIM_CNT * 0.034f) / 2.0f;

    vTaskDelay(pdMS_TO_TICKS(100));

    return distance;
}
