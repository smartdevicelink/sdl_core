/**
 * \file libMBDebugHelper.h
 * \brief DebugHelper.
 * \author AKara
 */

#ifndef MB_DEBUG_HELPER_H
#define MB_DEBUG_HELPER_H

#include <cstdio>

/**
* \def DEBUG_MB_ON
* \brief Switches on MessageBroker debug messages.
*/
#ifdef DEBUG_MB_ON

/**
* \def DBG_MSG
* \brief Debug message output with file name and line number.
* \param x formatted debug message.
* \return printf construction.
*/ 
#define DBG_MSG(x) printf("%s:%d_lib ", __FILE__, __LINE__);\
                   printf x

/**
 * \def DBG_MSG_ERROR
 * \brief Debug ERROR message output with file name and line number.
 * \param x formatted debug message.
 * \return printf construction.
 */
#define DBG_MSG_ERROR(x) printf("ERROR!!! %s:%d_lib ", __FILE__, __LINE__);\
                         printf x

#else

#define DBG_MSG(x)
#define DBG_MSG_ERROR(x)

#endif

#endif /*MB_DEBUG_HELPER_H*/
