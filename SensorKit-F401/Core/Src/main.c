/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// https://github.com/libdriver/dht20/blob
#include "driver_dht20.h"

//https://github.com/STMicroelectronics/stm32-ssd1315
//#include "ssd1315.h"
#include "ssd1306.h"
#include "fonts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t temp_raw=0;
float temperature=0;
//char* temperatureString;
char temperatureString[10];


float lightLevel2=0;
char lightLevelString[6];
uint16_t lightLevel = 0;



uint32_t hum_raw=0;
uint8_t humidity=0;
float humidity3=0;
char humidityString[3];

uint32_t potValue=0;
uint16_t adcValues[3]={0};

//SSD1315_Object_t ahogyszeretnemcsillagnelegyenott;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
void makeSound(uint16_t tone, uint8_t volume, uint16_t duration) {

	htim3.Instance->ARR = tone;
	htim3.Instance->CCR1 = tone * (volume / 100.0);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_Delay(duration);
//	htim3.Instance->CCR1 = 0;
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}
*/

void iicWriteData(uint8_t addr, uint8_t *buf, uint16_t len) {
	HAL_I2C_Master_Transmit(&hi2c1, addr, buf, len, 1);
}

void iicReadData(uint8_t addr, uint8_t *buf, uint16_t len) {
	HAL_I2C_Master_Receive(&hi2c1, addr, buf, len, 1);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	dht20_handle_t hdth;
	hdth.delay_ms = &HAL_Delay;
	hdth.iic_read_cmd = &iicReadData;
	hdth.iic_write_cmd = &iicWriteData;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */

	dht20_init(&hdth);
	//SSD1315_Unselect();
	//(SSD1315_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation)
	//SSD1315_Init(&ahogyszeretnemcsillagnelegyenott,0,SSD1315_ORIENTATION_LANDSCAPE);

	//SSD1315_FillScreen(SSD1315_COLOR_BLACK );
	//SSD1315_WriteString(0, 0, "Font_7x10, red on black, lorem ipsum dolor sit amet", Font_7x10, SSD1315_COLOR_WHITE , SSD1315_COLOR_BLACK );

	//SSD1315_DisplayOn(&ahogyszeretnemcsillagnelegyenott);
	//SSD1315_SetBrightness(&ahogyszeretnemcsillagnelegyenott, 10);
	//SSD1315_FillRGBRect(&ahogyszeretnemcsillagnelegyenott, 0, 0, , 7, 8);
	//SSD1315_FillRect(&ahogyszeretnemcsillagnelegyenott, 10, 10, 100, 100, 0);
	ssd1306_Init(&hi2c1);
	ssd1306_Fill(Black);

	//ssd1306_WriteString("Ho: ", Font_7x10, White);
	//ssd1306_WriteString(&temperatureString, Font_7x10, White);
	ssd1306_UpdateScreen(&hi2c1);

	MX_ADC1_Init();

	HAL_ADC_Start_DMA(&hadc1, adcValues, 1);





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		dht20_read_temperature(&hdth, &temp_raw, &temperature);
		dht20_read_humidity(&hdth, &hum_raw, &humidity);

		ssd1306_SetCursor(10,10);
		ssd1306_WriteString("Temp: ", Font_7x10, White);
		//sprintf(temperatureString, "%f", temperature);
		snprintf(temperatureString, sizeof(temperatureString), "%f", temperature);
		ssd1306_WriteString(temperatureString, Font_7x10, White);


		ssd1306_SetCursor(10,30);
		ssd1306_WriteString("Hum: ", Font_7x10, White);
		humidity3 = humidity;
		//sprintf(humidityString, "%f", humidity);
		snprintf(humidityString, sizeof(humidityString), "%f", humidity3);

		ssd1306_WriteString(humidityString, Font_7x10, White);

		//temp hum OLED end
		//HAL_ADC_PollForConversion(&hadc1, 1);
		HAL_ADC_Start_DMA(&hadc1, adcValues, 1);

		lightLevel = adcValues[0];

		/*
		HAL_ADC_PollForConversion(&hadc1, 1);

		HAL_ADC_GetValue(&hadc1);

		HAL_ADC_PollForConversion(&hadc1, 1);

		HAL_ADC_GetValue(&hadc1);
		*/



		ssd1306_SetCursor(10,50);
		ssd1306_WriteString("Light: ", Font_7x10, White);
		lightLevel = adcValues[0];
		lightLevel2 = lightLevel;
		//sprintf(humidityString, "%", humidity);
		snprintf(lightLevelString, sizeof(lightLevelString), "%f", lightLevel2);

		ssd1306_WriteString(lightLevelString, Font_7x10, White);




		ssd1306_UpdateScreen(&hi2c1);
		ssd1306_Fill(Black);
		ssd1306_SetCursor(10,10);

		HAL_Delay(10);




		/*
		HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
		HAL_Delay(100);
		HAL_ADC_Start_DMA(&hadc1, adcValues, 3);
		makeSound(540*(adcValues[1]/4096.0), 60, 100);
		 */

//	  if(HAL_GPIO_ReadPin(BUTTON_GPIO_Port, BUTTON_Pin)){
//		  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, SET);
//		  makeSound(10, 80, 300);
//	  }else{
//		  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
//		  dht20_read_temperature(&hdth, &temp_raw, &temperature);
//		  HAL_Delay(300);
//	  }

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
