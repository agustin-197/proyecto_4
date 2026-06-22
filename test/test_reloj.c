//CONSIGNAS A SEGUIR
//Al ajustar la hora el reloj queda en hora y es valida
//Despues de n ciclos de reloj la hora avanza un segundo, diez
//y un dia completo . fijar la hora de la alarma y consultarla.
//la alarma, deshabilitada y avanzar el reloj para no suene
//y cancelarla hasta el otro dia.
//probar que el create no devuelve algo nulo
//probar que no se pone en hora si el argumento es erroneo
//decidir que sucede con el reloj desconfigurado y el avance de la hora

#include "unity.h"
#include "reloj.h"

static const hora_t DEFAULT_TIME = {0, 0, 0, 0, 0, 0};

// Al ajustar la hora el reloj queda en hora y es valida
void test_reloj_ajustar_hora(void){
    clock_t reloj = RelojCreate(1, NULL);
    hora_t nueva_hora = {1, 2, 3, 4, 5, 6}; // Representa las 12:34:56
    hora_t hora_leida;

    // Configuramos la nueva hora y esperamos que la funcion devuelva true (indicando éxito)
    TEST_ASSERT_TRUE(RelojSetupCurrentTime(reloj, nueva_hora));

    // Al consultar la hora ahora, debe devolver true (hora válida)
    TEST_ASSERT_TRUE(RelojGetCurrentTime(reloj, hora_leida));
    
    // El arreglo devuelto debe coincidir exactamente con la hora que configuramos
    TEST_ASSERT_EQUAL_UINT8_ARRAY(nueva_hora, hora_leida, 6);
}

// Probar que no se pone en hora si el argumento es erroneo
void test_ajuste_hora_invalida(void) {
    clock_t reloj = RelojCreate(1, NULL);
    hora_t hora_invalida = {2, 5, 0, 0, 0, 0}; // Las 25:00:00 (Inválido)
    hora_t hora_leida;

    // Esperamos que devuelva false porque la hora es inválida
    TEST_ASSERT_FALSE(RelojSetupCurrentTime(reloj, hora_invalida));

    // El reloj debería seguir con hora inválida porque rechazó el ajuste
    TEST_ASSERT_FALSE(RelojGetCurrentTime(reloj, hora_leida));
}

// Despues de n ciclos de reloj la hora avanza un segundo
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

// La hora avanza al siguiente minuto (cambio de decena de segundos)
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

// La hora avanza un dia completo (reinicio a medianoche)
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