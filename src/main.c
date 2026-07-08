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
#include "placa.h"
#include "reloj.h"
#include "chip.h"
#include "maquina_estados.h"

/* Variables globales estáticas */
static board_t board;
static clock_t reloj;

/* Callback de la alarma */
void AlarmaSonando(void) {
    DigitalOutputActivate(board->buzzer);
}

/* Interrupción del Hardware (1 ms) */
void SysTick_Handler(void) {
    // Le delegamos el trabajo pesado en la interrupción a nuestra máquina de estados
    MaquinaEstados_Tick(); 
}

int main(void) {
    // 1. Inicializar el hardware y el reloj
    board = BoardCreate();
    reloj = RelojCreate(1, AlarmaSonando);

    // 2. Inicializar nuestra máquina de estados
    MaquinaEstados_Init(board, reloj);

    // 3. Configurar el hardware del SysTick (Microcontrolador)
    SystemCoreClockUpdate();
    SysTick_Config(SystemCoreClock / 1000);

    // 4. Bucle infinito súper limpio
    while (1) {
        // La máquina de estados se encarga de leer botones y actualizar el display
        MaquinaEstados_Update(); 
    }
    return 0;
}