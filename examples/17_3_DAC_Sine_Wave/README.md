## DAC_Sine_Wave example<a name="brief"></a>

### 1 Brief
The function of this code is to press the KEY0 button and the PA4 pin outputs the sine wave 1. Press the WKUP button, pin PA4 outputs sine wave 2. The serial port displays the DAC conversion value, the voltage value, and the voltage value of the ADC.
### 2 Hardware Hookup
The hardware resources used in this experiment are:
+ LED - PA8
+ USART1 - PA9\PA10
+ ADC1 - Channel1(PA1)
+ DAC1 - Channel1(PA4)
+ TIM1
+ ALIENTEK DS100 oscilloscope

The DAC used in this experiment is an on-chip resource of STM32H503, so there is no corresponding connection schematic.

### 3 STM32CubeIDE Configuration


We copy the **04_UART** project and name both the project and the.ioc file **17_3_DAC_Sine_Wave**.Next we start the DAC configuration by double-clicking the **17_3_DAC_Sine_Wave.ioc** file.

In **Pinout&Configuration**, click **PA4** pin to set mode.

![](../../1_docs/3_figures/17_3_DAC_Sine_Wave/dac4.png)

Then open **Analog->DAC1** to configure.

![](../../1_docs/3_figures/17_3_DAC_Sine_Wave/dac8.png)

The configuration in **Analog->ADC1** is as follows:

![](../../1_docs/3_figures/17_3_DAC_Sine_Wave/dac9.png)

Then configure TIM1 as shown.

![](../../1_docs/3_figures/17_3_DAC_Sine_Wave/dac10.png)

Click **File > Save**, and you will be asked to generate code.Click **Yes**.

Among them, ADC and TIM files have been introduced in the previous chapters, and the added content is basically the same. You can open the source code to check it by yourself. The logic code added for this experiment is described below.

##### code
###### dac.c
```c#
 void dac_creat_sin_buf(uint16_t maxval, uint16_t samples)
{
    uint8_t i;
    float inc = (2 * 3.1415962) / samples;      /* Increment of computation (DAC SIN BUF points per cycle) */
    float outdata = 0;

    for (i = 0; i < samples; i++)
    {
        outdata = maxval * (1 + sin(inc * i));  /* The value of each point with a period of dots points is calculated,
                                                 * enlarged by a factor of maxval, and offset to the positive number region. */
        if (outdata > 4095)
            outdata = 4095;                     /* Upper limit limit */
        g_dac_sin_buf[i] = outdata;
    }
}

/**
 * @brief   DAC DMA enable waveform output
 * @note    TIM1's input clock frequency (f) is from APB2, f = 250Mhz.
 *          DAC trigger frequency ftrgo = f/((psc + 1) * (arr + 1))
 *          Waveform frequency = ftrgo/ndtr;
 *
 * @param   ndtr : Amount of data transferred in a single DMA channel
 * @param   arr  : automatic reload value for TIM1
 * @param   psc  : the frequency division coefficient of TIM1
 * @retval  None
 */
void dac_dma_wave_enable(uint16_t ndtr, uint16_t arr, uint16_t psc)
{
    __HAL_TIM_SET_PRESCALER(&htim1, psc);
    __HAL_TIM_SET_AUTORELOAD(&htim1, arr);
    HAL_TIM_Base_Start(&htim1);              /* Start timer 1 */

    HAL_DAC_Stop_DMA(&hdac1, DAC_CHANNEL_1); /* Stop the previous transmission first */
    HAL_DAC_Start_DMA(&hdac1, DAC_CHANNEL_1, (uint32_t *)g_dac_sin_buf, ndtr, DAC_ALIGN_12B_R);
}
```
The function ``dac_creat_sin_buf`` is used to generate the sine wave sequence, which is stored in the **g_dac_sin_buf** array for DAC conversion.
The function ``dac_dma_wave_enable`` is used to configure the TIM1 overflow frequency, so the TIM1 overflow frequency determines the DAC output sine wave frequency and also enables DAC DMA transfer.

##### main.c
Here's the main function.
```c#
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint16_t adcx;
  float temp;
  uint8_t t = 0;
  uint8_t key;
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
  MX_GPDMA1_Init();
  MX_USART1_UART_Init();
  MX_DAC1_Init();
  MX_ICACHE_Init();
  MX_TIM1_Init();

  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
  stm32h503cb_show_mesg();
  dac_creat_sin_buf(2048, 100);
  dac_dma_wave_enable(100, 10 - 1, 250 - 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    t++;
    key = key_scan(0);                               /* Scan key */

    if (key == KEY0_PRES)                            /* High sampling rate, about 5Khz waveform */
    {
        printf("DAC Wave1 \r\n");
        dac_creat_sin_buf(2048, 100);                /* Generate a sequence of sine wave functions */
        dac_dma_wave_enable(100, 20 - 1, 25 - 1);    /* 5Khz trigger frequency, 100 points to obtain a sine wave of up to 5KHz. */
    }
    else if (key == WKUP_PRES)                       /* Low sampling rate, about 1Khz waveform */
    {
        printf("DAC Wave2 \r\n");
        dac_creat_sin_buf(2048, 10);                 /* Generate a sequence of sine wave functions */
        dac_dma_wave_enable(10, 20 - 1, 25 - 1);     /* With 50Khz trigger frequency and 10 points, a sine wave up to 50KHz can be obtained. */
    }

    adcx = DAC1->DHR12R1;                            /* Get the output status of DAC1_OUT1 */
    printf("adcx:%d\n", adcx);                       /* Display DAC register values */

    temp = (float)adcx * (3.3 / 4096);               /* The DAC voltage value is obtained */
    printf("temp:%f\n", temp);                       /* Display voltage value */

    adcx = adc_get_result_average(20);               /* The conversion result of ADC channel 1 is obtained */
    temp = (float)adcx * (3.3 / 65536);              /* Get the ADC voltage value (adc is 16bit) */
    printf("temp:%f\n", temp);                       /* Display the voltage value acquired by the ADC */

    if (t == 10)
    {
        LED_TOGGLE();
        t = 0;
    }
    
    HAL_Delay(10);                                   /* delay 10ms */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}
```
When this part of the code is initialized, a set of sine wave data is generated to control the DAC channel 1 to output the specified sine wave. Then, according to the scanned keys, the sine waves with different frequencies are output.


### 4 Running
#### 4.1 Compile & Download
After the compilation is complete, connect the DAP and the Mini Board, and then connect to the computer together to download the program to the Mini Board.
#### 4.2 Phenomenon
Press the reset button to restart the Mini Board, observe the LED flashing on the Mini Board, open the serial port and the host computer ATK-XCOM can see the prompt information of the experiment, indicating that the code download is successful. Connect the **PA4** pin with a **ALIENTEK DS100 oscilloscope** and press the KEY0 and WKUP keys respectively. You can see two different types of sine waves, as shown in the following figure.

![](../../1_docs/3_figures/17_3_DAC_Sine_Wave/dac12.png)

![](../../1_docs/3_figures/17_3_DAC_Sine_Wave/dac11.png)

[jump to title](#brief)