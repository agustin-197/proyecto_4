/*********************************************************************************************************************
Copyright 2016-2025, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología
Universidad Nacional de Tucuman
http://www.microprocesadores.unt.edu.ar/

Copyright 2016-2025, Esteban Volentini <evolentini@herrera.unt.edu.ar>

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

#ifndef PLACA_H_
#define PLACA_H_

/** @file bsp.h
 ** @brief Declaraciones de la capa de abstracción de la placa (Board Support
 * Package)
 ** @details Define la estructura de datos que contiene todos los recursos
 * funcionales
 ** de hardware de la placa (LEDs y Teclas) y declara la función de
 * inicialización.
 **/

/* === Headers files inclusions
 * ================================================================ */

#include "digital.h"

/* === Header for C++ compatibility
 * ============================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public data type declarations
 * =========================================================== */

/**
 * @brief Estructura que agrupa todos los recursos de hardware de la placa
 * EDU-CIAA
 * @details Esta estructura contiene los punteros a los objetos de las salidas
 * (LEDs) y entradas (Teclas) abstractas. El tipo \c board_t se define como
 * un puntero constante a esta estructura para protegerla de modificaciones
 * accidentales.
 */
typedef struct board_s {
    digital_output_t
        led_rojo; /**< Salida digital asignada al LED Rojo del RGB */
    digital_output_t
        led_verde; /**< Salida digital asignada al LED Verde del RGB */
    digital_output_t
        led_azul; /**< Salida digital asignada al LED Azul del RGB */
    digital_output_t
        led_1; /**< Salida digital asignada al LED 1 (rojo individual) */
    digital_output_t
        led_2; /**< Salida digital asignada al LED 2 (amarillo individual) */
    digital_output_t
        led_3; /**< Salida digital asignada al LED 3 (verde individual) */

    digital_input_t tecla_1; /**< Entrada digital asignada a la Tecla 1 */
    digital_input_t tecla_2; /**< Entrada digital asignada a la Tecla 2 */
    digital_input_t tecla_3; /**< Entrada digital asignada a la Tecla 3 */
    digital_input_t tecla_4; /**< Entrada digital asignada a la Tecla 4 */

} const *board_t;

/* === Public function declarations
 * ============================================================ */

/**
 * @brief Inicializa toda la placa y sus periféricos
 * @details Configura los puertos GPIO, la multiplexación de pines y crea
 * internamente las instancias para todas las entradas y salidas de la placa.
 * @return board_t Puntero a la estructura con los recursos funcionales de la
 * placa listos para usar
 */
board_t BoardCreate(void);

/* === End of conditional blocks
 * =============================================================== */

#ifdef __cplusplus
}
#endif

#endif /* PLACA_H_ */
