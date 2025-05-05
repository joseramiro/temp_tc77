/**
 * @file plib_tc77_spi.c
 * @brief Pilote SPI pour le TC77 (capteur de température)
 * @author Ramiro Najera
 * @version 1.0.1
 * @date 2025-04-24
 * @copyright Copyright (c) 2025
 */

#include "plib_tc77_spi.h"
#include "peripheral/spi/spi_master/plib_spi2_master.h"
#include "peripheral/spi/spi_master/plib_spi1_master.h"

void TC77_Read_Byte(SPI_t *spi, unsigned char* data)
{
    if(spi->channel == SPI_CH1)
        SPI1_Read(data, 1);
    else if(spi->channel == SPI_CH2)
        SPI2_Read(data, 1);
}

void TC77_Read_Buffer(SPI_t *spi, unsigned char* data, unsigned int size)
{
    unsigned char i;
    
    for(i = 0; i < size; i++)
    {
        TC77_Read_Byte(spi, &data[i]);
    } 
}