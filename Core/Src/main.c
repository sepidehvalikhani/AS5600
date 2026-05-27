/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "i2c.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define AS5600_ADDR 0x36<<1   // HAL uses 8-bit address
#define AD5600_status    0x0B 
#define AS5600_RAW_ANGLE 0x0C 

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t status=0,AGC=0; 
uint16_t angle,Magnitude,P;
float r;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t AS5600_ReadRawAngle(void)
{
//   uint8_t high, low;
//	  HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDR, 0x0C, I2C_MEMADD_SIZE_8BIT, &high, 1, 100);
//   HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDR, 0x0D, I2C_MEMADD_SIZE_8BIT, &low, 1, 100);
//	 angle= (high << 8) | low;
    uint8_t buf[2];
    HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDR, 0x0C, I2C_MEMADD_SIZE_8BIT, buf, 2, 100);
    angle = ((uint16_t)buf[0]<<8) | buf[1];
    return angle;
}

uint8_t  AS5600_status(void)
{  
	 uint8_t s;
   HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDR, 0x0B, I2C_MEMADD_SIZE_8BIT, &s, 1, 100);
   status=s;
    return status;
}

uint8_t  AS5600_AGC(void)
{  
	 uint8_t s;
   HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDR, 0x1A, I2C_MEMADD_SIZE_8BIT, &s, 1, 100);
   AGC=s;
    return AGC;
}

uint16_t AS5600_GetMagnitude(void)
{
		uint8_t buf[2];
    HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDR, 0x1B, I2C_MEMADD_SIZE_8BIT, buf, 2, 100);
    Magnitude = ((uint16_t)buf[0]<<8) | buf[1];
    return Magnitude ;
}
/***************************** Angle Programming I2C********************/
void Write_ZPOS(void){
	AS5600_ReadRawAngle();
	uint8_t buf[2];
	buf[0] = angle >> 8;      // High byte
  buf[1] = angle & 0xFF;
	
	HAL_I2C_Mem_Write(&hi2c1,AS5600_ADDR,0x01,I2C_MEMADD_SIZE_8BIT,buf,2,100);	
	HAL_Delay(10);	
}

void Write_MPOS(void){
	AS5600_ReadRawAngle();
	uint8_t buf[2];
	buf[0] = angle >> 8;      // High byte
  buf[1] = angle & 0xFF;
	
  HAL_I2C_Mem_Write(&hi2c1,AS5600_ADDR,0x03,I2C_MEMADD_SIZE_8BIT,buf,2,100);
	HAL_Delay(10);
	
} 

void AS5600_BurnAngle(void) {
    uint8_t cmd = 0x80;
    HAL_I2C_Mem_Write(&hi2c1, AS5600_ADDR, 0xFF, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    HAL_Delay(10);
}

void verify_OTP(void){
uint8_t seq1 = 0x01;
HAL_I2C_Mem_Write(&hi2c1, 0x36<<1, 0xFF, I2C_MEMADD_SIZE_8BIT, &seq1, 1, 100);

uint8_t seq2 = 0x11;
HAL_I2C_Mem_Write(&hi2c1, 0x36<<1, 0xFF, I2C_MEMADD_SIZE_8BIT, &seq2, 1, 100);

uint8_t seq3 = 0x10;
HAL_I2C_Mem_Write(&hi2c1, 0x36<<1, 0xFF, I2C_MEMADD_SIZE_8BIT, &seq3, 1, 100);
}

/****Reading ZPOS & MPOS****/
uint16_t Read_Position(uint8_t reg){
	  uint8_t buf[2];
    HAL_I2C_Mem_Read(&hi2c1, AS5600_ADDR, (uint16_t)&reg, I2C_MEMADD_SIZE_8BIT, buf, 2, 100);
    P= ((uint16_t)buf[0]<<8) | buf[1];
    return P;	
}
/********************************************************************************************/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

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
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		AS5600_status();
		AS5600_AGC();
		AS5600_GetMagnitude();
		r=(360.0f*AS5600_ReadRawAngle())/4095.0f;
		HAL_Delay(50);
		
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
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
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
