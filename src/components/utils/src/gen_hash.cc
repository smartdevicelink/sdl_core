/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "utils/gen_hash.h"
#include <cstdlib>
#include <string>
#include <locale>
#include "utils/custom_string.h"

namespace utils {

const std::string gen_hash(size_t size) {
  static const char symbols[] =
      "0123456789"
      "abcdefghijklmnopqrstuvwxyz"
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  static const size_t capacity = sizeof(symbols) - 1;

  std::string hash(size, '\0');
  for (std::string::iterator i = hash.begin(); i != hash.end(); ++i) {
    int index = std::rand() % capacity;
    *i = symbols[index];
  }
  return hash;
}

int32_t Djb2HashFromString(const std::string& str_to_hash) {
  uint32_t hash = 5381U;
  std::string::const_iterator it = str_to_hash.begin();
  std::string::const_iterator it_end = str_to_hash.end();

  for (; it != it_end; ++it) {
    hash = ((hash << 5) + hash) + (*it);
  }

  // Reset sign bit in case it has been set.
  // This is needed to avoid overflow for signed int.
  const int32_t result = hash & 0x7FFFFFFF;
  return result;
}

uint32_t CaseInsensitiveFaq6HashFromString(const char* cstr) {
  uint32_t hash = 0;
  std::locale loc;
  for (; *cstr; ++cstr) {
    char lower_char = std::tolower(*cstr, loc);
    hash += static_cast<uint32_t>(lower_char);
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

uint32_t CaseInsensitiveFaq6HashFromString(
    const custom_string::CustomString& str_to_hash) {
  uint32_t hash = 0;
  if (str_to_hash.is_ascii_string()) {
    hash = CaseInsensitiveFaq6HashFromString(str_to_hash.c_str());
  } else {
    const std::wstring& wstr = str_to_hash.ToWStringLowerCase();
    size_t size = wstr.size();
    for (size_t i = 0; i < size; ++i) {
      hash += static_cast<uint32_t>(wstr[i]);
      hash += (hash << 10);
      hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
  }
  return hash;
}

}  // namespace utils
