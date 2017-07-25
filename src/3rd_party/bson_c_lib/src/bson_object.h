#ifndef BSON_OBJECT_H
#define BSON_OBJECT_H

#include <stdbool.h>
#include <stdio.h>

#include "bson_util.h"
#include "bson_array.h"
#include "emhashmap/emhashmap.h"

typedef struct BsonArray BsonArray;

typedef enum element_type element_type;
typedef enum bson_boolean bson_boolean;

struct BsonObject {
  //Internal map implementation
  HashMap data;
};
typedef struct BsonObject BsonObject;

struct BsonElement {
  //The value of this element
  void *value;
  //The data type of this element
  element_type type;
  //Size of the element in bytes when converted to BSON 
  //Unused for TYPE_DOCUMENT and TYPE_ARRAY
  size_t size;
};
typedef struct BsonElement BsonElement;

struct BsonObjectEntry {
  char key[255];
  BsonElement *element;
};
typedef struct BsonObjectEntry BsonObjectEntry;

#ifdef __cplusplus
extern "C" {
#endif

/*
  @brief Initalize BSON Object
  
  @param obj - The uninitialized BSON Object
  @param size - The maximum capacity of the map used to store object data
  @param loadFactor - The load factor of the map used to store object data

  @return - true if the object was initialized successfully, false if not
*/
bool bson_object_initialize(BsonObject *obj, size_t capacity, float loadFactor);
/*
  @brief Initalize BSON object with default map size (64) and load factor (0.5)
  
  @param obj - The uninitialized BSON Object

  @return - true if the object was initialized successfully, false if not
*/
bool bson_object_initialize_default(BsonObject *obj);
/*
  @brief Deinitalize BSON object, free all associated memory, 
  and recursively clean up all sub-objects
  
  @param obj - The BSON object to be deinitalized
*/
void bson_object_deinitialize(BsonObject *obj);

/*
  @brief Calculate the size, in bytes, of a given object when converted to a BSON document

  @param obj - The BSON object from which the size is calculated

  @return - The calculated size
*/
size_t bson_object_size(BsonObject *obj);

/*
  @brief Get the BSON represention of an object

  @param obj - The array to be converted to BSON

  @return - A byte array containing the BSON representation of the object,
  this data must be freed by the caller after use
*/
uint8_t *bson_object_to_bytes(BsonObject *obj);
/*
  @brief Parse BSON data into an object

  @param data - The BSON data to be parsed

  @return - The BSON object created from the BSON data
*/
BsonObject bson_object_from_bytes(uint8_t *data);

/*
  @brief Get a JSON string representation of a BSON object

  @param obj - The object to be converted to a JSON string
  @param out - The output string of the function

  @return - The value of out
*/
char *bson_object_to_string(BsonObject *obj, char *out);

/*
  @brief Put a new BSON object into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new object 
               (if it matches an existing key, 
               the associated value will be overwritten)
  @param value - The pointer to the BSON object to be added to the object

  @return - true if the value was set successfully, false if not
*/
bool bson_object_put_object(BsonObject *obj, const char *key, BsonObject *value);
/*
  @brief Put a new BSON array into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new array 
               (if it matches an existing key, 
               the associated value will be overwritten)
  @param value - The pointer to the BSON array to be added to the object

  @return - true if the value was set successfully, false if not
*/
bool bson_object_put_array(BsonObject *obj, const char *key, BsonArray *value);
/*
  @brief Put a new 32-bit integer value into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new integer value 
               (if it matches an existing key, 
               the associated value will be overwritten)
  @param value - The integer value to be added to the object

  @return - true if the value was set successfully, false if not
*/
bool bson_object_put_int32(BsonObject *obj, const char *key, int32_t value);
/*
  @brief Put a new 64-bit integer value into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new integer value 
               (if it matches an existing key, 
               the associated value will be overwritten)
  @param value - The integer value to be added to the object

  @return - true if the value was set successfully, false if not
*/
bool bson_object_put_int64(BsonObject *obj, const char *key, int64_t value);
/*
  @brief Put a new string value into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new string value 
               (if it matches an existing key, 
               the associated value will be overwritten)
  @param value - The string value to be added to the object

  @return - true if the value was set successfully, false if not
*/
bool bson_object_put_string(BsonObject *obj, const char *key, char *value);
/*
  @brief Put a new boolean value into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new boolean value 
               (if it matches an existing key, 
               the associated value will be overwritten)
  @param value - The boolean value to be added to the object

  @return - true if the value was set successfully, false if not
*/
bool bson_object_put_bool(BsonObject *obj, const char *key, bson_boolean value);
/*
  @brief Put a new floating-point value into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new floating-point value 
               (if it matches an existing key, 
               the associated value will be overwritten)
  @param value - The floating-point value to be added to the object

  @return - true if the value was set successfully, false if not
*/
bool bson_object_put_double(BsonObject *obj, const char *key, double value);

/*
  @brief Put a new element into a given object

  @param obj - The object to be modified
  @param key - The key used to reference the new element
  @param type - The type of the element to be added
  @param value - The value of the element to be added
  @param allocSize - The size, in bytes, to be allocated for the element
  @param elementSize - The size, in bytes, of the element when converted to BSON format 

  @return - true if the addition was successful, false if not
*/
BsonElement *bson_object_get(BsonObject *obj, const char *key);
/*
  @brief Retrieve the BSON object to which the specified key is mapped in an object

  @param obj - The object to be accessed
  @param key - The key associated with the object to be retrieved

  @return - The pointer to the BSON object mapped to the given key if it exists, NULL otherwise
*/
BsonObject *bson_object_get_object(BsonObject *obj, const char *key);
/*
  @brief Retrieve the BSON array to which the specified key is mapped in an object

  @param obj - The object to be accessed
  @param key - The key associated with the array to be retrieved

  @return - The pointer to the BSON array mapped to the given key if it exists, NULL otherwise
*/
BsonArray *bson_object_get_array(BsonObject *obj, const char *key);
/*
  @brief Retrieve the 32-bit integer value to which the specified key is mapped in an object

  @param obj - The object to be accessed
  @param key - The key associated with the integer value to be retrieved

  @return - The 32-bit integer value mapped to the given key if it exists, NULL otherwise
*/
int32_t bson_object_get_int32(BsonObject *obj, const char *key);
/*
  @brief Retrieve the 64-bit integer value to which the specified key is mapped in an object

  @param obj - The object to be accessed
  @param key - The key associated with the integer value to be retrieved

  @return - The 64-bit integer value mapped to the given key if it exists, NULL otherwise
*/
int64_t bson_object_get_int64(BsonObject *obj, const char *key);
/*
  @brief Retrieve the string value to which the specified key is mapped in an object

  @param obj - The object to be accessed
  @param key - The key associated with the string value to be retrieved

  @return - The string value mapped to the given key if it exists, NULL otherwise
*/
char *bson_object_get_string(BsonObject *obj, const char *key);
/*
  @brief Retrieve the boolean value to which the specified key is mapped in an object

  @param obj - The object to be accessed
  @param key - The key associated with the boolean value to be retrieved

  @return - The boolean value mapped to the given key if it exists, NULL otherwise
*/
bson_boolean bson_object_get_bool(BsonObject *obj, const char *key);
/*
  @brief Retrieve the floating-point value to which the specified key is mapped in an object

  @param obj - The object to be accessed
  @param key - The key associated with the floating-point value to be retrieved

  @return - The floating-point value mapped to the given key if it exists, NULL otherwise
*/
double bson_object_get_double(BsonObject *obj, const char *key);

/*
  @brief Get an iterator for a given BSON object

  @param obj - The object from which the iterator is created

  @return - The iterator
*/
MapIterator bson_object_iterator(BsonObject *obj);
/*
  @brief Get the next value from an object iterator

  @param iterator - The iterator to be advanced

  @return - The next BSON object entry in the object if it exists, 
            NULL if the iterator has moved past the end of the entry list
*/
BsonObjectEntry bson_object_iterator_next(MapIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif
