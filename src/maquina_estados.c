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

/** @file maquina_estados.c
 ** @brief Implementación de la máquina de estados del reloj
 ** @details Contiene la lógica de transiciones, el control de botones con
 ** filtro anti-rebote y la gestión de la pantalla multiplexada.
 **/

/* === Headers files inclusions
 * =================================================== */

#include "maquina_estados.h"
#include <stdbool.h>

/* === Macros definitions
 * ========================================================= */

/** @brief Frecuencia de parpadeo de los dígitos de la pantalla en milisegundos
 */
#define FREC_PARPADEO 200

/* === Private data type declarations
 * ============================================= */

/**
 * @brief Definición de los posibles estados operativos del sistema
 */
typedef enum {
    MODO_SIN_AJUSTAR, /**< El reloj arranca sin hora definida, titilando */
    MODO_NORMAL, /**< Mostrando la hora actual y transcurriendo los segundos */
    MODO_AJUSTE_HORAS,         /**< Ajustando las horas del reloj */
    MODO_AJUSTE_MINUTOS,       /**< Ajustando los minutos del reloj */
    MODO_AJUSTE_HORAS_ALARMA,  /**< Ajustando las horas de la alarma */
    MODO_AJUSTE_MINUTOS_ALARMA /**< Ajustando los minutos de la alarma */
} modo_reloj_t;

/* === Private variable declarations
 * ============================================== */

/* === Private function declarations
 * ============================================== */

/**
 * @brief Incrementa en una unidad la hora en formato BCD
 * @param hora Arreglo que contiene la hora a modificar
 */
static void IncrementarHoraBCD(hora_t hora);

/**
 * @brief Decrementa en una unidad la hora en formato BCD
 * @param hora Arreglo que contiene la hora a modificar
 */
static void DecrementarHoraBCD(hora_t hora);

/**
 * @brief Incrementa en una unidad el minuto en formato BCD
 * @param hora Arreglo que contiene la hora a modificar
 */
static void IncrementarMinutoBCD(hora_t hora);

/**
 * @brief Decrementa en una unidad el minuto en formato BCD
 * @param hora Arreglo que contiene la hora a modificar
 */
static void DecrementarMinutoBCD(hora_t hora);

/* === Public variable definitions
 * ================================================ */

/* === Private variable definitions
 * =============================================== */

static board_t board; /**< Puntero a los recursos de hardware de la placa */
static clock_t reloj; /**< Puntero a la instancia del reloj en memoria */
static modo_reloj_t
    estado_actual; /**< Almacena el estado operativo actual de la máquina */
static hora_t hora_ajuste; /**< Buffer temporal para modificar horas y minutos
                              sin afectar el reloj */
static uint16_t temporizador_antirebote =
    0; /**< Temporizador para ignorar rebotes mecánicos de las teclas */

/* === Private function definitions
 * =============================================== */

static void IncrementarHoraBCD(hora_t hora) {
    hora[1]++;
    if (hora[0] == 2 && hora[1] > 3) {
        hora[0] = 0;
        hora[1] = 0;
    } else if (hora[1] > 9) {
        hora[1] = 0;
        hora[0]++;
    }
}

static void DecrementarHoraBCD(hora_t hora) {
    if (hora[0] == 0 && hora[1] == 0) {
        hora[0] = 2;
        hora[1] = 3;
    } else if (hora[1] == 0) {
        hora[1] = 9;
        hora[0]--;
    } else {
        hora[1]--;
    }
}

static void IncrementarMinutoBCD(hora_t hora) {
    hora[3]++;
    if (hora[3] > 9) {
        hora[3] = 0;
        hora[2]++;
    }
    if (hora[2] > 5) {
        hora[2] = 0;
    }
}

static void DecrementarMinutoBCD(hora_t hora) {
    if (hora[2] == 0 && hora[3] == 0) {
        hora[2] = 5;
        hora[3] = 9;
    } else if (hora[3] == 0) {
        hora[3] = 9;
        hora[2]--;
    } else {
        hora[3]--;
    }
}

/* === Public function implementation
 * ============================================= */

void MaquinaEstados_Init(board_t b, clock_t r) {
    board = b;
    reloj = r;
    estado_actual = MODO_SIN_AJUSTAR;

    // Al arrancar, ordenamos que todos los números parpadeen
    DisplayFlashDigits(board->display, 0, 3, FREC_PARPADEO);
}

void MaquinaEstados_Tick(void) {
    static uint16_t contador_milisegundos = 0;
    static uint16_t contador_medio_segundo = 0;

    DisplayRefresh(board->display);

    // Descontar el tiempo del filtro antirebote (hasta llegar a cero)
    if (temporizador_antirebote > 0) {
        temporizador_antirebote--;
    }

    // Avanzar el motor del reloj (1 segundo = 1000 ms)
    contador_milisegundos++;
    if (contador_milisegundos >= 1000) {
        contador_milisegundos = 0;
        RelojNewTick(reloj);
    }

    // Parpadear el punto separador si estamos en el modo normal
    if (estado_actual == MODO_NORMAL) {
        contador_medio_segundo++;
        if (contador_medio_segundo >= 500) {
            contador_medio_segundo = 0;
            DisplayToggleDots(board->display, 1, 1);
        }
    }
}

void MaquinaEstados_Update(void) {
    // 1. Leemos TODAS las teclas (Para limpiar la memoria interna del hardware)
    bool f1 = DigitalInputHasActivated(board->tecla_f1);
    bool f2 = DigitalInputHasActivated(board->tecla_f2);
    bool f3 = DigitalInputHasActivated(board->tecla_f3);
    bool f4 = DigitalInputHasActivated(board->tecla_f4);
    bool acc = DigitalInputHasActivated(board->tecla_accept);
    bool can = DigitalInputHasActivated(board->tecla_cancel);

    // 2. Filtro Anti-Rebote (Lockout)
    if (temporizador_antirebote > 0) {
        // Si el temporizador está corriendo, "borramos" las teclas apretadas
        f1 = false;
        f2 = false;
        f3 = false;
        f4 = false;
        acc = false;
        can = false;
    } else if (f1 || f2 || f3 || f4 || acc || can) {
        // Si detectamos una pulsación válida, bloqueamos la botonera por 250
        // milisegundos
        temporizador_antirebote = 250;
    }

    // 3. Máquina de Estados
    switch (estado_actual) {

    case MODO_SIN_AJUSTAR:
        if (f1) {
            estado_actual = MODO_AJUSTE_HORAS;
            DisplayFlashDigits(board->display, 0, 1, FREC_PARPADEO);
            hora_ajuste[0] = 0;
            hora_ajuste[1] = 0;
            hora_ajuste[2] = 0;
            hora_ajuste[3] = 0;
            hora_ajuste[4] = 0;
            hora_ajuste[5] = 0; // Segundos a cero (Fix Prueba 3)
        }
        break;

    case MODO_NORMAL:
        // Control de Alarma Sonando
        if (DigitalOutputGetState(board->buzzer)) {
            if (acc) {
                DigitalOutputDeactivate(board->buzzer);
                RelojPostponeAlarm(reloj, 5);
            }
            if (can) {
                DigitalOutputDeactivate(board->buzzer);
            }
        } else {
            // Activar/Desactivar alarma silenciosa
            if (acc)
                RelojSetAlarmState(reloj, true);
            if (can)
                RelojSetAlarmState(reloj, false);

            // Ir a ajustes
            if (f1) {
                estado_actual = MODO_AJUSTE_HORAS;
                DisplayFlashDigits(board->display, 0, 1, FREC_PARPADEO);
                RelojGetCurrentTime(reloj, hora_ajuste);
                hora_ajuste[4] = 0;
                hora_ajuste[5] = 0;
            }
            if (f2) {
                estado_actual = MODO_AJUSTE_HORAS_ALARMA;
                DisplayFlashDigits(board->display, 0, 1, FREC_PARPADEO);
                RelojGetAlarm(reloj, hora_ajuste);
                hora_ajuste[4] = 0;
                hora_ajuste[5] = 0;
            }
        }
        break;

    case MODO_AJUSTE_HORAS:
        if (f4)
            IncrementarHoraBCD(hora_ajuste);
        if (f3)
            DecrementarHoraBCD(hora_ajuste);
        if (acc) {
            estado_actual = MODO_AJUSTE_MINUTOS;
            DisplayFlashDigits(board->display, 2, 3, FREC_PARPADEO);
        }
        if (can) {
            estado_actual = MODO_NORMAL;
            DisplayFlashDigits(board->display, 0, 0, 0); // Apagar parpadeo
        }
        break;

    case MODO_AJUSTE_MINUTOS:
        if (f4)
            IncrementarMinutoBCD(hora_ajuste);
        if (f3)
            DecrementarMinutoBCD(hora_ajuste);
        if (acc) {
            RelojSetupCurrentTime(reloj, hora_ajuste);
            estado_actual = MODO_NORMAL;
            DisplayFlashDigits(board->display, 0, 0, 0);
        }
        if (can) {
            estado_actual = MODO_NORMAL;
            DisplayFlashDigits(board->display, 0, 0, 0);
        }
        break;

    case MODO_AJUSTE_HORAS_ALARMA:
        if (f4)
            IncrementarHoraBCD(hora_ajuste);
        if (f3)
            DecrementarHoraBCD(hora_ajuste);
        if (acc) {
            estado_actual = MODO_AJUSTE_MINUTOS_ALARMA;
            DisplayFlashDigits(board->display, 2, 3, FREC_PARPADEO);
        }
        if (can) {
            estado_actual = MODO_NORMAL;
            DisplayFlashDigits(board->display, 0, 0, 0);
        }
        break;

    case MODO_AJUSTE_MINUTOS_ALARMA:
        if (f4)
            IncrementarMinutoBCD(hora_ajuste);
        if (f3)
            DecrementarMinutoBCD(hora_ajuste);
        if (acc) {
            RelojSetupAlarm(reloj, hora_ajuste);
            estado_actual = MODO_NORMAL;
            DisplayFlashDigits(board->display, 0, 0, 0);
        }
        if (can) {
            estado_actual = MODO_NORMAL;
            DisplayFlashDigits(board->display, 0, 0, 0);
        }
        break;
    }

    // 4. Escribir en pantalla (SOLO si los números cambiaron)
    hora_t hora_actual;
    uint8_t digitos_nuevos[4] = {0, 0, 0, 0};

    // Cargamos los números que deberían mostrarse
    if (estado_actual == MODO_NORMAL) {
        if (RelojGetCurrentTime(reloj, hora_actual)) {
            digitos_nuevos[0] = hora_actual[0];
            digitos_nuevos[1] = hora_actual[1];
            digitos_nuevos[2] = hora_actual[2];
            digitos_nuevos[3] = hora_actual[3];
        }
    } else {
        digitos_nuevos[0] = hora_ajuste[0];
        digitos_nuevos[1] = hora_ajuste[1];
        digitos_nuevos[2] = hora_ajuste[2];
        digitos_nuevos[3] = hora_ajuste[3];
    }

    // Comparamos con una memoria interna para no saturar la pantalla
    static uint8_t digitos_viejos[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    if (digitos_nuevos[0] != digitos_viejos[0] ||
        digitos_nuevos[1] != digitos_viejos[1] ||
        digitos_nuevos[2] != digitos_viejos[2] ||
        digitos_nuevos[3] != digitos_viejos[3]) {

        // Si hay algún cambio, le enviamos la orden al hardware
        DisplayWriteBCD(board->display, digitos_nuevos, 4);

        // Guardamos el nuevo estado en la memoria
        digitos_viejos[0] = digitos_nuevos[0];
        digitos_viejos[1] = digitos_nuevos[1];
        digitos_viejos[2] = digitos_nuevos[2];
        digitos_viejos[3] = digitos_nuevos[3];
    }
}
