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

inline size_t Boolean::DbusSignature(char *buff, size_t buff_len) {
  if (buff_len != 0) {
    *buff = DBUS_TYPE_BOOLEAN;
    return 1;
  }
  return 0;
}

template<typename T, T minval, T maxval>
size_t Integer<T, minval, maxval>::DbusSignature(char *buff, size_t buff_len) {
  if (buff_len != 0) {
    *buff = DBUS_TYPE_INT32;
    return 1;
  }
  return 0;
}

template<int64_t minnum, int64_t maxnum, int64_t minden, int64_t maxden>
size_t Float<minnum, maxnum, minden, maxden>::DbusSignature(
    char *buff, size_t buff_len) {
  if (buff_len != 0) {
    *buff = DBUS_TYPE_DOUBLE;
    return 1;
  }
  return 0;
}

template<size_t minlen, size_t maxlen>
size_t String<minlen, maxlen>::DbusSignature(
    char *buff, size_t buff_len) {
  if (buff_len != 0) {
    *buff = DBUS_TYPE_STRING;
    return 1;
  }
  return 0;
}

template<typename T>
size_t Enum<T>::DbusSignature(
    char *buff, size_t buff_len) {
  if (buff_len != 0) {
    *buff = DBUS_TYPE_INT32;
    return 1;
  }
  return 0;
}

template<typename T, size_t minsize, size_t maxsize>
size_t Array<T, minsize, maxsize>::DbusSignature(
    char *buff, size_t buff_len) {
  if (buff_len != 0) {
    *buff = DBUS_TYPE_ARRAY;
    size_t array_sign_len = T::DbusSignature(buff + 1, buff_len - 1);
    if (array_sign_len != 0) {
      return 1 + array_sign_len;
    }
  }
  return 0;
}

template<typename T, size_t minsize, size_t maxsize>
size_t Map<T, minsize, maxsize>::DbusSignature(
    char *buff, size_t buff_len) {
  if (buff_len > 3) {
    buff[0] = DBUS_TYPE_ARRAY;
    buff[1] = DBUS_DICT_ENTRY_BEGIN_CHAR;
    buff[2] = DBUS_TYPE_STRING;
    size_t sign_len = 3;
    size_t value_sign_len =
        T::DbusSignature(buff + sign_len, buff_len - sign_len);
    if (value_sign_len != 0) {
      sign_len += value_sign_len;
      if (buff_len > sign_len) {
        buff[sign_len] = DBUS_DICT_ENTRY_END_CHAR;
        return sign_len + 1;
      }
    }
  }
  return 0;
}

template<typename T>
size_t Mandatory<T>::DbusSignature(char *buff, size_t buff_len) {
  return T::DbusSignature(buff, buff_len);
}

template<typename T>
size_t Optional<T>::DbusSignature(char *buff, size_t buff_len) {
  if (buff_len > 1) {
    buff[0] = DBUS_STRUCT_BEGIN_CHAR;
    buff[1] = DBUS_TYPE_BOOLEAN;
    size_t sign_len = 2;
    size_t value_sign_len = T::DbusSignature(buff + sign_len,
                                             buff_len - sign_len);
    if (value_sign_len != 0) {
      sign_len += value_sign_len;
      if (buff_len > sign_len) {
        buff[sign_len] = DBUS_STRUCT_END_CHAR;
        return sign_len + 1;
      }
    }
  }
  return 0;
}

}  // namespace rpc

#endif // RPC_BASE_DBUS_INL_H_
