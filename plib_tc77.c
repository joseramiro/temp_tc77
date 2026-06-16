/**
 * @file plib_tc77.c
 * @brief Pilote pour le TC77 (capteur de température)
 * @author Ramiro Najera
 * @version 1.0.4
 * @date 2025-04-30
 * @copyright Copyright (c) 2025
 */

#include <string.h>
#include "plib_tc77.h"

// Static functions

static void TC77_StartTranmission(SPI_t *spi);
static void TC77_EndTranmission(SPI_t *spi);
static void TC77_Read_Byte(SPI_t *spi, uint8_t* data);
static void TC77_Read_Buffer(SPI_t *spi, uint8_t* data, uint16_t size);

// Public API

void TC77_ReadTemperatureReg(SPI_t *spi, uint8_t* readData)
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
    uint8_t rawTemperature[2];
    union IntUsCharUnion convertedTemperature;

    // Read raw register, right shift unused bits (b2-b0)
    TC77_ReadTemperatureReg(&obj->spi, rawTemperature);
    convertedTemperature.IntValue = (CONCAT(rawTemperature[0], rawTemperature[1]) >> 3);

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

// Static functions

static void TC77_StartTranmission(SPI_t *spi)
{
    if(spi->pinEN.Clear)
        spi->pinEN.Clear();
    if(spi->pinCS.Clear) 
        spi->pinCS.Clear();
}

static void TC77_EndTranmission(SPI_t *spi)
{
    if(spi->pinCS.Set)
        spi->pinCS.Set();
    if(spi->pinEN.Set)
        spi->pinEN.Set();
}

static void TC77_Read_Byte(SPI_t *spi, uint8_t* data)
{
    if(spi->Read)
        spi->Read(data, 1);
}

static void TC77_Read_Buffer(SPI_t *spi, uint8_t* data, uint16_t size)
{
    for(uint8_t i = 0; i < size; i++)
    {
        TC77_Read_Byte(spi, &data[i]);
    } 
}