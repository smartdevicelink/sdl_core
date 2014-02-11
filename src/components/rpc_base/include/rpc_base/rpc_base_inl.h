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

inline PrimitiveType::PrimitiveType(bool initialized, bool valid)
    : initialized_(initialized), valid_(valid) {
}

inline bool PrimitiveType::is_initialized() const {
  return initialized_;
}

inline bool PrimitiveType::is_valid() const {
  return valid_;
}

/*
 * Boolean class
 */
inline Boolean::Boolean()
    : PrimitiveType(false, false), value_(false) {
}

inline Boolean::Boolean(bool value)
    : PrimitiveType(true, true), value_(value) {
}

inline Boolean& Boolean::operator=(bool new_val) {
  value_ = new_val;
  initialized_ = true;
  valid_ = true;
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
    : PrimitiveType(false, false),
      value_(range_.min()) {
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>::Integer(IntType value)
    : PrimitiveType(true, range_.Includes(value)),
      value_(value) {
}

template<typename T, T minval, T maxval>
Integer<T, minval, maxval>& Integer<T, minval, maxval>::operator=(IntType new_val) {
  value_ = new_val;
  initialized_ = true;
  valid_ = range_.Includes(value_);
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
    : PrimitiveType(false, false),
      value_(range_.min()) {
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>::Float(double value)
    : PrimitiveType(true, range_.Includes(value)),
      value_(value) {
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
Float<minnum, maxnum, minden, maxden>&
Float<minnum, maxnum, minden, maxden>::operator=(double new_val) {
  value_ = new_val;
  initialized_ = true;
  valid_ = range_.Includes(new_val);
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
    : PrimitiveType(false, false) {
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>::String(const std::string& value)
    : PrimitiveType(true, length_range_.Includes(value.length())),
      value_(value) {
}

template<size_t minlen, size_t maxlen>
String<minlen, maxlen>& String<minlen, maxlen>::operator=(const std::string& new_val) {
  value_ = new_val;
  initialized_ = true;
  valid_ = length_range_.Includes(new_val.length());
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
    : PrimitiveType(false, false),
      value_(EnumType()) {
}

template<typename T>
Enum<T>::Enum(EnumType value)
    : PrimitiveType(true, IsValidEnum(value_)),
      value_(value) {
}

template<typename T>
Enum<T>& Enum<T>::operator=(EnumType new_val) {
  value_ = new_val;
  initialized_ = true;
  valid_ = IsValidEnum(value_);
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
Array<T, minsize, maxsize>::Array() {
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
Array<T, minsize, maxsize>::Array(const U& value)
    : ArrayType(value.begin(), value.end() ){
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
  if (!Range<size_t>(minsize, maxsize).Includes(this->size()))
    return false;
  for (typename ArrayType::const_iterator i = this->begin();
      i != this->end(); ++i) {
    if (!i->is_valid())
      return false;
  }
  return true;
}

template<typename T, size_t minsize, size_t maxsize>
bool Array<T, minsize, maxsize>::is_initialized() const {
  return !this->empty();
}

/*
 * Map class
 */
template<typename T, size_t minsize, size_t maxsize>
Map<T, minsize, maxsize>::Map() {
}

template<typename T, size_t minsize, size_t maxsize>
template<typename U>
Map<T, minsize, maxsize>::Map(const U& value) {
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
  if (!Range<size_t>(minsize, maxsize).Includes(this->size()))
    return false;
  for (typename Map::const_iterator i = this->begin();
      i != this->end(); ++i) {
    if (!i->second.is_valid())
      return false;
  }
  return true;
}

template<typename T, size_t minsize, size_t maxsize>
bool Map<T, minsize, maxsize>::is_initialized() const {
  return !this->empty();
}

/*
 * Mandatory class
 */
template<typename T>
Mandatory<T>::Mandatory() {
}

template<typename T>
template<typename U>
Mandatory<T>::Mandatory(const U& value)
    : T(value) {
}

template<typename T>
template<typename U>
Mandatory<T>& Mandatory<T>::operator=(const U& new_val) {
  this->T::operator=(new_val);
  return *this;
}

template<typename T>
bool Mandatory<T>::is_valid() const {
  return T::is_initialized() && T::is_valid();
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

}  // namespace rpc



#endif /* VALIDATED_TYPES_INL_H_ */
