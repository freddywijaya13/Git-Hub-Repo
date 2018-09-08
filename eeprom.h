#ifndef _EEPROM_H    /* Guard against multiple inclusion */
#define _EEPROM_H
//Command
#define DUMMY_BYTE                      0x00
#define STATUS_REGISTER1                0X05
#define STATUS_REGISTER2                0X35
#define STATUS_REGISTER3                0X33
#define WRITE_ENABLE                    0X06
#define WRITE_ENABLE_VOLATILE           0X50
#define WRITE_DISABLE                   0X04
#define WRITE_REGISTER                  0X01
#define PAGE_PROGRAM                    0X02
#define SECTOR_ERASE                    0X20
#define BLOCK_ERASE                     0X08
#define CHIP_ERASE                      0XC7
#define READ_DATA                       0X03
#define FAST_READ                       0X0B
#define FAST_READ_DUAL                  0X3B
#define FAST_READ_QUAD                  0X6B
#define READ_JEDEC_ID                   0X9F

void EEPROM_init(int baud_rate);
unsigned char EEPROM_write_byte(unsigned char byte);
unsigned char EEPROM_read_status(unsigned short status_num);
void EEPROM_wait_busy();
void EEPROM_write_enable();
void EEPROM_read_JEDEC_ID(unsigned char *buff);
void EEPROM_page_program(const char *buff,unsigned int start_address, unsigned long size);
void EEPROM_read_data(char *buff,unsigned int start_address, unsigned short size);
void EEPROM_fast_read_data(char *buff,unsigned int start_address, unsigned short size);
void EEPROM_erase_chip();

#endif /* _EXAMPLE_FILE_NAME_H */
