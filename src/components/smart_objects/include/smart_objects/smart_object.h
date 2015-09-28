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

#ifndef SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_OBJECT_H_
#define SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_OBJECT_H_

#include <set>
#include <string>
#include <vector>
#include <map>

#include "smart_objects/smart_schema.h"
#include "utils/custom_string.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {

namespace custom_str = utils::custom_string;

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
   * @brief Binary data value. Gives possibility for object to store binary
   *data.
   **/
  SmartType_Binary = 8,

  /**
   * @brief Unsigned Integer value.
   **/
  SmartType_UInteger = 9,

  /**
   * @brief Invalid value. Represents invalid object that cannot change his
   *type.
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
typedef std::vector<uint8_t> SmartBinary;

typedef utils::SharedPtr<SmartObject> SmartObjectSPtr;

/**
 * @brief List of SmartObjects
 */
typedef std::vector<SmartObjectSPtr> SmartObjectList;

/**
 * @brief Main SmartObject class
 *
 * This class act as Variant type from other languages and can be used as
 *primitive type
 * like bool, int32_t, char, double, string and as complex type like array and
 *map.
 **/

class SmartObject FINAL {
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
   * @brief Constructor for avoid cast
   * from unknown type
   *
   * @param pointer
   **/
  template <typename UnknownType>
  SmartObject(const UnknownType&);

  /**
   * @brief Constructor for creating object of given primitive type.
   *
   * Only primitive types (bool, int32_t, char, double, string) are supported.
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
   * @name Support of type: int32_t
   * @{
   */
  /**
   * @brief Constructor for creating object of type: int32_t
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(const int32_t InitialValue);

  /**
   * @brief Returns current object converted to int64_t
   *
   * @return int64_t
   **/
  int64_t asInt() const;

  /**
   * @brief Assignment operator for type: int32_t
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const int32_t NewValue);

  /**
   * @brief Comparison operator for comparing object with integer value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(const int32_t Value) const;

  // Support of type: uint32_t
  /**
   * @brief Constructor for creating object of type: int32_t
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(const uint32_t InitialValue);

  /**
   * @brief Returns current object converted to uint64_t
   *
   * @return uint64_t
   **/
  uint64_t asUInt() const;

  /**
   * @brief Assignment operator for type: int32_t
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const uint32_t NewValue);

  /**
   * @brief Comparison operator for comparing object with uint32_t value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(const uint32_t Value) const;

  /**
   * @name Support of type: int64_t
   * @{
   */
  /**
   * @brief Constructor for creating object of type: int64_t
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(const int64_t InitialValue);

  /**
   * @brief Returns current object converted to int64_t
   *
   * @return int64_t
   **/
  int64_t asInt64() const;

  /**
   * @brief Assignment operator for type: int64_t
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const int64_t NewValue);

  /**
   * @brief Comparison operator for comparing object with integer value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(const int64_t Value) const;

  /**
    * @name Support of type: uint64_t
    * @{
   **/

  /**
   * @brief Assignment operator for type: uint64_t
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const uint64_t NewValue);

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
   * @brief Constructor for creating object of type: CustomString
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(const custom_str::CustomString& InitialValue);

  /**
   * @brief Constructor for creating object of type: string
   *
   * @param InitialValue Initial object value
   **/
  explicit SmartObject(const char* InitialValue);

  /**
   * @brief Returns current object converted to CustomString
   *
   * @return custom_str::CustomString
   **/
  custom_str::CustomString asCustomString() const;

  /**
   * @brief Returns current object converted to string
   *
   * @return std::string
   **/
  std::string asString() const;

  /**
   * @brief Returns char array from SmartObject data if exist. Otherwise returns
   *        empty string
   *
   * @return const char*
   **/
  const char* asCharArray() const;

  /**
   * @brief Assignment operator for type: CustomString
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const custom_str::CustomString& NewValue);

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
  bool operator==(const std::string& Value) const;

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
   * @brief Returns current object converted to binary
   *
   * @return SmartBinary
   **/
  SmartBinary asBinary() const;

  /**
   * @brief Returns current object converted to array
   *
   * @return SmartArray
   **/
  SmartArray* asArray() const;

  /**
   * @brief Assignment operator for type: binary
   *
   * @param  NewValue New object value
   * @return SmartObject&
   **/
  SmartObject& operator=(const SmartBinary&);

  /**
   * @brief Comparison operator for comparing object with binary value
   *
   * @param  Value Value to compare object with
   * @return bool
   **/
  bool operator==(const SmartBinary&) const;
  /** @} */

  /**
   * @name Array interface support
   * @{
   */
  /**
   * @brief Support of array-like access
   *
   * @param  Index index of element to return, on -1 create new element at end
   * @return SmartObject&
   **/
  SmartObject& operator[](int32_t Index);
  const SmartObject& operator[](int32_t Index) const;

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
  const SmartObject& getElement(size_t Index) const;
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
  SmartObject& operator[](const std::string& Key);
  const SmartObject& operator[](const std::string& Key) const;

  /**
   * @brief Support of map-like access
   *
   * @param  Key Key of element to return
   * @return SmartObject&
   **/
  SmartObject& operator[](char* Key);
  const SmartObject& operator[](char* Key) const;

  /**
   * @brief Support of map-like access
   *
   * @param  Key Key of element to return
   * @return SmartObject&
   **/
  SmartObject& operator[](const char* Key);

  /**
   * @brief Support of map-like access
   *
   * @param  Key Key of element to return
   * @return const SmartObject&
   **/
  const SmartObject& operator[](const char* Key) const;

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
  const SmartObject& getElement(const std::string& Key) const;

  /**
   * @brief Enumerates content of the object when it behaves like a map.
   *
   * @return Set of map keys or empty set if object has type other than map
   **/
  std::set<std::string> enumerate() const;

  SmartMap::iterator map_begin() const {
    DCHECK(m_type == SmartType_Map);
    return m_data.map_value->begin();
  }
  SmartMap::iterator map_end() const {
    DCHECK(m_type == SmartType_Map);
    return m_data.map_value->end();
  }

  /**
   * @brief Checks for key presense when object is behaves like a map
   *
   * @param Key Key to check presense for
   * @return bool
   **/
  bool keyExists(const std::string& Key) const;

  /**
   * @brief Removes element from the map.
   *
   * @param Key Key of the element.
   *
   * @return true if success, false if there is no such element in the map
   */
  bool erase(const std::string& Key);
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
  bool isValid() const;

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
  void setSchema(const CSmartSchema& schema);

  /**
   * @brief Returns attached schema
   *
   * @return CSmartSchema
   **/
  CSmartSchema getSchema();

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
   * @return size_t Length of the object
   **/
  size_t length() const;

  /**
   * @brief Object string/array/map/binary empty state
   *
   * @return Returns object string/array/map/binary empty state
   * or true on other object typed
   **/
  bool empty() const;

  /**
   * @brief Nequation template operator
   *
   * @param  Other value to be compared with
   * @return bool Result of nequation
   **/
  template <typename Type>
  bool operator!=(const Type& Other) const {
    return !(*this == Other);
  }

 protected:
  static std::string OperatorToTransform(const SmartMap::value_type& pair);
  /**
   * @name Support of type: int32_t (internal)
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
  inline void set_value_integer(int64_t NewValue);

  /**
   * @brief Converts object to int32_t type
   *
   * @return int32_t Converted value or invalid_int_value if conversion not
   *possible
   **/
  inline int64_t convert_int() const;
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
   * @return int32_t Converted value or invalid_char_value if conversion not
   *possible
   **/
  inline char convert_char() const;
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
   * @return int32_t Converted value or invalid_double_value if conversion not
   *possible
   **/
  inline double convert_double() const;
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
   * @return int32_t Converted value or invalid_bool_value if conversion not
   *possible
   **/
  inline bool convert_bool() const;
  /** @} */

  /**
   * @brief Sets new string value to the object.
   *
   * This method changes also internal object type
   *
   * @param  NewValue New object value
   * @return void
   **/
  inline void set_value_string(const custom_str::CustomString& NewValue);

  /**
   * @brief Sets new CustomString value to the object.
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
   * @return int32_t Converted value or invalid_string_value if conversion not
   *possible
   **/
  inline std::string convert_string() const;
  /** @} */

  /**
   * @brief Converts object to CustomString type
   *
   * @return CustomString Converted value or
   * invalid_string_value if conversion not possible
   **/
  inline custom_str::CustomString convert_custom_string() const;

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
  inline void set_value_binary(const SmartBinary& NewValue);

  /**
   * @brief Converts object to binary type
   *
   * @return int32_t Converted value or invalid_binary_value if conversion not
   *possible
   **/
  inline SmartBinary convert_binary() const;

  /**
   * @brief Returns SmartObject from internal array data by it's index
   *
   * @param Index Index of element to retrieve
   * @return SmartObject&
   **/
  SmartObject& handle_array_access(const int32_t Index);

  /**
   * @brief Returns SmartObject from internal map data by it's key
   *
   * @param Key Key of element to retrieve
   * @return SmartObject&
   **/
  inline SmartObject& handle_map_access(const std::string& Key);

  /**
   * @brief Converts string to double
   *
   * @param Value Pointer to string to convert
   * @return double
   **/
  static double convert_string_to_double(const custom_str::CustomString* Value);

  /**
   * @brief Converts string to int64_t
   *
   * @param Value Pointer to string to convert
   * @return int64_t int64_t
   **/
  static uint64_t convert_string_to_integer(
      const custom_str::CustomString* Value);

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
    int64_t int_value;
    custom_str::CustomString* str_value;
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
 * @brief Value that is used as invalid value for int32_t type
 **/
static const int32_t invalid_int_value = -1;
static const uint32_t invalid_unsigned_int_value = 0;
static const int64_t invalid_int64_value = -1;

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
 * @brief Value that is used as invalid value for object type
 **/
static SmartObject invalid_object_value(SmartType_Invalid);

/**
 * @brief Value that is used as invalid value for object type
 **/
static const SmartBinary invalid_binary_value;
}  // namespace NsSmartObjects
}  // namespace NsSmartDeviceLink
#endif  // SRC_COMPONENTS_SMART_OBJECTS_INCLUDE_SMART_OBJECTS_SMART_OBJECT_H_
