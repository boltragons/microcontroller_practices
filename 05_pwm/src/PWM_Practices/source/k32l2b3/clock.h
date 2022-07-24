#ifndef CLOCK_H
#define CLOCK_H

#include <k32l2b3/port.h>

#define CLK_SOURCE_LIRCLK_2MHz_FREQ		2000000L
#define CLK_SOURCE_LIRCLK_8MHz_FREQ		8000000L
#define CLK_SOURCE_HIRCLK_FREQ			48000000L
#define CLK_SOURCE_OSCERCLK_FREQ		32768L

typedef enum {
	clkPORT_A = 9,
	clkPORT_B = 10,
	clkPORT_C = 11,
	clkPORT_D = 12,
	clkPORT_E = 13,
	clkTPM_0 = 56,
	clkTPM_1 = 57,
	clkTPM_2 = 58,
	clkADC_0 = 59
} clock_peripheral;

typedef enum {
	clkSOURCE_LIRCLK_2MHz,
	clkSOURCE_LIRCLK_8MHz,
	clkSOURCE_HIRCLK,
	clkSOURCE_OSCERCLK
} clock_source;

extern void clkEnablePeripheralClock(clock_peripheral peripheral);

/*!
 * @brief Enable the given clock in the microprocessor. The options are:
 *
 * 		  * The Low-frequency Internal Reference Clock (LIRC), with the
 * 		    informed frequency: 2 MHz or 8 MHz.
 *
 * 		  * The High-frequency Internal Reference Clock (HIRC), with 48 MHz.
 *
 * 		  * The Oscillator External Reference Clock (OSCERCLK), with 32.768 KHz.
 */
extern void clkEnableClock(clock_source clock_source);

extern void clkSetMainClock(clock_source clock_source);

extern uint32_t clkGetMainClock(void);

extern uint32_t clkGetERClock(void);

extern uint32_t clkGetIRClock(void);

#endif
