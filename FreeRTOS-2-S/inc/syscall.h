/*
 * @Author: x-codingman 
 * @Date: 2020-10-14 21:02:24 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-19 20:23:39
 */
 
 #ifndef SOS_SYSCALL_H
 #define SOS_SYSCALL_H

enum{
    SYS_module_return = 5

};

void syscall_return(int ret);



 #endif