/*
 * memory.c
 *
 * Created: 2020/11/9 13:43:18
 *  Author: dn
 */ 


#include "inc/memory.h"
#include "secure_heap.h"

void* memory_alloc(size_t size){
	return pvPortMalloc(size);
}

void memory_free(void *pv){
	vPortFree(pv);
}


