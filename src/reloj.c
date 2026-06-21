#include "reloj.h"
#include <string.h>


struct clock_s {
    bool time_is_valid;
    hora_t current_time;
}



RelojCreate(unsigned int ticks_per_second, void * alarm_handler){
    static struct clock_s instance = (0);
    clock_t self = &instance;
    self->time_is_valid = ; //CONTINUAR completando
}



