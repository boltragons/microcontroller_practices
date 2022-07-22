#include "K32L2B31A.h"

#include "k32l2b3/tpm.h"
#include "k32l2b3/clock.h"

// ================================================================================
// PRIVATE VARIABLES
// ================================================================================

static TPM_Type *tpm_modules[3] = {TPM0, TPM1, TPM2};

// ================================================================================
// PRIVATE FUNCTIONS PROTOTYPES
// ================================================================================

TPM_Type *tpmGetTpmModule(tpm_module module);

// ================================================================================
// PUBLIC FUNCTIONS
// ================================================================================

void tpmGetDefaultConfig(tpm_handler_t *tpm) {
	tpm->module = tpmMODULE_0;
	tpm->channel = tpmCHANNEL_0;
	tpm->clk = tpmCLOCK_MCGPCLK;
	tpm->ps = tpmPRESCALER_128;
	tpm->channel_mode = tpmCHANNEL_DISABLED;
}

void tpmInitModule(tpm_handler_t *tpm) {
	clkEnablePeripheralClock(clkTPM_0 + tpm->module);
	SIM->SOPT2 &= ~(~(tpm->clk) << 24);
	SIM->SOPT2 |= (tpm->clk << 24);

	TPM_Type *tpm_reg = tpmGetTpmModule(tpm->module);

	tpm_reg->SC = 0;
	tpm_reg->CONTROLS[tpm->channel].CnSC = (tpm->channel_mode << 2);
	tpm_reg->SC = (tpm->ps << 0);
}

void tpmSetModuleValue(tpm_handler_t *tpm, uint16_t module_value) {
	if(tpm->module > tpmMODULE_2 || tpm->module < tpmMODULE_0) {
		while(1);
	}
	tpmGetTpmModule(tpm->module)->MOD = module_value;
}

uint16_t tpmGetModuleValue(tpm_handler_t *tpm) {
	if(tpm->module > tpmMODULE_2 || tpm->module < tpmMODULE_0) {
		while(1);
	}
	return tpmGetTpmModule(tpm->module)->MOD;
}

void tpmSetChannelValue(tpm_handler_t *tpm, uint16_t channel_value) {
	if(tpm->module > tpmMODULE_2 || tpm->module < tpmMODULE_0 || tpm->channel > tpmCHANNEL_5 || tpm->channel < tpmCHANNEL_0) {
		while(1);
	}
	tpmGetTpmModule(tpm->module)->CONTROLS[tpm->channel].CnV = channel_value;
}

uint16_t tpmGetChannelValue(tpm_handler_t *tpm) {
	if(tpm->module > tpmMODULE_2 || tpm->module < tpmMODULE_0 || tpm->channel > tpmCHANNEL_5 || tpm->channel < tpmCHANNEL_0) {
		while(1);
	}
	return tpmGetTpmModule(tpm->module)->CONTROLS[tpm->channel].CnV;
}

void tpmStartTimer(tpm_handler_t *tpm) {
	tpmGetTpmModule(tpm->module)->SC |= (1U << 3);
}

void tpmStopTimer(tpm_handler_t *tpm) {
	tpmGetTpmModule(tpm->module)->SC &= ~(1U << 3);
}

void tpmStartModuleTimer(tpm_module module) {
	tpmGetTpmModule(module)->SC |= (1U << 3);
}

void tpmStopModuleTimer(tpm_module module) {
	tpmGetTpmModule(module)->SC &= ~(1U << 3);
}

// ================================================================================
// PRIVATE FUNCTIONS IMPLEMENTATION
// ================================================================================

TPM_Type *tpmGetTpmModule(tpm_module module) {
	if(module > tpmMODULE_2 || module < tpmMODULE_0) {
		while(1);
	}
	return tpm_modules[module];
}
