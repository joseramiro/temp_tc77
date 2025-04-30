/**
 * @file plib_tc77.c
 * @brief Pilote pour le TC77 (capteur de température)
 * @author Ramiro Najera
 * @version 1.0.0
 * @date 2025-04-30
 * @copyright Copyright (c) 2025
 */

#include <string.h>
#include "plib_tc77.h"
#include "plib_tc77_spi.h"

void TC77_StartTranmission(SPI_t *spi)
{
    spi->cs.clear();
}

void TC77_EndTranmission(SPI_t *spi)
{
    spi->cs.set();
}

// Basic functions
void TC77_ReadTemperatureReg(SPI_t *spi, unsigned char* readData)
{
    // Start tranmission
    MAX6627_StartTranmission(spi);
    // Read register 
    MAX6627_Read_Buffer(spi, readData, TC77_TEMP_REG_SIZE);
    // Stop tranmission
    MAX6627_EndTranmission(spi);
}

void TC77_UpdateTemperature(TC77_t* obj)
{
    unsigned char rawTemperature[2];
    union IntUsCharUnion convertedTemperature;

    // Read raw register
    TC77_ReadTemperatureReg(&obj->spi, rawTemperature);
    memcpy(&convertedTemperature.IntValue, &rawTemperature, 2);

    // Right shift (bit 2-bit 0 from register unused)
    convertedTemperature.IntValue = (convertedTemperature.IntValue >> 3);

    // Sign-extend 13-bit value (bit 12 is the sign bit)
    if (convertedTemperature.IntValue & TC77_SIGNESS_MASK)  // If negative
    {
        //rawData.IntValue |= 0xFFFFE000;  // Sign-extend to 32 bits
        convertedTemperature.IntValue |= TC77_TEMPERATURE_MASK;  // Sign-extend to 16 bits
    }

    // Update temperature vlaue in float
    obj->temperature = convertedTemperature.IntValue * TC77_TEMP_COEFF;
}

float TC77_GetTemperature(TC77_t* obj)
{
    return obj->temperature;
}