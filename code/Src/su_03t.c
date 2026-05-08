#include "su_03t.h"
#include "Headfile.h"
#include "string.h"
#include "stdint.h"
#include "task.h"
#include "freertos.h"

#define UART2_RECV_LEN          5

TaskHandle_t xUART2_Recv_Task_Handle; // UART2接收任务句柄

static uint8_t UART2_Recv_dma_buf[UART2_RECV_LEN] = {0};

static uint8_t UART2_Recv_buf[UART2_RECV_LEN] = {0};
static uint16_t UART2_recv_len = 0;

void su03t_Init(void)
{
    xTaskCreate(su03t_Send_Task, "su03t_Send_Task", 256, NULL, 1, NULL);
    xTaskCreate(su03t_Receive_Task, "su03t_Receive_Task", 256, NULL, 1, &xUART2_Recv_Task_Handle);

    HAL_UART_Receive_DMA(&huart2, UART2_Recv_dma_buf, UART2_RECV_LEN);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);  // 开启空闲中断
}



void Process_UART2_Recv_Data(uint8_t* data)
{
    // 处理数据
    // ...
}



void su03t_Send_Task(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}





void su03t_Receive_Task(void *pvParameters)
{
    while(1)
    {
        // 等待通知（阻塞状态，不占用CPU）
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        // 处理数据
        UART2_recv_len = UART2_RECV_LEN - __HAL_DMA_GET_COUNTER(huart2.hdmarx); // 计算接收的长度
		
		// 数据拷贝到缓冲区
		memcpy(UART2_Recv_buf, UART2_Recv_dma_buf, UART2_recv_len);
		
		// 进入数据处理函数
		Process_UART2_Recv_Data(UART2_Recv_buf);
		
		// 清空缓存
		memset(UART2_Recv_dma_buf, 0, UART2_RECV_LEN);
		memset(UART2_Recv_buf, 0, UART2_RECV_LEN);
        
		// 重新启动 DMA 接收
        HAL_UART_Receive_DMA(&huart2, UART2_Recv_dma_buf, UART2_RECV_LEN);
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
