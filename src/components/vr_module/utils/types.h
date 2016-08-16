#ifndef SRC_COMPONENTS_IVDCM_MODULE_UTILS_TYPES_H_
#define SRC_COMPONENTS_IVDCM_MODULE_UTILS_TYPES_H_

#include <sys/types.h>

/** @typedef Int8
 *  @brief Definition for integral type with 8 bits
 *
 * Type Int8 has a sign. It can contain negative values.
 * Range of Int8: -128..127
 */
typedef signed char Int8;

/** @typedef UInt8
 *  @brief Definition for integral type with 8 bits.
 *
 * Type UInt8 contains only positive values.
 * Range of UInt8: 0..255
 */
typedef unsigned char UInt8;

/** @typedef  Int16
 *  @brief Definition for integral type with 16 bits
 *
 * Type Int16 has a sign. It can contain negative values.
 * Range of Int16: -32,768..32,767
 */
typedef signed short Int16;

/** @typedef  UInt16
 *  @brief Definition for integral type with 16 bits
 *
 * Type UInt16 contains only positive values.
 * Range of UInt16: 0..65,535
 */
typedef unsigned short UInt16;

/** @typedef  WChar
 *  @brief Definition for integral type with 32 bits
 *
 * This is a synonym for UInt32.
 */
typedef unsigned int WChar;

/** @typedef  Int32
 *  @brief Definition for integral type with 32 bits
 *
 * Type Int32 has a sign. It can contain negative values.
 * Range of Int32: -2,147,483,648..2,147,483,647
 */
typedef signed int Int32;

/** @typedef  UInt32
 *  @brief Definition for integral type with 32 bits
 *
 * Type UInt32 contains only positive values.
 * Range of UInt32: 0..4,294,967,295
 */
typedef unsigned int UInt32;

/** @typedef  Int64
 *  @brief Definition for integral type with 64 bits
 *
 * Type Int64 has a sign. It can contain negative values.
 * Range of Int64: -9,223,372,036,854,775,808..9,223,372,036,854,775,807
 */
typedef signed long long int Int64;

/** @typedef  UInt64
 *  @brief Definition for integral type with 64 bits
 *
 * Type UInt64 contains only positive values.
 * Range of UInt64: 0..18,446,744,073,709,551,615
 */
typedef unsigned long long int UInt64;

#endif  // SRC_COMPONENTS_IVDCM_MODULE_UTILS_TYPES_H_
