/**
 * \file MBDebugHelper.h
 * \brief DebugHelper.
 * \author AKara
 */

#ifndef MB_DEBUG_HELPER_H
#define MB_DEBUG_HELPER_H

#include <cstdio>

/**
* \def DEBUG_ON
* \brief Switches on debug messages.
*/ 
//#define DEBUG_ON

#ifdef DEBUG_ON 
/**
* \def DBG_MSG
* \brief Debug message output with file name and line number.
* \param x formatted debug message.
* \return printf construction.
*/
#define DBG_MSG(x) printf("%s:%d ", __FILE__, __LINE__);\
                   printf x
#else
#define DBG_MSG(x)
#endif

/**
* \def DBG_MSG_ERROR
* \brief Debug ERROR message output with file name and line number.
* \param x formatted debug message.
* \return printf construction.
*/ 
#define DBG_MSG_ERROR(x) printf("ERROR!!! %s:%d ", __FILE__, __LINE__);\
                         printf x

#endif /*MB_DEBUG_HELPER_H*/
