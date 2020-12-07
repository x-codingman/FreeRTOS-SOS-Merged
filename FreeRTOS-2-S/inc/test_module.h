/*
 * @Author: x-codingman 
 * @Date: 2020-10-15 16:51:38 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-16 15:54:56
 */
#ifndef SOS_TEST_MODULE_H
#define SOS_TEST_MODULE_H

#include "manager.h"
#include "types.h"

#define SOS_TEST_MOUDLE_ID 0x1234 

#define CMD_PING 1
#define CMD_PONG 2 


SOS_Result_t invoke_entry(uint32_t commandID,SOS_Operation_t  *operation) __attribute__ (( naked )) ;
SOS_Result_t init_test_module();

#endif