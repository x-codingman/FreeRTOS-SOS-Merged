


#include "inc/manager.h"
#include "inc/results.h"
#include "inc/types.h"
#include "inc/test_module.h"

SOS_Result_t kernel_init(){
	init_stack();
    init_modules();
    init_test_module();
    

}





