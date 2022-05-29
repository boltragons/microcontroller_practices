#include <TPM_Select.h>
#if TPM_PROJECT == 3

#include "MKL25Z4.h"
#include <TPM_KL25Z.h>

/* INITIALIZES THE LEDS 01 AND 02 */
void LEDS_Init(void);

/* STARTS THE MODULE 0 OF TPM WITH 2s TO BLINK LED 01 */
void TPM0_Init(void);

/* STARTS THE MODULE 1 OF TPM WITH 8s TO BLINK LED 02 */
void TPM1_Init(void);

int main(void) {
	LEDS_Init();
	TPM0_Init();
	TPM1_Init();
	while(1);
}

void TPM0_IRQHandler(void) {
	// CLEAR INTERRUPT FLAG
	TPM0->SC |= TPM_INTERRUPT_FLAG;

	// BLINK THE TWO LEDS
	LED_01_GPIO->PTOR |= LED_01_MASK;
}

void TPM1_IRQHandler(void) {
	// CLEAR INTERRUPT FLAG
	TPM1->SC |= TPM_INTERRUPT_FLAG;

	LED_02_GPIO->PTOR |= LED_02_MASK;
}

void LEDS_Init(void) {
	SIM->SCGC5 |= LED_01_CLOCK | LED_02_CLOCK;

	LED_01_PORT->PCR[LED_01] |= PORT_MUX_GPIO;
	LED_02_PORT->PCR[LED_02] |= PORT_MUX_GPIO;

	LED_01_GPIO->PDDR |= LED_01_MASK;
	LED_02_GPIO->PDDR |= LED_02_MASK;
}

void TPM0_Init(void) {
	// ENABLE MCGIRCLK.
	MCG->C1 |= MCG_MCGIRCLK_ENABLE;

	// SELECT 32,768KHz.
	SIM->SOPT2 |= CLOCK_MCGIRCLK;

	// ENABLE CLOCK FOR TPM0
	SIM->SCGC6 |= TPM0_CLOCK;

	TPM0->SC |= TPM_INTERRUPT_ENABLE | TPM_PRESCALER_32;

	// MOD = 2s * (32,768KHz/32) = 2048
	TPM0->MOD = 2047;

	// ENABLE INTERRUPTS FOR TMP0 IN NVIC
	NVIC->ISER[0] |= NVIC_ENABLE_TPM0;

	// ENABLE TPM TO COUNT
	TPM0->SC |= TPM_ENABLE_COUNTER;
}

void TPM1_Init(void) {
	// ENABLE MCGIRCLK.
	MCG->C1 |= MCG_MCGIRCLK_ENABLE;

	// SELECT 32,768KHz.
	SIM->SOPT2 |= CLOCK_MCGIRCLK;

	// ENABLE CLOCK FOR TPM0
	SIM->SCGC6 |= TPM1_CLOCK;

	TPM1->SC |= TPM_INTERRUPT_ENABLE | TPM_PRESCALER_32;

	// MOD = 8s * (32,768KHz/32) = 8192
	// TPM0->MOD = 8191;
	TPM1->MOD = 8191;

	// ENABLE INTERRUPTS FOR TMP0 IN NVIC
	NVIC->ISER[0] |= NVIC_ENABLE_TPM1;

	// ENABLE TPM TO COUNT
	TPM1->SC |= TPM_ENABLE_COUNTER;
}

#endif
