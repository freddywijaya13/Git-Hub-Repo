#include <xc.h>
#include "config.h"
#include "spi.h"

void SPI_configure_registers(int baud_rate){
    // configures SPI1
    SPI1CONbits.ON = 0;
    SPI1BRG = (unsigned short)(PB_FRQ / (2 * baud_rate) - 1);
    SPI1STATbits.SPIROV = 0;
    SPI1CONbits.CKE = 1;   // SPI Clock Edge   
    SPI1CONbits.MSTEN = 1; 
    SPI1CONbits.ON = 1;       // enable SPI
    
}
void SPI_configure_pins(){
    // Configure SPIFLASH signals as digital outputs.
    tris_SPIFLASH_CE = 0;
    tris_SPIFLASH_SCK = 0;
    tris_SPIFLASH_SI = 0;
    
    // Configure SPIFLASH signals as digital inputs.
    tris_SPIFLASH_SO = 1;
    
    // configure remapable pins
    rp_SPIFLASH_SI = 0x08; // RPF2R = 1000 = SDO1 
    rp_SPIFLASH_SO = 0x0F;// SDI1R = 1111 = RPF7
    
}
void SPI_init(int baud_rate){
    SPI_configure_pins();
    SPI_configure_registers(baud_rate);
}