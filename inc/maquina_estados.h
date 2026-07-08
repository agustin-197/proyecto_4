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
*********************************************************************************************************************/

#ifndef MAQUINA_ESTADOS_H_
#define MAQUINA_ESTADOS_H_

/** @file maquina_estados.h
 ** @brief Declaraciones de la máquina de estados del reloj
 ** @details Expone las funciones públicas para inicializar y actualizar
 ** la lógica principal de la interfaz de usuario del reloj digital.
 **/

/* === Headers files inclusions
 * =================================================== */

#include "placa.h"
#include "reloj.h"

/* === Header for C++ compatibility
 * =============================================== */

#ifdef __cplusplus
extern "C" {
#endif

/* === Public macros definitions
 * ================================================== */

/* === Public data type declarations
 * ============================================== */

/* === Public variable declarations
 * =============================================== */

/* === Public function declarations
 * =============================================== */

/**
 * @brief Inicializa la máquina de estados
 * @details Guarda las referencias al hardware y al modelo del reloj,
 * y configura el estado inicial del sistema.
 * @param b Puntero a la estructura de la placa inicializada
 * @param r Puntero a la instancia del reloj inicializado
 */
void MaquinaEstados_Init(board_t b, clock_t r);

/**
 * @brief Tarea periódica de la máquina de estados
 * @details Esta función debe ser llamada desde la rutina de servicio
 * de interrupción del SysTick (típicamente cada 1 ms) para gestionar
 * los tiempos del sistema, el multiplexado y los retardos.
 */
void MaquinaEstados_Tick(void);

/**
 * @brief Bucle principal de evaluación de la máquina de estados
 * @details Evalúa las entradas del usuario (botones), ejecuta las
 * transiciones de estado correspondientes y actualiza la memoria
 * de la pantalla. Debe llamarse continuamente dentro de un while(1).
 */
void MaquinaEstados_Update(void);

/* === End of conditional blocks
 * ================================================== */

#ifdef __cplusplus
}
#endif

#endif /* MAQUINA_ESTADOS_H_ */
