#define RTC_ADD 0xD0

void i2c_start(void);
void i2c_restart(void);
void i2c_device(void);
unsigned char i2c_read(void);
void i2c_write(unsigned int data);
void i2c_stop(void);
void i2c_init();
void i2c_interface_init();
void i2c_interface_deinit();