/*
 * @Author: x-codingman 
 * @Date: 2020-10-17 12:51:20 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-17 12:58:29
 */

#include "sos_portasm.h"

int is_privileged( void ) 
{
	__asm volatile
	(
	"	mrs r0, control									\n" /* r0 = CONTROL. */
	"	movs r1, #1										\n" /* r1 = 1. */
	"	tst r0, r1										\n" /* Perform r0 & r1 (bitwise AND) and update the conditions flag. */
	"	beq running_privileged							\n" /* If the result of previous AND operation was 0, branch. */
	"	movs r0, #0										\n" /* CONTROL[0]!=0. Return false to indicate that the processor is not privileged. */
	"	bx lr											\n" /* Return. */
	" running_privileged:								\n"
	"	movs r0, #1										\n" /* CONTROL[0]==0. Return true to indicate that the processor is privileged. */
	"	bx lr											\n" /* Return. */
	"													\n"
	"	.align 4										\n"
	::: "r0", "r1", "memory"
	);
}

void reset_privilege( void ) 
{
	__asm volatile
	(
	"	mrs r0, control									\n" /* r0 = CONTROL. */
	"	movs r1, #1										\n" /* r1 = 1. */
	"	orr r0, r1										\n" /* r0 = r0 | r1. */
	"	msr control, r0									\n" /* CONTROL = r0. */
	"	bx lr											\n" /* Return to the caller. */
	:::"r0", "r1", "memory"
	);
}

void raise_privilege( void ) /* __attribute__ (( naked )) PRIVILEGED_FUNCTION */
{
	__asm volatile
	(
	"	mrs r0, control									\n" /* Read the CONTROL register. */
	"	movs r1, #1										\n" /* r1 = 1. */
	//bics
	"	bic r0, r1										\n" /* Clear the bit 0. */
	"	msr control, r0									\n" /* Write back the new CONTROL value. */
	"	bx lr											\n" /* Return to the caller. */
	::: "r0", "r1", "memory"
	);
}

void SVCall_Handler( void ) /* __attribute__ (( naked )) PRIVILEGED_FUNCTION */
{
	__asm volatile
	(
	"	movs r0, #4										\n"
	"	mov r1, lr										\n"
	"	tst r0, r1										\n"
	"	beq stacking_used_msp							\n"
	"	mrs r0, psp										\n"
	"	ldr r2, svchandler_address_const				\n"
	"	bx r2											\n"
	" stacking_used_msp:								\n"
	"	mrs r0, msp										\n"
	"	ldr r2, svchandler_address_const				\n"
	"	bx r2											\n"
	"													\n"
	"	.align 4										\n"
	"svchandler_address_const: .word vPortSVCHandler_C	\n"
	);
}