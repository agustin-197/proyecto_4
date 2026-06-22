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

/** @file digital.c
 ** @brief Implementacion de la biblioteca para gestion de entradas y salidas
 * digitales
 ** @details En este archivo se implementan las funciones y estructuras privadas
 ** necesarias para interactuar con los pines GPIO del microcontrolador.
 **/

/* === Headers files inclusions
 * ================================================================ */

#include "digital.h"
#include <stdlib.h>
#include "chip.h"

/* === Macros definitions
 * ====================================================================== */

/* === Private data type declarations
 * ========================================================== */

/**
 * @brief Estructura interna para almacenar el estado de una salida digital
 */
struct digital_output_s {
    uint32_t puerto;  /**< Puerto GPIO asociado a la salida */
    uint8_t terminal; /**< Terminal o bit dentro del puerto GPIO */
};

/**
 * @brief Estructura interna para almacenar el estado de una entrada digital
 */
struct digital_input_s {
    uint32_t puerto;  /**< Puerto GPIO asociado a la entrada */
    uint8_t terminal; /**< Terminal o bit dentro del puerto GPIO */
    bool invertido;   /**< Indica si la entrada requiere invertir la lectura
                         física (true) o no (false) */
    bool last_state;  /**< Memoria del estado anterior para la detección de
                         flancos */
};

/* === Private function declarations
 * =========================================================== */

/* === Private variable definitions
 * ============================================================ */

/* === Public variable definition
 * ============================================================= */

/* === Private function definitions
 * ============================================================ */

/* === Public function implementation
 * ========================================================== */

/**
 * @brief Instancia e inicializa un objeto de salida digital
 * @param puerto Número de puerto GPIO
 * @param terminal Número de terminal (bit) dentro del puerto
 * @return digital_output_t Puntero al objeto creado, o NULL si falla la
 * asignación de memoria
 */
digital_output_t DigitalOutputCreate(uint32_t puerto, uint8_t terminal) {
    digital_output_t self;
    self = malloc(sizeof(struct digital_output_s));
    if (self) {
        self->puerto = puerto;
        self->terminal = terminal;
        DigitalOutputDeactivate(self);
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->puerto, self->terminal, true);
    }
    return self;
}

/**
 * @brief Instancia e inicializa un objeto de entrada digital
 * @param puerto Número de puerto GPIO
 * @param terminal Número de terminal (bit) dentro del puerto
 * @param invertido Configuración de la lógica (true para lógica negada, false
 * para directa)
 * @return digital_input_t Puntero al objeto creado, o NULL si falla la
 * asignación de memoria
 */
digital_input_t DigitalInputCreate(uint32_t puerto, uint8_t terminal,
                                   bool invertido) {
    digital_input_t self;
    self = malloc(sizeof(struct digital_input_s));
    if (self) {
        self->puerto = puerto;
        self->terminal = terminal;
        self->invertido = invertido;
        self->last_state = false; // Inicializamos la memoria de estado
        Chip_GPIO_SetPinDIR(LPC_GPIO_PORT, self->puerto, self->terminal, false);
    }
    return self;
}

/**
 * @brief Obtiene el estado lógico actual de una entrada digital
 * @param self Puntero al objeto de la entrada digital
 * @return true Si el evento esperado está ocurriendo (activado)
 * @return false Si la entrada está en reposo
 */
bool DigitalInputGetState(digital_input_t self) {
    bool estado =
        Chip_GPIO_ReadPortBit(LPC_GPIO_PORT, self->puerto, self->terminal);
    return self->invertido ? !estado : estado;
}

/**
 * @brief Detecta si hubo un cambio en el estado de la Entrada Digital
 * @param self Puntero al objeto de la entrada a consultar
 * @return true Si el estado cambió desde la última vez que se consultó
 */
bool DigitalInputHasChanged(digital_input_t self) {
    bool estado_actual = DigitalInputGetState(self);
    bool cambio = (estado_actual != self->last_state);
    self->last_state = estado_actual; // Actualiza la memoria
    return cambio;
}

/**
 * @brief Detecta si la Entrada Digital acaba de ser presionada (Flanco de
 * activación)
 * @param self Puntero al objeto de la entrada a consultar
 * @return true Si pasó de inactiva a activa
 */
bool DigitalInputHasActivated(digital_input_t self) {
    bool estado_actual = DigitalInputGetState(self);
    bool activado = (estado_actual && !self->last_state);
    self->last_state = estado_actual; // Actualiza la memoria
    return activado;
}

/**
 * @brief Detecta si la Entrada Digital acaba de ser soltada (Flanco de
 * desactivación)
 * @param self Puntero al objeto de la entrada a consultar
 * @return true Si pasó de activa a inactiva
 */
bool DigitalInputHasDeactivated(digital_input_t self) {
    bool estado_actual = DigitalInputGetState(self);
    bool desactivado = (!estado_actual && self->last_state);
    self->last_state = estado_actual; // Actualiza la memoria
    return desactivado;
}

/**
 * @brief Enciende o activa una salida digital
 * @param self Puntero al objeto de la salida digital a encender
 */
void DigitalOutputActivate(digital_output_t self) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, true);
}

/**
 * @brief Apaga o desactiva una salida digital
 * @param self Puntero al objeto de la salida digital a apagar
 */
void DigitalOutputDeactivate(digital_output_t self) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, self->puerto, self->terminal, false);
}

/**
 * @brief Invierte el estado actual de una salida digital (Toggle)
 * @param self Puntero al objeto de la salida digital a invertir
 */
void DigitalOutputToggle(digital_output_t self) {
    Chip_GPIO_SetPinToggle(LPC_GPIO_PORT, self->puerto, self->terminal);
}

/* === End of documentation
 * ==================================================================== */
