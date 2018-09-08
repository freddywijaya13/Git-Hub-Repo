#include <xc.h>
#include "config.h"
#include "spi.h"
#include "eeprom.h"

void EEPROM_init(int baud_rate){
    SPI_init(baud_rate);
}
unsigned char EEPROM_write_byte(unsigned char byte){
    while(!SPI1STATbits.SPITBE);	// wait for TX buffer to be empty
    SPI1BUF = byte;
    while(!SPI1STATbits.SPIRBF);	// wait for RX buffer to be empty
    return SPI1BUF;
}
unsigned char EEPROM_read_status(unsigned short status_num){
    lat_SPIFLASH_CE = 0;
    switch(status_num){
        case 1:
            EEPROM_write_byte(STATUS_REGISTER1);
            break;
        case 2:
            EEPROM_write_byte(STATUS_REGISTER2);
            break;
        case 3:
            EEPROM_write_byte(STATUS_REGISTER3);
            break;
    }
    unsigned char busy_status = EEPROM_write_byte(DUMMY_BYTE);
    lat_SPIFLASH_CE = 1;
    return busy_status;
}
void EEPROM_wait_busy(){
    while(EEPROM_read_status(1) & 0x01 == 1);
}
void EEPROM_write_enable(){
    lat_SPIFLASH_CE = 0;
    EEPROM_write_byte(WRITE_ENABLE);
    lat_SPIFLASH_CE = 1;
    EEPROM_wait_busy();
}
void EEPROM_read_JEDEC_ID(unsigned char *buff){
    lat_SPIFLASH_CE = 0;
    EEPROM_write_byte(READ_JEDEC_ID);
    buff[0] = EEPROM_write_byte(DUMMY_BYTE);
    buff[1] = EEPROM_write_byte(DUMMY_BYTE);
    buff[2] = EEPROM_write_byte(DUMMY_BYTE);
    lat_SPIFLASH_CE = 1;
    EEPROM_wait_busy();
}
void EEPROM_page_program(const char *buff,unsigned int start_address, unsigned long size){
    EEPROM_wait_busy();
    EEPROM_write_enable();
    lat_SPIFLASH_CE = 0;
    unsigned short address1 = (start_address >> 16) & 0xFF;
    unsigned short address2 = (start_address >> 8) & 0XFF;
    unsigned short address3 = start_address & 0XFF;
    if(size == 0xFF)
        address3 = 0X00;
    EEPROM_write_byte(PAGE_PROGRAM);
    EEPROM_write_byte(address1);
    EEPROM_write_byte(address2);
    EEPROM_write_byte(address3);
    unsigned long i = 0;
    for(i;i<size;i++)
        EEPROM_write_byte(buff[i]);
    lat_SPIFLASH_CE = 1;
    EEPROM_wait_busy();
}
void EEPROM_read_data(char *buff,unsigned int start_address, unsigned short size){
    EEPROM_wait_busy();
    lat_SPIFLASH_CE = 0;
    unsigned short address1 = (start_address >> 16) & 0xFF;
    unsigned short address2 = (start_address >> 8) & 0XFF;
    unsigned short address3 = start_address & 0XFF;
    EEPROM_write_byte(READ_DATA);
    EEPROM_write_byte(address1);
    EEPROM_write_byte(address2);
    EEPROM_write_byte(address3);
    int i = 0;
    for(i;i<size;i++)
        buff[i] = EEPROM_write_byte(DUMMY_BYTE);
    lat_SPIFLASH_CE = 1;
    EEPROM_wait_busy();
}
void EEPROM_fast_read_data(char *buff,unsigned int start_address, unsigned short size){
    //EEPROM_wait_busy();
    lat_SPIFLASH_CE = 0;
    unsigned short address1 = (start_address >> 16) & 0xFF;
    unsigned short address2 = (start_address >> 8) & 0XFF;
    unsigned short address3 = start_address & 0XFF;
    EEPROM_write_byte(FAST_READ);
    EEPROM_write_byte(address1);
    EEPROM_write_byte(address2);
    EEPROM_write_byte(address3);
    EEPROM_write_byte(DUMMY_BYTE);
    int i = 0;
    for(i;i<size;i++)
        buff[i] = EEPROM_write_byte(DUMMY_BYTE);
    lat_SPIFLASH_CE = 1;
    //EEPROM_wait_busy();
}
void EEPROM_erase_chip(){
    EEPROM_wait_busy();
    EEPROM_write_enable();
    lat_SPIFLASH_CE = 0;
    EEPROM_write_byte(CHIP_ERASE);
    lat_SPIFLASH_CE = 1;
    EEPROM_wait_busy();
}