#pragma once

/******************************************************
 * 
 *  BASIC TYPES
 * 
 ******************************************************/

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

typedef signed char sint8_t;
typedef signed short sint16_t;
typedef signed int sint32_t;
typedef signed long long sint64_t;

typedef unsigned long uintptr_t;
typedef unsigned long size_t;
typedef unsigned long ptrdiff_t;


/********************************************************
 * 
 * BOOLEAN
 * 
 *******************************************************/

#define true 1
#define false 0

typedef _Bool bool;

/*******************************************************
 * 
 * VA LIST
 * 
 *******************************************************/
typedef __builtin_va_list __gnuc_va_list;

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)   __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)

#define __va_copy(d,s)  __builtin_va_copy(d,s)

#define _VA_LIST va_list;
typedef __gnuc_va_list va_list;