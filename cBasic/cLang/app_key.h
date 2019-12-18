/*
modify by cc
**/
#ifndef APP_KEY_H__
#define APP_KEY_H__

#include "app_util.h"

void app_key_init(void);
bool app_get_key_init_flag(void);
void app_set_key_init_flag(bool flag);

uint32_t app_key_add(uint8_t a, uint8_t b);
uint32_t app_key_sub(uint8_t a, uint8_t b);
uint32_t app_key_mul(uint8_t a, uint8_t b);
uint32_t app_key_div(uint8_t a, uint8_t b);

#endif

