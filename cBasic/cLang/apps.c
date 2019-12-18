/**
modify by cc
**/
#include "apps.h"
#include "app_util.h"
#include "app_key.h"

static unsigned char app_flag = 0;
void app_init(void)
{
	app_set_app_flag(1);
	app_key_init();
}

unsigned char app_get_app_flag(void)
{
	return app_flag;
}

void app_set_app_flag(unsigned char flag)
{
	app_flag = flag;
}
