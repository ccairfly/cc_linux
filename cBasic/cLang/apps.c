/**
modify by cc
**/
#include "apps.h"
#include "app_util.h"

static bool app_flag = false;
void app_init(void)
{
	static uint8_t arg1;
    	
}

bool app_get_app_flag(void)
{
	return app_flag;
}

void app_set_app_flag(bool flag)
{
	app_flag = flag;
}
