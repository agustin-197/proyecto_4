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

#ifndef RELOJ_H_
#define RELOJ_H_

/** @file reloj.h
 ** @brief Definiciones y declaraciones para la biblioteca de gestión de reloj y
 * alarma
 **/

/* === Headers files inclusions
 * ====================================================================================
 */
#include <stdbool.h>
#include <stdint.h>

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
 * @brief Puntero opaco a la estructura principal del reloj.
 */
typedef struct clock_s *clock_t;

/**
 * @brief Arreglo para almacenar la hora en formato BCD sin compactar.
 * * Posiciones del arreglo:
 * [0] Decena de hora
 * [1] Unidad de hora
 * [2] Decena de minuto
 * [3] Unidad de minuto
 * [4] Decena de segundo
 * [5] Unidad de segundo
 */
typedef uint8_t hora_t[6];

/* === Public variable declarations
 * ================================================================================
 */

/* === Public function declarations
 * ================================================================================
 */

/**
 * @brief Crea e inicializa una nueva instancia del reloj.
 * * @param ticks_per_second Cantidad de llamadas a RelojNewTick que equivalen a
 * un segundo.
 * @param alarm_handler Puntero a la función de callback que se ejecutará al
 * sonar la alarma.
 * @return clock_t Puntero a la instancia del reloj creado.
 */
clock_t RelojCreate(unsigned int ticks_per_second, void *alarm_handler);

/**
 * @brief Obtiene la hora actual del reloj.
 * * @param clock Puntero a la instancia del reloj.
 * @param current_time Arreglo donde se copiará la hora actual.
 * @retval true Si la hora devuelta es válida (fue ajustada previamente).
 * @retval false Si la hora devuelta es inválida (reloj no configurado).
 */
bool RelojGetCurrentTime(clock_t clock, hora_t current_time);

/**
 * @brief Configura y ajusta la hora actual del reloj.
 * * @param clock Puntero a la instancia del reloj.
 * @param current_time Arreglo con la nueva hora a configurar en formato BCD.
 * @retval true Si la hora proporcionada tiene un formato válido y se configuró
 * con éxito.
 * @retval false Si la hora proporcionada no cumple el formato BCD (ej. horas >
 * 23, minutos > 59).
 */
bool RelojSetupCurrentTime(clock_t clock, const hora_t current_time);

/**
 * @brief Incrementa el contador interno del reloj.
 * * Debe ser llamada periódicamente. Al alcanzar los ticks por segundo
 * configurados, incrementa la hora y verifica si debe dispararse la alarma.
 * * @param clock Puntero a la instancia del reloj.
 */
void RelojNewTick(clock_t clock);

/**
 * @brief Configura la hora de la alarma.
 * * @param clock Puntero a la instancia del reloj.
 * @param alarm_time Arreglo con la hora de la alarma a configurar.
 */
void RelojSetupAlarm(clock_t clock, const hora_t alarm_time);

/**
 * @brief Obtiene la hora configurada para la alarma.
 * * @param clock Puntero a la instancia del reloj.
 * @param alarm_time Arreglo donde se copiará la hora de la alarma actual.
 */
void RelojGetAlarm(clock_t clock, hora_t alarm_time);

/**
 * @brief Habilita o deshabilita la alarma del reloj.
 * * @param clock Puntero a la instancia del reloj.
 * @param state true para habilitar la alarma, false para deshabilitarla.
 */
void RelojSetAlarmState(clock_t clock, bool state);

/**
 * @brief Consulta el estado actual de la alarma.
 * * @param clock Puntero a la instancia del reloj.
 * @retval true Si la alarma está habilitada.
 * @retval false Si la alarma está deshabilitada.
 */
bool RelojGetAlarmState(clock_t clock);

/**
 * @brief Pospone la alarma una cantidad específica de minutos.
 * * @param clock Puntero a la instancia del reloj.
 * @param delay_minutes Cantidad de minutos a posponer.
 */
void RelojPostponeAlarm(clock_t clock, unsigned int delay_minutes);

/* === End of conditional blocks
 * ===================================================================================
 */

#ifdef __cplusplus
}
#endif

#endif /* RELOJ_H_ */
