# STM32-SPI-DAC128S085

Driver for TI 8-Channel 12-bit DAC (DAC128S085)
  - Assumes power-on default of WRM mode (register write doesn't enable output)
  - Assumes SPI setup using STM32CubeMX:
    - Motorola, 16-bits, MSB first
    - Clock Polarity (CPOL) - Low
    - Clock Phase (CPHA) - 1 Edge
    - NSS Mode (Chip Select) - Enabled, Signal Type - Hardware

-------- Example usage --------

Set channel A(0) register to maximum value and enable output
--------

E_DAC_STATUS dacStatus = DAC128S085_DirectOutput(0, 4095);

Set channel H(7) register to half rail, no output enabled
--------

E_DAC_STATUS dacStatus = DAC128S085_SetChannelReg(7, 2048);

Enable outputs for channels A, B, C
--------

uint16_t chBits = (UPDATE_SEL | CH_A_EN | CH_B_EN | CH_C_EN);

E_DAC_STATUS dacStatus = DAC128S085_SetChannelOutputs(chBits);
