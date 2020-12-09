/*
 * @Author: x-codingman 
 * @Date: 2020-10-15 11:47:14 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-19 20:40:38
 */

#include "inc/test_module.h"
#include "inc/results.h"
#include "inc/manager.h"
#include "inc/module.h"
#include "inc/syscall.h"



static SOS_Manifest_t maniifest;


SOS_Result_t invoke_entry(
    uint32_t command_id,
    SOS_Operation_t  *operation
){
    switch (command_id)
    {
        case CMD_PING:
        {
            printf("l m test module of ping\r\n");
			delay_ms(5000);
            break;

        }
		case CMD_PONG:
		{
			 printf("l m test module of pong\r\n");
			 break;
		}
        default:
        {
            break;
        }
            
    }

     syscall_return(SYS_module_return);
}

SOS_Result_t init_test_module(){
    return add_module(invoke_entry,SOS_TEST_MOUDLE_ID,maniifest);
    
}


