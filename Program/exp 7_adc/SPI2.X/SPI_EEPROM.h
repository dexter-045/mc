#define SPI_CS      LATAbits.LATA5  // Chip Select Pin on Serial EEPROM
#define EEPROM_HOLD LATEbits.LE1  // Chip Select Pin on Serial EEPROM

#define SPIEEPROM_WRSR  0x01
#define SPIEEPROM_WRITE 0x02
#define SPIEEPROM_READ  0x03
#define SPIEEPROM_WRDI  0x04
#define SPIEEPROM_RDSR  0x05
#define SPIEEPROM_WREN  0x06


void SPI_Transmit(unsigned char);
unsigned char SPI_Receive();
void InitSPI();