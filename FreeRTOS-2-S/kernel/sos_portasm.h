/*
 * @Author: x-codingman 
 * @Date: 2020-10-17 12:50:37 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-17 12:58:13
 */

#ifndef SOS_PORT_ASM_H
#define SOS_PORT_ASM_H

int is_privileged( void )__attribute__((naked));
void reset_privilege( void )__attribute__((naked));
void raise_privilege( void )__attribute__((naked));
/**
 * @brief SVC Handler.
 */
void SVCall_Handler( void )__attribute__((naked)) ;



#endif