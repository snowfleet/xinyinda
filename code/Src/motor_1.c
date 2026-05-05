#include "motor_1.h"
#include "Headfile.h"
#include <stdint.h>

#define I2C_TIMEOUT  100

static HAL_StatusTypeDef Motor1_I2C_Write(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
    return HAL_I2C_Mem_Write(&hi2c2, CAM_DEFAULT_I2C_ADDRESS<<1 , Reg, I2C_MEMADD_SIZE_8BIT, Buf, Len, I2C_TIMEOUT);
}

static HAL_StatusTypeDef Motor1_I2C_Read(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
    return HAL_I2C_Mem_Read(&hi2c2, CAM_DEFAULT_I2C_ADDRESS<<1 , Reg, I2C_MEMADD_SIZE_8BIT, Buf, Len, I2C_TIMEOUT);
}

void Motor1_Init(void)
{
    int8_t MotorType = MOTOR_TYPE_JGB;
    int8_t MotorEncoderPolarity = 0;

    Motor1_I2C_Write(MOTOR_TYPE_ADDR, (uint8_t*)&MotorType, 1);
    DelayMs(5);
    Motor1_I2C_Write(MOTOR_ENCODER_POLARITY_ADDR, (uint8_t*)&MotorEncoderPolarity, 1);
    DelayMs(5);
}

void Motor1_Set_Speed(motor_speed_t speed)
{
    int8_t Speed_buf[4] = {speed.speed_m1, speed.speed_m2, speed.speed_m3, speed.speed_m4};
    Motor1_I2C_Write(MOTOR_FIXED_SPEED_ADDR, (uint8_t*)Speed_buf, 4);
}

uint16_t Motor1_Get_Vol(void)
{
    uint8_t data[2] = {0};
    HAL_StatusTypeDef ret = Motor1_I2C_Read(ADC_BAT_ADDR, data, 2);
    if(ret != HAL_OK)
    {
        return 0;
    }
    return (data[0] | (data[1] << 8)) / 10;
}

void Motor1_Get_Encoder(motor_encoder_t* Encoder)
{
    uint8_t buf[16] = {0};
    HAL_StatusTypeDef ret = Motor1_I2C_Read(MOTOR_ENCODER_TOTAL_ADDR, buf, 16);
    if(ret != HAL_OK)
    {
        Encoder->encoder_m1 = 0;
        Encoder->encoder_m2 = 0;
        Encoder->encoder_m3 = 0;
        Encoder->encoder_m4 = 0;
        return;
    }
    Encoder->encoder_m1 = (int32_t)(buf[0] | (buf[1]<<8) | (buf[2]<<16) | (buf[3]<<24));
    Encoder->encoder_m2 = (int32_t)(buf[4] | (buf[5]<<8) | (buf[6]<<16) | (buf[7]<<24));
    Encoder->encoder_m3 = (int32_t)(buf[8] | (buf[9]<<8) | (buf[10]<<16) | (buf[11]<<24));
    Encoder->encoder_m4 = (int32_t)(buf[12] | (buf[13]<<8) | (buf[14]<<16) | (buf[15]<<24));
}

void Motor1_Reset_Encoder(void)
{
    uint8_t EncodeReset[16] = {0};
    Motor1_I2C_Write(MOTOR_ENCODER_TOTAL_ADDR, EncodeReset, 16);
}
