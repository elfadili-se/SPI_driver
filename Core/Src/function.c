#include "function.h"
#include "stm32f4xx.h"

void SPI_INIT(void){

    //-----------------------------------
    // 1) Activer l'horloge GPIOA et SPI1
    //-----------------------------------
    RCC->AHB1ENR |= (1<<0);    // GPIOA clock enable
    RCC->APB2ENR |= (1<<12);   // SPI1 clock enable

    //-----------------------------------
    // 2) Configurer PA4, PA5, PA6, PA7 en Alternate Function (AF5 pour SPI1)
    //-----------------------------------
    // Effacer les bits MODER pour PA4..PA7
    GPIOA->MODER &= ~(0b11<<(4*2) | 0b11<<(5*2) | 0b11<<(6*2) | 0b11<<(7*2));
    // Mettre Alternate Function (10)
    GPIOA->MODER |=  (0b10<<(4*2) | 0b10<<(5*2) | 0b10<<(6*2) | 0b10<<(7*2));

    // Configurer l'Alternate Function : AF5 = 0101
    // AFRL : 4 bits par pin => PA4=bits[19:16], PA5=[23:20], PA6=[27:24], PA7=[31:28]
    GPIOA->AFR[0] &= ~(0xFFFF << 16);        // Clear AF for pins 4..7
    GPIOA->AFR[0] |=  (0x5555 << 16);        // AF5 (0101) pour PA4..PA7

    // Mettre Very High Speed pour ces pins
    GPIOA->OSPEEDR |= (0b11<<(4*2) | 0b11<<(5*2) | 0b11<<(6*2) | 0b11<<(7*2));

    // Push-pull
    GPIOA->OTYPER &= ~(0b1111<<4);

    // Pas de pull-up / pull-down
    GPIOA->PUPDR &= ~(0b11<<(4*2) | 0b11<<(5*2) | 0b11<<(6*2) | 0b11<<(7*2));

    //-----------------------------------
    // 3) Configurer SPI1->CR1
    //-----------------------------------
    /*
       Bits importants :
       - MSTR = 1 => mode maître
       - BR[2:0] = prescaler (ici /16)
       - CPOL=0, CPHA=0 => mode SPI (mode 0)
       - SSM=1 et SSI=1 => NSS géré en logiciel
       - DFF=0 => 8 bits
       - SPE=1 => activer SPI
    */
    SPI1->CR1 = 0;              // Reset CR1

    SPI1->CR1 |= (1<<2);        // MSTR = 1 (maître)
    SPI1->CR1 |= (0b011<<3);    // BR = fPCLK/16
    SPI1->CR1 &= ~(1<<11);      // DFF=0 (8 bits)
    SPI1->CR1 &= ~(1<<1);       // CPHA=0
    SPI1->CR1 &= ~(1<<0);       // CPOL=0
    SPI1->CR1 |= (1<<9);        // SSM = 1 (NSS logiciel)
    SPI1->CR1 |= (1<<8);        // SSI = 1 (force NSS high)
    SPI1->CR1 |= (1<<6);        // SPE = 1 (activer SPI)

    //-----------------------------------
    // 4) CR2 inutile en mode bloquant simple
    //-----------------------------------
    // SPI1->CR2 = 0;
}



//   ENVOYER UN OCTET EN SPI1    // Attendre que la réception soit terminée (RXNE=1)

//=============================
void spi_transmit(char data){

    // Attendre que le buffer TX soit vide (TXE=1)
    while (!(SPI1->SR & (1<<1)));

    // Ecrire la donnée dans le Data Register
    SPI1->DR = data;

    // Attendre que la réception soit terminée (RXNE=1)    // Attendre que la réception soit terminée (RXNE=1)

    while (!(SPI1->SR & (1<<0)));

    // Lire DR pour vider le buffer (valeur reçue en même temps que l'envoi)
    volatile uint8_t dummy = SPI1->DR;
    (void)dummy; // éviter un warning

    // Attendre que le SPI ne soit plus occupé (BSY=0)
    while (SPI1->SR & (1<<7));
}

uint8_t spi_receive(void) {
    // Attendre que des données soient reçues (RXNE = 1)
    while (!(SPI1->SR & (1 << 0)));

    // Lire les données reçues
    return SPI1->DR;
}







