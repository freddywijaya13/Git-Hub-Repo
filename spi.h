#ifndef _SPI_H
#define _SPI_H

void SPI_configure_pins();
void SPI_configure_registers(int baud_rate);
void SPI_init(int baud_rate);

#endif
