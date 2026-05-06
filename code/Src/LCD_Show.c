#include "Headfile.h"
#include "LCD_Show.h"


static void LCD_Show_Init(void)
{
    SPI_LCD_Init();
    LCD_SetBackColor(LCD_WHITE);
    LCD_Clear();
    LCD_SetTextFont(&CH_Font24); 
    LCD_SetColor(LCD_BLACK);

}

void LCD_Show_Task(void* param)
{
    
    LCD_Show_Init();
    Servo_Init();
	volatile int i = 0;
    char Text[20];
    HAL_TIM_Base_Start(&htim3);
    
    volatile uint16_t V = 0;
    
    motor_speed_t speed;
    speed.speed_m1 = 0;
    speed.speed_m2 = 0;
    speed.speed_m3 = 0;
    speed.speed_m4 = 0;

    motor_encoder_t Encoder;

    uint16_t Angle = 0;
    int8_t dir = 1;
    
    while (1)
    {
        Angle += dir * 10;
        if (Angle >= 180)
        {
            Angle = 180;
            dir = -1;
        }
        else if (Angle <= 0)
        {
            Angle = 0;
            dir = 1;
        }
        Servo_SetAngle(Angle);
        Motor_Set_Speed(speed);
        Motor_Get_Encoder(&Encoder);
        
        snprintf(Text, sizeof(Text), "Encoder = %d        ", Encoder.encoder_m1);
        LCD_DisplayText(10, 20, Text);

        V = Motor_Get_Vol();
		snprintf(Text, sizeof(Text), "V = %dmV       ", V);
        LCD_DisplayText(10, 40, Text);

        float distance = HCSR04_Get_Distance();
        snprintf(Text, sizeof(Text), "Distance = %.2f      ", distance);
        LCD_DisplayText(10, 60, Text);

        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}
