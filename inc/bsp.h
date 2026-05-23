#ifndef BSP_H_
#define BSP_H_

#include "digital.h"


typedef struct board_s
{
    digital_output_t led_rojo;
    digital_output_t led_verde;
    digital_output_t led_azul;
    digital_output_t led_1;
    digital_output_t led_2;
    digital_output_t led_3;

    digital_input_t tecla_1;
    digital_input_t tecla_2;
    digital_input_t tecla_3;
    digital_input_t tecla_4;

}const * board_t;

//Funcion principal para inicializar toda la placa
board_t BoardCreate(void);

#endif
