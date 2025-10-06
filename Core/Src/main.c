#include "function.h"

int main(void) {


    // Initialize SPI
    SPI_INIT();

    // Example transmit
    spi_transmit(0x55);



    while (1) {
        // main loop
    }
}
