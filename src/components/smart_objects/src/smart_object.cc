/**
 * @file SmartObject.cpp
 * @brief SmartObject source file.
 */
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "smart_objects/smart_object.h"

#include <errno.h>
#include <stdlib.h>
#include <cstdio>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

/**
 * @brief Value that is used as invalid value for string type
 **/
static const char* invalid_cstr_value = "";

SmartObject::SmartObject()
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
}

SmartObject::SmartObject(const SmartObject& Other)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  duplicate(Other);
}

SmartObject::SmartObject(SmartType Type)
    : m_type(SmartType_Null),
      m_schema() {
  switch (Type) {
    case SmartType_Integer:
      set_value_unsigned_int(0);
      break;
    case SmartType_Double:
      set_value_double(0);
      break;
    case SmartType_Boolean:
      set_value_bool(false);
      break;
    case SmartType_Character:
      set_value_char(' ');
      break;
    case SmartType_String:
      set_value_string("");
      break;
    case SmartType_Map:
      m_data.map_value = new SmartMap();
      m_type = SmartType_Map;
      break;
    case SmartType_Array:
      m_data.array_value = new SmartArray();
      m_type = SmartType_Array;
      break;
    case SmartType_Binary:
      set_value_binary(SmartBinary());
      break;
    case SmartType_Invalid:
      m_type = SmartType_Invalid;
      break;
    default: {
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      break;
    }
  }
}

SmartObject::~SmartObject() {
  cleanup_data();
}

SmartObject& SmartObject::operator=(const SmartObject& Other) {
  if (this != &Other)
    duplicate(Other);
  return *this;
}

bool SmartObject::operator==(const SmartObject& Other) const {
  if (m_type != Other.m_type)
    return false;

  switch (m_type) {
    case SmartType_Integer:
      return m_data.int_value == Other.m_data.int_value;
    case SmartType_Double:
      return m_data.double_value == Other.m_data.double_value;
    case SmartType_Boolean:
      return m_data.bool_value == Other.m_data.bool_value;
    case SmartType_Character:
      return m_data.char_value == Other.m_data.char_value;
    case SmartType_String:
      return *(m_data.str_value) == *(Other.m_data.str_value);
    case SmartType_Map:
      if (m_data.map_value == Other.m_data.map_value)
        return true;
      return std::equal(m_data.map_value->begin(), m_data.map_value->end(),
                        Other.m_data.map_value->begin());
    case SmartType_Array:
      if (m_data.array_value == Other.m_data.array_value)
        return true;
      return std::equal(m_data.array_value->begin(), m_data.array_value->end(),
                        Other.m_data.array_value->begin());
    case SmartType_Binary:
      if (m_data.binary_value == Other.m_data.binary_value)
        return true;
      return std::equal(m_data.binary_value->begin(),
                        m_data.binary_value->end(),
                        Other.m_data.binary_value->begin());
    case SmartType_Null:
      return true;
    case SmartType_Invalid:
      return true;
    default: {
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      break;
    }
  }
  return false;
}

// =============================================================
// INTEGER TYPE SUPPORT
// =============================================================
SmartObject::SmartObject(int32_t InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_integer(InitialValue);
}

int32_t SmartObject::asInt() const {
  return convert_int();
}

SmartObject& SmartObject::operator=(int32_t NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_integer(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(int32_t Value) const {
  int32_t comp = convert_int();
  if (comp == invalid_int_value) {
    return false;
  } else {
    return comp == Value;
  }
}

void SmartObject::set_value_integer(int32_t NewValue) {
  set_new_type(SmartType_Integer);
  m_data.int_value = NewValue;
}

int32_t SmartObject::convert_int() const {
  int32_t retval;

  switch (m_type) {
    case SmartType_String:
      retval = convert_string_to_unsigned_int(m_data.str_value);
      break;
    case SmartType_Boolean:
      retval = (m_data.bool_value == true) ? 1 : 0;
      break;
    case SmartType_Integer:
      retval = m_data.int_value;
      break;
    case SmartType_Double:
      retval = static_cast<uint32_t>(m_data.double_value);
      break;
    default: {
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      retval = invalid_int_value;
      break;
    }
  }

  return static_cast<int32_t>(retval);
}

// =============================================================
// uint32_t TYPE SUPPORT
// =============================================================
SmartObject::SmartObject(uint32_t InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_unsigned_int(InitialValue);
}

uint32_t SmartObject::asUInt() const {
  return convert_unsigned_int();
}

SmartObject& SmartObject::operator=(uint32_t NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_unsigned_int(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(uint32_t Value) const {
  int32_t comp = convert_unsigned_int();
  if (comp == invalid_int_value) {
    return false;
  } else {
    return static_cast<uint32_t>(comp) == Value;
  }
}

void SmartObject::set_value_unsigned_int(uint32_t NewValue) {
  set_new_type(SmartType_Integer);
  m_data.int_value = NewValue;
}

uint32_t SmartObject::convert_unsigned_int() const {
  uint32_t retval;

  switch (m_type) {
    case SmartType_String:
      retval = convert_string_to_unsigned_int(m_data.str_value);
      break;
    case SmartType_Boolean:
      return (m_data.bool_value == true) ? 1 : 0;
    case SmartType_Integer:
      retval = m_data.int_value;
      break;
    case SmartType_Double:
      retval = static_cast<uint32_t>(m_data.double_value);
      break;
    default: {
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      return invalid_int_value;
    }
  }

  return retval;
}

// =============================================================
// DOUBLE TYPE SUPPORT
// =============================================================
SmartObject::SmartObject(double InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_double(InitialValue);
}

double SmartObject::asDouble() const {
  return convert_double();
}

SmartObject& SmartObject::operator=(double NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_double(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(double Value) const {
  double comp = convert_double();
  if (comp == invalid_double_value) {
    return false;
  } else {
    return comp == Value;
  }
}

void SmartObject::set_value_double(double NewValue) {
  set_new_type(SmartType_Double);
  m_data.double_value = NewValue;
}

double SmartObject::convert_double(void) const {
  double retval;

  switch (m_type) {
    case SmartType_String:
      retval = convert_string_to_double(m_data.str_value);
      break;
    case SmartType_Boolean:
      retval = (m_data.bool_value) ? 1.0 : 0.0;
      break;
    case SmartType_Integer:
      retval = static_cast<double>(convert_int());
      break;
    case SmartType_Double:
      retval = m_data.double_value;
      break;
    default:
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      retval = invalid_double_value;
      break;
  }

  return retval;
}

// =============================================================
// BOOL TYPE SUPPORT
// =============================================================

SmartObject::SmartObject(bool InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_bool(InitialValue);
}

bool SmartObject::asBool() const {
  return convert_bool();
}

SmartObject& SmartObject::operator=(bool NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_bool(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(bool Value) const {
  bool comp = convert_bool();
  if (comp == invalid_bool_value) {
    return false;
  } else {
    return comp == Value;
  }
}

void SmartObject::set_value_bool(bool NewValue) {
  set_new_type(SmartType_Boolean);
  m_data.bool_value = NewValue;
}

bool SmartObject::convert_bool() const {
  bool retval;

  switch (m_type) {
    case SmartType_Boolean:
      retval = m_data.bool_value;
      break;
    case SmartType_Integer:
      retval = (m_data.int_value != 0);
      break;
    case SmartType_Double:
      retval = (m_data.double_value != 0.0);
      break;
    default:
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      retval = invalid_bool_value;
      break;
  }

  return retval;
}

// =============================================================
// CHAR TYPE SUPPORT
// =============================================================

SmartObject::SmartObject(char InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_char(InitialValue);
}

char SmartObject::asChar() const {
  return convert_char();
}

SmartObject& SmartObject::operator=(char NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_char(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(char Value) const {
  char comp = convert_char();
  if (comp == invalid_char_value) {
    return false;
  } else {
    return comp == Value;
  }
}

void SmartObject::set_value_char(char NewValue) {
  set_new_type(SmartType_Character);
  m_data.char_value = NewValue;
}

char SmartObject::convert_char() const {
  char retval;

  switch (m_type) {
    case SmartType_String:
      retval =
          (m_data.str_value->length() == 1) ?
              m_data.str_value->at(0) : invalid_char_value;
      break;
    case SmartType_Character:
      retval = m_data.char_value;
      break;
    default:
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      retval = invalid_char_value;
      break;
  }

  return retval;
}

// =============================================================
// STD::STRING TYPE SUPPORT
// =============================================================

SmartObject::SmartObject(const std::string& InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_string(InitialValue);
}

std::string SmartObject::asString() const {
  return convert_string();
}

const char* SmartObject::asCharArray() const {
  if (m_data.str_value != NULL) {
    return m_data.str_value->c_str();
  }

  return "";
}

SmartObject& SmartObject::operator=(const std::string& NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_string(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(std::string Value) const {
  std::string comp = convert_string();
  if (comp == invalid_string_value) {
    return false;
  } else {
    return comp == Value;
  }
}

void SmartObject::set_value_string(const std::string& NewValue) {
  set_new_type(SmartType_String);
  m_data.str_value = new std::string(NewValue);
}

std::string SmartObject::convert_string(void) const {
  std::string retval;

  switch (m_type) {
    case SmartType_String:
      retval = *(m_data.str_value);
      break;
    case SmartType_Integer:
      char val[20];
      sprintf(val, "%d", m_data.int_value);
      retval = std::string(val);
      break;
    case SmartType_Character:
      retval = std::string(1, m_data.char_value);
      break;
    case SmartType_Double:
      retval = convert_double_to_string(m_data.double_value);
      break;
    default:
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      retval = NsSmartDeviceLink::NsSmartObjects::invalid_cstr_value;
      break;
  }
  return retval;
}

// =============================================================
// CHAR* TYPE SUPPORT
// =============================================================

SmartObject::SmartObject(char* InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_cstr(InitialValue);
  return;
}

SmartObject& SmartObject::operator=(const char* NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_cstr(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const char* Value) const {
  std::string comp = convert_string();
  std::string val(Value);
  if (comp == invalid_string_value) {
    return false;
  } else {
    return comp == val;
  }
}

void SmartObject::set_value_cstr(const char* NewValue) {
  if (NewValue) {
    set_value_string(std::string(NewValue));
  } else {
    std::string tmp;
    set_value_string(tmp);
  }
}

// =============================================================
// BINARY TYPE SUPPORT
// =============================================================
SmartObject::SmartObject(const SmartBinary& InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_binary(InitialValue);
}

SmartBinary SmartObject::asBinary() const {
  return convert_binary();
}

SmartArray* SmartObject::asArray() const {
  if (m_type != SmartType_Array) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return NULL;
  }

  return m_data.array_value;
}

SmartObject& SmartObject::operator=(SmartBinary NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_binary(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(SmartBinary Value) const {
  SmartBinary comp = convert_binary();
  if (comp == invalid_binary_value) {
    return false;
  } else {
    return std::equal(comp.begin(), comp.end(), Value.begin());
  }
}

void SmartObject::set_value_binary(SmartBinary NewValue) {
  set_new_type(SmartType_Binary);
  m_data.binary_value = new SmartBinary(NewValue);
}

SmartBinary SmartObject::convert_binary(void) const {
  switch (m_type) {
    case SmartType_Binary:
      return *(m_data.binary_value);
    default: {
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      return invalid_binary_value;
    }
  }

  return invalid_binary_value;
}

// =============================================================
// ARRAY INTERFACE SUPPORT
// =============================================================

SmartObject& SmartObject::operator[](int32_t Index) {
  return handle_array_access(Index);
}

const SmartObject& SmartObject::operator[](int32_t Index) const {
  return getElement(Index);
}

inline SmartObject& SmartObject::handle_array_access(int32_t Index) {
  if (m_type == SmartType_Invalid) {
    return *this;
  }

  if (m_type != SmartType_Array) {
    cleanup_data();
    m_type = SmartType_Array;
    m_data.array_value = new SmartArray();
  }

  int32_t sz = m_data.array_value->size();
  if (Index == -1) {
    Index = sz;
  }
  if (Index == sz) {
    SmartObject uc;
    m_data.array_value->push_back(uc);
  }
  if (Index > sz || Index < 0) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return invalid_object_value;
  }
  return m_data.array_value->at(Index);
}

// =============================================================
// MAP INTERFACE SUPPORT
// =============================================================

SmartObject& SmartObject::operator[](const std::string Key) {
  return handle_map_access(Key);
}

const SmartObject& SmartObject::operator[] (const std::string Key) const {
  return getElement(Key);
}

SmartObject& SmartObject::operator[](char* Key) {
  std::string str(Key);
  return handle_map_access(str);
}
 
const SmartObject& SmartObject::operator[](char* Key) const {
  std::string str(Key);
  return getElement(str);
}

SmartObject& SmartObject::operator[](const char* Key) {
  std::string str(Key);
  return handle_map_access(str);
}

const SmartObject& SmartObject::operator[](const char* Key) const {
  return getElement(Key);
}

const SmartObject& SmartObject::getElement(size_t Index) const {
  if (SmartType_Array == m_type) {
    if (Index < m_data.array_value->size()) {
      return m_data.array_value->at(Index);
    }
  }

/*
#if !defined UNIT_TESTS
  NOTREACHED();
#endif
*/
  return invalid_object_value;
}

const SmartObject& SmartObject::getElement(const std::string & Key) const {
  if (SmartType_Map == m_type) {
    SmartMap::const_iterator i = m_data.map_value->find(Key);

    if (i != m_data.map_value->end()) {
      return i->second;
    }
  }

/*
#if !defined UNIT_TESTS
  NOTREACHED();
#endif
*/
  return invalid_object_value;
}

SmartObject& SmartObject::handle_map_access(const std::string Key) {
  if (m_type == SmartType_Invalid) {
    return *this;
  }

  // TODO(404): implement handling of non-existing keys similar to array
  if (m_type != SmartType_Map) {
    cleanup_data();
    m_type = SmartType_Map;
    m_data.map_value = new SmartMap();
  }

  // TODO(404): Add check for key presense
  return (*(m_data.map_value))[Key];
}

// =============================================================
// OTHER METHODS
// =============================================================
void SmartObject::duplicate(const SmartObject& OtherObject) {
  SmartData newData;
  SmartType newType = OtherObject.m_type;
  CSmartSchema newSchema = OtherObject.m_schema;

  switch (newType) {
    case SmartType_Map:
      newData.map_value = new SmartMap(*OtherObject.m_data.map_value);
      break;
    case SmartType_Array:
      newData.array_value = new SmartArray(*OtherObject.m_data.array_value);
      break;
    case SmartType_Integer:
      newData.int_value = OtherObject.m_data.int_value;
      break;
    case SmartType_Double:
      newData.double_value = OtherObject.m_data.double_value;
      break;
    case SmartType_Boolean:
      newData.bool_value = OtherObject.m_data.bool_value;
      break;
    case SmartType_Character:
      newData.char_value = OtherObject.m_data.char_value;
      break;
    case SmartType_String:
      newData.str_value = new std::string(*OtherObject.m_data.str_value);
      break;
    case SmartType_Binary:
      newData.binary_value = new SmartBinary(*OtherObject.m_data.binary_value);
      break;
    default:
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      break;
  }

  cleanup_data();

  m_type = newType;
  m_data = newData;
  m_schema = newSchema;
}

void SmartObject::cleanup_data() {
  switch (m_type) {
    case SmartType_String:
      delete m_data.str_value;
      break;
    case SmartType_Map:
      delete m_data.map_value;
      break;
    case SmartType_Array:
      delete m_data.array_value;
      break;
    case SmartType_Binary:
      delete m_data.binary_value;
      break;
    default:
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      break;
  }
}

size_t SmartObject::length() const {
  size_t size = 0;

  switch (m_type) {
    case SmartType_String:
      size = m_data.str_value->size();
      break;
    case SmartType_Array:
      size = m_data.array_value->size();
      break;
    case SmartType_Map:
      size = m_data.map_value->size();
      break;
    default:
/*
#if !defined UNIT_TESTS
      NOTREACHED();
#endif
*/
      size = 0;
      break;
  }

  return size;
}

void SmartObject::set_new_type(SmartType NewType) {
  cleanup_data();
  m_type = NewType;
}

double SmartObject::convert_string_to_double(const std::string* Value) {
  if (0 == Value->size()) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return invalid_double_value;
  }

  char* ptr;
  errno = 0;

  double result = strtod(Value->c_str(), &ptr);
  if (errno || (ptr != (Value->c_str() + Value->length()))) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return invalid_double_value;
  }

  return result;
}

std::string SmartObject::convert_double_to_string(const double& Value) {
  std::stringstream ss;

  // convert double to string w fixed notation, hi precision
  ss << std::fixed << std::setprecision(10) << Value;

  // output to std::string
  std::string s = ss.str();

  // remove trailing 000s    (123.1200 => 123.12,  123.000 => 123.)
  s.erase(s.find_last_not_of('0') + 1, std::string::npos);
  if (s[s.size() - 1] == '.') {
    // remove dangling decimal (123. => 123)
    s.erase(s.end() - 1);
  }
  return s;
}

uint32_t SmartObject::convert_string_to_unsigned_int(
    const std::string* Value) {
  if (0 == Value->size()) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return invalid_int_value;
  }

  char* ptr;
  errno = 0;
  uint32_t result = strtol(Value->c_str(), &ptr, 10);
  if (errno || (ptr != (Value->c_str() + Value->length()))) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return invalid_int_value;
  }

  return result;
}

SmartType SmartObject::getType() const {
  return m_type;
}

std::string NsSmartDeviceLink::NsSmartObjects::SmartObject::OperatorToTransform(const SmartMap::value_type &pair) {
    return pair.first;
}

std::set<std::string> SmartObject::enumerate() const {
  std::set<std::string> keys;

  if (m_type == SmartType_Map) {
        std::transform(
            m_data.map_value->begin(),
            m_data.map_value->end(),
            std::inserter(keys, keys.end()),
            //operator[](const SmartMap::value_type &pair){return pair.first;}
            &NsSmartDeviceLink::NsSmartObjects::SmartObject::OperatorToTransform
        );
  }
  return keys;
}

bool SmartObject::keyExists(const std::string & Key) const {
  if (m_type != SmartType_Map) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return false;
  }

  return m_data.map_value->count(Key);
}

bool SmartObject::erase(const std::string & Key) {
  if (m_type != SmartType_Map) {
/*
#if !defined UNIT_TESTS
    NOTREACHED();
#endif
*/
    return false;
  }

  return (1 == m_data.map_value->erase(Key));
}

bool SmartObject::isValid() const {
  return (Errors::OK == m_schema.validate(*this));
}

Errors::eType SmartObject::validate() {
  return m_schema.validate(*this);
}

void SmartObject::setSchema(CSmartSchema schema) {
  m_schema = schema;
}

CSmartSchema SmartObject::getSchema() {
  return m_schema;
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
