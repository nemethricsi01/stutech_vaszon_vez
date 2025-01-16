/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright ( c ) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ssd1306.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MENU_PRESSED 0x1
#define FEL_PRESSED 0x2
#define LE_PRESSED 0x4

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim17;

/* USER CODE BEGIN PV */
int actgomb[ 4 ];
int lastgomb[ 4 ];
int prelltime = 10;
int prelltimer;
char buffer[ 40 ];
char buffer2[ 40 ];
uint8_t flags;
int menutime;
int delaycount = 0;
uint8_t menucount;
uint8_t disptime;
int menukilepes = 20000;
int temp[ 4 ];
uint8_t state = 0;
uint32_t vaszonfeldelay,vaszonledelay,motorfeldelay,motorledelay,motorle1mp = 0,motorfel1mp = 0;
uint8_t laststate;
uint8_t ledone = 1,feldone = 1;
int lecounter,felcounter,feltime,letime;
uint8_t villtimer;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config( void );
static void MX_GPIO_Init( void );
static void MX_TIM17_Init( void );
static void MX_I2C1_Init( void );
/* USER CODE BEGIN PFP */
void readgomb( void );
int eeprom_read( uint8_t id );
void eeprom_write( int data,int addr );
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void readgomb( void )
					{
						actgomb[ 0 ] = HAL_GPIO_ReadPin( fel_GPIO_Port, fel_Pin );
						actgomb[ 1 ] = HAL_GPIO_ReadPin( le_GPIO_Port, le_Pin );
						actgomb[ 2 ] = HAL_GPIO_ReadPin( menu_GPIO_Port, menu_Pin );
					}
int eeprom_read( uint8_t id )
{
	int eepromvalue;
	switch ( id )
	{
		case 0:
			eepromvalue = *(__IO uint16_t *)(0x8003C02);
		break;
		case 1:
			eepromvalue = *(__IO uint16_t *)(0x8003C02 + 2);
		break;
		case 2:
			eepromvalue = *(__IO uint16_t *)(0x8003C02 + 4);
		break;
		case 3:
			eepromvalue = *(__IO uint16_t *)(0x8003C02 + 6);
		break;
	}
	if( eepromvalue > 300)
	{
		eepromvalue = 300;
	}
	return eepromvalue;
}
void eeprom_write( int data,int addr )
{

			
	switch ( addr )
	{
		case 0:
			HAL_FLASH_Unlock();
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,0x8003C02,data);
			HAL_FLASH_Lock();
		break;
		case 1:
			HAL_FLASH_Unlock();
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,0x8003C02+2,data);
			HAL_FLASH_Lock();
		break;
		case 2:
			HAL_FLASH_Unlock();
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,0x8003C02+4,data);
			HAL_FLASH_Lock();
		break;
		case 3:
			HAL_FLASH_Unlock();
			HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,0x8003C02+6,data);
			HAL_FLASH_Lock();
		break;
	}
	
	
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main( void )
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init( );

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config( );

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init( );
  MX_TIM17_Init( );
  MX_I2C1_Init( );
  /* USER CODE BEGIN 2 */
	readgomb( );
	lastgomb[ 0 ] = actgomb[ 0 ];
	lastgomb[ 1 ] = actgomb[ 1 ];
	lastgomb[ 2 ] = actgomb[ 2 ];
	
	temp[ 0 ] = eeprom_read( 0 );
	temp[ 1 ] = eeprom_read( 1 );
	temp[ 2 ] = eeprom_read( 2 );
	temp[ 3 ] = eeprom_read( 3 );
//temp[0] = 0;
//temp[1] = 0;
//temp[2] = 0;
//temp[3] = 0;
		
	ssd1306_Init( );
	ssd1306_SetCursor( 0,0 );
	ssd1306_WriteString( "hello",Font_7x10,White );
	ssd1306_UpdateScreen( );
	HAL_TIM_Base_Start_IT( &htim17 );
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while ( 1 )
    {
			if( ( disptime > 100 ) && ( prelltimer == 0 ) )
			{
				switch ( menucount )
				{
					case 0:
						if( state == 0 )
						{
							switch( laststate )
							{
								case 0:
									sprintf( buffer,"1 egyseg =    " );
									sprintf( buffer2,"0.5 mp       ");
									HAL_GPIO_WritePin(le_led_GPIO_Port,le_led_Pin,GPIO_PIN_RESET);
									HAL_GPIO_WritePin(fel_led_GPIO_Port,fel_led_Pin,GPIO_PIN_RESET);
									break;
								case 1:
									sprintf( buffer,"leengedve    " );
									sprintf( buffer2,"M:%d V:%d         ", temp[ 0 ], temp[ 1 ] );
									HAL_GPIO_WritePin(le_led_GPIO_Port,le_led_Pin,GPIO_PIN_RESET);
									HAL_GPIO_WritePin(fel_led_GPIO_Port,fel_led_Pin,GPIO_PIN_RESET);
									break;
								case 2:
									sprintf( buffer,"felhuzva    " );
									sprintf( buffer2,"M:%d V:%d         ", temp[ 2 ], temp[ 3 ] );
									HAL_GPIO_WritePin(le_led_GPIO_Port,le_led_Pin,GPIO_PIN_RESET);
									HAL_GPIO_WritePin(fel_led_GPIO_Port,fel_led_Pin,GPIO_PIN_RESET);
									break;
								case 3:
									sprintf( buffer,"le stop    " );
									sprintf( buffer2,"M:%d V:%d         ", motorledelay/500, vaszonledelay/500 );
									HAL_GPIO_WritePin(le_led_GPIO_Port,le_led_Pin,GPIO_PIN_SET);
									HAL_GPIO_WritePin(fel_led_GPIO_Port,fel_led_Pin,GPIO_PIN_RESET);
									break;
								case 4:
									sprintf( buffer,"fel stop    " );
									sprintf( buffer2,"M:%d V:%d         ", motorfeldelay/500, vaszonfeldelay/500 );
									HAL_GPIO_WritePin(le_led_GPIO_Port,le_led_Pin,GPIO_PIN_RESET);
									HAL_GPIO_WritePin(fel_led_GPIO_Port,fel_led_Pin,GPIO_PIN_SET);
									break;
							}
						}
						if( state == 1 )
						{
							villtimer++;
							if( villtimer > 4 )
							{
								HAL_GPIO_TogglePin(le_led_GPIO_Port,le_led_Pin);
								HAL_GPIO_WritePin(fel_led_GPIO_Port,fel_led_Pin,GPIO_PIN_RESET);
								villtimer = 0;
							}
							sprintf( buffer,"leenged      " );
							sprintf( buffer2,"M:%d V:%d        ", ( motorledelay + motorle1mp )/500, vaszonledelay/500 );
						}
						if( state == 2 )
						{
							villtimer++;
							if( villtimer > 4 )
							{
								HAL_GPIO_TogglePin(fel_led_GPIO_Port,fel_led_Pin);
								HAL_GPIO_WritePin(le_led_GPIO_Port,le_led_Pin,GPIO_PIN_RESET);
								villtimer = 0;
							}
							sprintf( buffer,"felhuz        " );
							sprintf( buffer2,"M:%d V:%d        ", ( motorfeldelay + motorfel1mp )/500, vaszonfeldelay/500 );
						}
					break;
					case 1:
						sprintf( buffer,"Motor le:     " );
						sprintf( buffer2,"%d x0.5mp       ", temp[ 0 ] );
					break;
					case 2:
						sprintf( buffer,"Vaszon le:    " );
						sprintf( buffer2,"%d x0.5mp       ", temp[ 1 ] );
					break;
					case 3:
						sprintf( buffer,"Motor fel:  " );
						sprintf( buffer2,"%d x0.5mp       ", temp[ 2 ] );
					break;
					case 4:
						sprintf( buffer,"Vaszon fel: " );
						sprintf( buffer2,"%d x0.5mp       ", temp[ 3 ] );
					break;
				}
				ssd1306_SetCursor( 0, 0 );
				ssd1306_WriteString( buffer,Font_11x18,White );
				ssd1306_SetCursor( 0, 17 );
				ssd1306_WriteString( buffer2,Font_11x18,White );
				ssd1306_UpdateScreen( );
			}
			
			
			readgomb( );
			if( ( lastgomb[ 0 ] != actgomb[ 0 ] ) ||
				  ( lastgomb[ 1 ] != actgomb[ 1 ] ) ||
				  ( lastgomb[ 2 ] != actgomb[ 2 ] ) )
						{
								prelltimer = prelltime;
								while( prelltimer > 0 )
												{
														readgomb( );
														if( lastgomb[ 0 ] != actgomb[ 0 ] )
															{
																	prelltimer = prelltime;
																	lastgomb[ 0 ] = actgomb[ 0 ];
															}
														if( lastgomb[ 1 ] != actgomb[ 1 ] )
															{
																	prelltimer = prelltime;
																	lastgomb[ 1 ] = actgomb[ 1 ];
															}
														if( lastgomb[ 2 ] != actgomb[ 2 ] )
															{
																	prelltimer = prelltime;
																	lastgomb[ 2 ] = actgomb[ 2 ];
															}
												}
								if( lastgomb[ 0 ] == 0 )//gomb1 FEL
								{
									flags |= FEL_PRESSED;
									feltime = 500;
									if( menucount == 0 )
										{
											if( state == 0 )
												{
													motorledelay = temp[ 0 ] * 500;
													vaszonledelay = temp[ 1 ] * 500;
													state = 2;
												}
											if( state != 2 )
												{
													state = 0;
													ledone = 1;
													laststate = 3;
												}
											if( ( state == 2 ) && ( feldone == 1 ) )
												{
													feldone = 0;
													laststate = 2;
													motorfel1mp = 1000;
													if( motorfeldelay == 0 )
														{
															motorfeldelay = temp[ 2 ] * 500;
														}
													if( vaszonfeldelay == 0 )
														{
															vaszonfeldelay = temp[ 3 ] * 500;
														}
												}									
										}
									if ( menucount != 0 )
									{
										menukilepes = 20000;
									}
									switch ( menucount )
									{
										case 1:
											temp[ 0 ] ++;
										if( temp[ 0 ] > 300 )
											{
												temp[ 0 ] = 0;
											}
										break;
										case 2:
											temp[ 1 ] ++;
										if( temp[ 1 ] > 300 )
											{
												temp[ 1 ] = 0;
											}
										break;
										case 3:
											temp[ 2 ] ++;
										if( temp[ 2 ] > 300 )
											{
												temp[ 2 ] = 0;
											}
										break;
										case 4:
											temp[ 3 ] ++;
										if( temp[ 3 ] > 300 )
											{
												temp[ 3 ] = 0;
											}
										break;
										default:
											break;
									}
								}
								else
									{
										flags &= ~FEL_PRESSED;
									}
								if( lastgomb[ 1 ] == 0 )//gomb2 LEFELE
									{
										flags |= LE_PRESSED;
										letime = 500;
										if( menucount == 0 )
											{
												if ( state == 0 )
													{
														state = 1;
														motorfeldelay = temp[ 2 ] * 500;
														vaszonfeldelay = temp[ 3 ] * 500;
													}
												if(  state != 1   )
													{
														state = 0;
														laststate = 4;
														feldone= 1;
													}
												if( ( state == 1 ) && ( ledone == 1 ) )
													{
														ledone = 0;
														laststate = 1;
														motorle1mp = 1000;
														if( motorledelay == 0 )
															{
																motorledelay = temp[ 0 ] * 500;
																
															}
														if( vaszonledelay == 0 )
															{
																vaszonledelay = temp[ 1 ] * 500;
																
															}
													}
											 }
										if ( menucount != 0 )
											{
												menukilepes = 20000;
											}
										switch ( menucount )
											{
												case 1:
													temp[ 0 ] --;
													if ( temp[ 0 ] < 0 )
														{
															temp[ 0 ] = 300;
														}
												break;
												case 2:
													temp[ 1 ] --;
													if ( temp[ 1 ] < 0 )
														{
															temp[ 1 ] = 300;
														}
												break;
												case 3:
													temp[ 2 ] --;
													if ( temp[ 2 ] < 0 )
														{
															temp[ 2 ] = 300;
														}
												break;
												case 4:
													temp[ 3 ] --;
													if ( temp[ 3 ] < 0 )
														{
															temp[ 3 ] = 300;
														}
												break;
												default:
													break;
											}
									}
									else
									{
										flags &= ~LE_PRESSED;
									}
								if( lastgomb[ 2 ] == 0 )//gomb3
									{
										if ( menucount != 0 )
											{
												menukilepes = 20000;
											}
											flags |= MENU_PRESSED;
									}
								else
									{
										flags &= ~MENU_PRESSED;
									}
	      }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config( void )
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if ( HAL_RCC_OscConfig( &RCC_OscInitStruct ) != HAL_OK )
  {
    Error_Handler( );
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if ( HAL_RCC_ClockConfig( &RCC_ClkInitStruct, FLASH_LATENCY_1 ) != HAL_OK )
  {
    Error_Handler( );
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_I2C1;
  PeriphClkInit.I2c1ClockSelection = RCC_I2C1CLKSOURCE_HSI;
  if ( HAL_RCCEx_PeriphCLKConfig( &PeriphClkInit ) != HAL_OK )
  {
    Error_Handler( );
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init( void )
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x0000020B;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if ( HAL_I2C_Init( &hi2c1 ) != HAL_OK )
  {
    Error_Handler( );
  }
  /** Configure Analogue filter
  */
  if ( HAL_I2CEx_ConfigAnalogFilter( &hi2c1, I2C_ANALOGFILTER_ENABLE ) != HAL_OK )
  {
    Error_Handler( );
  }
  /** Configure Digital filter
  */
  if ( HAL_I2CEx_ConfigDigitalFilter( &hi2c1, 0 ) != HAL_OK )
  {
    Error_Handler( );
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM17 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM17_Init( void )
{

  /* USER CODE BEGIN TIM17_Init 0 */

  /* USER CODE END TIM17_Init 0 */

  /* USER CODE BEGIN TIM17_Init 1 */

  /* USER CODE END TIM17_Init 1 */
  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 10;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 4469;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if ( HAL_TIM_Base_Init( &htim17 ) != HAL_OK )
  {
    Error_Handler( );
  }
  /* USER CODE BEGIN TIM17_Init 2 */

  /* USER CODE END TIM17_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init( void )
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE( );
  __HAL_RCC_GPIOA_CLK_ENABLE( );
  __HAL_RCC_GPIOB_CLK_ENABLE( );

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin( GPIOA, fel_led_Pin|le_led_Pin|vaszon_le_Pin|vaszon_fel_Pin
                          |motor_le_Pin, GPIO_PIN_RESET );

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin( motor_fel_GPIO_Port, motor_fel_Pin, GPIO_PIN_RESET );

  /*Configure GPIO pins : fel_led_Pin le_led_Pin vaszon_le_Pin vaszon_fel_Pin
                           motor_le_Pin */
  GPIO_InitStruct.Pin = fel_led_Pin|le_led_Pin|vaszon_le_Pin|vaszon_fel_Pin
                          |motor_le_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

  /*Configure GPIO pins : fel_Pin le_Pin menu_Pin */
  GPIO_InitStruct.Pin = fel_Pin|le_Pin|menu_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init( GPIOA, &GPIO_InitStruct );

  /*Configure GPIO pin : motor_fel_Pin */
  GPIO_InitStruct.Pin = motor_fel_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init( motor_fel_GPIO_Port, &GPIO_InitStruct );

}

/* USER CODE BEGIN 4 */
void HAL_TIM_PeriodElapsedCallback( TIM_HandleTypeDef *htim )
					{
						if( flags & LE_PRESSED )
						{
							letime--;
							if( letime <= 1 )
							{
								letime = 1;
								lecounter++;
								if( lecounter > 50 )
								{
									if ( menucount != 0 )
									{
										menukilepes = 20000;
									}
									switch ( menucount )
											{
												case 1:
													temp[ 0 ] --;
													if ( temp[ 0 ] < 0 )
														{
															temp[ 0 ] = 300;
														}
												break;
												case 2:
													temp[ 1 ] --;
													if ( temp[ 1 ] < 0 )
														{
															temp[ 1 ] = 300;
														}
												break;
												case 3:
													temp[ 2 ] --;
													if ( temp[ 2 ] < 0 )
														{
															temp[ 2 ] = 300;
														}
												break;
												case 4:
													temp[ 3 ] --;
													if ( temp[ 3 ] < 0 )
														{
															temp[ 3 ] = 300;
														}
												break;
												default:
													break;
											}
											lecounter = 0;
								}
							}
						}
						if( flags & FEL_PRESSED )
						{
							feltime--;
							if( feltime <=1 )
							{
								feltime = 1;
								felcounter++;
								if( felcounter > 50 )
								{
									if ( menucount != 0 )
									{
										menukilepes = 20000;
									}
									switch ( menucount )
									{
										case 1:
											temp[ 0 ] ++;
										if( temp[ 0 ] > 300 )
											{
												temp[ 0 ] = 0;
											}
										break;
										case 2:
											temp[ 1 ] ++;
										if( temp[ 1 ] > 300 )
											{
												temp[ 1 ] = 0;
											}
										break;
										case 3:
											temp[ 2 ] ++;
										if( temp[ 2 ] > 300 )
											{
												temp[ 2 ] = 0;
											}
										break;
										case 4:
											temp[ 3 ] ++;
										if( temp[ 3 ] > 300 )
											{
												temp[ 3 ] = 0;
											}
										break;
										default:
											break;
									}
									felcounter = 0;
								}
							}
						}
						disptime++;
						switch ( state )
						{
							case 0:
							HAL_GPIO_WritePin(motor_fel_GPIO_Port,motor_fel_Pin,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(motor_le_GPIO_Port,motor_le_Pin,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(vaszon_fel_GPIO_Port,vaszon_fel_Pin,GPIO_PIN_RESET);
							HAL_GPIO_WritePin(vaszon_le_GPIO_Port,vaszon_le_Pin,GPIO_PIN_RESET);
							break;
							
							case 1:
							if( motorle1mp > 0 )
							{
								motorle1mp--;
							}
							else if( motorle1mp == 0 )
							{
								if( vaszonledelay > 0 )
								{
									HAL_GPIO_WritePin(vaszon_le_GPIO_Port,vaszon_le_Pin,GPIO_PIN_SET);
									vaszonledelay--;
								}
								else
								{
									HAL_GPIO_WritePin(vaszon_le_GPIO_Port,vaszon_le_Pin,GPIO_PIN_RESET);
								}
							}
								if( motorledelay > 0 )
								{
									HAL_GPIO_WritePin(motor_le_GPIO_Port,motor_le_Pin,GPIO_PIN_SET);
									motorledelay--;
									
								}
								else
								{
									HAL_GPIO_WritePin(motor_le_GPIO_Port,motor_le_Pin,GPIO_PIN_RESET);
								}
								if( (vaszonledelay == 0) && (motorledelay == 0))
								{
									state = 0;
									ledone = 1;
								}
							
							break;
							
							case 2:
								if( motorfel1mp > 0 )
									{
										motorfel1mp--;
									}
								if( motorfel1mp == 0 )
									{
										if( vaszonfeldelay > 0 )
										{
											HAL_GPIO_WritePin( vaszon_fel_GPIO_Port,vaszon_fel_Pin,GPIO_PIN_SET );
											vaszonfeldelay--;
										}
										else
										{
											HAL_GPIO_WritePin( vaszon_fel_GPIO_Port,vaszon_fel_Pin,GPIO_PIN_RESET );
										}
									}
									if( motorfeldelay > 0 )
										{
											HAL_GPIO_WritePin( motor_fel_GPIO_Port,motor_fel_Pin,GPIO_PIN_SET );
											motorfeldelay--;
										}
										else
										{
											HAL_GPIO_WritePin( motor_fel_GPIO_Port,motor_fel_Pin,GPIO_PIN_RESET );
										}
									if( ( vaszonfeldelay == 0 ) && ( motorfeldelay == 0 ) )
										{
											state = 0;
											feldone = 1;
										}
							
							break;
							
							case 3:
							break;
							
							case 4:
							break;
						}
						if( menucount != 0 )
						{
							if( menukilepes > 0 )
							{
								menukilepes--;
							}
							if ( menukilepes == 0 )
							{
								uint32_t error;
									HAL_FLASH_Unlock();
									FLASH_EraseInitTypeDef EraseInitStruct;
									EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
									EraseInitStruct.NbPages = 1;
									EraseInitStruct.PageAddress = 0x8003C00; //Specify sector number!
									HAL_FLASHEx_Erase(&EraseInitStruct,&error);
									HAL_FLASH_Lock();
									
										eeprom_write( temp[ 0 ], 0 );
										eeprom_write( temp[ 1 ], 1 );
										eeprom_write( temp[ 2 ], 2 );
										eeprom_write( temp[ 3 ], 3 );
								menucount = 0;
							}
						}
						if( prelltimer > 0 )
						{
							prelltimer--;
						}
						if( flags & MENU_PRESSED )
						{
							++menutime;
							if( menutime >2000 )
							{
								state = 0;
								motorledelay = 0;
								vaszonledelay = 0;
								motorfeldelay = 0;
								vaszonfeldelay = 0;
								feldone = 1;
								ledone = 1;
								laststate = 0;
								menutime = 0;
								HAL_GPIO_WritePin(le_led_GPIO_Port,le_led_Pin,GPIO_PIN_RESET);
								HAL_GPIO_WritePin(fel_led_GPIO_Port,fel_led_Pin,GPIO_PIN_RESET);
								if( ( menucount == 1 ) || ( menucount == 2 ) || ( menucount == 3 ) || ( menucount == 4 ) )
								{
									uint32_t error;
									HAL_FLASH_Unlock();
									FLASH_EraseInitTypeDef EraseInitStruct;
									EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
									EraseInitStruct.NbPages = 1;
									EraseInitStruct.PageAddress = 0x8003C00; //Specify sector number!
									HAL_FLASHEx_Erase(&EraseInitStruct,&error);
									HAL_FLASH_Lock();
									
										eeprom_write( temp[ 0 ], 0 );
										eeprom_write( temp[ 1 ], 1 );
										eeprom_write( temp[ 2 ], 2 );
										eeprom_write( temp[ 3 ], 3 );
									
								}
								menucount++;
								if ( menucount != 0 )
											{
												menukilepes = 20000;
											}
								
								if( menucount > 4 )
								{
									menucount = 0;
								}									
							}
						}
						else
						{
							menutime = 0;
						}
					
					}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler( void )
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */

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
void assert_failed( uint8_t *file, uint32_t line )
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf( "Wrong parameters value: file %s on line %d\r\n", file, line ) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ ( C ) COPYRIGHT STMicroelectronics *****END OF FILE****/
