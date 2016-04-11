/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "utils/convert_utils.h"
#include <stdint.h>
#include <limits>
#include <algorithm>
#include "utils/macro.h"

long long int utils::ConvertInt64ToLongLongInt(const int64_t value) {
  DCHECK(value >= std::numeric_limits<long long int>::min());
  DCHECK(value <= std::numeric_limits<long long int>::max());
  return static_cast<long long int>(value);
}

int64_t utils::ConvertLongLongIntToInt64(const long long int value) {
  DCHECK_OR_RETURN(
      value >= std::numeric_limits<int64_t>::min(),
      std::min<int64_t>(value, std::numeric_limits<int64_t>::min()));
  DCHECK_OR_RETURN(
      value <= std::numeric_limits<int64_t>::max(),
      std::max<int64_t>(value, std::numeric_limits<int64_t>::max()));
  return static_cast<int64_t>(value);
}

unsigned long long int utils::ConvertUInt64ToLongLongUInt(const uint64_t value) {
  DCHECK(value >= std::numeric_limits<unsigned long long int>::min());
  DCHECK(value <= std::numeric_limits<unsigned long long int>::max());
  return static_cast<unsigned long long int>(value);
}

uint64_t utils::ConvertLongLongUIntToUInt64(const unsigned long long int value) {
  DCHECK_OR_RETURN(
      value >= std::numeric_limits<uint64_t>::min(),
      std::min<uint64_t>(value, std::numeric_limits<uint64_t>::min()));
  DCHECK_OR_RETURN(
      value <= std::numeric_limits<uint64_t>::max(),
      std::max<uint64_t>(value, std::numeric_limits<uint64_t>::max()));

  return static_cast<uint64_t>(value);
}
