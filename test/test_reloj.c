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

/** @file test_reloj.c
 ** @brief Batería de pruebas unitarias (TDD) para la biblioteca de reloj y
 * alarma.
 **/

/* === Headers files inclusions
 * ================================================================ */

#include "unity.h"
#include "reloj.h"

/* === Macros definitions
 * ====================================================================== */

/* === Private data type declarations
 * ========================================================== */

/* === Private function declarations
 * =========================================================== */

void FuncionSimuladaAlarma(void);

/* === Private variable definitions
 * ============================================================ */

/** @brief Hora por defecto (00:00:00) utilizada para comparaciones */
static const hora_t DEFAULT_TIME = {0, 0, 0, 0, 0, 0};

/** @brief Bandera global para registrar si el evento de alarma ocurrió durante
 * la prueba */
static bool alarma_sonando = false;

/* === Public variable definition
 * ============================================================= */

/* === Private function definitions
 * ============================================================ */

/**
 * @brief Función "callback" simulada (Mock) que el reloj deberá llamar al
 * coincidir la alarma.
 */
void FuncionSimuladaAlarma(void) {
    alarma_sonando = true;
}

/* === Public function implementation
 * ========================================================== */

/**
 * @brief Verifica que al ajustar la hora, el reloj la guarda correctamente y la
 * marca como válida.
 */
void test_reloj_ajustar_hora(void) {
    clock_t reloj = RelojCreate(1, NULL);
    hora_t nueva_hora = {1, 2, 3, 4, 5, 6}; // Representa las 12:34:56
    hora_t hora_leida;

    // Configuramos la nueva hora y esperamos que la funcion devuelva true
    // (indicando éxito)
    TEST_ASSERT_TRUE(RelojSetupCurrentTime(reloj, nueva_hora));

    // Al consultar la hora ahora, debe devolver true (hora válida)
    TEST_ASSERT_TRUE(RelojGetCurrentTime(reloj, hora_leida));

    // El arreglo devuelto debe coincidir exactamente con la hora que
    // configuramos
    TEST_ASSERT_EQUAL_UINT8_ARRAY(nueva_hora, hora_leida, 6);
}

/**
 * @brief Verifica que el reloj rechaza ajustes con formato BCD inválido y
 * mantiene su estado.
 */
void test_ajuste_hora_invalida(void) {
    clock_t reloj = RelojCreate(1, NULL);
    hora_t hora_invalida = {2, 5, 0, 0, 0, 0}; // Las 25:00:00 (Inválido)
    hora_t hora_leida;

    // Esperamos que devuelva false porque la hora es inválida
    TEST_ASSERT_FALSE(RelojSetupCurrentTime(reloj, hora_invalida));

    // El reloj debería seguir con hora inválida porque rechazó el ajuste
    TEST_ASSERT_FALSE(RelojGetCurrentTime(reloj, hora_leida));
}

/**
 * @brief Verifica que después de N ciclos de reloj configurados, la hora avanza
 * exactamente un segundo.
 */
void test_reloj_avanza_un_segundo(void) {
    // Configuramos el reloj para que avance 1 segundo cada 5 ticks
    int ticks_por_segundo = 5;
    clock_t reloj = RelojCreate(ticks_por_segundo, NULL);

    hora_t tiempo_inicial = {1, 2, 3, 4, 5, 6};  // 12:34:56
    hora_t tiempo_esperado = {1, 2, 3, 4, 5, 7}; // 12:34:57
    hora_t tiempo_leido;

    RelojSetupCurrentTime(reloj, tiempo_inicial);

    // Simulamos el paso del tiempo llamando a la función 5 veces
    for (int i = 0; i < ticks_por_segundo; i++) {
        RelojNewTick(reloj);
    }

    // Consultamos la hora y verificamos que avanzó un segundo
    RelojGetCurrentTime(reloj, tiempo_leido);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(tiempo_esperado, tiempo_leido, 6);
}

/**
 * @brief Verifica la lógica de acarreo BCD al pasar de 59 segundos al siguiente
 * minuto.
 */
void test_reloj_avanza_siguiente_minuto(void) {
    int ticks_por_segundo = 5;
    clock_t reloj = RelojCreate(ticks_por_segundo, NULL);

    hora_t tiempo_inicial = {1, 2, 3, 4, 5, 9};  // 12:34:59
    hora_t tiempo_esperado = {1, 2, 3, 5, 0, 0}; // 12:35:00
    hora_t tiempo_leido;

    RelojSetupCurrentTime(reloj, tiempo_inicial);

    // Simulamos el paso de 1 segundo
    for (int i = 0; i < ticks_por_segundo; i++) {
        RelojNewTick(reloj);
    }

    RelojGetCurrentTime(reloj, tiempo_leido);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(tiempo_esperado, tiempo_leido, 6);
}

/**
 * @brief Verifica la lógica de acarreo para el reinicio a medianoche (de
 * 23:59:59 a 00:00:00).
 */
void test_reloj_avanza_un_dia_completo(void) {
    int ticks_por_segundo = 5;
    clock_t reloj = RelojCreate(ticks_por_segundo, NULL);

    hora_t tiempo_inicial = {2, 3, 5, 9, 5, 9};  // 23:59:59
    hora_t tiempo_esperado = {0, 0, 0, 0, 0, 0}; // 00:00:00
    hora_t tiempo_leido;

    RelojSetupCurrentTime(reloj, tiempo_inicial);

    // Simulamos el paso de 1 segundo
    for (int i = 0; i < ticks_por_segundo; i++) {
        RelojNewTick(reloj);
    }

    RelojGetCurrentTime(reloj, tiempo_leido);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(tiempo_esperado, tiempo_leido, 6);
}

/**
 * @brief Verifica que se puede fijar una hora para la alarma y consultarla
 * correctamente.
 */
void test_reloj_fijar_alarma(void) {
    clock_t reloj = RelojCreate(5, NULL);
    hora_t hora_alarma = {0, 7, 3, 0, 0, 0}; // Las 07:30:00
    hora_t hora_leida;

    // Configuramos la alarma
    RelojSetupAlarm(reloj, hora_alarma);

    // Consultamos la alarma
    RelojGetAlarm(reloj, hora_leida);

    // Verificamos que lo que leemos es exactamente lo que guardamos
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_alarma, hora_leida, 6);
}

/**
 * @brief Verifica que la alarma arranca deshabilitada, y que se puede habilitar
 * y deshabilitar.
 */
void test_reloj_estado_alarma(void) {
    clock_t reloj = RelojCreate(5, NULL);

    // Al iniciar, la alarma debería estar deshabilitada por defecto
    TEST_ASSERT_FALSE(RelojGetAlarmState(reloj));

    // Habilitamos la alarma y verificamos
    RelojSetAlarmState(reloj, true);
    TEST_ASSERT_TRUE(RelojGetAlarmState(reloj));

    // Deshabilitamos la alarma y verificamos
    RelojSetAlarmState(reloj, false);
    TEST_ASSERT_FALSE(RelojGetAlarmState(reloj));
}

/**
 * @brief Verifica que se dispara el callback de la alarma si está habilitada y
 * la hora coincide.
 */
void test_reloj_dispara_alarma(void) {
    // Reseteamos la bandera por si quedó sucia de otro lado
    alarma_sonando = false;

    // IMPORTANTE: Ahora le pasamos nuestra función simulada al crear el reloj
    clock_t reloj = RelojCreate(1, FuncionSimuladaAlarma);

    hora_t tiempo_actual = {0, 7, 2, 9, 5, 9}; // 07:29:59
    hora_t tiempo_alarma = {0, 7, 3, 0, 0, 0}; // 07:30:00

    RelojSetupCurrentTime(reloj, tiempo_actual);
    RelojSetupAlarm(reloj, tiempo_alarma);
    RelojSetAlarmState(reloj, true); // Encendemos la alarma

    // Antes del Tick, la alarma NO debe estar sonando
    TEST_ASSERT_FALSE(alarma_sonando);

    // Hacemos avanzar el tiempo 1 segundo (ahora serán las 07:30:00)
    RelojNewTick(reloj);

    // Al coincidir la hora, la bandera debería haber cambiado a true
    TEST_ASSERT_TRUE(alarma_sonando);
}

/**
 * @brief Verifica que la hora de la alarma se puede posponer en una cantidad
 * arbitraria de minutos.
 */
void test_reloj_posponer_alarma(void) {
    clock_t reloj = RelojCreate(5, NULL);
    hora_t hora_alarma = {1, 2, 3, 4, 0, 0};   // 12:34:00
    hora_t hora_esperada = {1, 2, 3, 9, 0, 0}; // 12:39:00 (5 minutos después)
    hora_t hora_leida;

    RelojSetupAlarm(reloj, hora_alarma);

    // Posponemos la alarma por 5 minutos
    RelojPostponeAlarm(reloj, 5);

    RelojGetAlarm(reloj, hora_leida);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(hora_esperada, hora_leida, 6);
}

/* === End of documentation
 * ==================================================================== */
