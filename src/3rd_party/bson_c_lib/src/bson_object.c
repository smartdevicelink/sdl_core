#include "bson_object.h"
#define DEFAULT_MAP_SIZE 32

size_t hash_function(const char* key, size_t maxValue) {
  size_t keyLength = strlen(key);
  size_t hash = 0;
  int i;
  for (i = 0; i < keyLength; i++) {
    hash += key[i];
    hash %= maxValue;
  }
  return hash;
}

bool bson_object_initialize(BsonObject *obj, size_t capacity, float loadFactor) {
  return emhashmap_initialize(&obj->data, (int)capacity, loadFactor, &hash_function);
}

bool bson_object_initialize_default(BsonObject *obj) {
  return bson_object_initialize(obj, DEFAULT_MAP_SIZE, 0.5f);
}

void bson_object_deinitialize(BsonObject *obj) {
  MapIterator iterator = emhashmap_iterator(&obj->data);
  MapEntry *current = emhashmap_iterator_next(&iterator);
  while (current != NULL) {
    BsonElement *element = (BsonElement *)current->value;
    if (element->type == TYPE_DOCUMENT) {
      bson_object_deinitialize((BsonObject *)element->value);
    }
    else if (element->type == TYPE_ARRAY) {
      bson_array_deinitialize((BsonArray *)element->value);
    }
    free(element->value);
    free(element);
    current = emhashmap_iterator_next(&iterator);
  }

  emhashmap_deinitialize(&obj->data);
}

size_t bson_object_size(BsonObject *obj) {
  size_t objSize = OBJECT_OVERHEAD_BYTES;
  MapIterator iterator = emhashmap_iterator(&obj->data);
  MapEntry *current = emhashmap_iterator_next(&iterator);
  while (current != NULL) {
    BsonElement *element = (BsonElement *)current->value;
    objSize += object_key_size(current->key) + ELEMENT_OVERHEAD_BYTES;
    if (element->type == TYPE_DOCUMENT) {
      objSize += bson_object_size((BsonObject *)element->value);
    }
    else if (element->type == TYPE_ARRAY) {
      objSize += bson_array_size((BsonArray *)element->value);
    }
    else {
      objSize += element->size;
    }
    current = emhashmap_iterator_next(&iterator);
  }
  return objSize;
}

uint8_t *bson_object_to_bytes(BsonObject *obj) {
  //TODO just move the pointer rather than keep a position variable
  MapIterator iterator = emhashmap_iterator(&obj->data);
  MapEntry *current = emhashmap_iterator_next(&iterator);
  size_t objSize = bson_object_size(obj);
  uint8_t *bytes = malloc(objSize);
  size_t position = 0;
  write_int32_le(bytes, (int32_t)objSize, &position);

  while (current != NULL) {
    BsonElement *element = (BsonElement *)current->value;

    bytes[position++] = element->type;

    uint8_t *keyBytes = string_to_byte_array(current->key);
    memcpy(&bytes[position], keyBytes, strlen(current->key));
    free(keyBytes);
    position += strlen(current->key);

    //Null-terminate
    bytes[position++] = 0x00;

    switch (element->type) {
      case TYPE_DOCUMENT: {
        BsonObject *subObject = (BsonObject *)element->value;
        uint8_t *subObjectBytes = bson_object_to_bytes(subObject);
        if (subObjectBytes == NULL) {
          printf("An error occured while parsing the object with key \"%s\"\n", current->key);
          free(bytes);
          return NULL;
        }
        size_t subObjectSize = bson_object_size(subObject);
        memcpy(&bytes[position], subObjectBytes, subObjectSize);
        free(subObjectBytes);
        position += subObjectSize;
        break;
      }
      case TYPE_ARRAY: {
        BsonArray *array = (BsonArray *)element->value;
        uint8_t *arrayBytes = bson_array_to_bytes(array);
        if (arrayBytes == NULL) {
          printf("An error occured while parsing the object with key \"%s\"\n", current->key);
          free(bytes);
          return NULL;
        }
        size_t arraySize = bson_array_size(array);
        memset(&bytes[position], 0, arraySize);
        memcpy(&bytes[position], arrayBytes, arraySize);
        free(arrayBytes);
        position += arraySize;
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
        position += sizeof(element->value);
      }
    }
    current = emhashmap_iterator_next(&iterator);
  }

  bytes[position++] = DOCUMENT_END;
  if (position != objSize) {
    printf("Something went horribly wrong. Unexpected size of map in bytes: %i, expected size: %i\n", (int)position, (int)objSize);
    free(bytes);
    return NULL;
  }
  return bytes;
}

BsonObject bson_object_from_bytes(uint8_t *data) {
  uint8_t *current = data;
  int32_t size = read_int32_le(&current);
  BsonObject obj;
  bson_object_initialize_default(&obj);
  uint8_t type = *current;
  current++;
  while (type != DOCUMENT_END) {
    char *key = byte_array_to_string(current);
    current += strlen(key) + 1;

    switch ((element_type)type) {
      case TYPE_DOCUMENT: {
        BsonObject subObject = bson_object_from_bytes(current);
        bson_object_put_object(&obj, key, &subObject);
        current += bson_object_size(&subObject);
        break;
      }
      case TYPE_ARRAY: {
        BsonArray array = bson_array_from_bytes(current);
        bson_object_put_array(&obj, key, &array);
        current += bson_array_size(&array);
        break;
      }
      case TYPE_INT32: {
        int32_t value = read_int32_le(&current);
        bson_object_put_int32(&obj, key, value);
        break;
      }
      case TYPE_INT64: {
        int64_t value = read_int64_le(&current);
        bson_object_put_int64(&obj, key, value);
        break;
      }
      case TYPE_STRING: {
        //String length is read first
        int32_t stringLength = read_int32_le(&current) - 1;

        char *stringVal = byte_array_to_bson_string(current, stringLength);
        bson_object_put_string(&obj, key, stringVal);
        free(stringVal);
        current += stringLength + 1;
        break;
      }
      case TYPE_DOUBLE: {
        double value = read_double_le(&current);
        bson_object_put_double(&obj, key, value);
        break;
      }
      case TYPE_BOOLEAN: {
        bson_object_put_bool(&obj, key, *current);
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
    printf("Unexpected parsed object size. Expected %i, got %i\n", (int) size, (int)(current - data));
  }
  return obj;
}

char *bson_object_to_string(BsonObject *obj, char *out) {
  //TODO just move the pointer rather than keep a position variable
  int position = 0;
  MapIterator iterator = emhashmap_iterator(&obj->data);
  MapEntry *current = emhashmap_iterator_next(&iterator);
  position += sprintf(out, "{ ");
  while (current != NULL) {
    BsonElement *element = (BsonElement *)current->value;
    position += sprintf(&out[position], "\"%s\":", current->key);
    switch (element->type) {
      case TYPE_DOCUMENT: {
        char docString[512];
        position += sprintf(&out[position], "%s", bson_object_to_string(bson_object_get_object(obj, current->key), docString));
        break;
      }
      case TYPE_ARRAY: {
        char docString[512];
        position += sprintf(&out[position], "%s", bson_array_to_string(bson_object_get_array(obj, current->key), docString));
        break;
      }
      case TYPE_INT32: {
        position += sprintf(&out[position], "%i", (int)bson_object_get_int32(obj, current->key));
        break;
      }
      case TYPE_INT64: {
        position += sprintf(&out[position], "%li", (long)bson_object_get_int64(obj, current->key));
        break;
      }
      case TYPE_STRING: {
        position += sprintf(&out[position], "\"%s\"", bson_object_get_string(obj, current->key));
        break;
      }
      case TYPE_DOUBLE: {
        position += sprintf(&out[position], "%f", bson_object_get_double(obj, current->key));
        break;
      }
      case TYPE_BOOLEAN: {
        position += sprintf(&out[position], "%s", (bson_object_get_bool(obj, current->key) == BOOLEAN_TRUE) ? "true" : "false");
        break;
      }
      default: {
        printf("Unrecognized BSON type: %i\n", element->type);
        position += sprintf(&out[position], "UNKNOWN_TYPE");
      }
    }
    current = emhashmap_iterator_next(&iterator);
    if (current != NULL) {
      position += sprintf(&out[position], ", ");    
    }
  }
  sprintf(&out[position], " }");

  return out;
}

bool bson_object_put_element(BsonObject *obj, const char *key, BsonElement *element, size_t allocSize) {
  BsonElement *allocElement = malloc(sizeof(BsonElement));
  allocElement->type = element->type;
  allocElement->size = element->size;
  allocElement->value = malloc(allocSize);
  memcpy(allocElement->value, element->value, allocSize);
  BsonElement *existingElement = emhashmap_remove(&obj->data, key);
  if (existingElement != NULL) {
    if (existingElement->type == TYPE_DOCUMENT) {
      bson_object_deinitialize((BsonObject *)existingElement->value);
    }
    else if (existingElement->type == TYPE_ARRAY) {
      bson_array_deinitialize((BsonArray *)existingElement->value);
    }
    free(existingElement->value);
    free(existingElement);
  }
  return emhashmap_put(&obj->data, key, (void *)allocElement);
}

bool bson_object_put(BsonObject *obj, const char *key, element_type type, void *value, size_t allocSize, size_t elementSize) {
  BsonElement element;
  element.type = type;
  element.value = value;
  element.size = elementSize;
  return bson_object_put_element(obj, key, &element, allocSize);
}

bool bson_object_put_object(BsonObject *obj, const char *key, BsonObject *value) {
  return bson_object_put(obj, key, TYPE_DOCUMENT, value, sizeof(BsonObject), 0);
}

bool bson_object_put_array(BsonObject *obj, const char *key, BsonArray *value) {
  return bson_object_put(obj, key, TYPE_ARRAY, value, sizeof(BsonArray), 0);
}

bool bson_object_put_int32(BsonObject *obj, const char *key, int32_t value) {
  return bson_object_put(obj, key, TYPE_INT32, &value, sizeof(int32_t),
                         SIZE_INT32);
}

bool bson_object_put_int64(BsonObject *obj, const char *key, int64_t value) {
  return bson_object_put(obj, key, TYPE_INT64, &value, sizeof(int64_t),
                         SIZE_INT64);
}

bool bson_object_put_string(BsonObject *obj, const char *key, char *value) {
  return bson_object_put(obj, key, TYPE_STRING, value, (strlen(value) + 1) * sizeof(char),
                         strlen(value) + STRING_OVERHEAD_BYTES);
}

bool bson_object_put_bool(BsonObject *obj, const char *key, bson_boolean value) {
  return bson_object_put(obj, key, TYPE_BOOLEAN, &value, sizeof(bson_boolean),
                         SIZE_BOOLEAN);
}

bool bson_object_put_double(BsonObject *obj, const char *key, double value) {
  return bson_object_put(obj, key, TYPE_DOUBLE, &value, sizeof(double),
                         SIZE_DOUBLE);
}

BsonElement *bson_object_get(BsonObject *obj, const char *key) {
  MapEntry *entry = emhashmap_get(&obj->data, key);
  return (entry == NULL) ? NULL : entry->value;
}

BsonObject *bson_object_get_object(BsonObject *obj, const char *key) {
  BsonElement *element = bson_object_get(obj, key);
  return (element == NULL || element->type != TYPE_DOCUMENT) ? 
          NULL : (BsonObject *)element->value;
}

BsonArray *bson_object_get_array(BsonObject *obj, const char *key) {
  BsonElement *element = bson_object_get(obj, key);
  return (element == NULL || element->type != TYPE_ARRAY) ? 
          NULL : (BsonArray *)element->value;
}

int32_t bson_object_get_int32(BsonObject *obj, const char *key) {
  BsonElement *element = bson_object_get(obj, key);
  return (element == NULL || element->type != TYPE_INT32) ? 
          -1 : *(int32_t *)element->value;
}

int64_t bson_object_get_int64(BsonObject *obj, const char *key) {
  BsonElement *element = bson_object_get(obj, key);
  return (element == NULL || element->type != TYPE_INT64) ? 
          -1 : *(int64_t *)element->value;
}

char *bson_object_get_string(BsonObject *obj, const char *key) {
  BsonElement *element = bson_object_get(obj, key);
  return (element == NULL || element->type != TYPE_STRING) ? 
          NULL : (char *)element->value;
}

bson_boolean bson_object_get_bool(BsonObject *obj, const char *key) {
  BsonElement *element = bson_object_get(obj, key);
  return (element == NULL || element->type != TYPE_BOOLEAN) ? 
          BOOLEAN_INVALID : *(bson_boolean *)element->value;
}

double bson_object_get_double(BsonObject *obj, const char *key) {
  BsonElement *element = bson_object_get(obj, key);
  return (element == NULL || element->type != TYPE_DOUBLE) ? 
          -1 : *(double *)element->value;
}

MapIterator bson_object_iterator(BsonObject *obj) {
  return emhashmap_iterator(&obj->data);
}

BsonObjectEntry bson_object_iterator_next(MapIterator *iterator) {
  MapEntry *entry = emhashmap_iterator_next(iterator);
  BsonObjectEntry bsonEntry;
  strncpy(bsonEntry.key, entry->key, 255);
  bsonEntry.element = (BsonElement *)entry->value;
  return bsonEntry;
}