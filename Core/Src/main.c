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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx.h"
#include "bsp_debug_usart.h"
#include "bsp_led.h"
#include <stdio.h>
#include "bsp_ili9341_lcd.h"
#include "HX711.h"
#include "bsp_spi_flash.h"

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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void LCD_Test(void);
static void Delay ( __IO uint32_t nCount );
static uint32_t value;
static uint32_t i=0;
static char dispBuff[100];
extern uint32_t maopi;
extern uint32_t weight_zheng;
extern uint8_t weight_xiao;

void Printf_Charater(void)   ;

extern uint16_t lcdid;
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
    LED_GPIO_Config();
    DEBUG_USART_Config();
    ILI9341_Init ();
    printf("\r\n ********** 液晶屏中文显示程序（任意大小??????*********** \r\n");
    printf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n");
    if(lcdid == LCDID_ILI9341)
    {
        ILI9341_GramScan ( 6 );
    }
    else if(lcdid == LCDID_ST7789V)
    {
        ILI9341_GramScan ( 0 );
    }

    Printf_Charater();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

      LCD_Test();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
/*用于测试各种液晶的函?????*/
void LCD_Test(void)
{
    /*演示显示变量*/
    static uint8_t testCNT = 0;
    char dispBuff[100];

    testCNT++;

    LCD_SetFont(&Font8x16);
    LCD_SetColors(RED,BLACK);

    ILI9341_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全????? */
    /********显示字符串示?????*******/
    ILI9341_DispStringLine_EN_CH(LINE(0),"野火YH");
    //显示指定大小的字?????
    ILI9341_DisplayStringEx(0,1*24,24,24,(uint8_t *)"野火YH",0);
    ILI9341_DisplayStringEx(2*48,0*48,48,48,(uint8_t *)"野火YH",0);

    /********显示变量示例*******/
    LCD_SetTextColor(GREEN);

    /*使用c标准库把变量转化成字符串*/
    sprintf(dispBuff,"显示变量????? %d ",testCNT);
    LCD_ClearLine(LINE(5));	/* 清除单行文字 */

    /*然后显示该字符串即可，其它变量也是这样处?????*/
    ILI9341_DispStringLine_EN_CH(LINE(5),dispBuff);

    /*******显示图形示例******/
    /* 画直????? */

    LCD_ClearLine(LINE(7));/* 清除单行文字 */
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN_CH(LINE(7),"画直线：");

    LCD_SetTextColor(RED);
    ILI9341_DrawLine(50,170,210,230);
    ILI9341_DrawLine(50,200,210,240);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawLine(100,170,200,230);
    ILI9341_DrawLine(200,200,220,240);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawLine(110,170,110,230);
    ILI9341_DrawLine(130,200,220,240);

    Delay(0xFFFFFF);

    ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全????? */


    /*画矩?????*/

    LCD_ClearLine(LINE(7));	/* 清除单行文字 */
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN_CH(LINE(7),"画矩形：");

    LCD_SetTextColor(RED);
    ILI9341_DrawRectangle(50,200,100,30,1);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawRectangle(160,200,20,40,0);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawRectangle(170,200,50,20,1);

    Delay(0xFFFFFF);

    ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全????? */

    /* 画圆 */
    LCD_ClearLine(LINE(7));	/* 清除单行文字 */
    LCD_SetTextColor(BLUE);

    ILI9341_DispStringLine_EN_CH(LINE(7),"画圆");

    LCD_SetTextColor(RED);
    ILI9341_DrawCircle(100,200,20,0);

    LCD_SetTextColor(GREEN);
    ILI9341_DrawCircle(100,200,10,1);

    LCD_SetTextColor(BLUE);
    ILI9341_DrawCircle(140,200,20,0);

    Delay(0xFFFFFF);

    ILI9341_Clear(0,16*8,LCD_X_LENGTH,LCD_Y_LENGTH-16*8);	/* 清屏，显示全????? */

}


/**
  * @brief  ?????单延时函?????
  * @param  nCount ：延时计数???
  * @retval ?????
  */
static void Delay ( __IO uint32_t nCount )
{
    for ( ; nCount != 0; nCount -- );

}



/*"?????"字符的字?????16x16 */
unsigned char charater_matrix[] =
        { /*"?????",0*/
                0x01,0x00,0x21,0x08,0x11,0x08,0x09,0x10,0x09,0x20,0x01,0x00,0x7F,0xF8,0x00,	0x08,
                0x00,0x08,0x00,0x08,0x3F,0xF8,0x00,0x08,0x00,0x08,0x00,0x08,0x7F,0xF8,0x00,0x08,

        };
void Printf_Charater(void)
{
    int i,j;
    unsigned char kk;

    /*i用作行计?????*/
    for ( i=0;i<16;i++)
    {
        /*j用作?????字节内数据的移位计数*/
        /*?????行像素的第一个字?????*/
        for(j=0; j<8; j++)
        {
            /*?????个数据位?????个数据位地处?????*/
            kk = charater_matrix[2*i] << j ;  //左移J?????
            if( kk & 0x80)
            {
                printf("*"); //如果?????高位?????1，输?????*号，表示笔迹
            }
            else
            {
                printf(" "); //如果?????高位?????0，输出空格，表示空白
            }
        }
        /*?????行像素的第二个字?????*/
        for(j=0; j<8; j++)
        {
            kk = charater_matrix[2*i+1] << j ;  //左移J?????

            if( kk & 0x80)
            {
                printf("*"); //如果?????高位?????1，输?????*号，表示笔迹
            }
            else
            {
                printf(" "); //如果?????高位?????0，输出空格，表示空白
            }
        }
        printf("\n");    //输出完一行像素，换行
    }
    printf("\n\n"); 		//?????个字输出完毕
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
