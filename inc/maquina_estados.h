#ifndef MAQUINA_ESTADOS_H_
#define MAQUINA_ESTADOS_H_

#include "placa.h"
#include "reloj.h"

/**
 * @brief Inicializa la máquina de estados
 * @param b Puntero a la placa inicializada
 * @param r Puntero al reloj inicializado
 */
void MaquinaEstados_Init(board_t b, clock_t r);

/**
 * @brief Función que debe ser llamada desde la interrupción del SysTick (cada 1 ms)
 */
void MaquinaEstados_Tick(void);

/**
 * @brief Bucle principal de la máquina de estados. Lee teclas y actualiza la pantalla.
 */
void MaquinaEstados_Update(void);

#endif /* MAQUINA_ESTADOS_H_ */