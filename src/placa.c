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

/** @file placa.c
 ** @brief Implementación del Board Support Package (BSP)
 ** @details Este archivo oculta los detalles de configuración de bajo nivel
 ** del microcontrolador y la asignación de pines, creando y exponiendo
 ** únicamente objetos abstractos de entradas y salidas para la aplicación.
 **/

/* === Headers files inclusions ================================================================ */

#include "placa.h"
#include "poncho.h"
#include "screen.h"
#include "chip.h"
#include <stdlib.h>

/* === Macros definitions ====================================================================== */

#define LED_R_PORT 2
#define LED_R_PIN  0
#define LED_R_FUNC SCU_MODE_FUNC4
#define LED_R_GPIO 5
#define LED_R_BIT  0

#define LED_G_PORT 2
#define LED_G_PIN  1
#define LED_G_FUNC SCU_MODE_FUNC4
#define LED_G_GPIO 5
#define LED_G_BIT  1

#define LED_1_PORT 2
#define LED_1_PIN  10
#define LED_1_FUNC SCU_MODE_FUNC0
#define LED_1_GPIO 0
#define LED_1_BIT  14

#define LED_2_PORT 2
#define LED_2_PIN  11
#define LED_2_FUNC SCU_MODE_FUNC0
#define LED_2_GPIO 1
#define LED_2_BIT  11

#define LED_3_PORT 2
#define LED_3_PIN  12
#define LED_3_FUNC SCU_MODE_FUNC0
#define LED_3_GPIO 1
#define LED_3_BIT  12

/* === Private data type declarations ========================================================== */

/* === Private function declarations =========================================================== */

/**
 * @brief Configura la multiplexación física y la dirección de los pines
 * @details Inicializa el ruteo de las teclas, el zumbador y la pantalla, y fuerza el apagado
 * de los LEDs internos no utilizados en la placa EDU-CIAA para evitar interferencias visuales.
 */
static void BoardPinsConfigure(void);

/**
 * @brief Enciende un dígito específico de la pantalla y apaga los demás
 * @param[in] digit Índice del dígito a encender (0 a 3)
 */
static void BoardDisplayUpdateDigits(uint8_t digit);

/**
 * @brief Enciende o apaga los segmentos físicos evaluando una máscara de bits
 * @param[in] segments Máscara de bits con los segmentos a encender
 */
static void BoardDisplayUpdateSegments(uint8_t segments);

/* === Private variable definitions ============================================================ */

/**
 * @brief Instancia estática de la estructura de la placa
 */
static struct board_s board;

/* === Public variable definition  ============================================================= */

/* === Private function definitions ============================================================ */

static void BoardPinsConfigure(void) {
    // 1. Ruteo de las teclas (Entradas con Pull-Up)
    Chip_SCU_PinMuxSet(KEY_F1_PORT, KEY_F1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F1_FUNC);
    Chip_SCU_PinMuxSet(KEY_F2_PORT, KEY_F2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F2_FUNC);
    Chip_SCU_PinMuxSet(KEY_F3_PORT, KEY_F3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F3_FUNC);
    Chip_SCU_PinMuxSet(KEY_F4_PORT, KEY_F4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_F4_FUNC);
    Chip_SCU_PinMuxSet(KEY_ACCEPT_PORT, KEY_ACCEPT_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_ACCEPT_FUNC);
    Chip_SCU_PinMuxSet(KEY_CANCEL_PORT, KEY_CANCEL_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | KEY_CANCEL_FUNC);

    // 2. Ruteo del Zumbador
    Chip_SCU_PinMuxSet(BUZZER_PORT, BUZZER_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | BUZZER_FUNC);

    // 3. Ruteo de los 4 Dígitos de la pantalla
    Chip_SCU_PinMuxSet(DIGIT_1_PORT, DIGIT_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_1_FUNC);
    Chip_SCU_PinMuxSet(DIGIT_2_PORT, DIGIT_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_2_FUNC);
    Chip_SCU_PinMuxSet(DIGIT_3_PORT, DIGIT_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_3_FUNC);
    Chip_SCU_PinMuxSet(DIGIT_4_PORT, DIGIT_4_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | DIGIT_4_FUNC);

    // 4. Ruteo de los 8 Segmentos
    Chip_SCU_PinMuxSet(SEGMENT_A_PORT, SEGMENT_A_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_A_FUNC);
    Chip_SCU_PinMuxSet(SEGMENT_B_PORT, SEGMENT_B_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_B_FUNC);
    Chip_SCU_PinMuxSet(SEGMENT_C_PORT, SEGMENT_C_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_C_FUNC);
    Chip_SCU_PinMuxSet(SEGMENT_D_PORT, SEGMENT_D_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_D_FUNC);
    Chip_SCU_PinMuxSet(SEGMENT_E_PORT, SEGMENT_E_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_E_FUNC);
    Chip_SCU_PinMuxSet(SEGMENT_F_PORT, SEGMENT_F_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_F_FUNC);
    Chip_SCU_PinMuxSet(SEGMENT_G_PORT, SEGMENT_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_G_FUNC);
    Chip_SCU_PinMuxSet(SEGMENT_P_PORT, SEGMENT_P_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | SEGMENT_P_FUNC);

    // 5. Configurar dirección GPIO como salidas para la pantalla
    Chip_GPIO_SetDir(LPC_GPIO_PORT, DIGITS_GPIO, DIGITS_MASK, 1);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, SEGMENTS_GPIO, SEGMENTS_MASK, 1);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, SEGMENT_P_GPIO, (1 << SEGMENT_P_BIT), 1);

    // APAGADO FORZOSO DE LEDS NO UTILIZADOS
    Chip_SCU_PinMuxSet(LED_R_PORT, LED_R_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_R_FUNC);
    Chip_SCU_PinMuxSet(LED_G_PORT, LED_G_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_G_FUNC);
    Chip_SCU_PinMuxSet(LED_1_PORT, LED_1_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_1_FUNC);
    Chip_SCU_PinMuxSet(LED_2_PORT, LED_2_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_2_FUNC);
    Chip_SCU_PinMuxSet(LED_3_PORT, LED_3_PIN, SCU_MODE_INBUFF_EN | SCU_MODE_INACT | LED_3_FUNC);

    Chip_GPIO_SetDir(LPC_GPIO_PORT, LED_R_GPIO, (1 << LED_R_BIT), 1);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, LED_G_GPIO, (1 << LED_G_BIT), 1);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, LED_1_GPIO, (1 << LED_1_BIT), 1);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, LED_2_GPIO, (1 << LED_2_BIT), 1);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, LED_3_GPIO, (1 << LED_3_BIT), 1);

    Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_R_GPIO, LED_R_BIT, 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_G_GPIO, LED_G_BIT, 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_1_GPIO, LED_1_BIT, 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_2_GPIO, LED_2_BIT, 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, LED_3_GPIO, LED_3_BIT, 0);
}

static void BoardDisplayUpdateDigits(uint8_t digit) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, 0);

    switch (digit) {
        case 0: Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_1_GPIO, DIGIT_1_BIT, 1); break;
        case 1: Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_2_GPIO, DIGIT_2_BIT, 1); break;
        case 2: Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_3_GPIO, DIGIT_3_BIT, 1); break;
        case 3: Chip_GPIO_SetPinState(LPC_GPIO_PORT, DIGIT_4_GPIO, DIGIT_4_BIT, 1); break;
    }
}

static void BoardDisplayUpdateSegments(uint8_t segments) {
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_A_GPIO, SEGMENT_A_BIT, (segments & SEGMENT_A) ? 1 : 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_B_GPIO, SEGMENT_B_BIT, (segments & SEGMENT_B) ? 1 : 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_C_GPIO, SEGMENT_C_BIT, (segments & SEGMENT_C) ? 1 : 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_D_GPIO, SEGMENT_D_BIT, (segments & SEGMENT_D) ? 1 : 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_E_GPIO, SEGMENT_E_BIT, (segments & SEGMENT_E) ? 1 : 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_F_GPIO, SEGMENT_F_BIT, (segments & SEGMENT_F) ? 1 : 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_G_GPIO, SEGMENT_G_BIT, (segments & SEGMENT_G) ? 1 : 0);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT, SEGMENT_P_GPIO, SEGMENT_P_BIT, (segments & SEGMENT_P) ? 1 : 0);
}

/* === Public function implementation ========================================================== */

board_t BoardCreate(void) {
    // 1. Inicializar GPIO y multiplexación de pines
    Chip_GPIO_Init(LPC_GPIO_PORT);
    BoardPinsConfigure();

    // 2. Instanciar la salida digital (Zumbador)
    board.buzzer = DigitalOutputCreate(BUZZER_GPIO, BUZZER_BIT);

    // 3. Instanciar las 6 entradas digitales (Teclas) usando los alias lógicos
    board.tecla_f1 = DigitalInputCreate(KEY_F1_GPIO, KEY_F1_BIT, true);
    board.tecla_f2 = DigitalInputCreate(KEY_F2_GPIO, KEY_F2_BIT, true);
    board.tecla_f3 = DigitalInputCreate(KEY_F3_GPIO, KEY_F3_BIT, true);
    board.tecla_f4 = DigitalInputCreate(KEY_F4_GPIO, KEY_F4_BIT, true);
    board.tecla_accept = DigitalInputCreate(KEY_ACCEPT_GPIO, KEY_ACCEPT_BIT, true);
    board.tecla_cancel = DigitalInputCreate(KEY_CANCEL_GPIO, KEY_CANCEL_BIT, true);

    // 4. Configurar e instanciar la pantalla multiplexada
    static const struct display_driver_s display_driver = {
        .UpdateDigits = BoardDisplayUpdateDigits,
        .UpdateSegments = BoardDisplayUpdateSegments
    };
    board.display = DisplayCreate(4, &display_driver);

    // 5. Retornar el puntero con toda la placa configurada
    return &board;
}

/* === End of documentation ==================================================================== */