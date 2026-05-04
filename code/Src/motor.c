#include "motor.h"
#include "Headfile.h"
#include <stdint.h>


//发送成功标志位
uint8_t Send_flag = 0;

int8_t I2C_Write_motor(uint16_t Reg_ADDR, uint8_t *Buf, uint16_t Len)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write_DMA(&hi2c2, CAM_DEFAULT_I2C_ADDRESS, Reg_ADDR, I2C_MEMADD_SIZE_8BIT, Buf, Len);
    if(status != HAL_OK)
    {
        return -1;
    }
    return 0;
}

int8_t I2C_Read_motor(uint16_t Reg_ADDR, uint8_t *Buf, uint16_t Len)
{
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read_DMA(&hi2c2, CAM_DEFAULT_I2C_ADDRESS, Reg_ADDR, I2C_MEMADD_SIZE_8BIT, Buf, Len);
    if(status != HAL_OK)
    {
        return -1;
    }
    return 0;
}

int8_t motor_init(void)
{
    int8_t status;
    uint8_t motor_type[4] = {MOTOR_TYPE_JGB, MOTOR_TYPE_JGB, MOTOR_TYPE_JGB, MOTOR_TYPE_JGB};
    uint8_t polarity = 0;

    status = I2C_Write_motor(MOTOR_TYPE_ADDR, motor_type, 4);
    if(status != 0)
    {
        return -1;
    }
    vTaskDelay(pdMS_TO_TICKS(5));

    status = I2C_Write_motor(MOTOR_ENCODER_POLARITY_ADDR, &polarity, 1);
    vTaskDelay(pdMS_TO_TICKS(5));

    return 0;
}

int8_t motor_set_speed(motor_speed_t speed)
{
    int8_t speed_buf[4] = {speed.speed_m1, speed.speed_m2, speed.speed_m3, speed.speed_m4};
    int8_t status;
    status = I2C_Write_motor(MOTOR_FIXED_SPEED_ADDR, (uint8_t*)speed_buf, 4);
    if(status != 0)
    {
        return -1;
    }
    return 0;
}

int8_t motor_read_Encoder(motor_encoder_t* encoder)
{
    int8_t status;
    int32_t encoder_buf[4];
    status = I2C_Read_motor(MOTOR_ENCODER_TOTAL_ADDR, (uint8_t*)encoder_buf, 16);
    if(status != 0)
    {
        return -1;
    }

    encoder->encoder_m1 = encoder_buf[0];
    encoder->encoder_m2 = encoder_buf[1];
    encoder->encoder_m3 = encoder_buf[2];
    encoder->encoder_m4 = encoder_buf[3];


    return 0;
}

int8_t motor_res_encoder(void)
{
    int8_t status;
    int32_t reset_buf = 0;  // 16个字节的0x00
    status = I2C_Write_motor(0x3C, (uint8_t*)&reset_buf, 16);
    if(status != 0)
    {
        return -1;
    }
    return 0;
}

uint16_t motor_read_Vol(void)
{
    int8_t status;
    uint8_t Vol_data[2];

    status = I2C_Read_motor(ADC_BAT_ADDR, Vol_data, 2);
    if(status != 0)
    {
        return -1;
    }

    return (Vol_data[0] | (Vol_data[1] << 8));

}


uint8_t motor_send_flag(void)
{
    //保存标志位后清除标志位
    uint8_t Send_flag_temp = Send_flag;
    Send_flag = 0;
    return Send_flag_temp;
}


//DMA回调函数 
