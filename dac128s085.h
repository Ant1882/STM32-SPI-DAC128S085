/*
 * dac128s085.h
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
 *  -------- Example usage --------
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

#ifndef INC_DAC128S085_H_
#define INC_DAC128S085_H_

#include <stdint.h>

typedef enum
{
  E_DAC_OK,
  E_DAC_PARAM_ERROR,
  E_DAC_SPI_ERROR
} E_DAC_STATUS;

// b1010 - Update output of selected channel
#define UPDATE_SEL  0xA000

// Channel output enables
#define CH_A_EN  (1 << 0)
#define CH_B_EN  (1 << 1)
#define CH_C_EN  (1 << 2)
#define CH_D_EN  (1 << 3)
#define CH_E_EN  (1 << 4)
#define CH_F_EN  (1 << 5)
#define CH_G_EN  (1 << 6)
#define CH_H_EN  (1 << 7)

/*
 * Update the data register of a single DAC channel
 *      channel  - Which channel register to update (0-7)
 *      dacValue - Value to update register with (0-4095)
 */
E_DAC_STATUS DAC128S085_SetChannelReg(uint16_t channel, uint16_t dacValue);

/*
 * Enable or disable DAC outputs according to a given bit-field
 *      channelBits - Bit-field for channel enables, see example above
 */
E_DAC_STATUS DAC128S085_SetChannelOutputs(uint16_t channelBits);

/*
 * Directly set the output of a given DAC channel
 *      channel  - Which channel register to update (0-7)
 *      dacValue - Value to update register with (0-4095)
 */
E_DAC_STATUS DAC128S085_DirectOutput(uint16_t channel, uint16_t dacValue);

/*
 * Output a short pulse directly on each DAC channel sequentially
 */
void DAC128S085_TEST_DirectOutput();

#endif /* INC_DAC128S085_H_ */
