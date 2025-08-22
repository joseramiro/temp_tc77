#ifndef PLIB_TC77_H
#define PLIB_TC77_H

/**
 * @file plib_tc77.h
 * @brief Pilote pour le TC77 (capteur de température)
 * @author Ramiro Najera
 * @version 1.0.3
 * @date 2025-04-30
 * @copyright Copyright (c) 2025
 */

#include "libs/common_c_libs/plib_comm_struct.h"
#include "libs/common_c_libs/plib_data_struct.h"

/** @brief Taille de registre de température */
#define TC77_TEMP_REG_SIZE      2

/** @brief Conversion de bit en °C */
#define TC77_TEMP_COEFF      0.0625

/** @brief Masque du bit 12 contenant le flag de signé (1 négatif, 0 positif) */
#define TC77_SIGNESS_MASK        0x1000

/** @brief Masque pour obtenir 3 bytes des données de température */
#define TC77_TEMPERATURE_MASK    0xF000

/**
 * @struct MAX6627_t
 * @brief Configuration d'un capteur de température TC77
 */
typedef struct
{
    /** @brief Identifiant du module */
    unsigned char id;
    /** @brief Temperature du capteur */
    float temperature;
    /** @brief Configuration de la communication SPI */
    SPI_t spi;
}TC77_t;

/**
 * @brief Démarre une transmission SPI avec le module MAX6627.
 * @param spi Pointeur vers la configuration SPI utilisée pour la communication.
 */
void TC77_StartTranmission(SPI_t *spi);

/**
 * @brief Termine une transmission SPI avec le module MAX6627.
 * @param spi Pointeur vers la configuration SPI utilisée pour la communication.
 */
void TC77_EndTranmission(SPI_t *spi);

/**
 * @brief Lit la température du campteur MAX6627
 * @param spi Configuration de la communication SPI
 * @param readData Température lue
 */
void TC77_ReadTemperatureReg(SPI_t *spi, unsigned char* readData);

/**
 * @brief Lit la température du capteur et met à jour la variable de température
 * @param obj Module du capteur de température MAX6627
 */
void TC77_UpdateTemperature(TC77_t* obj);

/**
 * @brief Retourne la valeur de température
 * @param obj Module du capteur de temperature MAX6627
 * @return float Dernière valeur de température
 */
float TC77_GetTemperature(TC77_t* obj);

#endif  // PLIB_TC77_H