/*
 * syscall.c
 *
 * Created: 2020/10/26 21:05:57
 *  Author: dn
 */ 



#include "inc/syscall.h"
#include <stdint.h>


static inline int32_t
syscall(uint8_t num,uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4){
	int32_t ret;
	__asm volatile
	(	
		"	push {r4-r7}										\n"
		"	mov r4, %2											\n"
		"	mov r5, %3											\n"
		"	mov r6, %4											\n"
		"	mov r7, %5											\n"
		"	push {r7}											\n"
		"	push {r6}											\n"
		"	push {r5}											\n"
		"	push {r4}											\n"
		"	svc %1												\n"
		"	add sp, sp, #16										\n"
		"	pop {r4-r7}											\n"
		:"=r"(ret)
		:"i"(num),"r"(a1),"r"(a2),"r"(a3),"r"(a4)
		
	);
}

void syscall_return(int ret){

	syscall(SYS_module_return,ret,3,4,9);
}

void syscall_nsc_call_module(uint32_t module_id,uint32_t command_id,uint32_t *operation){
	syscall(SYS_nsc_call_module, module_id, command_id, operation, 0);
}