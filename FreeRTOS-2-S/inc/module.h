/*
 * @Author: x-codingman 
 * @Date: 2020-10-14 21:15:13 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-16 15:53:01
 */

#ifndef SOS_MODULE_H
#define SOS_MODULE_H


#include "types.h"




/*

*/




typedef struct {
	void *buffer;
	uint32_t size;
} SOS_TempMemoryReference_t;

typedef struct {
	uint32_t a;
	uint32_t b;
} SOS_Value_t;

typedef struct SOS_Manifest{
	int a;      //wait to implemented
}SOS_Manifest_t;


typedef uint32_t SOS_ModuleID_t;

typedef union {
	SOS_TempMemoryReference_t tmpref;
	SOS_Value_t value;
} SOS_Parameter_t;

typedef struct SOS_Operation{
	uint32_t paramTypes;
	SOS_Parameter_t params[TEEC_CONFIG_PAYLOAD_REF_COUNT];
	
}SOS_Operation_t;

typedef void (*SOS_ModuleEntry_t) (uint32_t command_id, SOS_Operation_t  *operation);

typedef struct Module{
	SOS_ModuleID_t module_id;
	SOS_Manifest_t module_manifest;
	SOS_ModuleEntry_t module_entry;
	StackType_t * module_stack;
	struct Module *module_next;
}Module_t;










#endif