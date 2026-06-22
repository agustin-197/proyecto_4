#include "reloj.h"
#include <string.h>

struct clock_s {
    bool time_is_valid;
    hora_t current_time;
    unsigned int ticks_per_second;
    unsigned int ticks_count;
    hora_t alarm_time;
    bool alarm_enabled;
    void (*alarm_handler)(void);
};

clock_t RelojCreate(unsigned int ticks_per_second, void * alarm_handler) {
    static struct clock_s instance = {0}; 
    clock_t self = &instance;
    
    memset(self->current_time, 0, sizeof(hora_t));
    
    self->time_is_valid = false;
    self->ticks_per_second = ticks_per_second;
    self->ticks_count = 0;
    self->alarm_enabled = false;
    self->alarm_handler = (void (*)(void))alarm_handler; 
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

        // Verificación de la alarma: se hace una vez que la hora ya se actualizó
        if (clock->alarm_enabled && clock->alarm_handler != NULL) {
            // memcmp compara dos bloques de memoria, devuelve 0 si son idénticos
            if (memcmp(clock->current_time, clock->alarm_time, sizeof(hora_t)) == 0) {
                // Si la alarma está habilitada y la hora coincide, disparamos el evento
                clock->alarm_handler();
            }
        }

    } 
}

void RelojSetupAlarm(clock_t clock, const hora_t alarm_time) {
    // Copiamos la hora configurada a nuestra estructura interna
    memcpy(clock->alarm_time, alarm_time, sizeof(hora_t));
}

void RelojGetAlarm(clock_t clock, hora_t alarm_time) {
    // Copiamos la hora de la estructura interna al arreglo solicitado
    memcpy(alarm_time, clock->alarm_time, sizeof(hora_t));
}

void RelojSetAlarmState(clock_t clock, bool state) {
    clock->alarm_enabled = state;
}

bool RelojGetAlarmState(clock_t clock) {
    return clock->alarm_enabled;
}

void RelojPostponeAlarm(clock_t clock, unsigned int delay_minutes) {
    // 1. Convertimos la hora de la alarma a minutos totales desde la medianoche
    unsigned int minutos_totales = (clock->alarm_time[0] * 600) + 
                                   (clock->alarm_time[1] * 60) + 
                                   (clock->alarm_time[2] * 10) + 
                                   (clock->alarm_time[3]);

    // 2. Le sumamos los minutos de demora
    minutos_totales += delay_minutes;

    // 3. Aplicamos módulo para que, si pasa de las 23:59, vuelva a empezar el día (24 horas = 1440 minutos)
    minutos_totales %= (24 * 60);

    // 4. Convertimos esos minutos totales nuevamente a formato BCD
    clock->alarm_time[0] = minutos_totales / 600;                 // Decena de hora
    clock->alarm_time[1] = (minutos_totales / 60) % 10;           // Unidad de hora
    clock->alarm_time[2] = (minutos_totales % 60) / 10;           // Decena de minuto
    clock->alarm_time[3] = (minutos_totales % 60) % 10;           // Unidad de minuto
    
    // Los segundos (posiciones 4 y 5) no se tocan porque posponemos por minutos cerrados
}