# 🖥️ STM32F4 SPI Driver#
## Description

This project is a bare-metal SPI driver for STM32F4 microcontrollers.
It allows sending and receiving data over SPI1 without using the HAL library. The driver is built from scratch by directly accessing STM32 registers.

## Features

⚙️ SPI Initialization: Configures SPI1 in master mode with software-managed NSS.

📡 Data Transmission: Send bytes using spi_transmit().

📡 Data Reception: Receive bytes using spi_receive().

🛠️ Bare-Metal Implementation: No HAL or CubeMX code; everything is done via direct register manipulation.

## Technologies Used

C programming language

STM32F4 microcontrollers

STM32CubeIDE (development environment)

## How It Works

SPI1 is initialized by setting up the GPIO pins (PA4-PA7) in alternate function mode.

The SPI control registers are configured for master mode, clock polarity, phase, and baud rate.

spi_transmit() waits for the transmit buffer to be empty, writes data to the SPI data register, and waits until the SPI is no longer busy.

spi_receive() sends a dummy byte to generate clock and reads the received data from the SPI data register.

Users can repeatedly send and receive bytes in a loop as needed for SPI communication.

## Author

👨‍💻 Developed by Elfadili, embedded engineering student passionate about bare-metal programming and low-level C development.
