/*
 * @Author: x-codingman 
 * @Date: 2020-10-12 21:08:44 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-11-09 10:36:44
 */

/* Standard includes */
#include <stdlib.h>


/* Secure OS include */

#include "inc/SecureOS.h"
#include "inc/manager.h"
#include "sos_portasm.h"
#include "sos_port.h"

#include "inc/results.h"
#include "inc/memory.h"




#define MODULES_MAX 10 // max amount of aviliable modules
#define SOS_KERNEL_STACK_SIZE 500
#define SOS_MODULE_STACK_SIZE 300


Module_t modules[MODULES_MAX];   // All modules
static uint32_t modules_index=0;  // Modules  used
static StackType_t * kernel_stack_pointer;
StackType_t kernel_stack[SOS_KERNEL_STACK_SIZE];       // Kernel stack with 1KB
struct CallModuleFrame* call_moudle_frame_head=NULL; // The head of the call_module_frame list, note it only used as an index.

static uint32_t call_ID=13;                                 // this is used to generate the call_ID for each module calling
                        

void init_stack(){
    kernel_stack_pointer=(StackType_t*) kernel_stack;
}



SOS_Result_t SOS_invoke_command(
    SOS_ModuleID_t module_id,
    uint32_t command_id, 
    SOS_Operation_t *operation
){
    SOS_Result_t res= -SOS_ERROR_GENERIC;
    struct CallModuleFrame* cur_call_moudle_frame;
    StackType_t *module_stack;

    //TODO:wait to implemented
    //check_paramaters(operation);
    
    // Look up the module
    int i=0;
    for(;i<modules_index;i++){
        if(modules[i].module_id==module_id){
            

            
            //TODO(x-codingman):
            // 1.alloc a CallModuleFrame_t
            // 2.insert the cur_call_module_frame to the head 
            // 3. alloc the module stack 
            // 4. save the kernel stack 
           cur_call_moudle_frame=memory_alloc(sizeof(struct CallModuleFrame));
		   
           if(call_moudle_frame_head!=NULL){
               cur_call_moudle_frame->next=call_moudle_frame_head;
               call_moudle_frame_head=cur_call_moudle_frame;
           }else{
               call_moudle_frame_head=cur_call_moudle_frame;
           }
           cur_call_moudle_frame->cur_call_ID=call_ID;
           call_ID++;

           
           module_stack = memory_alloc(SOS_MODULE_STACK_SIZE);
		   module_stack+=(SOS_MODULE_STACK_SIZE);
		   cur_call_moudle_frame->module_stack=module_stack;
		   
           // save the context of kernel 
		   __asm volatile(
				  "	  push {r0-r4}									\n"	// store r0-r4 in the stack
				  "   push {r4}     								\n" // Store r4 in the stack
				  "   mov r4, %0                                    \n" // Move the pointer to cur_call_moudle_frame to r4
				  "   stmia r4!, {r0-r3}                            \n" // Store r0-r3
				  "   mov r3, r4                                    \n" // Mov  r4 to r3
				  "   pop {r4}                                      \n" // Restore r4
				  "   stmia r3!, {r4-r7}                            \n" // Store r4-r7
				  "	  mov r0, r8									\n" // r0 = r8. 
				  "	  mov r1, r9									\n" // r1 = r9. 
				  "	  mov r2, r10									\n" // r2 = r10. 
				  "	  mov r4, r11									\n" // r3 = r11. 
				  "	  stmia r3!, {r0-r2,r4}							\n" // Store the high registers that are not saved automatically. 
				  "	  mrs r0, msp									\n" // r0 = MSP. 
				  "	  mrs r1, control								\n" // r1 = CONTROL. 
				  "	  ldr r2, =sys_call_return_label				\n" // r2 = sys_call_return. 
				  "	  mov r4,	lr									\n"	// r4 = lr
				  "	  stmia r3!, {r0-r2,r4}							\n" /* Store MSP, CONTROL ,module_return_address and LR on the stack. */
				  "   mrs r0, ipsr									\n" // r0 = xPSR
				  "   stmia r3!, {r0}								\n" // store the xPSR register
				  "	  pop {r0-r4}									\n"	// restore r0-r4
					::"r"(cur_call_moudle_frame)
					:"memory"
			);
		
	

			// Store call_ID
			// Set the module_stack to psp 
		    // __asm volatile(
			// "   push {r0-r1}									\n"
			// "	mov r1,	%1										\n"
			// "	mov r0, %0										\n"
		    // "	sub r0, r0, #4									\n"
		    // "	stmia r0!, {r1}									\n"
		    // "	sub r0, r0, #4									\n"
		    // "	msr psp, r0										\n"
			// "	pop {r0-r1}										\n"
		    // ::"r"(module_stack),"r"(cur_call_moudle_frame->cur_call_ID)
			// : "memory"
		    // );
		   
			// Call the module entry
			// This function will not return.
			call_module( command_id,operation,&modules[i]);
			
			// The sys_call_return_label is used for the syscall to return to the kernel.
			__asm volatile
			(
			"	sys_call_return_label:							\n"
				"	pop {r1}								\n"
				"	mov lr, r1								\n"
				"   pop {r1-r4}								\n"
				"	pop {r0}								\n"
				"	movs %0, r0 							\n"
			:"=r"(res)
			);
			return res;
        }

    } 
}



SOS_Result_t init_modules(void){
    int i;
    for (i=0;i<MODULES_MAX;i++){
        modules[i].module_id=0;
        modules[i].module_entry=NULL;

    }
}

SOS_Result_t add_module(SOS_ModuleEntry_t module_enrty,SOS_ModuleID_t module_id,SOS_Manifest_t manifest){
    if((module_id==0)||(module_enrty==NULL)){
        return -SOS_ERROR_INVAL;
    }
    
    modules[modules_index].module_id=module_id;
    modules[modules_index].module_entry=module_enrty;
    modules[modules_index].module_manifest=manifest;

    modules_index++;
    if(modules_index>=MODULES_MAX){
        return -SOS_ERROR_NO_FREE_SM;
    }

    
}

 
//
// Set up the exception stack frame and call the secure module.
// This function switch to the thread mode from handler mode by svc return.
// 

 
 
void call_module( uint32_t command_id, 
    SOS_Operation_t *operation,Module_t* module, StackType_t *module_stack){	

		//Store the call_ID on the top of module stack.
		module_stack--;
		*module_stack = call_ID-1;	

		// Set up the exception stack frame.
		module_stack--;
		*module_stack = INITIAL_XPSR;			
		module_stack--;
		*module_stack = module->module_entry;
		module_stack--;
		*module_stack = MODULE_RETURN_ADDRESS;	// LR
		module_stack--;
		*module_stack = 0;						// r12
		module_stack--;
		*module_stack = 0;						// r3
		module_stack--;
		*module_stack = module->module_entry;			// r2
		module_stack--;
		*module_stack = operation;				//r1
		module_stack--;
		*module_stack = command_id;				//r0



		__asm volatile(
		  " mov r5, %0										\n"
		  " msr psp, r5										\n" // Set the psp register for the secure module
		  "	mov r5, %1										\n"
		  " mov lr, r5										\n"	// Set the EXC_RETURN for the LR register.
		  "	mrs r5, control									\n" // Move the conrtol register to r5
		  "	mov r4, #0x3									\n" // Set the privilege bit and the stack_switch bit of control
		  "	orr r5, r5, r4									\n"	// Xor r5 with r4
		  "	msr control, r5									\n" // update the control value
		  "	dsb                                             \n" // Data flush
		  "	isb												\n"
		  "	bx lr											\n"
		  ::"r"(module_stack), "r"(INITIAL_EXC_RETURN)
		  : "r4", "r5", "memory"
      );
}



















