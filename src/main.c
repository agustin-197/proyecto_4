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

/** @file main.c
 ** @brief Programa de prueba para el barrido de la pantalla de 7 segmentos
 ** @details Archivo principal de la aplicación que inicializa el hardware y
 * ejecuta
 ** un bucle infinito para validar el funcionamiento del display multiplexado.
 **/

/* === Headers files inclusions
 * ================================================================ */

#include "placa.h"
#include "screen.h"

/* === Macros definitions
 * ====================================================================== */

/**
 * @brief Cantidad de ciclos de reloj para el retardo
 * @details Este valor permite ajustar la velocidad a la que se refresca la
 * pantalla durante la prueba visual.
 */
#define CICLOS_RETARDO 10000

/* === Private data type declarations
 * ========================================================== */

/* === Private function declarations
 * =========================================================== */

/**
 * @brief Función de retardo muy rudimentaria para la prueba visual
 * @details Utiliza un bucle for para perder ciclos de reloj.
 * No es exacta y solo sirve para esta prueba inicial.
 * @param[in] iteraciones Cantidad de iteraciones del bucle for para generar la
 * demora.
 */
void RetardoBloqueante(uint32_t iteraciones) {
    for (volatile uint32_t i = 0; i < iteraciones; i++)
        ;
}

/* === Private variable definitions
 * ============================================================ */

/* === Public variable definition
 * ============================================================= */

/* === Private function definitions
 * ============================================================ */

/* === Public function implementation
 * ========================================================== */

/**
 * @brief Función principal del programa
 * @details Inicializa la placa EDU-CIAA, configura la pantalla y el zumbador,
 * y ejecuta el bucle principal de barrido y prueba.
 * @return int Retorna 0 al finalizar la ejecución (nunca debería ocurrir en un
 * sistema embebido).
 */
int main(void) {
    board_t mi_placa = BoardCreate();

    // Arreglo con todos los dígitos del 0 al 9 para probar la tabla BCD
    uint8_t digitos_completos[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    uint8_t offset = 0; // Controla qué número empieza a dibujarse
    uint8_t buffer_display[4];

    // --- CARGA INICIAL ANTES DE ENTRAR AL BUCLE ---
    buffer_display[0] = digitos_completos[(offset + 0) % 10];
    buffer_display[1] = digitos_completos[(offset + 1) % 10];
    buffer_display[2] = digitos_completos[(offset + 2) % 10];
    buffer_display[3] = digitos_completos[(offset + 3) % 10];
    DisplayWriteBCD(mi_placa->display, buffer_display, 4);

    while (1) {
        // 1. Refrescar la pantalla (a esta velocidad, el ojo ve los 4 a la vez)
        DisplayRefresh(mi_placa->display);

        // 2. PRUEBA DE TECLAS Y FUNCIONES:

        // Tecla ACEPTAR: Desplaza los números (0123 -> 1234 -> 2345)
        if (DigitalInputHasActivated(mi_placa->tecla_accept)) {
            offset++;
            // Cargamos los nuevos números al buffer
            buffer_display[0] = digitos_completos[(offset + 0) % 10];
            buffer_display[1] = digitos_completos[(offset + 1) % 10];
            buffer_display[2] = digitos_completos[(offset + 2) % 10];
            buffer_display[3] = digitos_completos[(offset + 3) % 10];

            // ACTUALIZAMOS LA MEMORIA SOLO CUANDO CAMBIAN LOS NÚMEROS
            DisplayWriteBCD(mi_placa->display, buffer_display, 4);

            DigitalOutputToggle(mi_placa->buzzer); // Sonido/LED de confirmación
        }

        // Tecla CANCELAR: Prueba de la función ToggleDots (enciende/apaga
        // puntos)
        if (DigitalInputHasActivated(mi_placa->tecla_cancel)) {
            DisplayToggleDots(mi_placa->display, 0, 3);
        }

        // Tecla F1: Activa el parpadeo (FlashDigits) en todos los dígitos
        // 100 significa que cambiará de estado cada 100 ciclos de barrido
        if (DigitalInputHasActivated(mi_placa->tecla_f1)) {
            DisplayFlashDigits(mi_placa->display, 0, 3, 100);
        }

        // Tecla F2: Apaga el parpadeo (frecuencia 0)
        if (DigitalInputHasActivated(mi_placa->tecla_f2)) {
            DisplayFlashDigits(mi_placa->display, 0, 0, 0);
        }

        RetardoBloqueante(CICLOS_RETARDO);
    }
    return 0;
}

/* === End of documentation
 * ==================================================================== */
