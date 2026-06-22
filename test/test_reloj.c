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

// Al inicializar el reloj esta en 00:00:00 y con hora invalida
void test_reloj_inicia_invalido(void){
    clock_t reloj;
    
    // Llenamos el arreglo con basura para asegurarnos de que la funcion lo sobrescribe
    hora_t hora_actual = {1, 2, 3, 4, 5, 6}; 

    // Configuramos el reloj 
    reloj = RelojCreate(1, NULL);

    // La funcion GetCurrentTime debe devolver FALSE porque la hora es invalida 
    TEST_ASSERT_FALSE(RelojGetCurrentTime(reloj, hora_actual));
    
    // Además, verificamos que el reloj haya devuelto la hora por defecto (00:00:00)
    TEST_ASSERT_EQUAL_UINT8_ARRAY(DEFAULT_TIME, hora_actual, 6);
}