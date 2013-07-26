/**
 * @file SmartObject.hpp
 * @brief SmartObject header file.
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

#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_OBJECT_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_OBJECT_H_

#include <set>
#include <string>
#include <vector>
#include <map>

#include "smart_objects/smart_schema.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;

/**
 * @brief Enumeration with all types, supported by SmartObject
 **/
enum SmartType {
  /**
   * @brief Null value. Act as initial value.
   **/
  SmartType_Null = 0,

  /**
   * @brief Boolean value.
   **/
  SmartType_Boolean = 1,

  /**
   * @brief Integer value.
   **/
  SmartType_Integer = 2,

  /**
   * @brief Character value.
   **/
  SmartType_Character = 3,

  /**
   * @brief String value.
   **/
  SmartType_String = 4,

  /**
   * @brief Double value.
   **/
  SmartType_Double = 5,

  /**
   * @brief Map value. Gives possibility for object to act like hashtable.
   **/
  SmartType_Map = 6,

  /**
   * @brief Array value. Gives possibility for object to act like array.
   **/
  SmartType_Array = 7,

  /**
   * @brief Binary data value. Gives possibility for object to store binary data.
   **/
  SmartType_Binary = 8,

  /**
   * @brief Invalid value. Represents invalid object that cannot change his type.
   **/
  SmartType_Invalid = -1
};

/**
 * @brief SmartArray type
 **/
typedef std::vector<SmartObject> SmartArray;

/**
 * @brief SmartMap type
 **/
typedef std::map<std::string, SmartObject> SmartMap;

/**
 * @brief SmartBinary type
 **/
typedef std::vector<unsigned char> SmartBinary;

/**
 * @brief Main SmartObject class
 *
 * This class act as Variant type from other languages and can be used as primitive type
 * like bool, int, char, double, string and as complex type like array and map.
 **/
class SmartObject {
 public:
  /**
   * @brief Constructor.
   *
   * Creates object with Null type.
   **/
  SmartObject();

  /**
   * @brief Copy constructor.
   *
   * @param Other Object to be copied from.
   **/
  SmartObject(const SmartObject& Other);

  /**
   * @brief Constructor for creating object of given primitive type.
   *
   * Only primitive types (bool, int, char, double, string) are supported.
   *
   * @param type Type of the created object.
   **/
  explicit SmartObject(SmartType Type);

  /**
   * @brief Destructor
   *
   **/
  ~SmartObject();

  /**
   * @brief Assignment operator.
   *
   * @param  Other Other SmartObject
   * @return SmartObject&
   **/
  SmartObject& operator=(const SmartObject& Other);

  /**
   * @brief Comparison operator
   *
   * @param  Other Other SmartObject to be compared with
   * @return bool Result of comparison
   **/
  bool operator==(const SmartObject& Other) const;

  /**
   * @name Support of type: int
   * @{
   */
  /**
   * @brief Constructor for creating object of type: int
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(int InitialValue);

  /**
   * @brief Conversion operator to type: int
   *
   * @return int Value of the object converted to int type or invalid_int_value if
   *         conversion is not possible
   **/
  operator int() const;

  /**
   * @brief Returns current object converted to int
   *
   * @return int
   **/
  int asInt() const;

  /**
   * @brief Assignment operator for type: int
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(int NewValue);

  /**
   * @brief Comparison operator for comparing object with integer value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(int Value) const;

  // Support of type: unsigned int
  /**
   * @brief Constructor for creating object of type: int
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(unsigned int InitialValue);

  /**
   * @brief Conversion operator to type: int
   *
   * @return unsigned int Value of the object converted to int type or invalid_int_value if
   *         conversion is not possible
   **/
  operator unsigned int(void) const;

  /**
   * @brief Returns current object converted to unsigned int int
   *
   * @return double
   **/
  unsigned int asUInt() const;

  /**
   * @brief Assignment operator for type: int
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(unsigned int NewValue);

  /**
   * @brief Comparison operator for comparing object with unsigned int value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(unsigned int Value) const;

  /** @} */

  /**
   * @name Support of type: double
   * @{
   */
  /**
   * @brief Constructor for creating object of type: double
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(double InitialValue);

  /**
   * @brief Conversion operator to type: double
   *
   * @return double Value of the object converted to double type or invalid_double_value if
   *         conversion is not possible
   **/
  operator double() const;

  /**
   * @brief Returns current object converted to double
   *
   * @return double
   **/
  double asDouble() const;

  /**
   * @brief Assignment operator for type: double
   *
   * @param  new_value New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(double new_value);

  /**
   * @brief Comparison operator for comparing object with double value
   *
   * @param  new_value Value to compare object with
   * @return bool
   **/
  bool operator==(double new_value) const;
  /** @} */

  /**
   * @name Support of type: bool
   * @{
   */
  /**
   * @brief Constructor for creating object of type: bool
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(bool InitialValue);

  /**
   * @brief Conversion operator to type: bool
   *
   * @return bool Value of the object converted to bool type or invalid_bool_value if
   *         conversion is not possible
   **/
  operator bool() const;

  /**
   * @brief Returns current object converted to bool
   *
   * @return bool
   **/
  bool asBool() const;

  /**
   * @brief Assignment operator for type: bool
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(bool NewValue);

  /**
   * @brief Comparison operator for comparing object with bool value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(bool Value) const;
  /** @} */

  /**
   * @name Support of type: char
   * @{
   */
  /**
   * @brief Constructor for creating object of type: char
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(char InitialValue);

  /**
   * @brief Conversion operator to type: char
   *
   * @return char Value of the object converted to bool type or invalid_char_value if
   *         conversion is not possible
   **/
  operator char() const;

  /**
   * @brief Returns current object converted to char
   *
   * @return char
   **/
  char asChar() const;

  /**
   * @brief Assignment operator for type: char
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(char NewValue);

  /**
   * @brief Comparison operator for comparing object with char value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(char Value) const;
  /** @} */

  /**
   * @name Support of type: string
   * @{
   */
  /**
   * @brief Constructor for creating object of type: string
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(const std::string& InitialValue);

  /**
   * @brief Constructor for creating object of type: string
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(char* InitialValue);

  /**
   * @brief Conversion operator to type: string
   *
   * @return std::string Value of the object converted to bool type or invalid_string_value if
   *         conversion is not possible
   **/
  operator std::string(void) const;

  /**
   * @brief Returns current object converted to string
   *
   * @return std::string
   **/
  std::string asString() const;

  /**
   * @brief Assignment operator for type: string
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const std::string& NewValue);

  /**
   * @brief Assignment operator for type: string
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const char* NewValue);

  /**
   * @brief Comparison operator for comparing object with string value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(std::string Value) const;

  /**
   * @brief Comparison operator for comparing object with string value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(const char* Value) const;
  /** @} */

  /**
   * @name Support of type: binary
   * @{
   */
  /**
   * @brief Constructor for creating object of type: binary
   *
   * @param InitialValue Initial binary value
   **/
  explicit SmartObject(const SmartBinary& InitialValue);

  /**
   * @brief Conversion operator to type: binary
   *
   * @return SmartBinary Value of the object converted to binary type or invalid_binary_value if
   *         conversion is not possible
   **/
  operator SmartBinary(void) const;

  /**
   * @brief Returns current object converted to binary
   *
   * @return SmartBinary
   **/
  SmartBinary asBinary() const;

  /**
   * @brief Assignment operator for type: binary
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(SmartBinary);

  /**
   * @brief Comparison operator for comparing object with binary value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(SmartBinary) const;
  /** @} */

  /**
   * @name Array interface support
   * @{
   */
  /**
   * @brief Support of array-like access
   *
   * @param  Index index of element to return
   * @return SmartObject&
   **/
  SmartObject& operator[](int Index);

  /**
   * @brief Get array element.
   *
   * This method does not automatically convert the object to an array or
   * add elements. If this object is not an array or index is out of
   * range then null object is returned.
   *
   * @param Index Index of an element.
   *
   * @return Element of array or null object if element can't be provided.
   **/
  const SmartObject & getElement(size_t Index) const;

  /** @} */

  /**
   * @name Map/Hashtable interface support
   * @{
   */
  /**
   * @brief Support of map-like access
   *
   * @param  Key Key of element to return
   * @return SmartObject&
   **/
  SmartObject& operator[](const std::string Key);

  /**
   * @brief Support of map-like access
   *
   * @param  Key Key of element to return
   * @return SmartObject&
   **/
  SmartObject& operator[](char* Key);

  /**
   * @brief Support of map-like access
   *
   * @param  Key Key of element to return
   * @return SmartObject&
   **/
  SmartObject& operator[](const char* Key);

  /**
   * @brief Get map element.
   *
   * This method does not automatically convert the object to a map or
   * add elements. If this object is not a map or it does not contain
   * key then invalid object is returned.
   *
   * @param Key Key of an element.
   *
   * @return Element of map or null object if element can't be provided.
   **/
  const SmartObject & getElement(const std::string & Key) const;

  /**
   * @brief Enumerates content of the object when it behaves like a map.
   *
   * @return Set of map keys or empty set if object has type other than map
   **/
  std::set<std::string> enumerate() const;

  /**
   * @brief Checks for key presense when object is behaves like a map
   *
   * @param Key Key to check presense for
   * @return bool
   **/
  bool keyExists(const std::string & Key) const;

  /**
   * @brief Removes element from the map.
   *
   * @param Key Key of the element.
   *
   * @return true if success, false if there is no such element in the map
   */
  bool erase(const std::string & Key);
  /** @} */

  /**
   * @name Validation and schema support
   * @{
   */
  /**
   * @brief Validates object according to attached schema.
   *
   * @return bolean validation result
   **/
  bool isValid();

  /**
   * @brief Validates object according to attached schema.
   *
   * @return Result of validation.
   */
  Errors::eType validate();

  /**
   * @brief Sets new schema
   *
   * @param schema Schema for object validation
   * @return void
   **/
  void setSchema(CSmartSchema schema);

  /**
   * @brief Returns attached schema
   *
   * @return CSmartSchema
   **/
  CSmartSchema getSchema();
  /** @} */

  /**
   * @brief Returns current object type
   *
   * @return NsSmartObjects::SmartType
   **/
  SmartType getType() const;

  /**
   * @brief Returns length of object
   *
   * If object has type string, array or map then method returns corresponded
   * size. Otherwise returns zero.
   *
   * @note This method does not return size of binary data.
   *
   * @return size_t Length of the object
   **/
  size_t length() const;

 protected:
  /**
   * @name Support of type: int (internal)
   * @{
   */
  /**
   * @brief Sets new integer value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_integer(int NewValue);

  /**
   * @brief Sets new integer value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_unsigned_int(unsigned int NewValue);

  /**
   * @brief Converts object to int type
   *
   * @return int Converted value or invalid_int_value if conversion not possible
   **/
  inline int convert_int(void) const;

  /**
   * @brief Converts object to int type
   *
   * @return int Converted value or invalid_int_value if conversion not possible
   **/
  inline unsigned int convert_unsigned_int(void) const;
  /** @} */

  /**
   * @name Support of type: char (internal)
   * @{
   */
  /**
   * @brief Sets new char value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_char(char NewValue);

  /**
   * @brief Converts object to char type
   *
   * @return int Converted value or invalid_char_value if conversion not possible
   **/
  inline char convert_char(void) const;
  /** @} */

  /**
   * @name Support of type: double (internal)
   * @{
   */
  /**
   * @brief Sets new double value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_double(double NewValue);

  /**
   * @brief Converts object to double type
   *
   * @return int Converted value or invalid_double_value if conversion not possible
   **/
  inline double convert_double(void) const;
  /** @} */

  /**
   * @name Support of type: bool (internal)
   * @{
   */
  /**
   * @brief Sets new bool value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_bool(bool NewValue);

  /**
   * @brief Converts object to bool type
   *
   * @return int Converted value or invalid_bool_value if conversion not possible
   **/
  inline bool convert_bool(void) const;
  /** @} */

  /**
   * @name Support of type: string (internal)
   * @{
   */
  /**
   * @brief Sets new string value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_string(const std::string& NewValue);

  /**
   * @brief Sets new string value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_cstr(const char* NewValue);

  /**
   * @brief Converts object to string type
   *
   * @return int Converted value or invalid_string_value if conversion not possible
   **/
  inline std::string convert_string(void) const;
  /** @} */

  /**
   * @name Support of type: binary (internal)
   * @{
   */
  /**
   * @brief Sets new binary value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_binary(SmartBinary NewValue);

  /**
   * @brief Converts object to binary type
   *
   * @return int Converted value or invalid_binary_value if conversion not possible
   **/
  inline SmartBinary convert_binary(void) const;
  /** @} */

  /**
   * @name Array-like interface support (internal)
   * @{
   */
  /**
   * @brief Returns SmartObject from internal array data by it's index
   *
   * @param Index Index of element to retrieve
   * @return SmartObject&
   **/
  SmartObject& handle_array_access(int Index);
  /** @} */

  /**
   * @name Map-like interface support (internal)
   * @{
   */
  /**
   * @brief Returns SmartObject from internal map data by it's key
   *
   * @param Key Key of element to retrieve
   * @return SmartObject&
   **/
  inline SmartObject& handle_map_access(std::string Key);
  /** @} */

  /**
   * @name Helper conversion methods
   * @{
   */
  /**
   * @brief Converts string to double
   *
   * @param Value Pointer to string to convert
   * @return double
   **/
  static double convert_string_to_double(const std::string* Value);

  /**
   * @brief Converts string to unsigned int
   *
   * @param Value Pointer to string to convert
   * @return unsigned int int
   **/
  static unsigned int convert_string_to_unsigned_int(const std::string* Value);

  /**
   * @brief Converts double value to string
   *
   * @param Value Value to be converted
   * @return std::string
   **/
  static std::string convert_double_to_string(const double& Value);
  /** @} */

  /**
   * @brief Duplicates another SmartObject
   *
   * After calling that function current SmartObject will have the same
   * type, schema and data, as passed.
   *
   * @param  OtherObject Object to be duplicated
   * @return void
   **/
  void duplicate(const SmartObject& OtherObject);

  /**
   * @brief Cleans up internal data for some types (like string, array or map)
   *
   * @return void
   **/
  void cleanup_data();

  /**
   * @brief Sets new internal type and cleans up if it changes
   *
   * @param NewType New object type
   * @return void
   **/
  void set_new_type(SmartType NewType);

  /**
   * @brief Current type of the object
   **/
  SmartType m_type;

  /**
   * @brief Union for holding actual internal object data
   **/
  typedef union {
    double double_value;
    bool bool_value;
    char char_value;
    // int int_value;
    unsigned int unsigned_int_value;
    std::string* str_value;
    SmartArray* array_value;
    SmartMap* map_value;
    SmartBinary* binary_value;
  } SmartData;

  /**
   * @brief Current internal object data
   **/
  SmartData m_data;

  /**
   * @brief Validation schema, attached to the object
   **/
  CSmartSchema m_schema;
};

/**
 * @brief Value that is used as invalid value for bool type
 **/
static const bool invalid_bool_value = false;

/**
 * @brief Value that is used as invalid value for int type
 **/
static const int invalid_int_value = -1;

/**
 * @brief Value that is used as invalid value for char type
 **/
static const char invalid_char_value = 0;

/**
 * @brief Value that is used as invalid value for string type
 **/
static const std::string invalid_string_value = "";

/**
 * @brief Value that is used as invalid value for double type
 **/
static const double invalid_double_value = -1;

/**
 * @brief Value that is used as invalid value for string type
 **/
static const char* invalid_cstr_value = "";

/**
 * @brief Value that is used as invalid value for object type
 **/
static SmartObject invalid_object_value(SmartType_Invalid);

/**
 * @brief Value that is used as invalid value for object type
 **/
static const SmartBinary invalid_binary_value;
}
}
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_OBJECT_H_
