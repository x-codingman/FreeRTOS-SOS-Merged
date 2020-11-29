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
		"	mov r4, %2											\n"
		"	mov r5, %3											\n"
		"	mov r6, %4											\n"
		"	mov r7, %5											\n"
		"	push {r7}											\n"
		"	push {r6}											\n"
		"	push {r5}											\n"
		"	push {r4}											\n"
		"	svc %1												\n"
		:"=r"(ret)
		:"i"(num),"r"(a1),"r"(a2),"r"(a3),"r"(a4)
		
	);
}

void syscall_return(int ret){
	syscall(SYS_module_return,ret,3,4,9);
}