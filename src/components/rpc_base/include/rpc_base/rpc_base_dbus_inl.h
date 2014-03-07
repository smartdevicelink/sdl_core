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

#include <dbus/dbus.h>
#include "rpc_base/rpc_base.h"
#include "json/value.h"

namespace rpc {
namespace impl {

// Type to DBus type id mappers
template<typename T> char DbusTypeCode();
template<> char DbusTypeCode<bool>() { return DBUS_TYPE_BOOLEAN; }
// There is no dbus type for signed byte, map to unsigned
template<> char DbusTypeCode<int8_t>() { return DBUS_TYPE_BYTE; }
template<> char DbusTypeCode<uint8_t>() { return DBUS_TYPE_BYTE; }
template<> char DbusTypeCode<int16_t>() { return DBUS_TYPE_INT16; }
template<> char DbusTypeCode<uint16_t>() { return DBUS_TYPE_UINT16; }
template<> char DbusTypeCode<int32_t>() { return DBUS_TYPE_INT32; }
template<> char DbusTypeCode<uint32_t>() { return DBUS_TYPE_UINT32; }
template<> char DbusTypeCode<int64_t>() { return DBUS_TYPE_INT64; }
template<> char DbusTypeCode<uint64_t>() { return DBUS_TYPE_UINT64; }
template<> char DbusTypeCode<double>() { return DBUS_TYPE_DOUBLE; }
template<> char DbusTypeCode<std::string>() { return DBUS_TYPE_STRING; }

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
    (*signature) += DBUS_DICT_ENTRY_BEGIN_CHAR;
    (*signature) +=
        rpc::impl::DbusTypeCode<typename Map<T, minsize, maxsize>::key_type>();
    rpc::impl::DbusSignatureHelper<T>::DbusSignature(signature);
    (*signature) += DBUS_DICT_ENTRY_END_CHAR;
  }
};

template<typename T>
struct DbusSignatureHelper<Mandatory<T> > {
  static void DbusSignature(std::string* signature) {
    rpc::impl::DbusSignatureHelper<T>::DbusSignature(signature);
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

}  // namespace impl


// Helper function that outputs dbus signature for type T
template<typename T>
void DbusSignature(std::string* signature) {
  impl::DbusSignatureHelper<T>::DbusSignature(signature);
}

template<typename T>
void DbusSignature(std::string* signature, const T& dummy) {
  impl::DbusSignatureHelper<T>::DbusSignature(signature);
}

}  // namespace rpc

#endif // RPC_BASE_DBUS_INL_H_
