/*
 * dac128s085.c
 *
 *  Created on: 2 Mar 2022
 *      Author: Anthony Marshall, AW Embedded
 *
 *  Driver for TI 8-Channel 12-bit DAC (DAC128S085)
 *    - Assumes power-on default of WRM mode (register write doesn't enable output)
 *    - Assumes SPI setup using STM32CubeMX:
 *      - Motorola, 16-bits, MSB first
 *      - Clock Polarity (CPOL) - Low
 *      - Clock Phase (CPHA) - 1 Edge
 *      - NSS Mode (Chip Select) - Enabled, Signal Type - Hardware
 *
 *   -------- Example usage --------
 *
 *  // Set channel A(0) register to maximum value and enable output
 *  E_DAC_STATUS dacStatus = DAC128S085_DirectOutput(0, 4095);
 *
 *  // Set channel H(7) register to half rail, no output enabled
 *  E_DAC_STATUS dacStatus = DAC128S085_SetChannelReg(7, 2048);
 *
 *  // Enable outputs for channels A, B, C
 *  uint16_t chBits = (UPDATE_SEL | CH_A_EN | CH_B_EN | CH_C_EN);
 *  E_DAC_STATUS dacStatus = DAC128S085_SetChannelOutputs(chBits);
 */

#include "dac128s085.h"
#include "stm32l4xx_hal.h"
#include "gpio.h"
#include "spi.h"

// HAL SPI Specific
#define SPI_TIMEOUT_MS  10
#define SPI_TX_SIZE     1  // 1 * 16-bit SPI transfer

// DAC mode commands [15:12]
#define WRM_MODE    0x8000  // b1000 - Write to registers only mode
#define WRT_MODE    0x9000  // b1011 - Write through to output mode

// DAC special write commands [15:12]
#define CHA_WRITE   0xB000  // b1011 - Write through for chA, other outputs updated from registers
#define BROADCAST   0xC000  // b1100 - Write through value to all outputs

// Command formatting
#define DAC_CH_MASK     0x7
#define DAC_CH_SHIFT    12  // DAC number in Bits [14:12] of 16-bit command
#define DAC_CH_MAX      8

#define DAC_VAL_MASK    0xFFF  // Lowest 12 bits of the 16-bit command
#define DAC_VAL_MAX     4095


// Update the data register of a single DAC channel
E_DAC_STATUS DAC128S085_SetChannelReg(uint16_t channel, uint16_t dacValue)
{
    if((channel >= DAC_CH_MAX) | (dacValue > DAC_VAL_MAX))
        return E_DAC_PARAM_ERROR;

    uint16_t temp = 0x0000;
    HAL_StatusTypeDef status = HAL_OK;

    // Update channel register
    temp = ((channel & DAC_CH_MASK) << DAC_CH_SHIFT) | (dacValue & DAC_VAL_MASK);
    status = HAL_SPI_Transmit(&hspi3, (uint8_t*)&temp, SPI_TX_SIZE, SPI_TIMEOUT_MS);

    if(status != HAL_OK)
        return E_DAC_SPI_ERROR;

    return E_DAC_OK;
}

// Enable or disable DAC outputs according to a given bit-field
E_DAC_STATUS DAC128S085_SetChannelOutputs(uint16_t channelBits)
{
    uint16_t temp = 0x0000;
    HAL_StatusTypeDef status = HAL_OK;

    // Enable outputs according to bit-field
    temp = channelBits;
    status = HAL_SPI_Transmit(&hspi3, (uint8_t*)&temp, SPI_TX_SIZE, SPI_TIMEOUT_MS);

    if(status != HAL_OK)
        return E_DAC_SPI_ERROR;

    return E_DAC_OK;
}

// Directly set the output of a given DAC channel
E_DAC_STATUS DAC128S085_DirectOutput(uint16_t channel, uint16_t dacValue)
{
    if((channel >= DAC_CH_MAX) | (dacValue > DAC_VAL_MAX))
        return E_DAC_PARAM_ERROR;

    E_DAC_STATUS dacStatus = E_DAC_OK;

    dacStatus = DAC128S085_SetChannelReg(channel, dacValue);

    if(dacStatus != E_DAC_OK)
        return dacStatus;

    // Only enable the one selected output channel
    uint16_t chBits = (UPDATE_SEL | (1 << channel));
    dacStatus = DAC128S085_SetChannelOutputs(chBits);

    return dacStatus;
}

// Output a short pulse directly on each DAC channel sequentially
void DAC128S085_TEST_DirectOutput()
{
    for (uint16_t channel = 0; channel < DAC_CH_MAX; ++channel)
    {
        DAC128S085_DirectOutput(channel, 4095);
        HAL_Delay(50);
        DAC128S085_DirectOutput(channel, 0);
    }
}
