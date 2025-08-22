/**
 * @file plib_tc77.c
 * @brief Pilote pour le TC77 (capteur de température)
 * @author Ramiro Najera
 * @version 1.0.3
 * @date 2025-04-30
 * @copyright Copyright (c) 2025
 */

#include <string.h>
#include "plib_tc77.h"
#include "plib_tc77_spi.h"

void TC77_StartTranmission(SPI_t *spi)
{
    if(spi->en.clear != NULL)
        spi->en.clear();
    if(spi->cs.clear != NULL) 
        spi->cs.clear();
}

void TC77_EndTranmission(SPI_t *spi)
{
    if(spi->cs.set != NULL)
        spi->cs.set();
    if(spi->en.set != NULL)
        spi->en.set();
}

// Basic functions
void TC77_ReadTemperatureReg(SPI_t *spi, unsigned char* readData)
{
    // Start tranmission
    TC77_StartTranmission(spi);
    // Read register 
    TC77_Read_Buffer(spi, readData, TC77_TEMP_REG_SIZE);
    // Stop tranmission
    TC77_EndTranmission(spi);
}

void TC77_UpdateTemperature(TC77_t* obj)
{
    unsigned char rawTemperature[2];
    union IntUsCharUnion convertedTemperature;

    // Read raw register, right shift unused bits (b2-b0)
    TC77_ReadTemperatureReg(&obj->spi, rawTemperature);
    convertedTemperature.IntValue = (CONCAT_BYTES(rawTemperature[0], rawTemperature[1]) >> 3);

    // Sign-extend 13-bit value (bit 12 is the sign bit)
    if (convertedTemperature.IntValue & TC77_SIGNESS_MASK)  // If negative
    {
        convertedTemperature.IntValue |= TC77_TEMPERATURE_MASK;  // Sign-extend to 16 bits
    }

    // Update temperature vlaue in float
    obj->temperature = convertedTemperature.IntValue * TC77_TEMP_COEFF;
}

float TC77_GetTemperature(TC77_t* obj)
{
    return obj->temperature;
}