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

#ifndef RPC_BASE_DBUS_INL_H_
#define RPC_BASE_DBUS_INL_H_

#include "dbus/dbus_message.h"
#include "json/value.h"
#include "rpc_base/rpc_base.h"

namespace rpc {

// static
inline PrimitiveType::ValueState PrimitiveType::InitHelper(bool is_next) {
  return is_next ? kValid : kInvalid;
}

// static
inline CompositeType::InitializationState CompositeType::InitHelper(bool is_next) {
  return is_next ? kInitialized : kUninitialized;
}

namespace impl {


// Helper functions to statically map int types to appropriate
// MessageReader functions

// Non-specialized versions doesn't have implementation

// Tells if next element of reader is of type T
template<typename T> inline bool NextIs(const dbus::MessageReader& reader);
// Takes next element of type T from reader
template<typename T> inline T Take(dbus::MessageReader* reader);

template<> inline bool NextIs<int8_t>(const dbus::MessageReader& reader) {
  return reader.NextIsByte();
}

template<> inline int8_t Take<int8_t>(dbus::MessageReader* reader) {
  return static_cast<int8_t>(reader->TakeByte());
}

inline void Put(int8_t val, dbus::MessageWriter* writer) {
  writer->PutByte(static_cast<uint8_t>(val));
}

template<> inline bool NextIs<uint8_t>(const dbus::MessageReader& reader) {
  return reader.NextIsByte();
}

template<> inline uint8_t Take<uint8_t>(dbus::MessageReader* reader) {
  return reader->TakeByte();
}

inline void Put(uint8_t val, dbus::MessageWriter* writer) {
  writer->PutByte(val);
}

template<> inline bool NextIs<int16_t>(const dbus::MessageReader& reader) {
  return reader.NextIsInt16();
}

template<> inline int16_t Take<int16_t>(dbus::MessageReader* reader) {
  return reader->TakeInt16();
}

inline void Put(int16_t val, dbus::MessageWriter* writer) {
  writer->PutInt16(val);
}

template<> inline bool NextIs<uint16_t>(const dbus::MessageReader& reader) {
  return reader.NextIsUint16();
}

template<> inline uint16_t Take<uint16_t>(dbus::MessageReader* reader) {
  return reader->TakeUint16();
}

inline void Put(uint16_t val, dbus::MessageWriter* writer) {
  writer->PutUint16(val);
}

template<> inline bool NextIs<int32_t>(const dbus::MessageReader& reader) {
  return reader.NextIsInt32();
}

template<> inline int32_t Take<int32_t>(dbus::MessageReader* reader) {
  return reader->TakeInt32();
}

inline void Put(int32_t val, dbus::MessageWriter* writer) {
  writer->PutInt32(val);
}

template<> inline bool NextIs<uint32_t>(const dbus::MessageReader& reader) {
  return reader.NextIsUint32();
}

template<> inline uint32_t Take<uint32_t>(dbus::MessageReader* reader) {
  return reader->TakeUint32();
}

inline void Put(uint32_t val, dbus::MessageWriter* writer) {
  writer->PutUint32(val);
}

template<> inline bool NextIs<int64_t>(const dbus::MessageReader& reader) {
  return reader.NextIsInt64();
}

template<> inline int64_t Take<int64_t>(dbus::MessageReader* reader) {
  return reader->TakeInt64();
}

inline void Put(int64_t val, dbus::MessageWriter* writer) {
  writer->PutInt64(val);
}

template<> inline bool NextIs<uint64_t>(const dbus::MessageReader& reader) {
  return reader.NextIsUint64();
}

template<> inline uint64_t Take<uint64_t>(dbus::MessageReader* reader) {
  return reader->TakeUint64();
}

inline void Put(uint64_t val, dbus::MessageWriter* writer) {
  writer->PutUint64(val);
}

template<> inline std::string Take<std::string>(dbus::MessageReader* reader) {
  return reader->TakeString();
}

inline void Put(const std::string val, dbus::MessageWriter* writer) {
  writer->PutString(val);
}

// Type to DBus type id mappers
template<typename T> char DbusTypeCode();
template<> inline char DbusTypeCode<bool>() { return DBUS_TYPE_BOOLEAN; }
// There is no dbus type for signed byte, map to unsigned
template<> inline char DbusTypeCode<int8_t>() { return DBUS_TYPE_BYTE; }
template<> inline char DbusTypeCode<uint8_t>() { return DBUS_TYPE_BYTE; }
template<> inline char DbusTypeCode<int16_t>() { return DBUS_TYPE_INT16; }
template<> inline char DbusTypeCode<uint16_t>() { return DBUS_TYPE_UINT16; }
template<> inline char DbusTypeCode<int32_t>() { return DBUS_TYPE_INT32; }
template<> inline char DbusTypeCode<uint32_t>() { return DBUS_TYPE_UINT32; }
template<> inline char DbusTypeCode<int64_t>() { return DBUS_TYPE_INT64; }
template<> inline char DbusTypeCode<uint64_t>() { return DBUS_TYPE_UINT64; }
template<> inline char DbusTypeCode<double>() { return DBUS_TYPE_DOUBLE; }
template<> inline char DbusTypeCode<std::string>() { return DBUS_TYPE_STRING; }
template<> inline char DbusTypeCode<const std::string>() { return DBUS_TYPE_STRING; }

// Non-specialized template supposes there is static
// void GetDbusSignature(std::string*) method in T
template<typename T>
struct DbusSignatureHelper {
  static void DbusSignature(std::string* signature) {
    T::GetDbusSignature(signature);
  }
};

template<>
struct DbusSignatureHelper<Boolean> {
  static void DbusSignature(std::string* signature) {
    (*signature) += rpc::impl::DbusTypeCode<bool>();
  }
};

template<typename T, T minval, T maxval>
struct DbusSignatureHelper<Integer<T, minval, maxval> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += rpc::impl::DbusTypeCode<T>();
  }
};

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
struct DbusSignatureHelper<Float<minnum, maxnum, minden, maxden> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += rpc::impl::DbusTypeCode<double>();
  }
};

template<typename T>
struct DbusSignatureHelper<Enum<T> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += rpc::impl::DbusTypeCode<int32_t>();
  }
};

template<size_t minlen, size_t maxlen>
struct DbusSignatureHelper<String<minlen, maxlen> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += rpc::impl::DbusTypeCode<std::string>();
  }
};

template<typename K, typename V>
struct DbusSignatureHelper<std::pair<K, V> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += DBUS_DICT_ENTRY_BEGIN_CHAR;
    (*signature) += rpc::impl::DbusTypeCode<K>();
    rpc::impl::DbusSignatureHelper<V>::DbusSignature(signature);
    (*signature) += DBUS_DICT_ENTRY_END_CHAR;
  }
};

template<typename T, size_t minsize, size_t maxsize>
struct DbusSignatureHelper<Array<T, minsize, maxsize> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += DBUS_TYPE_ARRAY;
    rpc::impl::DbusSignatureHelper<T>::DbusSignature(signature);
  }
};

template<typename T, size_t minsize, size_t maxsize>
struct DbusSignatureHelper<Map<T, minsize, maxsize> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += DBUS_TYPE_ARRAY;
    rpc::impl::DbusSignatureHelper<
        typename Map<T, minsize, maxsize>::value_type>::DbusSignature(signature);
  }
};

template<typename T>
struct DbusSignatureHelper<Optional<T> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += DBUS_STRUCT_BEGIN_CHAR;
    (*signature) += rpc::impl::DbusTypeCode<bool>();
    rpc::impl::DbusSignatureHelper<T>::DbusSignature(signature);
    (*signature) += DBUS_STRUCT_END_CHAR;
  }
};

template<typename T>
struct DbusSignatureHelper<Nullable<T> > {
  static void DbusSignature(std::string* signature) {
    (*signature) += DBUS_STRUCT_BEGIN_CHAR;
    (*signature) += rpc::impl::DbusTypeCode<bool>();
    rpc::impl::DbusSignatureHelper<T>::DbusSignature(signature);
    (*signature) += DBUS_STRUCT_END_CHAR;
  }
};

// Helper Optional type initialization functipon
template<typename T>
T TakeOptional(dbus::MessageReader* reader) {
  dbus::MessageReader struct_reader = reader->TakeStructReader();
  bool available = struct_reader.TakeBool();
  T value(&struct_reader);
  return available ? value : T();
}

// Helper Nullable type initialization functipon
template<typename T>
bool TakeNullable(dbus::MessageReader* reader) {
  dbus::MessageReader struct_reader = reader->TakeStructReader();
  bool is_null = struct_reader.TakeBool();
  return is_null;
}

}  // namespace impl


// Helper function that outputs dbus signature for type T
template<typename T>
void DbusSignature(std::string* signature) {
  impl::DbusSignatureHelper<T>::DbusSignature(signature);
}

// Type constructors
inline Boolean::Boolean(dbus::MessageReader* reader)
  : PrimitiveType(InitHelper(reader->NextIsBool())),
    value_(reader->TakeBool()) {
}

template<typename T, T minval, T maxval>
inline Integer<T, minval, maxval>::Integer(dbus::MessageReader* reader)
    : PrimitiveType(InitHelper(impl::NextIs<IntType>(*reader))),
      value_(impl::Take<IntType>(reader)) {
  if (is_valid()) {
    value_state_ = range_.Includes(value_) ? kValid : kInvalid;
  }
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
inline Float<minnum, maxnum, minden, maxden>::Float(dbus::MessageReader* reader)
    : PrimitiveType(InitHelper(reader->NextIsDouble())),
      value_(reader->TakeDouble()) {
  if (is_valid()) {
    value_state_ = range_.Includes(value_) ? kValid : kInvalid;
  }
}

template<size_t minlen, size_t maxlen>
inline String<minlen, maxlen>::String(dbus::MessageReader* reader)
    : PrimitiveType(InitHelper(reader->NextIsString())),
      value_(reader->TakeString()) {
  if (is_valid()) {
    value_state_ = length_range_.Includes(value_.length()) ? kValid : kInvalid;
  }
}

template<typename T>
inline Enum<T>::Enum(dbus::MessageReader* reader)
    : PrimitiveType(InitHelper(reader->NextIsInt32())),
      value_(EnumType(reader->TakeInt32())) {
  if (is_valid()) {
    value_state_ = IsValidEnum(value_) ? kValid : kInvalid;
  }
}

template<typename T, size_t minsize, size_t maxsize>
inline Array<T, minsize, maxsize>::Array(dbus::MessageReader* reader)
    : CompositeType(InitHelper(reader->NextIsArray())) {
  dbus::MessageReader array_reader = reader->TakeArrayReader();
  if (array_reader.has_failed()) {
    push_back(T());
  } else {
    while(array_reader.HasNext()) {
      push_back(T(&array_reader));
    }
  }
}

template<typename T, size_t minsize, size_t maxsize>
inline Map<T, minsize, maxsize>::Map(dbus::MessageReader* reader)
    : CompositeType(InitHelper(reader->NextIsStruct())) {
  // Map key-value pairs are serialized into array
  dbus::MessageReader array_reader = reader->TakeArrayReader();
  if (array_reader.has_failed()) {
    this->insert(typename MapType::value_type("", T()));
  } else {
    while(array_reader.HasNext()) {
      dbus::MessageReader dictvalue_reader = array_reader.TakeDictEntryReader();
      typename MapType::key_type key =
          impl::Take<typename MapType::key_type>(&dictvalue_reader);
      this->insert(typename MapType::value_type(key, T(&dictvalue_reader)));
    }
  }
}

template<typename T>
inline Nullable<T>::Nullable(dbus::MessageReader* reader)
  : T(reader),
    marked_null_(impl::TakeNullable<T>(reader)){
}

template<typename T>
inline Optional<T>::Optional(dbus::MessageReader* reader)
  : value_(impl::TakeOptional<T>(reader)){
}

// MessageWriter serializers
inline void Boolean::ToDbusWriter(dbus::MessageWriter* writer) const {
  writer->PutBool(value_);
}

template<typename T, T minval, T maxval>
inline void Integer<T, minval, maxval>::ToDbusWriter(
    dbus::MessageWriter* writer) const {
  impl::Put(value_, writer);
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
inline void Float<minnum, maxnum, minden, maxden>::ToDbusWriter(
    dbus::MessageWriter* writer) const {
  writer->PutDouble(value_);
}

template<size_t minlen, size_t maxlen>
inline void String<minlen, maxlen>::ToDbusWriter(
    dbus::MessageWriter* writer) const {
  writer->PutString(value_);
}

template<typename T>
inline void Enum<T>::ToDbusWriter(dbus::MessageWriter* writer) const {
  writer->PutInt32(static_cast<int32_t>(value_));
}

template<typename T, size_t minsize, size_t maxsize>
inline void Array<T, minsize, maxsize>::ToDbusWriter(
    dbus::MessageWriter* writer) const {
  std::string array_signature;
  DbusSignature<typename ArrayType::value_type>(&array_signature);
  dbus::MessageWriter array_writer(writer, dbus::kArray,
                                   array_signature.c_str());
  for (typename ArrayType::const_iterator i = this->begin(); i != this->end(); ++i) {
    i->ToDbusWriter(&array_writer);
  }
}

template<typename T, size_t minsize, size_t maxsize>
inline void Map<T, minsize, maxsize>::ToDbusWriter(
    dbus::MessageWriter* writer) const {
  std::string array_signature;
  DbusSignature<typename MapType::value_type>(&array_signature);
  dbus::MessageWriter array_writer(writer, dbus::kArray,
                                   array_signature.c_str());
  for (typename MapType::const_iterator i = this->begin(); i != this->end();
       ++i) {
    dbus::MessageWriter dictentry_writer(&array_writer, dbus::kDictEntry, NULL);
    impl::Put(i->first, &dictentry_writer);
    i->second.ToDbusWriter(&dictentry_writer);
  }
}

template<typename T>
inline void Optional<T>::ToDbusWriter(dbus::MessageWriter* writer) const {
  dbus::MessageWriter struct_writer(writer, dbus::kStruct, NULL);
  struct_writer.PutBool(value_.is_initialized());
  value_.ToDbusWriter(&struct_writer);
}

}  // namespace rpc

#endif // RPC_BASE_DBUS_INL_H_
