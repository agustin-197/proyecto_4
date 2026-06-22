#ifndef RELOJ_H_
#define RELOJ_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct clock_s * clock_t;
typedef uint8_t hora_t[6]; // BCD: Decena Hora, Unidad Hora, Dec. Minuto, Un. Minuto, Dec. Segundo, Un. Segundo

clock_t RelojCreate(unsigned int ticks_per_second, void * alarm_handler);
bool RelojGetCurrentTime(clock_t clock, hora_t current_time);
bool RelojSetupCurrentTime(clock_t clock, const hora_t current_time);
void RelojNewTick(clock_t clock); // Solo la firma de la función
void RelojSetupAlarm(clock_t clock, const hora_t alarm_time);
void RelojGetAlarm(clock_t clock, hora_t alarm_time);
void RelojSetAlarmState(clock_t clock, bool state);
bool RelojGetAlarmState(clock_t clock);
void RelojPostponeAlarm(clock_t clock, unsigned int delay_minutes);

#endif /* RELOJ_H_ */