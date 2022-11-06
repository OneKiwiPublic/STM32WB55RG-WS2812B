#ifndef WS2812B_H_
#define WS2812B_H_

#include <stdint.h>

typedef struct{
	uint8_t blue;
	uint8_t red;
	uint8_t green;
	uint32_t color;
}Led_Color_t;

void ws2818b_clear();
void ws2818b_update();
void ws2812b_set_color(uint8_t red, uint8_t blue, uint8_t green);
void ws2812b_set_color_number(uint8_t index, uint8_t red, uint8_t blue, uint8_t green);

void ws2818b_rainbow(void);
/**
 * https://github.com/lbthomsen/stm32-ws2812b
 * https://forum.digikey.com/t/controlling-neopixels-with-stm32/20527/3
 * https://controllerstech.com/interface-ws2812-with-stm32/
 */

/**
 * WS2812B = 1.2us => 800kHz => Ftimer = 800KHz
 * Ftimer = TIM1_CLK / ((Prescaler + 1)*(Period + 1))
 * TIM1_CLK = SystemCoreClock = 64MHz
 * chose Prescaler = 0 => Period = 79
 */
#endif /* WS2812B_H_ */
