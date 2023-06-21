/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stdio.h"
#include "string.h"
#include "exti.h"
#include "stdlib.h"
#include "TDOA.h"
#include "oled.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LENGTH 30
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t key=0;
uint8_t str[10];

extern DMA_HandleTypeDef hdma_adc1;

extern int i;
uint32_t a1[4];
uint64_t T_count;

uint8_t x[16];
uint8_t y[16];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */

	TDOA_Init(&TDOA_S);
	
	HAL_TIM_Base_Start_IT(&htim1);
    
    HAL_ADC_Start_DMA(&hadc1,(uint32_t*)&a1,(uint32_t)4);
    MIT_init();
    
    HAL_Delay(200); 
    uint8_t A[]="x:";
    uint8_t B[]="y:";
	
	uint8_t x[]="--------------->";
	uint8_t y1[]="^";
    uint8_t y[]="|";
  
  OLED_Display_On();
  OLED_Clear(); 
  OLED_Init();
  OLED_ShowString(0,0,A,sizeof(A));
  OLED_ShowString(0,6,B,sizeof(B));
  
  OLED_ShowString(0,4,x,sizeof(x));
  OLED_ShowString(10*6,0,y1,sizeof(y1));
  OLED_ShowString(10*6,1,y,sizeof(y));
  OLED_ShowString(10*6,2,y,sizeof(y));
  OLED_ShowString(10*6,3,y,sizeof(y));
  OLED_ShowString(10*6,4,y,sizeof(y));
  OLED_ShowString(10*6,5,y,sizeof(y));
  OLED_ShowString(10*6,6,y,sizeof(y));
  
  
  HAL_Delay(500);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
					  
	if(MIT1_S.mit_flag&&MIT2_S.mit_flag&&MIT3_S.mit_flag)
		{
			TDOA_Get_distanceANDangle(&TDOA_S);
				if(fabs(TDOA_S.x)<100&&fabs(TDOA_S.y)<100)
				{
					sprintf((char*)x,"%.2f",TDOA_S.x);
					sprintf((char*)y,"%.2f",TDOA_S.y);
				
					OLED_ShowString(10*8,0,x,16);
					OLED_ShowString(10*8,6,y,16);
					
				}
						HAL_Delay(500);
						T_count=0;
						MIT1_S.mit_flag=0;
						MIT2_S.mit_flag=0;
						MIT3_S.mit_flag=0;
						MIT4_S.mit_flag=0;
		} 
//		printf("a1:%d\r\n",a1[0]);
//		printf("a2:%d\r\n",a1[1]);
//		printf("a3:%d\r\n",a1[2]);
//		printf("a4:%d\r\n",a1[3]);
		
		//HAL_Delay(100);
	  
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
//void HAL_GPIO_EXTI_Callback( uint16_t GPIO_pin)
//	{
//		if(GPIO_pin==KEY0_Pin)
//		{
//			
//			key++;
//			if(key>1)
//			{
//				key=0;
//			}
//		}
//	
//	}
	
	
	void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	{
		if(htim->Instance == TIM1)
		{
          if(a1[0]>3500&&MIT1_S.mit_flag==0){
             
             MIT1_S.mit_count= T_count;
			MIT1_S.mit_flag=1;
             }
          else if(a1[1]>3500&&MIT2_S.mit_flag==0){
            
             MIT2_S.mit_count= T_count;
			 MIT2_S.mit_flag=1;
            }
          else if(a1[2]>3500&&MIT3_S.mit_flag==0){

             MIT3_S.mit_count= T_count;
			             MIT3_S.mit_flag=1;
            }
		  else if(a1[3]>3500&&MIT4_S.mit_flag==0){
            
             MIT4_S.mit_count= T_count;
			 MIT4_S.mit_flag=1;
            }
          
		    T_count++;
		}
}
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
  while (1)
  {
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
