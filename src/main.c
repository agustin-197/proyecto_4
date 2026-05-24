/************************************************************************************************
Copyright (c) 2022-2023, Laboratorio de Microprocesadores
Facultad de Ciencias Exactas y Tecnología, Universidad Nacional de Tucumán
https://www.microprocesadores.unt.edu.ar/

Copyright (c) 2022-2023, Esteban Volentini <evolentini@herrera.unt.edu.ar>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

SPDX-License-Identifier: MIT
*************************************************************************************************/

/** \brief EDU-CIAA-NXP board sample application
 **
 ** \addtogroup samples Samples
 ** \brief Samples applications with MUJU Framwork
 ** @{ */

/**
 * @file main.c
 * @brief Programa principal para el Laboratorio 4
 * @details Implementación de lógicas de control de LEDs usando teclas
 * a través de capas de abstracción de hardware (HAL y BSP).
 */

/* === Headers files inclusions =============================================================== */

#include "bsp.h"
#include <stdbool.h>

/* === Private function implementations ======================================================= */

/**
 * @brief Genera un retardo bloqueante
 * * Implementa un retardo por software de aproximadamente 100 ms
 * mediante bucles anidados iterando variables y ejecutando la 
 * instrucción NOP (No Operation) en ensamblador.
 */
static void Delay(void) {
    for (int index = 0; index < 100; index++)
    {
        for (int delay = 0; delay < 25000; delay++)
        {
            __asm("NOP");
        }
    }
}

/* === Public function implementations ======================================================== */

/**
 * @brief Función principal del sistema
 * * Punto de entrada del programa. Inicializa el hardware a través 
 * de la capa Board Support Package (BSP) y ejecuta un bucle infinito 
 * con las lógicas de control de las entradas y salidas digitales.
 * * @return int El programa nunca debería retornar de esta función.
 */
int main(void){
    /* 1. Inicialización de la placa y creación de objetos funcionales */
    board_t edu_ciaa = BoardCreate();

    /* Variable de estado para detectar el flanco de la tecla 3 */
    bool ultimo_estado_tecla_3 = false;

    while (true)
    {
        /* ==================================================================== */
        /* Lógica Switch: Encendido y apagado con teclas separadas              */
        /* - Tecla 1: Enciende el LED 1                                         */
        /* - Tecla 2: Apaga el LED 1                                            */
        /* ==================================================================== */
        if (DigitalInputGetState(edu_ciaa->tecla_1))
        {
            DigitalOutputActivate(edu_ciaa->led_1);
        }
        if (DigitalInputGetState(edu_ciaa->tecla_2))
        {
            DigitalOutputDeactivate(edu_ciaa->led_1);
        }

        /* ==================================================================== */
        /* Lógica Toggle: Alternancia de estado                                 */
        /* - Tecla 3: Cada vez que se presiona, invierte el estado del LED Rojo */
        /* ==================================================================== */
        bool estado_tecla_3 = DigitalInputGetState(edu_ciaa->tecla_3);

        // Se verifica si la tecla pasó de no presionada (false) a presionada (true)
        if (estado_tecla_3 && !ultimo_estado_tecla_3)
        {
            DigitalOutputToggle(edu_ciaa->led_rojo);
        }
        ultimo_estado_tecla_3 = estado_tecla_3;

        /* ==================================================================== */
        /* Lógica Test: Modo Pulsador                                           */
        /* - Tecla 4: El LED Verde enciende solo mientras se mantenga presionada*/
        /* ==================================================================== */
        if (DigitalInputGetState(edu_ciaa->tecla_4))
        {
            DigitalOutputActivate(edu_ciaa->led_verde);
        } else {
            DigitalOutputDeactivate(edu_ciaa->led_verde);
        }

        /* Retardo para evitar rebotes (debounce) y reducir la carga del CPU */
        Delay();   
    }
    return 0;
}

/** @} doxygen end group definition */