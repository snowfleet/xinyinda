#include "Headfile.h"
#include "IIC.h"

void I2C_SDA_OUT(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=IIC_IO_SDA;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOX,&GPIO_InitStructure);
}

void I2C_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin=IIC_IO_SDA;
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStructure.Mode=GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull=GPIO_PULLUP;
	HAL_GPIO_Init(GPIOX,&GPIO_InitStructure);
}

void IIC_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitStructure.Pin = IIC_IO_SDA |IIC_IO_SCL;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOX, &GPIO_InitStructure);
	HAL_GPIO_WritePin(GPIOX,IIC_IO_SDA|IIC_IO_SCL, GPIO_PIN_SET);
}

void IIC_start()
{
	I2C_SDA_OUT();
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET);
	DelayUs(10);  // 加长延时
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_RESET);
	DelayUs(10);
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
}

void IIC_stop()
{
	I2C_SDA_OUT();
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_RESET);
	DelayUs(10);
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_SET);
	DelayUs(10);
}

void IIC_ack()
{
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
	I2C_SDA_OUT();
  HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_RESET);
  DelayUs(5);
  HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET);
  DelayUs(5);
  HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
}

void IIC_noack()
{
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
	I2C_SDA_OUT();
  HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_SET);
  DelayUs(5);
  HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET);
  DelayUs(5);
  HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
}

uint8_t IIC_wait_ack()
{
	uint8_t tempTime=0;
	I2C_SDA_IN();
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_SET);
	DelayUs(2);
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET);
	DelayUs(2);

	while(HAL_GPIO_ReadPin(GPIOX, IIC_IO_SDA))
	{
		tempTime++;
		if(tempTime>300)  // 超时加长，更稳
		{
			IIC_stop();
			return 1;
		}
	}
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
	return 0;
}

void IIC_send_byte(uint8_t txd)
{
	uint8_t i=0;
	I2C_SDA_OUT();
	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
	for(i=0;i<8;i++)
	{
		if(txd&0x80)
			HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_SET);
		else
			HAL_GPIO_WritePin(GPIOX, IIC_IO_SDA, GPIO_PIN_RESET);
		txd<<=1;
		DelayUs(5);
		HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET);
		DelayUs(5);
		HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
		DelayUs(2);
	}
}

uint8_t IIC_read_byte(uint8_t ack)
{
	uint8_t i=0,receive=0;
	I2C_SDA_IN();
  for(i=0;i<8;i++)
  {
   	HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_RESET);
		DelayUs(5);
		HAL_GPIO_WritePin(GPIOX, IIC_IO_SCL, GPIO_PIN_SET);
		DelayUs(5);
		receive<<=1;
		if(HAL_GPIO_ReadPin(GPIOX, IIC_IO_SDA))
		   receive++;
		DelayUs(2);
  }

  if(!ack)
	  IIC_noack();
	else
		IIC_ack();

	return receive;
}

// 读数据（不动）
uint8_t I2C_Read_Len(uint8_t Reg,uint8_t *Buf,uint8_t Len)
{
	uint8_t i;
	IIC_start();
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0);
	if(IIC_wait_ack() == 1)
	{
		IIC_stop();
		return 1;
	}
	IIC_send_byte(Reg);
	if(IIC_wait_ack() == 1)
	{
		IIC_stop();
		return 1;
	}
	IIC_start();
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 1);
	if(IIC_wait_ack() == 1)
	{
		IIC_stop();
		return 1;
	}
	for(i=0;i<Len;i++)
	{
		if(i != Len-1)
			Buf[i] = IIC_read_byte(1);
		else
			Buf[i] = IIC_read_byte(0);
	}
	IIC_stop();
	return 0;
}

// 写数据（不动）
int8_t I2C_Write_Len(int8_t Reg,int8_t *Buf,int8_t Len)
{
	uint8_t i;
	IIC_start();
	IIC_send_byte((CAM_DEFAULT_I2C_ADDRESS << 1) | 0);
	if(IIC_wait_ack() == 1)
	{
		IIC_stop();
		return 1;
	}
	IIC_send_byte(Reg);
	if(IIC_wait_ack() == 1)
	{
		IIC_stop();
		return 1;
	}
	for(i =0;i<Len;i++)
	{
		IIC_send_byte(Buf[i]);
		if(IIC_wait_ack() == 1)
		{
			IIC_stop();
			return 1;
		}
	}
	IIC_stop();
	return 0;
}