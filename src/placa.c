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

/** @file placa.c
 ** @brief Implementación del Board Support Package (BSP)
 ** @details Este archivo oculta los detalles de configuración de bajo nivel
 ** del microcontrolador y la asignación de pines, creando y exponiendo
 ** únicamente objetos abstractos de entradas y salidas para la aplicación.
 **/

/* === Headers files inclusions
 * ================================================================ */

#include "placa.h"
#include "chip.h"
#include <stdlib.h>

/* === Macros definitions
 * ====================================================================== */

#define LED_R_PORT 2
#define LED_R_PIN 0
#define LED_R_FUNC SCU_MODE_FUNC4
#define LED_R_GPIO 5
#define LED_R_BIT 0

#define LED_G_PORT 2
#define LED_G_PIN 1
#define LED_G_FUNC SCU_MODE_FUNC4
#define LED_G_GPIO 5
#define LED_G_BIT 1

#define LED_B_PORT 2
#define LED_B_PIN 2
#define LED_B_FUNC SCU_MODE_FUNC4
#define LED_B_GPIO 5
#define LED_B_BIT 2

#define LED_1_PORT 2
#define LED_1_PIN 10
#define LED_1_FUNC SCU_MODE_FUNC0
#define LED_1_GPIO 0
#define LED_1_BIT 14

#define LED_2_PORT 2
#define LED_2_PIN 11
#define LED_2_FUNC SCU_MODE_FUNC0
#define LED_2_GPIO 1
#define LED_2_BIT 11

#define LED_3_PORT 2
#define LED_3_PIN 12
#define LED_3_FUNC SCU_MODE_FUNC0
#define LED_3_GPIO 1
#define LED_3_BIT 12

#define TEC_1_PORT 1
#define TEC_1_PIN 0
#define TEC_1_FUNC SCU_MODE_FUNC0
#define TEC_1_GPIO 0
#define TEC_1_BIT 4

#define TEC_2_PORT 1
#define TEC_2_PIN 1
#define TEC_2_FUNC SCU_MODE_FUNC0
#define TEC_2_GPIO 0
#define TEC_2_BIT 8

#define TEC_3_PORT 1
#define TEC_3_PIN 2
#define TEC_3_FUNC SCU_MODE_FUNC0
#define TEC_3_GPIO 0
#define TEC_3_BIT 9

#define TEC_4_PORT 1
#define TEC_4_PIN 6
#define TEC_4_FUNC SCU_MODE_FUNC0
#define TEC_4_GPIO 1
#define TEC_4_BIT 9

/* === Private data type declarations
 * ========================================================== */

/* === Private function declarations
 * =========================================================== */

/* === Private variable definitions
 * ============================================================ */

/**
 * @brief Instancia estática de la estructura de la placa
 * @details Almacena los punteros a todos los objetos de hardware (entradas y
 * salidas). Al ser estática, su memoria persiste durante toda la ejecución y no
 * es accesible directamente desde otros archivos, garantizando el
 * encapsulamiento.
 */
static struct board_s board;

/* === Public variable definition
 * ============================================================= */

/* === Private function definitions
 * ============================================================ */

/* === Private function implementations
 * ============================================================ */

/**
 * @brief Configura la multiplexación física de los pines del microcontrolador
 * @details Utiliza la unidad de control del sistema (SCU) de NXP para asignar
 * la función correcta a cada pin físico, habilitando los buffers de entrada y
 * las resistencias de pull-up necesarias para los pulsadores.
 */
static void BoardPinsConfigure(void) {
    Chip_SCU_PinMuxSet(LED_R_PORT, LED_R_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_R_FUNC);
    Chip_SCU_PinMuxSet(LED_G_PORT, LED_G_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_G_FUNC);
    Chip_SCU_PinMuxSet(LED_B_PORT, LED_B_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_B_FUNC);
    Chip_SCU_PinMuxSet(LED_1_PORT, LED_1_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_1_FUNC);
    Chip_SCU_PinMuxSet(LED_2_PORT, LED_2_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_2_FUNC);
    Chip_SCU_PinMuxSet(LED_3_PORT, LED_3_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_3_FUNC);

    Chip_SCU_PinMuxSet(TEC_1_PORT, TEC_1_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_1_FUNC);
    Chip_SCU_PinMuxSet(TEC_2_PORT, TEC_2_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_2_FUNC);
    Chip_SCU_PinMuxSet(TEC_3_PORT, TEC_3_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_3_FUNC);
    Chip_SCU_PinMuxSet(TEC_4_PORT, TEC_4_PIN,
                       SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | TEC_4_FUNC);
}

/* === Public function implementation
 * ========================================================== */

/**
 * @brief Inicializa y configura todos los recursos de hardware de la placa
 * @details Esta función debe ser llamada una única vez al inicio del programa.
 * Configura los periféricos GPIO, rutea los pines eléctricos y crea las
 * abstracciones lógicas (ADTs) correspondientes a cada LED y Tecla.
 * @return board_t Puntero a la estructura de la placa lista para ser utilizada.
 */
board_t BoardCreate(void) {
    // 1. Inicializa el periférico GPIO completo con la biblioteca de NXP
    Chip_GPIO_Init(LPC_GPIO_PORT);

    // 2. Configura la multiplexación física de los pines eléctricos
    BoardPinsConfigure();

    // 3. Creamos los objetos lógicos de abstracción para cada Salida Digital
    // (LEDs)
    board.led_rojo = DigitalOutputCreate(LED_R_GPIO, LED_R_BIT);
    board.led_verde = DigitalOutputCreate(LED_G_GPIO, LED_G_BIT);
    board.led_azul = DigitalOutputCreate(LED_B_GPIO, LED_B_BIT);
    board.led_1 = DigitalOutputCreate(LED_1_GPIO, LED_1_BIT);
    board.led_2 = DigitalOutputCreate(LED_2_GPIO, LED_2_BIT);
    board.led_3 = DigitalOutputCreate(LED_3_GPIO, LED_3_BIT);

    // 4. Creamos los objetos lógicos de abstracción para cada Entrada Digital
    // (Teclas)
    board.tecla_1 = DigitalInputCreate(TEC_1_GPIO, TEC_1_BIT);
    board.tecla_2 = DigitalInputCreate(TEC_2_GPIO, TEC_2_BIT);
    board.tecla_3 = DigitalInputCreate(TEC_3_GPIO, TEC_3_BIT);
    board.tecla_4 = DigitalInputCreate(TEC_4_GPIO, TEC_4_BIT);

    // 5. Devolvemos el puntero a la estructura que tiene todo listo para usar
    return &board;
}

/* === End of documentation
 * ==================================================================== */
