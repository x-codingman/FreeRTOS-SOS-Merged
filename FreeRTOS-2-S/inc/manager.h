/*
 * @Author: x-codingman 
 * @Date: 2020-10-12 16:47:53 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-11-08 11:21:03
 */

#ifndef SOS_MANAGER_H
#define SOS_MANAGER_H



#define TEEC_CONFIG_PAYLOAD_REF_COUNT 4


#include "module.h"
#include "types.h"
















SOS_Result_t init_modules();

SOS_Result_t SOS_invoke_command(
SOS_ModuleID_t module_id,
uint32_t command_id,
SOS_Operation_t *operation
);

SOS_Result_t check_paramaters(SOS_Operation_t *operation);

SOS_Result_t add_module(SOS_ModuleEntry_t module_entry,SOS_ModuleID_t module_id,SOS_Manifest_t manifest);

void call_module( uint32_t command_id,
SOS_Operation_t *operation,Module_t* module_entry) __attribute__((naked));






 
    









#endif