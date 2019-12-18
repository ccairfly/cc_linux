/**
modify by cc
**/
#include "app_key.h"
#include "app_util.h"

void app_au_init(void)
{
	static uint8_t au_a = 100, au_b = 2;
	static uint32_t au_div = 0;
    	au_div = app_key_div(au_a, au_b);
       if(au_div > 10){
		au_div = 0;
        	au_a--;
            	au_b++;
       }
}