/**
 * \file debug.h
 * \brief It defines the debugging facilities
 *
 * mCASE software
 * Multi-Constraint Access Selection in Heterogeneous Environtment
 * 
 * This file includes the definition of all the macros that are used for debugging
 * purposes (to be used by the rest of the modules)
 * 
 * \author University of Cantabria: Ramon Aguero (ramon@tlmat.unican.es) and 
 *                                  Johnny Choque (jchoque@tlmat.unican.es)
 * \date 2008-09-24
 */
#ifndef _INC_DEBUG_H_
#define _INC_DEBUG_H_

#include <time.h>
#include <string.h>
#include <cstdio>
#include <ncurses.h>

#ifndef DBG_LEVEL
#define DBG_LEVEL 1
#endif

/**
 * \brief Color is ON when DBG_COLOR=1 
 * Color is OFF when DBG_COLOR=0 
 */
#define DBG_COLOR 1

/**
 * \brief Printing function is (user space) printf
 */
#ifndef NCURSES
#define PRINT printf
#define REFRESH
#else
#define PRINT printw
#define REFRESH refresh();
#endif

/**
 * \brief Debug the current time
 *
 * It prints the current time (in the format hh:mm.ss)
 */
#define DBG_TIMENOW \
{ time_t result; struct tm timeptr; \
   time(&result); \
   localtime_r(&result,&timeptr); \
   PRINT("%02d:%02d.%02d",timeptr.tm_hour, timeptr.tm_min, timeptr.tm_sec); \
}

/**
 * \brief It prints a common header for all messages
 * 
 * It prints a common debugging header, consisting on the file the message came from,
 * the current time (calling DBG_TIMENOW) and the type of debug message
 */
#define dbgHeader(x) \
  PRINT("mCASE (%-18s) ", \
	(strrchr(__FILE__, '/') != NULL) ? strrchr(__FILE__, '/') + 1 : __FILE__); \
  DBG_TIMENOW;	\
  PRINT(": [%-4s] ",x);

/**
 * \brief It prints error messages
 *
 * It prints an error message, along with some information (provided by the caller)
 *
 * \param fmt Same as printf (it is the string to be printed itself)
 * \param args... Variable # of arguments (the same we would pass to a printf call)
 */

#define DBG_ERR(fmt, args...) \
    {if(DBG_LEVEL>0) {if(DBG_COLOR) PRINT("\033[31;1m"); \
		dbgHeader("ERR"); PRINT(fmt, ##args); PRINT("\n"); REFRESH \
		if(DBG_COLOR) PRINT("\033[0m");}}
		
#define DBG_WARN(fmt, args...) \
    {if(DBG_LEVEL>1) {if(DBG_COLOR) PRINT("\033[33;1m"); \
		dbgHeader("WARN"); PRINT(fmt, ##args); PRINT("\n"); REFRESH \
		if(DBG_COLOR) PRINT("\033[0m");}}

#define DBG_HIGH(fmt, args...) \
    {if(DBG_LEVEL>1) {if(DBG_COLOR) PRINT("\033[32;1m"); \
		dbgHeader("HIGH"); PRINT(fmt, ##args); PRINT("\n"); REFRESH \
		if(DBG_COLOR) PRINT("\033[0m");}}
		
/**
 * \brief Informational Message
 * Shows any kind of information, following the same rules as the traditional printf stuff
 *
 * \param fmt Same as printf, the string itself
 * \param args... Arguments of the string (variables,...)
 */
#define DBG_INFO(fmt, args...) \
    {if(DBG_LEVEL>1) {dbgHeader("INFO"); PRINT(fmt, ##args); PRINT("\n"); REFRESH}}


/**
 * \brief Informational Message - Level 1
 * Shows any kind of information, following the same rules as the traditional printf stuff. It's used to print more detail info 
 *
 * \param fmt Same as printf, the string itself
 * \param args... Arguments of the string (variables,...)
 */
#define DBG_INFO2(fmt, args...) \
    {if(DBG_LEVEL>2) {dbgHeader("INFO"); PRINT(fmt, ##args); PRINT("\n"); REFRESH}}

/**
 * \brief Debugging Input of a Function
 *
 * It prints a message when we enter a function
 *
 * \param funct Name of the function
 */
#define DBG_IN(funct) \
    {if(DBG_LEVEL>3) {dbgHeader("IN"); PRINT(funct); PRINT("\n"); REFRESH}}

/**
 * \brief Debugging Output from a Function
 *
 * It prints a message to report that a function has returned
 *
 * \param funct Name of the function
 */
#define DBG_OUT(funct) \
    {if(DBG_LEVEL>3) {dbgHeader("OUT");PRINT(funct);PRINT("\n");REFRESH}}

/**
 * \brief Dumps a memory block in hexadecimal
 *
 * This prints a chunk of memory in hexadecimal format, this may be appropriate 
 * e.g. for debugging transmitted/received packets, as well as addresses (MAC), etc
 *
 * \param label Reference of what it is being dumped
 * \param data Pointer to the first memory position
 * \param len Lenght of the data to be dumped
 */
#define DBG_DUMP(label, data, len) \
{if(DBG_LEVEL>2) \
{ int i; \
  dbgHeader("DUMP"); \
  PRINT("%-4s [%2d] ", label, len) ; \
  for (i=0;i<len;i++) { \
      if(i%8 == 0 && i!=0) { \
           PRINT("\n") ; dbgHeader("DUMP") ; PRINT("          ") ; } \
      PRINT("%02X ", ((unsigned char*) data)[i]) ; } \
  PRINT("\n") ; REFRESH \
  }}
#endif // _INC_DEBUG_H_
