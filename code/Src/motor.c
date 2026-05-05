#include "motor.h"
#include "Headfile.h"
#include <stdint.h>

void Motor_Set_Speed(motor_speed_t speed)
{
    int8_t Speed_buf[4] = {speed.speed_m1, speed.speed_m2, speed.speed_m3, speed.speed_m4};
    I2C_Write_Len(MOTOR_FIXED_SPEED_ADDR,Speed_buf,4);
}

void Motor_Init(void)
{
    int8_t MotorType = MOTOR_TYPE_JGB;
    int8_t MotorEncoderPolarity = 0;

    I2C_Write_Len(MOTOR_TYPE_ADDR,&MotorType,1);
    DelayMs(5);
    I2C_Write_Len(MOTOR_ENCODER_POLARITY_ADDR,&MotorEncoderPolarity,1);
	DelayMs(5);
}

uint16_t Motor_Get_Vol(void)
{
    uint8_t data[2] = {0x01, 0x02};
    uint8_t ret = I2C_Read_Len(ADC_BAT_ADDR,data,2);
    if(ret != 0)
    {
        return 0;
    }
    return (data[0]|(data[1]<<8))	;
}

void Motor_Get_Encoder(motor_encoder_t* Encoder)
{
    uint8_t buf[16];
    I2C_Read_Len(MOTOR_ENCODER_TOTAL_ADDR,buf,16);

    Encoder->encoder_m1 = (int32_t)(buf[0] | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24));
    Encoder->encoder_m2 = (int32_t)(buf[4] | (buf[5]<<8) | (buf[6]<<16) | (buf[7]<<24));
    Encoder->encoder_m3 = (int32_t)(buf[8] | (buf[9]<<8) | (buf[10]<<16) | (buf[11]<<24));
    Encoder->encoder_m4 = (int32_t)(buf[12] | (buf[13]<<8) | (buf[14]<<16) | (buf[15]<<24));
}

void Motor_Reset_Encoder(void)
{
    int8_t EncodeReset[16]={0};   								//用于重置脉冲值
    I2C_Write_Len(MOTOR_ENCODER_TOTAL_ADDR,EncodeReset,16);		//重置脉冲值
}
