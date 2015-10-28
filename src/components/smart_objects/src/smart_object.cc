/*
 * Copyright (c) 2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "smart_objects/smart_object.h"

#include <errno.h>
#include <inttypes.h>
#include <limits>
#include <stdlib.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <iterator>
#include <limits>

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
    case SmartType_Null:
      break;
    case SmartType_Integer:
      set_value_integer(0);
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
    default:
      DCHECK(!"Unhandled smart object type");
      break;
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
    case SmartType_Map: {
      if (m_data.map_value == Other.m_data.map_value)
        return true;
      if (m_data.map_value->size() != Other.m_data.map_value->size())
        return false;
      return std::equal(m_data.map_value->begin(), m_data.map_value->end(),
                        Other.m_data.map_value->begin());
      }
    case SmartType_Array: {
      if (m_data.array_value == Other.m_data.array_value)
        return true;
      if (m_data.array_value->size() != Other.m_data.array_value->size())
        return false;
      return std::equal(m_data.array_value->begin(), m_data.array_value->end(),
                        Other.m_data.array_value->begin());
      }
    case SmartType_Binary: {
      if (m_data.binary_value == Other.m_data.binary_value)
        return true;
      if (m_data.array_value->size() != Other.m_data.array_value->size())
        return false;
      return std::equal(m_data.binary_value->begin(),
                        m_data.binary_value->end(),
                        Other.m_data.binary_value->begin());
      }
    case SmartType_Null:
      return true;
    case SmartType_Invalid:
      return true;
    default:
      DCHECK(!"Unhandled smart object type");
      break;
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
  const int64_t convert = convert_int();
  if (invalid_int64_value == convert) {
    return invalid_int_value;
  }
#ifdef OS_WIN32
  DCHECK(convert >= INT_MIN);
  DCHECK(convert <= INT_MAX);
#else
  DCHECK(convert >= std::numeric_limits<int32_t>::min());
  DCHECK(convert <= std::numeric_limits<int32_t>::max());
#endif

  return static_cast<int32_t>(convert);
}

SmartObject& SmartObject::operator=(const int32_t NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_integer(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const int32_t Value) const {
  const int64_t comp = convert_int();
  if (comp == invalid_int64_value) {
    return false;
  }
  return comp == static_cast<int64_t>(Value);
}

void SmartObject::set_value_integer(int64_t NewValue) {
  set_new_type(SmartType_Integer);
  m_data.int_value = NewValue;
}

int64_t SmartObject::convert_int() const {
  switch (m_type) {
    case SmartType_String:
      return convert_string_to_integer(m_data.str_value);
    case SmartType_Boolean:
      return (m_data.bool_value == true) ? 1 : 0;
    case SmartType_Integer:
      return m_data.int_value;
    case SmartType_Double:
      return static_cast<int64_t>(m_data.double_value);
    default:
      break;
  }
  return invalid_int64_value;
}

// =============================================================
// uint32_t TYPE SUPPORT
// =============================================================
SmartObject::SmartObject(uint32_t InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_integer(InitialValue);
}

uint32_t SmartObject::asUInt() const {
  const int64_t convert = convert_int();
  if (invalid_int64_value == convert) {
    return invalid_unsigned_int_value;
  }
#ifdef OS_WIN32
  DCHECK(convert >= 0);
  DCHECK(convert <= UINT_MAX);
#else
  DCHECK(convert >= std::numeric_limits<uint32_t>::min());
  DCHECK(convert <= std::numeric_limits<uint32_t>::max());
#endif
  return static_cast<uint32_t>(convert);
}

SmartObject& SmartObject::operator=(const uint32_t NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_integer(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const uint32_t Value) const {
  const int64_t comp = convert_int();
  if (comp == invalid_int_value) {
    return false;
  }
  return comp == static_cast<int64_t>(Value);
}

// =============================================================
// int64_t TYPE SUPPORT
// =============================================================
SmartObject::SmartObject(int64_t InitialValue)
    : m_type(SmartType_Null),
      m_schema() {
  m_data.str_value = NULL;
  set_value_integer(InitialValue);
}

int64_t SmartObject::asInt64() const {
  return convert_int();
}

SmartObject& SmartObject::operator=(const int64_t NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_integer(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const int64_t Value) const {
  const int64_t comp = convert_int();
  if (comp == invalid_int_value) {
    return false;
  }
  return comp == Value;
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

SmartObject& SmartObject::operator=(const double NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_double(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const double Value) const {
  const double comp = convert_double();
  if (comp == invalid_double_value) {
    return false;
  }
  return comp == Value;
}

void SmartObject::set_value_double(const double NewValue) {
  set_new_type(SmartType_Double);
  m_data.double_value = NewValue;
}

double SmartObject::convert_double() const {
  switch (m_type) {
    case SmartType_String:
      return convert_string_to_double(m_data.str_value);
    case SmartType_Boolean:
      return (m_data.bool_value) ? 1.0 : 0.0;
    case SmartType_Integer:
      return static_cast<double>(convert_int());
    case SmartType_Double:
      return m_data.double_value;
    default:
      break;
  }
  return invalid_double_value;
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

SmartObject& SmartObject::operator=(const bool NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_bool(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const bool Value) const {
  bool comp = convert_bool();
  // FIXME(EZamakhov): invalide bool?
  if (comp == invalid_bool_value) {
    return false;
  }
  return comp == Value;
}

void SmartObject::set_value_bool(bool NewValue) {
  set_new_type(SmartType_Boolean);
  m_data.bool_value = NewValue;
}

bool SmartObject::convert_bool() const {
  switch (m_type) {
    case SmartType_Boolean:
      return m_data.bool_value;
    case SmartType_Integer:
      return (m_data.int_value != 0);
    case SmartType_Double:
      return (m_data.double_value != 0.0);
    default:
      break;
  }
  return invalid_bool_value;
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

SmartObject& SmartObject::operator=(const char NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_char(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const char Value) const {
  const char comp = convert_char();
  if (comp == invalid_char_value) {
    return false;
  }
  return comp == Value;
}

void SmartObject::set_value_char(char NewValue) {
  set_new_type(SmartType_Character);
  m_data.char_value = NewValue;
}

char SmartObject::convert_char() const {
  switch (m_type) {
    case SmartType_String:
     return
          (m_data.str_value->length() == 1) ?
              m_data.str_value->at(0) : invalid_char_value;
    case SmartType_Character:
      return m_data.char_value;
    default:
      break;
  }
  return invalid_char_value;
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

bool SmartObject::operator==(const std::string& Value) const {
  const std::string comp = convert_string();
  if (comp == invalid_string_value) {
    return false;
  }
  return comp == Value;
}

void SmartObject::set_value_string(const std::string& NewValue) {
  set_new_type(SmartType_String);
  m_data.str_value = new std::string(NewValue);
}

std::string SmartObject::convert_string() const {
  switch (m_type) {
    case SmartType_String:
      return *(m_data.str_value);
    case SmartType_Integer: {
        std::stringstream stream;
        stream << m_data.int_value;
        return stream.str();
      }
    case SmartType_Character:
      return std::string(1, m_data.char_value);
    case SmartType_Double:
      return convert_double_to_string(m_data.double_value);
    default:
      break;
  }
  return NsSmartDeviceLink::NsSmartObjects::invalid_cstr_value;
}

// =============================================================
// CHAR* TYPE SUPPORT
// =============================================================

SmartObject::SmartObject(const char* const InitialValue)
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
  const std::string comp = convert_string();
  if (comp == invalid_string_value) {
    return false;
  }
  return (0 == comp.compare(Value));
}

void SmartObject::set_value_cstr(const char* NewValue) {
  set_value_string(NewValue ? std::string(NewValue) : std::string());
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
    return NULL;
  }
  return m_data.array_value;
}

SmartObject& SmartObject::operator=(const SmartBinary& NewValue) {
  if (m_type != SmartType_Invalid) {
    set_value_binary(NewValue);
  }
  return *this;
}

bool SmartObject::operator==(const SmartBinary& Value) const {
  const SmartBinary comp = convert_binary();
  if (comp == invalid_binary_value) {
    return false;
  }
  if (comp.size() != Value.size())
    return false;
  return std::equal(comp.begin(), comp.end(), Value.begin());
}

void SmartObject::set_value_binary(const SmartBinary& NewValue) {
  set_new_type(SmartType_Binary);
  m_data.binary_value = new SmartBinary(NewValue);
}

SmartBinary SmartObject::convert_binary() const {
  switch (m_type) {
    case SmartType_Binary:
      return *(m_data.binary_value);
    default:
      break;
  }
  return invalid_binary_value;
}

// =============================================================
// ARRAY INTERFACE SUPPORT
// =============================================================

SmartObject& SmartObject::operator[](const int32_t Index) {
  return handle_array_access(Index);
}

const SmartObject& SmartObject::operator[](const int32_t Index) const {
  return getElement(Index);
}

inline SmartObject& SmartObject::handle_array_access(const int32_t Index) {
  if (m_type == SmartType_Invalid) {
    return *this;
  }

  if (m_type != SmartType_Array) {
    cleanup_data();
    m_type = SmartType_Array;
    m_data.array_value = new SmartArray();
  }
  SmartArray& array = *m_data.array_value;
  if (Index == -1 || static_cast<size_t>(Index) == array.size()) {
    array.push_back(SmartObject());
    return array[array.size() - 1];
  }
  if (Index >= 0 && (static_cast<size_t>(Index) < array.size())) {
    DCHECK(sizeof(Index) <= sizeof(array.size()));
    return array[Index];
  }
  // FIXME(EZamakhov): return always the same reference - multi-thread problem?
  return invalid_object_value;
}

// =============================================================
// MAP INTERFACE SUPPORT
// =============================================================

SmartObject& SmartObject::operator[](const std::string& Key) {
  return handle_map_access(Key);
}

const SmartObject& SmartObject::operator[] (const std::string& Key) const {
  return getElement(Key);
}

SmartObject& SmartObject::operator[](char* Key) {
  return handle_map_access(std::string(Key));
}

const SmartObject& SmartObject::operator[](char* Key) const {
  return getElement(std::string(Key));
}

SmartObject& SmartObject::operator[](const char* Key) {
  return handle_map_access(std::string(Key));
}

const SmartObject& SmartObject::operator[](const char* Key) const {
  return getElement(std::string(Key));
}

const SmartObject& SmartObject::getElement(size_t Index) const {
  if (SmartType_Array == m_type) {
    if (Index < m_data.array_value->size()) {
      return m_data.array_value->at(Index);
    }
  }
  return invalid_object_value;
}

const SmartObject& SmartObject::getElement(const std::string& Key) const {
  if (SmartType_Map == m_type) {
    SmartMap::const_iterator it = m_data.map_value->find(Key);
    if (it != m_data.map_value->end()) {
      return it->second;
    }
  }
  return invalid_object_value;
}

SmartObject& SmartObject::handle_map_access(const std::string& Key) {
  if (m_type == SmartType_Invalid) {
    return *this;
  }

  if (m_type != SmartType_Map) {
    cleanup_data();
    m_type = SmartType_Map;
    m_data.map_value = new SmartMap();
  }
  SmartMap& map = *m_data.map_value;

  return map[Key];
}

// =============================================================
// OTHER METHODS
// =============================================================
void SmartObject::duplicate(const SmartObject& OtherObject) {
  SmartData newData;
  const SmartType newType = OtherObject.m_type;
  switch (newType) {
    case SmartType_Null: // on duplicate empty SmartObject
      return;
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
      DCHECK(!"Unhandled smart object type");
      return;
  }
  m_schema = OtherObject.m_schema;

  cleanup_data();

  m_type = newType;
#ifdef OS_WIN32
  if (m_type != SmartType_Null && m_type != SmartType_Invalid){
	  m_data = newData;
  }
#else
  m_data = newData;
#endif
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
      break;
  }
}

size_t SmartObject::length() const {
  switch (m_type) {
    case SmartType_String:
      return m_data.str_value->size();
    case SmartType_Array:
      return m_data.array_value->size();
    case SmartType_Map:
      return m_data.map_value->size();
    case SmartType_Binary:
      return m_data.binary_value->size();
    default:
      break;
  }
  return 0;
}

bool SmartObject::empty() const {
  switch (m_type) {
    case SmartType_String:
      return m_data.str_value->empty();
    case SmartType_Array:
      return m_data.array_value->empty();
    case SmartType_Map:
      return m_data.map_value->empty();
    case SmartType_Binary:
      return m_data.binary_value->empty();
    default:
      break;
  }
  return true;
}

void SmartObject::set_new_type(SmartType NewType) {
  cleanup_data();
  m_type = NewType;
}

double SmartObject::convert_string_to_double(const std::string* Value) {
  if (!Value || Value->empty()) {
    return invalid_double_value;
  }

  char* ptr;
  errno = 0;

  double result = strtod(Value->c_str(), &ptr);
  if (errno || (ptr != (Value->c_str() + Value->length()))) {
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

uint64_t SmartObject::convert_string_to_integer(const std::string* Value) {
  if (!Value || Value->empty()) {
    return invalid_int64_value;
  }
  int64_t result;
  std::stringstream stream(*Value);
  stream >> result;
  if (stream.eof()) {
    return result;
  }
  return invalid_int64_value;
}

SmartType SmartObject::getType() const {
  return m_type;
}

std::string SmartObject::OperatorToTransform(const SmartMap::value_type &pair) {
    return pair.first;
}

std::set<std::string> SmartObject::enumerate() const {
  std::set<std::string> keys;

  if (m_type == SmartType_Map) {
        std::transform(
            m_data.map_value->begin(),
            m_data.map_value->end(),
            std::inserter(keys, keys.end()),
            &SmartObject::OperatorToTransform
        );
  }
  return keys;
}

bool SmartObject::keyExists(const std::string& Key) const {
  if (m_type != SmartType_Map) {
    return false;
  }
  return m_data.map_value->find(Key) != m_data.map_value->end();
}

bool SmartObject::erase(const std::string& Key) {
  if (m_type != SmartType_Map) {
    return false;
  }
  return (m_data.map_value->erase(Key) > 0);
}

bool SmartObject::isValid() const {
  return (Errors::OK == m_schema.validate(*this));
}

Errors::eType SmartObject::validate() {
  return m_schema.validate(*this);
}

void SmartObject::setSchema(const CSmartSchema& schema) {
  m_schema = schema;
}

CSmartSchema SmartObject::getSchema() {
  return m_schema;
}

}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
