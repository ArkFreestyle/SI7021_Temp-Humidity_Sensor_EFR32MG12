#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "si7013.h"
//#include "si7021.h"
#include "i2cspm.h"
#include "stdio.h"
#include "retargetserial.h"

#include "hal-config.h"
//Define ports
#define sensorEnable_port gpioPortB
#define sensorSCL_port gpioPortC
#define sensorSDA_port gpioPortC

//Define pins
#define sensorEnable_pin 10
#define sensorSCL_pin 10
#define sensorSDA_pin 11

static void performMeasurements(I2C_TypeDef *i2c, uint32_t *rhData, int32_t *tData)
{
  Si7013_MeasureRHAndTemp(i2c, SI7021_ADDR, rhData, tData);
}

int main(void)
{

	I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;
	CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;
	bool			 detected;
	uint32_t         rhData = 0;
	uint32_t        tempData = 0;

/* Chip errata */
  CHIP_Init();
  RETARGET_SerialInit();
  //CMU_ClockEnable(cmuClock_GPIO, true);
  //Print wali pin high
  GPIO_PinModeSet(gpioPortA, 5, gpioModePushPull, 1);
  /* Initalize hardware */

  CMU_HFXOInit(&hfxoInit);

  //initialize i2c
  I2CSPM_Init(&i2cInit);

  //Enable on-board clock, i2c clock
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_I2C0, true);

  //Set temperature sensor enable pin high
  GPIO_PinModeSet(sensorEnable_port, sensorEnable_pin, gpioModePushPull, 1);

  //initialize sensor
  //SI7021_init();

  //Is the sensor alive?
  detected = Si7013_Detect(i2cInit.port, SI7021_ADDR, 0);
  printf("detected = %d",detected);
  printf("\n");


  /* Infinite loop */
  while (1) {
	  performMeasurements(i2cInit.port, &rhData, &tempData);
	  //int a = tempData;
	  printf("%d",tempData);
	  printf("   ");
	  printf("%d",rhData);
	  printf("\n");
	  for(int i=0;i<1000000;i++);

	  //printf("Hello World");

  }
}
