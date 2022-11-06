#include "ws2812b.h"
#include "main.h"

#define NEOPIXEL_ZERO		25
#define NEOPIXEL_ONE		51
#define NUM_LED				10
#define DMA_BUFF_SIZE		((NUM_LED*24))

extern TIM_HandleTypeDef htim1;
uint8_t sent_flag = 0;
Led_Color_t leds[NUM_LED] = {0};
uint32_t dmaBuffer[DMA_BUFF_SIZE] = {0};

#define RAINBOW_SIZE		14
uint8_t rainbow_index = 0;
uint32_t rainbow_color[14] = {
	0xfe0000,
	0xfe6a00,
	0xffd800,
	0x00fe21,
	0x0094fe,
	0x0026ff,
	0xb100fe,
	0x800001,
	0x803400,
	0x806b00,
	0x007f0e,
	0x00497e,
	0x001280,
	0x590080,
};

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	sent_flag = 1;
}

void ws2818b_clear()
{
	uint16_t i;

	for(i = 0; i < NUM_LED; i++)
	{
		leds[i].red = 0x00;
		leds[i].blue = 0x00;
		leds[i].green = 0x00;
	}

	for(i = 0; i < DMA_BUFF_SIZE; i++)
	{
		dmaBuffer[i] = 0;
	}
}

void ws2818b_send_data(){
	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)dmaBuffer, 2*DMA_BUFF_SIZE);
	//while (!sent_flag){};
	//sent_flag = 0;
}

void ws2818b_update(){

	uint32_t indx = 0;
	uint32_t color;


	for (int i= 0; i < NUM_LED; i++)
	{

		color = ((leds[i].green<<16) | (leds[i].red<<8) | (leds[i].blue));

		for (int j=23; j>=0; j--)
		{
			if (color&(1<<j))
			{
				dmaBuffer[indx] = NEOPIXEL_ONE;  // 2/3 of 90
			}else {
				dmaBuffer[indx] = NEOPIXEL_ZERO;//30;  // 1/3 of 90
			}
			indx++;
		}

	}

	//dmaBuffer[DMA_BUFF_SIZE - 1] = 0;
	ws2818b_send_data();
}

void ws2812b_set_color(uint8_t red, uint8_t blue, uint8_t green)
{
	for(uint8_t i = 0; i < NUM_LED; i++)
	{
		leds[i].red = red;
		leds[i].blue = blue;
		leds[i].green = green;
	}
}

void ws2812b_set_color_number(uint8_t index, uint8_t red, uint8_t blue, uint8_t green)
{
	leds[index].red = red;
	leds[index].blue = blue;
	leds[index].green = green;
}

void ws2818b_rainbow(void)
{
	uint8_t red, blue, green;
	uint8_t temp = rainbow_index;

	for(uint8_t i = 0; i < NUM_LED; i++)
	{
		temp = (rainbow_index + i) % RAINBOW_SIZE;
		blue = rainbow_color[temp] & 0xff;
		green = (rainbow_color[temp]>>8) & 0xff;
		red = (rainbow_color[temp]>>16) & 0xff;

		leds[i].red = red;
		leds[i].blue = blue;
		leds[i].green = green;
	}

	rainbow_index++;
	if (rainbow_index >= RAINBOW_SIZE){
		rainbow_index = 0;
	}
}
