/**
modify by cc
**/
#include "app_key.h"
#include "app_util.h"

void app_batt_init(void)
{
	static uint8_t batt_a = 2, batt_b = 3;
	static uint32_t batt_mul = 0;
    	batt_mul = app_key_mul(batt_a, batt_b);
       if(batt_mul > 10){
		batt_mul = 0;
        	batt_a = batt_a - 1;
            	batt_b = batt_b + batt_a;
       }
       	
}