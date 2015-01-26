/**
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

#ifndef VALIDATED_TYPES_INL_H_
#define VALIDATED_TYPES_INL_H_

#include "rpc_base.h"

#include <cassert>
#include <cstdio>

#include "rpc_base/validation_report.h"

namespace rpc {

/*
 * Range helper class
 */
template <typename T>
Range<T>::Range(T min, T max)
    : min_(min),
      max_(max) {
}

template <typename T>
T Range<T>::min() const {
  return min_;
}

template <typename T>
T Range<T>::max() const {
  return max_;
}

template <typename T>
template <typename U>
bool Range<T>::Includes(U val) const {
  return min() <= val && val <= max();
}


/*
 * PrimitiveType base class
 */
inline PrimitiveType::PrimitiveType(ValueState value_state)
    : value_state_(value_state),
      policy_table_type_(policy_table_interface_base::PT_PRELOADED) {
}

inline bool PrimitiveType::is_initialized() const {
  return value_state_ != kUninitialized;
}

inline bool PrimitiveType::is_valid() const {
  return value_state_ == kValid;
}

inline void PrimitiveType::ReportErrors(ValidationReport* report) const {
  switch (value_state_) {
    case kUninitialized: {
      report->set_validation_info("value is not initialized");
      break;
    }
    case kInvalid: {
      report->set_validation_info("value initialized incorrectly");
      break;
    }
    case kValid: {
      // No error
      break;
    }
    default: {
      assert(!"Unexpected value state");
      break;
    }
  }
}

/*
 * CompositeType base class
 */
inline void CompositeType::mark_initialized() {
  initialization_state__ = kInitialized;
}

inline CompositeType::CompositeType(InitializationState init_state)
    : initialization_state__(init_state),
      policy_table_type_(policy_table_interface_base::PT_PRELOADED) {
}

inline void CompositeType::ReportErrors(ValidationReport* report) const {
  switch (initialization_state__) {
    case kUninitialized: {
      report->set_validation_info("object is not initialized");
      break;
    }
    case kInvalidInitialized: {
      report->set_validation_info("object initialized incorrectly");
      break;
    }
    case kInitialized: {
      // No error
      break;
    }
    default:
      assert(!"Unexpected initialization state");
      break;
  }
}

/*
 * Boolean class
 */
inline Boolean::Boolean()
    : PrimitiveType(kUninitialized),
      value_(false) {
}

inline Boolean::Boolean(bool value)
    : PrimitiveType(kValid), value_(value) {
}

inline Boolean& Boolean::operator=(bool new_val) {
  value_ = new_val;
  value_state_ = kValid;
  return *this;
}

inline Boolean::operator bool() const {
  return value_;
}

/*
 * Integer class
 */
template<typename T, T minval, T maxval>
const Range<T> Integer<T, minval, maxval>::range_(minval, maxval);

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer()
    : PrimitiveType(kUninitialized),
      value_(range_.min()) {
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(IntType value)
    : PrimitiveType(range_.Includes(value) ? kValid : kInvalid),
      value_(value) {
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>& Integer<T, minval, maxval>::operator=(IntType new_val) {
  value_ = new_val;
  value_state_ = range_.Includes(value_) ? kValid : kInvalid;
  return *this;
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>& Integer<T, minval, maxval>::operator++() {
  ++value_;
  return *this;
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>& Integer<T, minval, maxval>::operator+=(int value) {
  value_ += value;
  return *this;
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::operator IntType() const {
  return value_;
}

/*
 * Float class
 */
template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
const Range<double> Float<minnum, maxnum, minden, maxden>::range_(
    (double(minnum)/minden), (double(maxnum)/maxden));


template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float()
    : PrimitiveType(kUninitialized),
      value_(range_.min()) {
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(double value)
    : PrimitiveType(range_.Includes(value) ? kValid : kInvalid),
      value_(value) {
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>&
Float<minnum, maxnum, minden, maxden>::operator=(double new_val) {
  value_ = new_val;
  value_state_ = range_.Includes(new_val) ? kValid : kInvalid;
  return *this;
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::operator double() const {
  return value_;
}

/*
 * String class
 */
template<size_t minlen, size_t maxlen>
const Range<size_t> String<minlen, maxlen>::length_range_(minlen, maxlen);

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String()
    : PrimitiveType(kUninitialized) {
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const std::string& value)
    : PrimitiveType(length_range_.Includes(value.length()) ? kValid : kInvalid),
      value_(value) {
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const char* value)
    : PrimitiveType(kUninitialized),
      value_(value) {
  value_state_ = length_range_.Includes(value_.length()) ? kValid : kInvalid;
}

template<size_t minlen, size_t maxlen>
bool String<minlen, maxlen>::operator<(String new_val) {
  return value_ < new_val.value_;
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>& String<minlen, maxlen>::operator=(const std::string& new_val) {
  value_ = new_val;
  value_state_ = length_range_.Includes(new_val.length()) ? kValid : kInvalid;
  return *this;
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::operator const std::string&() const {
  return value_;
}

/*
 * Enum class
 */
template<typename T>
Enum<T>::Enum()
    : PrimitiveType(kUninitialized),
      value_(EnumType()) {
}

template<typename T>
Enum<T>::Enum(EnumType value)
    : PrimitiveType(IsValidEnum(value) ? kValid : kInvalid),
      value_(value) {
}

template<typename T>
Enum<T>& Enum<T>::operator=(EnumType new_val) {
  value_ = new_val;
  value_state_ = IsValidEnum(value_) ? kValid : kInvalid;
  return *this;
}

template<typename T>
Enum<T>::operator EnumType() const {
  return value_;
}

/*
 * Array class
 */
template<typename T, size_t minsize, size_t maxsize>
Array<T, minsize, maxsize>::Array()
    : CompositeType(kUninitialized) {
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
Array<T, minsize, maxsize>::Array(const U& value)
    : ArrayType(value.begin(), value.end()),
      CompositeType(kUninitialized) {
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
Array<T, minsize, maxsize>&
Array<T, minsize, maxsize>::operator=(const U& that) {
  this->assign(that.begin(), that.end());
  return *this;
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
void Array<T, minsize, maxsize>::push_back(const U& value) {
  ArrayType::push_back(T(value));
}

template<typename T, size_t minsize, size_t maxsize>
bool Array<T, minsize, maxsize>::is_valid() const {
  // Empty array might be valid only if marked initialized
  if (this->empty() && (initialization_state__ != kInitialized)) {
    return false;
  }
  // Array size must be within allowed range
  if (!Range<size_t>(minsize, maxsize).Includes(this->size())) {
    return false;
  }
  // All array elements must be valid
  for (typename ArrayType::const_iterator i = this->begin();
      i != this->end(); ++i) {
    if (!i->is_valid()) {
      return false;
    }
  }
  return true;
}

template<typename T, size_t minsize, size_t maxsize>
bool Array<T, minsize, maxsize>::is_initialized() const {
  // Array that is not empty is initialized for sure
  if (!this->empty()) {
    return true;
  }
  // Empty array is initialized if not marked as unitialized
  if (initialization_state__ != kUninitialized) {
    return true;
  }
  return false;
}

template<typename T, size_t minsize, size_t maxsize>
void Array<T, minsize, maxsize>::ReportErrors(ValidationReport* report) const {
  if (this->empty()) {
    CompositeType::ReportErrors(report);
  } else {
    if (!Range<size_t>(minsize, maxsize).Includes(this->size())) {
      report->set_validation_info("array has invalid size");
    } else {
      // No error
    }
  }
  for (size_t i = 0; i != this->size(); ++i) {
    const T& elem = this->operator [](i);
    if (!elem.is_valid()) {
      char elem_idx[32] = {};
      snprintf(elem_idx, 32, "[%zu]", i);
      ValidationReport& elem_report =
          report->ReportSubobject(elem_idx);
      elem.ReportErrors(&elem_report);
    }
  }
}

template<typename T, size_t minsize, size_t maxsize>
void Array<T, minsize, maxsize>::SetPolicyTableType(
    rpc::policy_table_interface_base::PolicyTableType pt_type) {

  for (typename ArrayType::iterator it = this->begin();
      it != this->end(); ++it) {
      it->SetPolicyTableType(pt_type);
  }
}

/*
 * Map class
 */
template<typename T, size_t minsize, size_t maxsize>
Map<T, minsize, maxsize>::Map()
    : CompositeType(kUninitialized) {
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
Map<T, minsize, maxsize>::Map(const U& value)
    : CompositeType(kUninitialized) {
  for (typename U::const_iterator i = value.begin(), e = value.end(); i != e;
      ++i) {
    // Explicitly convert that value to T because all rpc_types have explicit
    // constructors
    insert(typename MapType::value_type(i->first, T(i->second)));
  }
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
Map<T, minsize, maxsize>&
Map<T, minsize, maxsize>::operator=(const U& that) {
  this->clear();
  for (typename U::const_iterator i = that.begin(), e = that.end(); i != e;
      ++i) {
    // Explicitly convert that value to T because all rpc_types have explicit
    // constructors
    insert(typename MapType::value_type(i->first, T(i->second)));
  }
  return *this;
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
void Map<T, minsize, maxsize>::insert(const std::pair<std::string, U>& value) {
  MapType::insert(typename MapType::value_type(value.first, T(value.second)));
}

template<typename T, size_t minsize, size_t maxsize>
bool Map<T, minsize, maxsize>::is_valid() const {
  // Empty map might be valid only if marked initialized
  if (this->empty() && (initialization_state__ != kInitialized)) {
    return false;
  }
  // Maps size must be within allowed range
  if (!Range<size_t>(minsize, maxsize).Includes(this->size())) {
    return false;
  }
  // All map elements must be valid
  for (typename Map::const_iterator i = this->begin();
      i != this->end(); ++i) {
    if (!i->second.is_valid()) {
      return false;
    }
  }
  return true;
}

template<typename T, size_t minsize, size_t maxsize>
bool Map<T, minsize, maxsize>::is_initialized() const {
  // Map that is not empty is initialized for sure
  if (!this->empty()) {
    return true;
  }
  // Empty map might be initialized only if not marked as unitialized
  if (initialization_state__ != kUninitialized) {
    return true;
  }
  return false;
}

template<typename T, size_t minsize, size_t maxsize>
void Map<T, minsize, maxsize>::ReportErrors(ValidationReport* report) const {
  if (this->empty()) {
    CompositeType::ReportErrors(report);
  } else {
    if (!Range<size_t>(minsize, maxsize).Includes(this->size())) {
      report->set_validation_info("map has invalid size");
    } else {
      // No error
    }
  }
  for (typename Map::const_iterator i = this->begin();
      i != this->end(); ++i) {
    if (!i->second.is_valid()) {
      std::string elem_name = "[\"" + i->first + "\"]";
      ValidationReport& elem_report = report->ReportSubobject(elem_name);
      i->second.ReportErrors(&elem_report);
    }
  }
}

template<typename T, size_t minsize, size_t maxsize>
void Map<T, minsize, maxsize>::SetPolicyTableType(
    rpc::policy_table_interface_base::PolicyTableType pt_type) {
  for (typename Map::iterator it = this->begin();
      it != this->end(); ++it) {
    it->second.SetPolicyTableType(pt_type);
  }
}

/*
 * Nullable class
 */
template<typename T>
Nullable<T>::Nullable()
    : marked_null_(false) {
}

template<typename T>
template<typename U>
Nullable<T>::Nullable(const U& value)
    : T(value),
      marked_null_(false) {
}

template<typename T>
template<typename U>
Nullable<T>& Nullable<T>::operator=(const U& new_val) {
  this->T::operator=(new_val);
  return *this;
}

template<typename T>
bool Nullable<T>::is_valid() const {
  return is_null() || T::is_valid();
}

template<typename T>
bool Nullable<T>::is_initialized() const {
  return is_null() || T::is_initialized();
}

template<typename T>
bool Nullable<T>::is_null() const {
  return marked_null_;
}

template<typename T>
void Nullable<T>::set_to_null() {
  marked_null_ = true;
}

template<typename T>
void Nullable<T>::ReportErrors(ValidationReport* report) const {
  if (marked_null_) {
    // No error
  } else {
    T::ReportErrors(report);
  }
}

/*
 * Optional class
 */
template<typename T>
Optional<T>::Optional() {
}

template<typename T>
template<typename U>
Optional<T>::Optional(const U& value)
    : value_(value) {
}

template<typename T>
T& Optional<T>::operator*() {
  return value_;
}

template<typename T>
const T& Optional<T>::operator*() const {
  return value_;
}

template<typename T>
T* Optional<T>::operator->() {
  return &value_;
}

template<typename T>
const T* Optional<T>::operator->() const {
  return &value_;
}

template<typename T>
Optional<T>::operator const void*() const {
  return is_initialized() ? &value_ : NULL;
}

template<typename T>
bool Optional<T>::is_valid() const {
  return !value_.is_initialized() || value_.is_valid();
}

template<typename T>
bool Optional<T>::is_initialized() const {
  return value_.is_initialized();
}

template<typename T>
void Optional<T>::ReportErrors(ValidationReport* report) const {
  if (!is_initialized()) {
    // No error
  } else {
    value_.ReportErrors(report);
  }
}
template<typename T>
inline rpc::policy_table_interface_base::PolicyTableType Optional<T>::GetPolicyTableType() const {
  return policy_table_type_;
}

template<typename T>
void rpc::Optional<T>::SetPolicyTableType(rpc::policy_table_interface_base::PolicyTableType pt_type) {
  policy_table_type_ = pt_type;
  value_.SetPolicyTableType(pt_type);
}

/*
 * Stringifyable class
 */
template<typename T>
Stringifyable<T>::Stringifyable()
  : predefined_string_("") {
}

template<typename T>
template<typename U>
Stringifyable<T>::Stringifyable(const U& value)
  : T(value),
    predefined_string_("") {
}

template<typename T>
template<typename U>
Stringifyable<T>& Stringifyable<T>::operator=(const U& new_val) {
  this->T::operator=(new_val);
  return *this;
}

template<typename T>
bool Stringifyable<T>::is_valid() const {
  return is_string() || T::is_valid();
}

template<typename T>
bool Stringifyable<T>::is_initialized() const {
  return is_string() || T::is_initialized();
}

template<typename T>
bool Stringifyable<T>::is_string() const {
  return !predefined_string_.empty();
}

template<typename T>
std::string Stringifyable<T>::get_string() const {
  return predefined_string_;
}

template<typename T>
void Stringifyable<T>::set_to_string(const std::string& input) {
  predefined_string_ = input;
}

template<typename T>
void Stringifyable<T>::ReportErrors(ValidationReport* report) const {
  if (is_string()) {
    // No error
  } else {
    T::ReportErrors(report);
  }
}

}  // namespace rpc



#endif /* VALIDATED_TYPES_INL_H_ */
