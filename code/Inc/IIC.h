#ifndef __IIC_H
#define __IIC_H

#include "stdint.h"

//*****模拟IIC相关
//*****修改的宏定义
//*****不同芯片时注意时钟和引脚
#define    IIC_IO_SDA      GPIO_PIN_9  //SDA的IO口
#define    IIC_IO_SCL      GPIO_PIN_8  //SCL的IO口
#define    GPIOX           GPIOD       //GPIOx选择
#define    CLOCK		   RCC_APB2Periph_GPIOA //时钟信号

#define    IIC_SCL_SET     HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET)
#define    IIC_SCL_RESET   HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET)
#define    IIC_SDA_SET     HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_SET)
#define    IIC_SDA_RESET   HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_RESET)
#define    READ_SDA        HAL_GPIO_ReadPin(GPIOX, IIC_IO_SDA)  //输入SDA


void I2C_SDA_OUT(void);
void I2C_SDA_IN(void);
void IIC_init(void);
void IIC_start(void);
void IIC_stop(void);
void IIC_ack(void);
void IIC_noack(void);
uint8_t IIC_wait_ack(void);
uint8_t IIC_read_byte(uint8_t ack);
void IIC_send_byte(uint8_t txd);
uint8_t I2C_Read_Len(uint8_t Reg,uint8_t *Buf,uint8_t Len);
int8_t I2C_Write_Len(int8_t Reg,int8_t *Buf,int8_t Len);
#endif
