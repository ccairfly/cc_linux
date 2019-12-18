/**
modify by cc
**/
#include "apps.h"
#include "app_key.h"
#include "app_util.h"

void app_factory_init(void)
{
	if(app_get_key_init_flag() && app_get_app_flag())
     		app_set_app_flag(0);
}