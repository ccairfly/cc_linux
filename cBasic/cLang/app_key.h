/*
modify by cc
**/
#ifndef APP_KEY_H__
#define APP_KEY_H__

void app_key_init(void);
unsigned char app_get_key_init_flag(void);
void app_set_key_init_flag(unsigned char flag);

unsigned int app_key_add(unsigned char a, unsigned char b);
unsigned int app_key_sub(unsigned char a, unsigned char b);
unsigned int app_key_mul(unsigned char a, unsigned char b);
unsigned int app_key_div(unsigned char a, unsigned char b);

#endif

