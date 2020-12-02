/*
 * @Author: x-codingman 
 * @Date: 2020-10-14 21:04:58 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-11-09 10:19:38
 */

#ifndef SOS_TYPE_H
#define SOS_TYPE_H


#include "stdint.h"




// define the secure os types

typedef int SOS_Result_t;

typedef uint8_t  StackType_t;

struct PushRegs{
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r4;
    uint32_t r5;
    uint32_t r6;
    uint32_t r7;
    uint32_t r8;
    uint32_t r9;
    uint32_t r10;
    uint32_t r11;
}__attribute__((packed));

struct Trapframe {
    struct PushRegs regs;
    uint32_t msp;
    uint32_t control;
	uint32_t module_return_address;
    uint32_t lr;
	uint32_t xPSR;
	
}__attribute__((packed));

struct CallModuleFrame
{
    struct Trapframe kernel_frame;
    uint32_t cur_call_ID;
	StackType_t *module_stack;
    struct CallModuleFrame* next;

}__attribute__((packed));




#endif