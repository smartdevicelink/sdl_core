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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_CONVERT_UTILS_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_CONVERT_UTILS_H_

#include <stdint.h>
#include <limits>

namespace utils {

/**
 * Convert int64 value to long long int value
 * Using when int64 value should be assign to JSON value
 */
long long int ConvertInt64ToLongLongInt(const int64_t value);

/**
 * Convert long long int value to int64 value
 */
int64_t ConvertLongLongIntToInt64(const long long int value);

/**
 * Convert uint64 value to unsigned long long int value
 * Using when uint64 value should be assign to JSON value
 */
unsigned long long int ConvertUInt64ToLongLongUInt(const uint64_t value);

/**
 * Convert unsigned long long int value to uint64 value
 */
uint64_t ConvertLongLongUIntToUInt64(const unsigned long long int value);


/**
 * Convert one number value to another type value
 */
template <typename InputType, typename OutputType>
OutputType SafeStaticCast(const InputType value) {
  DCHECK_OR_RETURN(value >= std::numeric_limits<OutputType>::min(),
                   std::numeric_limits<OutputType>::min());
  DCHECK_OR_RETURN(value <= std::numeric_limits<OutputType>::max(),
                   std::numeric_limits<OutputType>::max());
  return static_cast<OutputType>(value);
}

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_CONVERT_UTILS_H_
