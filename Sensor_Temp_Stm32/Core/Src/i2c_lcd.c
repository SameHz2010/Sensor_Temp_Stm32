#include <i2c_lcd.h>
#include "stm32f1xx_hal.h"
#include <stdio.h>
#include <stdarg.h>

extern I2C_HandleTypeDef hi2c1;

#define SLAVE_ADDRESS_LCD 0x4E

void lcd_send_cmd(char cmd) {
    char data_u = (cmd & 0xF0);
    char data_l = ((cmd << 4) & 0xF0);
    uint8_t data_t[4];

    data_t[0] = data_u | 0x0C; 
    data_t[1] = data_u | 0x08;  
    data_t[2] = data_l | 0x0C;  
    data_t[3] = data_l | 0x08;  

    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_t, 4, 100);
    HAL_Delay(2);
}

void lcd_send_data(char data) {
    char data_u = (data & 0xF0);
    char data_l = ((data << 4) & 0xF0);
    uint8_t data_t[4];

    data_t[0] = data_u | 0x0D;  
    data_t[1] = data_u | 0x09;  
    data_t[2] = data_l | 0x0D;  
    data_t[3] = data_l | 0x09;  

    HAL_I2C_Master_Transmit(&hi2c1, SLAVE_ADDRESS_LCD, data_t, 4, 100);
    HAL_Delay(2);
}

void lcd_clear(void) {
    lcd_send_cmd(0x01);  
    lcd_send_cmd(0x02);  
    HAL_Delay(2);
}

void lcd_put_cur(int row, int col) {
    uint8_t address;
    switch (row) {
        case 0: address = 0x80 + col; break;
        case 1: address = 0xC0 + col; break;
        case 2: address = 0x94 + col; break;  
        case 3: address = 0xD4 + col; break;  
        default: return;
    }
    lcd_send_cmd(address);
}

void lcd_init(void) {
    HAL_Delay(50);

    lcd_send_cmd(0x30);
    HAL_Delay(5);
    lcd_send_cmd(0x30);
    HAL_Delay(1);
    lcd_send_cmd(0x30);
    HAL_Delay(10);
    lcd_send_cmd(0x20);  
    HAL_Delay(10);

    lcd_send_cmd(0x28); 
    HAL_Delay(1);
    lcd_send_cmd(0x08); 
    HAL_Delay(1);
    lcd_send_cmd(0x01); 
    HAL_Delay(2);
    lcd_send_cmd(0x06); 
    HAL_Delay(1);
    lcd_send_cmd(0x0C); 
}

void lcd_send_string(char *str) {
    while (*str) lcd_send_data(*str++);
}

void lcd_printf(const char *format, ...) {
    char buffer[32];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    lcd_send_string(buffer);
}
