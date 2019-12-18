/**
modify by cc
**/
#include "app_key.h"
#include "app_util.h"

void app_test_init(void)
{
	static unsigned char test_a = 5, test_b = 6;
	static unsigned int test_add = 0;
    	test_add = app_key_add(test_a, test_b);
       if(test_add > 10){
		test_add = 0;
        	test_a = test_a - 1;
            	test_b = test_a + test_b;
       }
       	
}

