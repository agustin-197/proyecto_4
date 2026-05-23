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

/* === Headers files inclusions =============================================================== */

#include "bsp.h"
#include <stdbool.h>

static void Delay(void) {
    for (int index = 0; index < 100; index++)
    {
        for (int delay = 0; delay < 25000; delay++)
        {
            __asm("NOP");
        }
    }
}

int main(void){
    //iinicializo toda la placa
    board_t edu_ciaa = BoardCreate();

    bool ultimo_estado_tecla_3 = false;

    while (true)
    {
        //tecla 1 prende led1, tecla 2 apaga led1
        if (DigitalInputGetState(edu_ciaa->tecla_1))
        {
            DigitalOutputActivate(edu_ciaa->led_1);
        }
        if (DigitalInputGetState(edu_ciaa->tecla_2))
        {
            DigitalOutputDeactivate(edu_ciaa->led_1);
        }

        //logica toggle led (tecla 3 invierte led rojo)
        bool estado_tecla_3 = DigitalInputGetState(edu_ciaa->tecla_3);

        //si se acaba de presionar la tecla pasa de false a true
        if (estado_tecla_3 && !ultimo_estado_tecla_3)
        {
            DigitalOutputToggle(edu_ciaa->led_rojo);
        }
        ultimo_estado_tecla_3 = estado_tecla_3;

        //logica testled (led verde encendido mientras se presiona tecla 4)
        if (DigitalInputGetState(edu_ciaa->tecla_4))
        {
            DigitalOutputActivate(edu_ciaa->led_verde);
        }else{
            DigitalOutputDeactivate(edu_ciaa->led_verde);
        }

        Delay();   
    }
    return 0;
}