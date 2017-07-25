#ifndef BSON_ARRAY_H
#define BSON_ARRAY_H

#include "bson_object.h"

typedef struct BsonElement BsonElement;
typedef struct BsonObject BsonObject;

typedef enum bson_boolean bson_boolean;
typedef enum element_type element_type;

//Object representing a BSON array
struct BsonArray {
  //Array of BSON elements
  BsonElement **elements;
  //Number of elements currently in the array
  size_t count;
  //The current maximum number of elements in the array
  size_t maxCount;
};
typedef struct BsonArray BsonArray;

#ifdef __cplusplus
extern "C" {
#endif

/*
  @brief Initalize BSON Array
  
  @param array - The uninitialized BSON Array
  @param initialSize - The initial maximum size of the array

  @return - true if the array was initialized successfully, false if not
*/
bool bson_array_initialize(BsonArray *array, size_t initialCapacity);
/*
  @brief Deinitalize BSON Array, free all associated memory, 
  and recursively clean up all sub-objects
  
  @param array - The BSON Array to be deinitalized
*/
void bson_array_deinitialize(BsonArray *array);

/*
  @brief Calculate the size, in bytes, of a given array when converted to a BSON document

  @param obj - The BSON array from which the size is calculated

  @return - The calculated size
*/
size_t bson_array_size(BsonArray *array);

/*
  @brief Get the BSON represention of an array

  @param array - The array to be converted to BSON

  @return - A byte array containing the BSON representation of the array,
  this data must be freed by the caller after use
*/
uint8_t *bson_array_to_bytes(BsonArray *array);
/*
  @brief Parse BSON data into an array

  @param data - The BSON data to be parsed

  @return - The BSON array created from the BSON data
*/
BsonArray bson_array_from_bytes(uint8_t *data);

/*
  @brief Get a JSON string representation of a BSON array

  @param array - The array to be converted to a JSON string
  @param out - The output string of the function

  @return - The value of out
*/
char *bson_array_to_string(BsonArray *array, char *out);

/*
  @brief Add a BSON object to the end of a given array

  @param array - The array to be modified
  @param value - The pointer to the BSON object to be added

  @return - true if the addition was successful, false if not
*/
bool bson_array_add_object(BsonArray *array, BsonObject *value);
/*
  @brief Add a BSON array to the end of a given array

  @param array - The array to be modified
  @param value - The pointer to the BSON array to be added

  @return - true if the addition was successful, false if not
*/
bool bson_array_add_array(BsonArray *array, BsonArray *value);
/*
  @brief Add a 32-bit integer value to the end of a given array

  @param array - The array to be modified
  @param value - The integer value to be added

  @return - true if the addition was successful, false if not
*/
bool bson_array_add_int32(BsonArray *array, int32_t value);
/*
  @brief Add a 64-bit integer value to the end of a given array

  @param array - The array to be modified
  @param value - The integer value to be added

  @return - true if the addition was successful, false if not
*/
bool bson_array_add_int64(BsonArray *array, int64_t value);
/*
  @brief Add a string value to the end of a given array

  @param array - The array to be modified
  @param value - The string value to be added

  @return - true if the addition was successful, false if not
*/
bool bson_array_add_string(BsonArray *array, char *value);
/*
  @brief Add a boolean value to the end of a given array

  @param array - The array to be modified
  @param value - The boolean value to be added

  @return - true if the addition was successful, false if not
*/
bool bson_array_add_bool(BsonArray *array, bson_boolean value);
/*
  @brief Add a floating-point value to the end of a given array

  @param array - The array to be modified
  @param value - The floating-point value to be added

  @return - true if the addition was successful, false if not
*/
bool bson_array_add_double(BsonArray *array, double value);

/*
  @brief Retrieve the object at a specified index

  @param array - The array to be accessed
  @param index - The index of the object within the array

  @return - The BSON element at the given index if it exists, 
  NULL if the index is out of bounds
*/
BsonElement *bson_array_get(BsonArray *array, size_t index);
/*
  @brief Retrieve the BSON object at a specified index in an array

  @param array - The array to be accessed
  @param index - The index of the BSON object within the array

  @return - The pointer to the BSON object at the given index if it exists, 
  NULL if the index is out of bounds or the value is not a BSON object
*/
BsonObject *bson_array_get_object(BsonArray *array, size_t index);
/*
  @brief Retrieve the BSON array at a specified index in an array

  @param array - The array to be accessed
  @param index - The index of the BSON array within the array

  @return - The pointer to the BSON array at the given index if it exists, 
  NULL if the index is out of bounds or the value is not a BSON array
*/
BsonArray *bson_array_get_array(BsonArray *array, size_t index);
/*
  @brief Retrieve the 32-bit integer value at a specified index in an array

  @param array - The array to be accessed
  @param index - The index of the integer value within the array

  @return - The integer value at the given index if it exists, 
  NULL if the index is out of bounds or the value is not a 32-bit integer
*/
int32_t bson_array_get_int32(BsonArray *array, size_t index);
/*
  @brief Retrieve the 64-bit integer value at a specified index in an array

  @param array - The array to be accessed
  @param index - The index of the integer value within the array

  @return - The integer value at the given index if it exists, 
  NULL if the index is out of bounds or the value is not a 64-bit integer
*/
int64_t bson_array_get_int64(BsonArray *array, size_t index);
/*
  @brief Retrieve the string value at a specified index in an array

  @param array - The array to be accessed
  @param index - The index of the string value within the array

  @return - The string value at the given index if it exists, 
  NULL if the index is out of bounds or the value is not a string
*/
char *bson_array_get_string(BsonArray *array, size_t index);
/*
  @brief Retrieve the boolean value at a specified index in an array

  @param array - The array to be accessed
  @param index - The index of the string value within the array

  @return - The boolean value at the given index if it exists, 
  NULL if the index is out of bounds or the value is not a boolean
*/
bson_boolean bson_array_get_bool(BsonArray *array, size_t index);
/*
  @brief Retrieve the floating-point value at a specified index in an array

  @param array - The array to be accessed
  @param index - The index of the floating-point value within the array

  @return - The floating-point value at the given index if it exists, 
  NULL if the index is out of bounds or the value is not a floating-point number
*/
double bson_array_get_double(BsonArray *array, size_t index);

#ifdef __cplusplus
}
#endif

#endif
