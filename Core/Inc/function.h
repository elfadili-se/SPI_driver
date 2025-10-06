#ifndef FUNCTION_H
#define FUNCTION_H

#include <stdint.h>

// Déclarations des fonctions
void SPI_INIT(void);
void spi_transmit(char data);
uint8_t spi_receive();


#endif
