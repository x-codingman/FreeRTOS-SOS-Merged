/*
 * @Author: x-codingman 
 * @Date: 2020-10-14 14:43:47 
 * @Last Modified by: x-codingman
 * @Last Modified time: 2020-10-15 16:25:27
 */


#ifndef SOS_RESULTS_H
#define  SOS_RESULTS_H





enum{
    SOS_SUCCESS =0,
    SOS_ERROR_GENERIC,
    SOS_ERROR_INVAL ,   //Invalid parameter
    SOS_ERROR_NOT_IMPLEMENTED, //The requested secure module is not implemented
    SOS_ERROR_NO_MEM,     //Request failed due to memory shortage  
    SOS_ERROR_NO_FREE_SM   //There is no free secure module to alloc


};


#endif