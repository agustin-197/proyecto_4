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

static const hora_t DEFAULT_TIME = {0,0,0,0,0,0};
//static const hora_t DEFAULT_TIME = {2,3,5,9,5,9};
static const hora_t DEFAULT_TIME = {1,2,3,4,5,6};

//Al inicializar el reloj esta en 00:00 y con hora invalida
void test_reloj_inicia_invalido(void){
    clock_t reloj;
    hora_t hora_actual = {1,2,3,4,5,6};
    bool es_valida = true;

    reloj = RelojCreate(1, NULL);
    //la funcion devuelve FALSE porque la hora no es valida
    TEST_ASSERT_FALSE(GetCurrentTime(reloj, hora_actual););
    TEST_ASSERT_EQUAL_UINT8_ARRAY(DEFAULT_TIME, hora_actual ,6);
}

//Al ajustar la hora el reloj queda en hora y es valida
void test_ajuste_de_hora(void){
    clock_t reloj;
    hora_t hora_actual = {1,2,3,4,5,6};
    bool es_valida = true;

    reloj = RelojCreate(1, NULL);
    
    TEST_ASSERT_TRUE(RelojSetupCurrentTime(reloj, nueva_hora));
    //LA FUNCION DEVUELVE TRUE PORQUE LA HORA ES VALIDA
    TEST_ASSERT_TRUE(RelojSetupCurrentTime(reloj, nueva_hora));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(INITIAL_TIME, hora_actual ,6);

}

//Despues de n ciclos de reloj la hora avanza un segundo, diez

void test_avanza_un_segundo(void){
    clock_t reloj;
    hora_t hora_actual;
    static const hora_t EXPECTED_TIME = {1,2,3,4,5,6};

    reloj = RelojCreate(3, NULL);
    (void)RelojSetupCurrentTime(reloj, INITIAL_TIME);
    
    for (size_t indice = 0; indice < 3; indice++)
    {
        RelojNewTick(reloj);
    }
    RelojGetCurrentTime(reloj, hora_actual);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(EXPECTED_TIME,hora_actual, 6);
    
    
}
