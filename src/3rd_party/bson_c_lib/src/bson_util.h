#ifndef BSON_UTIL_H
#define BSON_UTIL_H

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

//4 bytes for length, one for ending null character
#define OBJECT_OVERHEAD_BYTES 5
//Same as object
#define ARRAY_OVERHEAD_BYTES 5
//1 byte for element type
#define ELEMENT_OVERHEAD_BYTES 1
//4 bytes for length, one for ending null character
#define STRING_OVERHEAD_BYTES 5

//Sizes in bytes of each primitive type, as defined by the BSON spec
#define SIZE_INT32 4
#define SIZE_INT64 8
#define SIZE_DOUBLE 8
#define SIZE_BOOLEAN 1

//Last byte in a BSON document
#define DOCUMENT_END 0x00

//Byte which defines the type of a value as defined in the BSON spec
enum element_type {
  TYPE_DOUBLE = 0x01,
  TYPE_STRING = 0x02,
  TYPE_DOCUMENT = 0x03,
  TYPE_ARRAY = 0x04,
  TYPE_BINARY = 0x05, //unused
  TYPE_UNDEFINED = 0x06, //deprecated
  TYPE_OBJECT_ID = 0x07, //unused
  TYPE_BOOLEAN = 0x08,
  TYPE_DATE_TIME = 0x09, //unused
  TYPE_NULL = 0x0A,
  TYPE_REGEX = 0x0B, //unused
  TYPE_DB_POINTER = 0x0C, //deprecated
  TYPE_JS_CODE = 0x0D, //unused
  TYPE_SYMBOL = 0x0E, //deprecated
  TYPE_JS_CODE_WITH_SCOPE = 0x0F, //unused
  TYPE_INT32 = 0x10,
  TYPE_TIMESTAMP = 0x11, //unused
  TYPE_INT64 = 0x12,
  TYPE_DEC128 = 0x13, //unused
  TYPE_MIN_KEY = 0xFF, //unused
  TYPE_MAX_KEY = 0x7F //unused
};
typedef enum element_type element_type;

//Definition of each boolean value according to the BSON spec
enum bson_boolean {
  BOOLEAN_INVALID = -1,
  BOOLEAN_FALSE = 0x00,
  BOOLEAN_TRUE = 0x01
};
typedef enum bson_boolean bson_boolean;

#ifdef __cplusplus
extern "C" {
#endif

/*
  @brief Write a little endian 32-bit integer value to a given buffer

  @param bytes - The byte buffer to be written to
  @param value - The integer value to be written to the buffer
  @param position - Pointer to the current position in the buffer, will be advanced past the written value
*/
void write_int32_le(uint8_t *bytes, int32_t value, size_t *position);
/*
  @brief Write a little endian 64-bit integer value to a given buffer

  @param bytes - The byte buffer to be written to
  @param value - The integer value to be written to the buffer
  @param position - Pointer to the current position in the buffer, will be advanced past the written value
*/
void write_int64_le(uint8_t *bytes, int64_t value, size_t *position);
/*
  @brief Write a little endian 64-bit floating-point value to a given buffer

  @param bytes - The byte buffer to be written to
  @param value - The integer value to be written to the buffer
  @param position - Pointer to the current position in the buffer, will be advanced past the written value
*/
void write_double_le(uint8_t *bytes, double value, size_t *position);

/*
  @brief Read a little endian 32-bit integer value from a given buffer

  @param bytes - Pointer to the byte buffer from which to read, 
                 this value will be advanced past the value that was read
  
  @return - The value that was read from the buffer
*/
int32_t read_int32_le(uint8_t **bytes);
/*
  @brief Read a little endian 64-bit integer value from a given buffer

  @param bytes - Pointer to the byte buffer from which to read, 
                 this value will be advanced past the value that was read
  
  @return - The value that was read from the buffer
*/
int64_t read_int64_le(uint8_t **bytes);
/*
  @brief Read a little endian 64-bit floating point value from a given buffer

  @param bytes - Pointer to the byte buffer from which to read, 
                 this value will be advanced past the value that was read
  
  @return - The value that was read from the buffer
*/
double read_double_le(uint8_t **bytes);

/*
  @brief Convert the give UTF-8 string into a byte array

  @param stringVal - the string value to be converted

  @return - The byte array representation of the string, must be freed by the caller after use
*/
uint8_t *string_to_byte_array(char *stringVal);
/*
  @brief Convert the given byte array to a UTF-8 string

  @param bytes - The byte array to be converted

  @return The converted string
*/
char *byte_array_to_string(uint8_t *bytes);
/*
  @brief Convert the given byte array to a UTF-8 BSON string

  @param bytes - The byte array to be converted
  @param length - The length of the array to be converted

  @return The converted string (may include null characters)
*/
char *byte_array_to_bson_string(uint8_t *bytes, size_t length);

/*
  @brief Convert the given a array index into a BSON key

  @param index - The index to be converted

  @return - A byte array containing the BSON key representation of index, must be freed by the caller after use
*/
uint8_t *index_to_key(size_t index);

/*
  @brief Calculate the size, in bytes, of a BSON object key

  @param key - The object key used for calculations

  @return - The size of the BSON object key, in bytes
*/
size_t object_key_size(char *key);
/*
  @brief Calculate the size, in bytes, of a BSON array key

  @param index - The array key (index) used for calculations

  @return - The size of the BSON array key, in bytes
*/
size_t array_key_size(size_t index);

/*
  @brief Calculate the number of decimal digits in a given integer value

  @param value - The value on which calculations are done

  @return The number of decimal digits in value
*/
size_t digits(size_t value);

#ifdef __cplusplus
}
#endif

#endif
