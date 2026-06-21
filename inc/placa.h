/*********************************************************************************************************************
Copyright 2016-2025, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología
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

#ifndef PLACA_H_
#define PLACA_H_

/** @file placa.h
 ** @brief Declaraciones de la capa de abstracción de la placa
 ** @details Define la estructura de datos que contiene todos los recursos funcionales
 ** de hardware de la placa (Zumbador, Teclas y Pantalla) y declara la función de
 ** inicialización.
 **/

/* === Headers files inclusions ==================================================================================== */

#include "digital.h"
#include "screen.h"

/* === Header for C++ compatibility ================================================================================ */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions =================================================================================== */

/* === Public data type declarations =============================================================================== */

/**
 * @brief Estructura que agrupa todos los recursos de hardware del poncho
 * @details Esta estructura contiene los punteros a los objetos abstractos
 * de la placa. El tipo \c board_t se define como un puntero constante a 
 * esta estructura para protegerla de modificaciones accidentales.
 */
typedef struct board_s {
    digital_output_t buzzer;      /**< Salida digital asignada al zumbador (o LED azul) */
    
    digital_input_t tecla_cancel; /**< Entrada digital asignada a la Tecla Cancelar */
    digital_input_t tecla_accept; /**< Entrada digital asignada a la Tecla Aceptar */
    
    digital_input_t tecla_f1;     /**< Entrada digital asignada a la Tecla F1 */
    digital_input_t tecla_f2;     /**< Entrada digital asignada a la Tecla F2 */
    digital_input_t tecla_f3;     /**< Entrada digital asignada a la Tecla F3 */
    digital_input_t tecla_f4;     /**< Entrada digital asignada a la Tecla F4 */
    
    display_t display;            /**< Descriptor de la pantalla de 7 segmentos */
} const * board_t;

/* === Public variable declarations ================================================================================ */

/* === Public function declarations ================================================================================ */

/**
 * @brief Inicializa toda la placa y sus periféricos
 * @details Configura los puertos GPIO, la multiplexación de pines y crea
 * internamente las instancias para todas las entradas y salidas de la placa.
 * @return board_t Puntero a la estructura con los recursos funcionales listos para usar
 */
board_t BoardCreate(void);

/* === End of conditional blocks =================================================================================== */

#ifdef __cplusplus
}
#endif

#endif /* PLACA_H_ */