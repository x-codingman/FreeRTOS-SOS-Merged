/*
 * @Author: x-codingman 
 * @Date: 2020-10-17 12:56:54 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-20 13:00:17
 */


#include "sos_port.h"
#include "inc/SecureOS.h"
#include "inc/syscall.h"
#include "sos_portasm.h"
#include "inc/types.h"
#include "inc/manager.h"
#include "cmsis_gcc.h"

extern struct CallModuleFrame* call_moudle_frame_head;
extern struct CallModuleFrame* cur_call_moudle_frame;
void sos_return(struct CallModuleFrame* index)__attribute__((naked));
void vPortSVCHandler_C( uint32_t *pulCallerStackAddress ) /* PRIVILEGED_FUNCTION portDONT_DISCARD */
{

uint8_t ucSVCNumber;
uint32_t ulPC;

	 /* Register are stored on the stack in the following order - R0, R1, R2, R3,
     * R12, LR, PC, xPSR. */
	ulPC = pulCallerStackAddress[ 6 ];
	ucSVCNumber = ( ( uint8_t *) ulPC )[ -2 ];

	uint32_t a1 = pulCallerStackAddress[8];
	uint32_t a2 = pulCallerStackAddress[9];
	uint32_t a3 = pulCallerStackAddress[10];
	uint32_t a4 = pulCallerStackAddress[11];

	switch( ucSVCNumber )
	{

// Return the kernel excution

// In this case, the syscall will not return the caller

// Step 1: Switch to the extra kernel stack
// Step 2: Pop the context of the last kernel excution to the registers
// Step 3: exit exception

		case SYS_module_return:
		{	
			uint32_t call_ID_from_stack;
			StackType_t* module_stack;
			struct CallModuleFrame* index;
			
			//__asm volatile(
				//"   push {r3-r4}									\n"
				//"	mrs r3, psp										\n" // Move the PSP register to r3
				//"	add r3, #32										\n"	// We don't need the information of exception stack because this syscall will not return to user  
				//"	add r3, #16										\n" // step over the SVC call parameters and move the r3 point to the call_ID
				//"	ldr	r4, [r3]									\n" // load call_ID to r4 
				//"	mov	%0, r4										\n"	// Assign call_ID to its variable
				//"	pop {r3-r4}										\n"
				//:"=r"(call_ID_from_stack)
				//::"r3", "r4"
			//);
			module_stack=(StackType_t*)__get_PSP();
			module_stack+=8;
			module_stack+=4;
			module_stack+=4;
			module_stack++;
			call_ID_from_stack=*module_stack;
			
			index=call_moudle_frame_head->next;
			while(index!=NULL){
				if(index->cur_call_ID==call_ID_from_stack){
					// Assign the index to the global variable cur_call_module_frame. 
					// We will free this frame structure in the sos_invoke_command function after the sos_return function.
					cur_call_moudle_frame=index;						
					sos_return( index);
				}
				index=index->next;
			}
		
		}

		case SYS_nsc_call_module:
		{
			return SOS_invoke_command((SOS_ModuleID_t)a1, (uint32_t)a2, (SOS_Operation_t *) a3);
			break;
		}

		

		default:
		{
			/* Incorrect SVC call. */
			
		}
	}
}


void sos_return(struct CallModuleFrame* index){
	__asm volatile(
						"	mov r4, %0										\n"
						"	add r4, r4, #48									\n"
						"	ldmia r4!, {r0-r3}								\n"	
						"	msr msp, r0										\n"
						"	msr control, r1									\n"
						"	mov r1, #1										\n"
						"	orr r3, r3, r1									\n"
						"	mov lr,	r3										\n"
						"	ldmia r4!, {r3}									\n"
						"	msr psp, r3										\n"	
						"	push {r2}										\n" // Push the return address in sos_invoke_command function
						"	sub r4, r4, #68									\n" // Move to the PushRegisters
						"	ldmia r4!, {r0-r3}								\n"	// Restore the r0-r3 
						"	push {r0}										\n" // Use r0 as the base address
						"   mov r0,	r4										\n"
						"   add r0, r0, #16									\n" //Move to the high registers		
						"	ldmia r0!, {r4-r7}								\n"	
						"	mov r8, r4										\n" /* r8 = r4. */
						"	mov r9, r5										\n" /* r9 = r5. */
						"	mov r10, r6										\n" /* r10 = r6. */
						"	mov r11, r7										\n" /* r11 = r7. */
						"	sub r0, r0, #32									\n" // Go back to the low registers
						"	ldmia r0!, {r4-r7}								\n"	// restore the low registers
						"	pop	{r0}										\n" 
						
						
						"	pop {pc}										\n"						
						::"r"(index)
					);
}