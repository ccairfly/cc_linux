/**
modify by cc
**/
#include "app_key.h"
#include "app_util.h"

void app_led_init(void)
{
	static unsigned char led_a = 100, led_b = 6;
	static unsigned int led_sub = 0;
    	led_sub = app_key_sub(led_a, led_b);
       if(led_sub > 10){
		led_sub = 0;
        	led_a = led_a - 1;
            	led_b = led_b + led_a;
       }
       	
}