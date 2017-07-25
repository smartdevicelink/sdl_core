#include "bson_array.h"

bool bson_array_initialize(BsonArray *array, size_t initialCapacity) {
  array->count = 0;
  array->maxCount = initialCapacity;
  array->elements = malloc(sizeof(BsonElement *) * initialCapacity);
  return true;
}

void bson_array_deinitialize(BsonArray *array) {
  size_t i = 0;
  for (i = 0; i < array->count; i++) {
    BsonElement *element = array->elements[i];
    if (element->type == TYPE_DOCUMENT) {
      bson_object_deinitialize((BsonObject *)element->value);
    }
    else if (element->type == TYPE_ARRAY) {
      bson_array_deinitialize((BsonArray *)element->value);
    }
    free(element->value);
    free(element);
  }

  free(array->elements);
}

size_t bson_array_size(BsonArray *array) {
  size_t arraySize = ARRAY_OVERHEAD_BYTES;
  size_t i = 0;
  for (i = 0; i < array->count; i++) {
    BsonElement *element = array->elements[i];
    arraySize += array_key_size(i) + ELEMENT_OVERHEAD_BYTES;
    if (element->type == TYPE_DOCUMENT) {
      arraySize += bson_object_size((BsonObject *)element->value);
    }
    else if (element->type == TYPE_ARRAY) {
      arraySize += bson_array_size((BsonArray *)element->value);
    }
    else {
      arraySize += element->size;
    }
  }
  return arraySize;
}

uint8_t *bson_array_to_bytes(BsonArray *array) {
  size_t arraySize = bson_array_size(array);
  uint8_t *bytes = malloc(arraySize);
  size_t position = 0;
  write_int32_le(bytes, (int32_t)arraySize, &position);
  size_t i = 0;
  for (i = 0; i < array->count; i++) {
    BsonElement *element = array->elements[i];

    bytes[position++] = element->type;

    uint8_t *keyBytes = index_to_key(i);
    memcpy(&bytes[position], keyBytes, digits(i));
    free(keyBytes);
    position += digits(i);

    //Null-terminate
    bytes[position++] = 0x00;

    switch (element->type) {
      case TYPE_DOCUMENT: {
        BsonObject *obj = (BsonObject *)element->value;
        uint8_t *objBytes = bson_object_to_bytes(obj);
        if (objBytes == NULL) {
          printf("An error occured while parsing the object with index \"%i\"\n", (int)i);
          free(bytes);
          return NULL;
        }
        size_t objSize = bson_object_size(obj);
        memcpy(&bytes[position], objBytes, objSize);
        free(objBytes);
        position += objSize;
        break;
      }
      case TYPE_ARRAY: {
        BsonArray *subArray = (BsonArray *)element->value;
        uint8_t *subArrayBytes = bson_array_to_bytes(subArray);
        if (subArrayBytes == NULL) {
          printf("An error occured while parsing the object with index \"%i\"\n", (int)i);
          free(bytes);
          return NULL;
        }
        size_t subArraySize = bson_array_size(subArray);
        memset(&bytes[position], 0, subArraySize);
        memcpy(&bytes[position], subArrayBytes, subArraySize);
        free(subArrayBytes);
        position += subArraySize;
        break;
      }
      case TYPE_INT32: {
        write_int32_le(bytes, *(int32_t *)element->value, &position);
        break;
      }
      case TYPE_INT64: {
        write_int64_le(bytes, *(int64_t *)element->value, &position);
        break;
      }
      case TYPE_STRING: {
        char *stringVal = (char *)element->value;
        //String length is written first
        write_int32_le(bytes, (int32_t)(strlen(stringVal) + 1), &position);

        uint8_t *stringBytes = string_to_byte_array(stringVal);
        memcpy(&bytes[position], stringBytes, strlen(stringVal));
        free(stringBytes);
        position += strlen(stringVal);

        //Null-terminate
        bytes[position++] = 0x00;
        break;
      }
      case TYPE_DOUBLE: {
        write_double_le(bytes, *(double *)element->value, &position);
        break;
      }
      case TYPE_BOOLEAN: {
        bytes[position++] = (uint8_t)(*(bson_boolean *)element->value);
        break;
      }
      default: {
        printf("Unrecognized BSON type: %i\n", element->type);
        position += sizeof(*element->value);
      }
    }
  }

  bytes[position++] = DOCUMENT_END;
  if (position != arraySize) {
    printf("Something went horribly wrong. Unexpected size of array in bytes: %i, expected size: %i\n", (int)position, (int)arraySize);
    free(bytes);
    return NULL;
  }
  return bytes;
}

BsonArray bson_array_from_bytes(uint8_t *data) {
  uint8_t *current = data;
  int32_t size = read_int32_le(&current);
  BsonArray array;
  bson_array_initialize(&array, 10);
  uint8_t type = *current;
  current++;
  while (type != DOCUMENT_END) {
    char *key = byte_array_to_string(current);
    current += strlen(key) + 1;

    switch ((element_type)type) {
      case TYPE_DOCUMENT: {
        BsonObject obj = bson_object_from_bytes(current);
        bson_array_add_object(&array, &obj);
        current += bson_object_size(&obj);
        break;
      }
      case TYPE_ARRAY: {
        BsonArray subArray = bson_array_from_bytes(current);
        bson_array_add_array(&array, &subArray);
        current += bson_array_size(&subArray);
        break;
      }
      case TYPE_INT32: {
        int32_t value = read_int32_le(&current);
        bson_array_add_int32(&array, value);
        break;
      }
      case TYPE_INT64: {
        int64_t value = read_int64_le(&current);
        bson_array_add_int64(&array, value);
        break;
      }
      case TYPE_STRING: {
        //String length is read first
        int32_t stringLength = read_int32_le(&current) - 1;

        char *stringVal = byte_array_to_bson_string(current, stringLength);
        bson_array_add_string(&array, stringVal);
        free(stringVal);
        current += stringLength + 1;
        break;
      }
      case TYPE_DOUBLE: {
        double value = read_double_le(&current);
        bson_array_add_double(&array, value);
        break;
      }
      case TYPE_BOOLEAN: {
        bson_array_add_bool(&array, *current);
        current++;
        break;
      }
      default: {
        printf("Unrecognized BSON type: %i\n", type);
      }
    }
    free(key);
    type = *current;
    current++;
  }

  if (data + size != current) {
    printf("Unexpected parsed array size. Expected %i, got %i\n", (int) size, (int)(current - data));
  }
  return array;
}

char *bson_array_to_string(BsonArray *array, char *out) {
  //TODO just move the pointer rather than keep a position variable
  int position = 0;
  position += sprintf(out, "[ ");
  size_t i = 0;
  for (i = 0; i < array->count; i++) {
    BsonElement *element = array->elements[i];
    switch (element->type) {
      case TYPE_DOCUMENT: {
        char docString[512];
        position += sprintf(&out[position], "%s", bson_object_to_string(bson_array_get_object(array, i), docString));
        break;
      }
      case TYPE_ARRAY: {
        char docString[512];
        position += sprintf(&out[position], "%s", bson_array_to_string(bson_array_get_array(array, i), docString));
        break;
      }
      case TYPE_INT32: {
        position += sprintf(&out[position], "%i", (int)bson_array_get_int32(array, i));
        break;
      }
      case TYPE_INT64: {
        position += sprintf(&out[position], "%li", (long)bson_array_get_int64(array, i));
        break;
      }
      case TYPE_STRING: {
        position += sprintf(&out[position], "\"%s\"", bson_array_get_string(array, i));
        break;
      }
      case TYPE_DOUBLE: {
        position += sprintf(&out[position], "%f", bson_array_get_double(array, i));
        break;
      }
      case TYPE_BOOLEAN: {
        position += sprintf(&out[position], "%s", (bson_array_get_bool(array, i) == BOOLEAN_TRUE) ? "true" : "false");
        break;
      }
      default: {
        printf("Unrecognized BSON type: %i\n", element->type);
        position += sprintf(&out[position], "UNKNOWN_TYPE");
      }
    }
    if (i != (array->count - 1)) {
      position += sprintf(&out[position], ", ");    
    }
  }
  sprintf(&out[position], " ]");

  return out;
}

bool bson_array_resize(BsonArray *array, size_t newSize) {
  if (array->count > newSize) {
    printf("Attempted to resize an array smaller than the number of elements it contains\n");
    return false;
  }
  int i = 0;
  BsonElement **newArray = malloc(sizeof(BsonElement *) * newSize);
  BsonElement **oldArray = array->elements;
  for (i = 0; i < array->maxCount; i++) {
    newArray[i] = oldArray[i];
  }
  free(oldArray);
  array->elements = newArray;
  array->maxCount = newSize;
  return true;
}

bool bson_array_add_element(BsonArray *array, BsonElement *element, size_t allocSize) {
  if (array->count == array->maxCount) {
    if (!bson_array_resize(array, array->maxCount * 2)) {
      return false;
    }
  }
  BsonElement *allocElement = malloc(sizeof(BsonElement));
  allocElement->type = element->type;
  allocElement->size = element->size;
  allocElement->value = malloc(allocSize);
  memcpy(allocElement->value, element->value, allocSize);
  array->elements[array->count] = allocElement;
  array->count++;
  return true;
}

/*
  @brief Add a new element to the end of a given array

  @param array - The array to be modified
  @param type - The type of the element to be added
  @param value - The value of the element to be added
  @param allocSize - The size, in bytes, to be allocated for the element
  @param elementSize - The size, in bytes, of the element when converted to BSON format

  @return - true if the addition was successful, false if not
*/
bool bson_array_add(BsonArray *array, element_type type, void *value, size_t allocSize, size_t elementSize) {
  BsonElement element;
  element.type = type;
  element.value = value;
  element.size = elementSize;
  return bson_array_add_element(array, &element, allocSize);
}

bool bson_array_add_object(BsonArray *array, BsonObject *value) {
  return bson_array_add(array, TYPE_DOCUMENT, value, sizeof(BsonObject), 0);
}

bool bson_array_add_array(BsonArray *array, BsonArray *value) {
  return bson_array_add(array, TYPE_ARRAY, value, sizeof(BsonArray), 0);
}

bool bson_array_add_int32(BsonArray *array, int32_t value) {
  return bson_array_add(array, TYPE_INT32, &value, sizeof(int32_t),
                        SIZE_INT32);
}

bool bson_array_add_int64(BsonArray *array, int64_t value) {
  return bson_array_add(array, TYPE_INT64, &value, sizeof(int64_t),
                        SIZE_INT64);
}

bool bson_array_add_string(BsonArray *array, char *value) {
  return bson_array_add(array, TYPE_STRING, value, (strlen(value) + 1) * sizeof(char),
                        strlen(value) + STRING_OVERHEAD_BYTES);
}

bool bson_array_add_bool(BsonArray *array, bson_boolean value) {
  return bson_array_add(array, TYPE_BOOLEAN, &value, sizeof(bson_boolean),
                        SIZE_BOOLEAN);
}

bool bson_array_add_double(BsonArray *array, double value) {
  return bson_array_add(array, TYPE_DOUBLE, &value, sizeof(double),
                        SIZE_DOUBLE);
}

BsonElement *bson_array_get(BsonArray *array, size_t index) {
  return (index >= array->count) ? NULL : array->elements[index];
}

BsonObject *bson_array_get_object(BsonArray *array, size_t index) {
  BsonElement *element = bson_array_get(array, index);
  return (element == NULL || element->type != TYPE_DOCUMENT) ? 
          NULL : (BsonObject *)element->value;
}

BsonArray *bson_array_get_array(BsonArray *array, size_t index) {
  BsonElement *element = bson_array_get(array, index);
  return (element == NULL || element->type != TYPE_ARRAY) ? 
          NULL : (BsonArray *)element->value;
}

int32_t bson_array_get_int32(BsonArray *array, size_t index) {
  BsonElement *element = bson_array_get(array, index);
  return (element == NULL || element->type != TYPE_INT32) ? 
          -1 : *(int32_t *)element->value;
}

int64_t bson_array_get_int64(BsonArray *array, size_t index) {
  BsonElement *element = bson_array_get(array, index);
  return (element == NULL || element->type != TYPE_INT64) ? 
          -1 : *(int64_t *)element->value;
}

char *bson_array_get_string(BsonArray *array, size_t index) {
  BsonElement *element = bson_array_get(array, index);
  return (element == NULL || element->type != TYPE_STRING) ? 
         NULL : (char *)element->value;
}

bson_boolean bson_array_get_bool(BsonArray *array, size_t index) {
  BsonElement *element = bson_array_get(array, index);
  return (element == NULL || element->type != TYPE_BOOLEAN) ? 
          BOOLEAN_INVALID : *(bson_boolean *)element->value;
}

double bson_array_get_double(BsonArray *array, size_t index) {
  BsonElement *element = bson_array_get(array, index);
  return (element == NULL || element->type != TYPE_DOUBLE) ? 
          -1 : *(double *)element->value;
}
