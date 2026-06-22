#include "reloj.h"
#include <string.h>

struct clock_s {
    bool time_is_valid;
    hora_t current_time;
    unsigned int ticks_per_second;
    unsigned int ticks_count;
};

clock_t RelojCreate(unsigned int ticks_per_second, void * alarm_handler) {
    static struct clock_s instance = {0}; 
    clock_t self = &instance;
    
    memset(self->current_time, 0, sizeof(hora_t));
    
    self->time_is_valid = false;
    self->ticks_per_second = ticks_per_second;
    self->ticks_count = 0; // <-- Inicializamos en cero
    
    return self;
}

bool RelojGetCurrentTime(clock_t clock, hora_t current_time) {
    
    memcpy(current_time, clock->current_time, sizeof(hora_t));
    
    return clock->time_is_valid; 
}

bool RelojSetupCurrentTime(clock_t clock, const hora_t current_time) {
    // 1. Validamos que el arreglo cumpla con el formato BCD sin compactar (00:00:00 a 23:59:59)
    if (current_time[0] > 2) return false; // Decena de hora no puede ser mayor a 2
    if (current_time[0] == 2 && current_time[1] > 3) return false; // Si es 20 algo, no puede ser mayor a 23
    if (current_time[1] > 9) return false; // Unidad de hora
    if (current_time[2] > 5) return false; // Decena de minuto (máximo 59)
    if (current_time[3] > 9) return false; // Unidad de minuto
    if (current_time[4] > 5) return false; // Decena de segundo (máximo 59)
    if (current_time[5] > 9) return false; // Unidad de segundo

    // 2. Si pasa la validación, recién ahí copiamos la hora
    memcpy(clock->current_time, current_time, sizeof(hora_t));
    
    // 3. Marcamos la hora como válida
    clock->time_is_valid = true;
    
    return true;
}


void RelojNewTick(clock_t clock) {
    clock->ticks_count++;
    
    // Si la cantidad de ticks alcanza los ticks por segundo, avanzamos la hora
    if (clock->ticks_count >= clock->ticks_per_second) {
        clock->ticks_count = 0; // Reiniciamos el contador
        
        clock->current_time[5]++; // Aumentamos la unidad de segundo
        
        // Lógica de acarreo (Cascada BCD)
        if (clock->current_time[5] > 9) {
            clock->current_time[5] = 0;
            clock->current_time[4]++; // Suma decena de segundo
            
            if (clock->current_time[4] > 5) {
                clock->current_time[4] = 0;
                clock->current_time[3]++; // Suma unidad de minuto
                
                if (clock->current_time[3] > 9) {
                    clock->current_time[3] = 0;
                    clock->current_time[2]++; // Suma decena de minuto
                    
                    if (clock->current_time[2] > 5) {
                        clock->current_time[2] = 0;
                        clock->current_time[1]++; // Suma unidad de hora
                        
                        // Lógica especial para el límite de 24 horas
                        if ((clock->current_time[0] == 2 && clock->current_time[1] > 3) || 
                            (clock->current_time[0] < 2 && clock->current_time[1] > 9)) {
                            
                            clock->current_time[1] = 0;
                            clock->current_time[0]++; // Suma decena de hora
                            
                            // Si llega a las 24:00:00, vuelve a 00:00:00
                            if (clock->current_time[0] > 2) {
                                clock->current_time[0] = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}