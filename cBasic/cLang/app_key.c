/**
modify by cc
**/
#include "app_key.h"

static bool app_key_init_flag = false;
void app_key_init(void)
{
    	app_set_key_init_flag(true);
}

bool app_get_key_init_flag(void)
{
	return app_key_init_flag;
}

void app_set_key_init_flag(bool flag)
{
	app_key_init_flag = flag;
}

uint32_t app_key_add(uint8_t a, uint8_t b)
{
	return a + b;
}

uint32_t app_key_sub(uint8_t a, uint8_t b)
{
	return a - b;
}

uint32_t app_key_mul(uint8_t a, uint8_t b)
{
	return a * b;
}

uint32_t app_key_div(uint8_t a, uint8_t b)
{
	return a / b;
}
