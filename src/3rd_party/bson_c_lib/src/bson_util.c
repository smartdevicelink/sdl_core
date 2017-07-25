#include "bson_util.h"

void write_int32_le(uint8_t *bytes, int32_t value, size_t *position) {
  int i = 0;
  for (i = 0; i < SIZE_INT32; i++) {
    bytes[(*position)++] = (uint8_t)value & 0x000000FF;
    value >>= 8;
  }
}

void write_int64_le(uint8_t *bytes, int64_t value, size_t *position) {
  int i = 0;
  for (i = 0; i < SIZE_INT64; i++) {
    bytes[(*position)++] = (uint8_t)value & 0x000000FFull;
    value >>= 8;
  }
}

void write_double_le(uint8_t *bytes, double value, size_t *position) {
  union doubleUnion_t {
    double value;
    uint64_t intValue;
  };
  union doubleUnion_t unionVal;
  unionVal.value = value;
  int i = 0;
  for (i = 0; i < SIZE_DOUBLE; i++) {
    bytes[(*position)++] = (uint8_t)unionVal.intValue & 0x000000FFull;
    unionVal.intValue >>= 8;
  }
}

int32_t read_int32_le(uint8_t **bytes) {
  int32_t value = 0;
  int i = 0;
  for (i = SIZE_INT32 - 1; i >= 0; i--) {
    value <<= 8;
    value += (*bytes)[i];
  }
  (*bytes) += SIZE_INT32;
  return value;
}

int64_t read_int64_le(uint8_t **bytes) {
  int32_t value = 0;
  int i = 0;
  for (i = SIZE_INT64 - 1; i >= 0; i--) {
    value <<= 8;
    value += (*bytes)[i];
  }
  (*bytes) += SIZE_INT64;
  return value;
}

double read_double_le(uint8_t **bytes) {
  union doubleUnion_t {
    double value;
    uint64_t intValue;
  };
  union doubleUnion_t unionVal;
  unionVal.intValue = 0;
  int i = 0;
  for (i = SIZE_DOUBLE - 1; i >= 0; i--) {
    unionVal.intValue <<= 8;
    unionVal.intValue += (*bytes)[i];
  }
  (*bytes) += SIZE_DOUBLE;
  return unionVal.value;
}

uint8_t *string_to_byte_array(char *stringVal) {
  size_t length = strlen(stringVal);
  uint8_t *bytes = malloc(length + 1);
  int i = 0;
  for (i = 0; i < length; i++) {
    bytes[i] = stringVal[i];
  }
  bytes[length] = 0x00;
  return bytes;
}

size_t c_string_length(uint8_t *value) {
  size_t length = 0;
  while (value[length] != 0x00) {
    length++;
  }
  return length;
}

char *byte_array_to_string(uint8_t *bytes) {
  size_t length = c_string_length(bytes);
  return byte_array_to_bson_string(bytes, length);
}

char *byte_array_to_bson_string(uint8_t *bytes, size_t length) {
  char *stringVal = malloc(sizeof(char) * (length + 1));
  
  int i = 0;
  for (i = 0; i < length; i++) {
    stringVal[i] = bytes[i] & 0xFF;
  }
  stringVal[length] = 0x00;
  return stringVal;
}

uint8_t *index_to_key(size_t index) {
  size_t length = digits(index);
  uint8_t *bytes = malloc(length);
  size_t modValue = index;
  int i = 0;
  for (i = (int)length - 1; i >= 0; i--) {
    //Convert digit to UTF-8
    bytes[i] = 0x30 + (modValue % 10);
    modValue /= 10;
  }
  return bytes;
}

size_t object_key_size(char *key) {
  return strlen(key) + 1;
}

size_t array_key_size(size_t index) {
  //One byte for each decimal digit in index plus null character
  return digits(index) + 1;
}

size_t digits(size_t value) {
  size_t modValue = value;
  size_t numDigits = 1;
  while (modValue >= 10) {
    numDigits++;
    modValue /= 10;
  }
  return numDigits;
}
