#include "reloj.h"
#include <string.h>

struct clock_s {
    bool time_is_valid;
    hora_t current_time;
    unsigned int ticks_per_second;
};

clock_t RelojCreate(unsigned int ticks_per_second, void * alarm_handler) {
    // Inicializamos toda la estructura en 0/false por defecto
    static struct clock_s instance = {0}; 
    clock_t self = &instance;
    
    self->time_is_valid = false;
    self->ticks_per_second = ticks_per_second;
    
    return self;
}

bool RelojGetCurrentTime(clock_t clock, hora_t current_time) {
    // Código temporal (Stub) para que el programa compile
    return false; 
}
