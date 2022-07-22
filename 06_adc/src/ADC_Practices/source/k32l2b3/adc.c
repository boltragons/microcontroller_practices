#include "K32L2B31A.h"

#include "k32l2b3/adc.h"
#include "k32l2b3/clock.h"

#include <stdbool.h>

long adcReadInput(adc_channel channel) {
	while(!adcCalibrate());
	ADC0->SC1[0] = channel;
	while(!(ADC0->SC1[0] & (1U << 7)));
	return ADC0->R[0];
}

void adcGetDefaultConfig(adc_config_t *adc) {
	adc->clock_div = adcCLOCK_DIVIDE_BY_8;
	adc->long_sample_time = true;
	adc->resolution = adcRESOLUTION_16_BIT;
	adc->sample_number = adcSAMPLE_NUMBER_4;
	adc->hardware_trigger = false;
	adc->input_clock = adcCLOCK_BUS;
}

void adcInitModule(adc_config_t *adc) {
	clkEnablePeripheralClock(clkADC_0);

	if(adc->sample_number != -1) {
		ADC0->SC3 |= (1U << 2) | (adc->sample_number << 0);
	}

	ADC0->CFG1 |= (adc->clock_div << 5) | (adc->resolution << 2) | (adc->input_clock << 0);

	ADC0->SC2 = (adc->hardware_trigger) ?
			ADC0->SC2 | (1U << 6) :
			ADC0->SC2 & ~(1U << 6);

	ADC0->SC2 = (adc->long_sample_time) ?
			ADC0->SC2 | (1U << 4) :
			ADC0->SC2 & ~(1U << 4);
}

bool adcCalibrate(void) {
	uint16_t calib;

	ADC0->SC3 |= (1 << 7);

	while (ADC0->SC3 & (1 << 7));
	
	if (ADC0->SC3 & (1 << 6)) {
		return false;
	}

	calib = 0;
	calib += ADC0->CLPS + ADC0->CLP4 + ADC0->CLP3 + ADC0->CLP2 + ADC0->CLP1 + ADC0->CLP0;
	calib /= 2;
	calib |= (1U << 15);
	ADC0->PG = calib;

	calib = 0;
	calib += ADC0->CLMS + ADC0->CLM4 + ADC0->CLM3 + ADC0->CLM2 + ADC0->CLM1 + ADC0->CLM0;
	calib /= 2;
	calib |= (1U << 15);
	ADC0->MG = calib;

	return true;
}
