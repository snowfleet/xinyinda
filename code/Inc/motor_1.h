#ifndef __MOTOR_1_H__
#define __MOTOR_1_H__

#include "stdint.h"


#define CAM_DEFAULT_I2C_ADDRESS       0x34
#define MOTOR_TYPE_ADDR               20
#define MOTOR_FIXED_SPEED_ADDR        51
#define MOTOR_ENCODER_POLARITY_ADDR   21
#define MOTOR_FIXED_PWM_ADDR          31
#define MOTOR_ENCODER_TOTAL_ADDR      60
#define ADC_BAT_ADDR                  0

#include "Typies.h"

#define MOTOR_TYPE_WITHOUT_ENCODER        0
#define MOTOR_TYPE_TT                     1
#define MOTOR_TYPE_N20                    2
#define MOTOR_TYPE_JGB                    3

void Motor1_Init(void);
void Motor1_Set_Speed(motor_speed_t speed);
uint16_t Motor1_Get_Vol(void);
void Motor1_Get_Encoder(motor_encoder_t* Encoder);
void Motor1_Reset_Encoder(void);

#endif
