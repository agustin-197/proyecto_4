/*********************************************************************************************************************
Copyright 2016-2025, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnologia
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2026, Gerardo Agustín Díaz <agustin041097@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

#ifndef DIGITAL_H_
#define DIGITAL_H_

/** @file digital.h
 ** @brief Declaraciones de la biblioteca para gestion de entradas y salidas
 * digitales
 ** @details Define los tipos de datos abstractos y las funciones públicas
 ** necesarias para interactuar con los pines GPIO configurados como entradas o
 * salidas.
 **/

/* === Headers files inclusions
 * ====================================================================================
 */

#include <stdint.h>
#include <stdbool.h>

/* === Header for C++ compatibility
 * ================================================================================
 */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions
 * ===================================================================================
 */

/* === Public data type declarations
 * ===============================================================================
 */

/**
 * @brief Puntero al tipo de dato abstracto que representa una Salida Digital
 */
typedef struct digital_output_s *digital_output_t;

/**
 * @brief Puntero al tipo de dato abstracto que representa una Entrada Digital
 */
typedef struct digital_input_s *digital_input_t;

/* === Public variable declarations
 * ================================================================================
 */

/* === Public function declarations
 * ================================================================================
 */

/**
 * @brief Crea y configura una nueva Salida Digital
 * @param puerto Puerto GPIO al que pertenece el pin
 * @param terminal Número de pin dentro del puerto GPIO
 * @return digital_output_t Puntero al objeto creado o NULL en caso de error
 */
digital_output_t DigitalOutputCreate(uint32_t puerto, uint8_t terminal);

/**
 * @brief Activa (enciende) una Salida Digital
 * @param salida Puntero al objeto de la salida a activar
 */
void DigitalOutputActivate(digital_output_t salida);

/**
 * @brief Desactiva (apaga) una Salida Digital
 * @param salida Puntero al objeto de la salida a desactivar
 */
void DigitalOutputDeactivate(digital_output_t salida);

/**
 * @brief Invierte el estado actual de una Salida Digital
 * @param salida Puntero al objeto de la salida a invertir
 */
void DigitalOutputToggle(digital_output_t salida);

/**
 * @brief Consulta el estado actual de una Salida Digital
 * @param salida Puntero al objeto de la salida a consultar
 * @return true Si la salida está activada
 * @return false Si la salida está desactivada
 */
bool DigitalOutputGetState(digital_output_t salida);

/**
 * @brief Crea y configura una nueva Entrada Digital
 * @param puerto Puerto GPIO al que pertenece el pin
 * @param terminal Número de pin dentro del puerto GPIO
 * @param invertido true si la entrada es activa en bajo (lógica negada), false
 * si es activa en alto
 * @return digital_input_t Puntero al objeto creado o NULL en caso de error
 */
digital_input_t DigitalInputCreate(uint32_t puerto, uint8_t terminal,
                                   bool invertido);

/**
 * @brief Consulta el estado lógico actual de una Entrada Digital
 * @param entrada Puntero al objeto de la entrada a consultar
 * @return true Si la entrada está activada/presionada
 * @return false Si la entrada está inactiva/suelta
 */
bool DigitalInputGetState(digital_input_t entrada);

/**
 * @brief Detecta si hubo un cambio en el estado de la Entrada Digital
 * @param entrada Puntero al objeto de la entrada a consultar
 * @return true Si el estado cambió desde la última vez que se consultó
 * @return false Si el estado se mantiene igual
 */
bool DigitalInputHasChanged(digital_input_t entrada);

/**
 * @brief Detecta si la Entrada Digital acaba de ser presionada (Flanco de
 * activación)
 * @param entrada Puntero al objeto de la entrada a consultar
 * @return true Si pasó de inactiva a activa
 * @return false En cualquier otro caso
 */
bool DigitalInputHasActivated(digital_input_t entrada);

/**
 * @brief Detecta si la Entrada Digital acaba de ser soltada (Flanco de
 * desactivación)
 * @param entrada Puntero al objeto de la entrada a consultar
 * @return true Si pasó de activa a inactiva
 * @return false En cualquier otro caso
 */
bool DigitalInputHasDeactivated(digital_input_t entrada);

/* === End of conditional blocks
 * ===================================================================================
 */

#ifdef __cplusplus
}
#endif

#endif /* DIGITAL_H_ */
