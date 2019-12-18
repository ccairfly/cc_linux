/**
modify by cc
**/
#include "app_key.h"

static unsigned char app_key_init_flag = 0;
void app_key_init(void)
{
    	app_set_key_init_flag(1);
}

unsigned char app_get_key_init_flag(void)
{
	return app_key_init_flag;
}

void app_set_key_init_flag(unsigned char flag)
{
	app_key_init_flag = flag;
}

unsigned int app_key_add(unsigned char a, unsigned char b)
{
	return a + b;
}

unsigned int app_key_sub(unsigned char a, unsigned char b)
{
	return a - b;
}

unsigned int app_key_mul(unsigned char a, unsigned char b)
{
	return a * b;
}

unsigned int app_key_div(unsigned char a, unsigned char b)
{
	return a / b;
}
