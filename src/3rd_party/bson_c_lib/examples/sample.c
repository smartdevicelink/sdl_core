#include "../src/bson_object.h"
#include <stdio.h>
BsonObject obj;

void construct_object() {
  printf("boolean value before: %i\n", BOOLEAN_TRUE);
  bson_object_put_bool(&obj, "correct", BOOLEAN_TRUE);
  bson_boolean getElement = bson_object_get_bool(&obj, "correct");
  printf("boolean value after: %i\n", getElement);
  
  bson_object_put_int32(&obj, "one", 64);
  bson_object_put_double(&obj, "two", 2.5);
  bson_object_put_string(&obj, "chars", "aaaaaaaaaaaa");

  BsonObject subObj;
  bson_object_initialize(&subObj, 1, 0.5f);
  bson_object_put_int32(&subObj, "four", 64);
  bson_object_put_object(&obj, "three", &subObj);
  
  BsonArray array;
  bson_array_initialize(&array, 2);
  bson_array_add_int32(&array, 23);
  bson_array_add_double(&array, 5.4);
  bson_array_add_string(&array, "A string");
  
  BsonObject subObj2;
  bson_object_initialize(&subObj2, 1, 0.5f);
  bson_object_put_int32(&subObj2, "i", 64);
  bson_array_add_object(&array, &subObj2);
  
  bson_object_put_array(&obj, "anArray", &array);
  char objString[1024];
  printf("Constructed object: %s\n", bson_object_to_string(&obj, objString));
}

int main(int argc, char** argv) {
  bson_object_initialize(&obj, 16, 0.5f);
  construct_object();
  uint8_t *bytes = bson_object_to_bytes(&obj);
  if (bytes == NULL) {
    printf("Conversion to bytes failed\n");
  }
  else {
    int i = 0;
    for (i = 0; i < bson_object_size(&obj); i++) {
      if (i % 8 == 0) {
        printf("\n");
      }
      printf("0x%02hhx ", bytes[i]);
    }
    printf("\n\n");
    BsonObject parsedObj = bson_object_from_bytes(bytes);
    char objString[1024];
    printf("Parsed object: %s\n", bson_object_to_string(&parsedObj, objString));
    bson_object_deinitialize(&parsedObj);
    free(bytes);
  }

  bson_object_deinitialize(&obj);
  return 0;
}
