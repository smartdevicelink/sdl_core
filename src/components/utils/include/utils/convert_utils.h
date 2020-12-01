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
#include <string>

namespace utils {

/**
 * @brief Convert int64 value to long long int value
 * Using when int64 value should be assign to JSON value
 * @param value to be converted
 * @return conversion result
 */
long long int ConvertInt64ToLongLongInt(const int64_t value);

/**
 * @brief Convert long long int value to int64 value
 * @param value to be converted
 * @return conversion result
 */
int64_t ConvertLongLongIntToInt64(const long long int value);

/**
 * @brief Convert uint64 value to unsigned long long int value
 * Using when uint64 value should be assign to JSON value
 * @param value to be converted
 * @return conversion result
 */
unsigned long long int ConvertUInt64ToLongLongUInt(const uint64_t value);

/**
 * @brief Convert unsigned long long int value to uint64 value
 * @param value to be converted
 * @return conversion result
 */
uint64_t ConvertLongLongUIntToUInt64(const unsigned long long int value);

/**
 * @brief Convert one number value to another type value
 * @param value to be converted
 * @return conversion result
 */
template <typename InputType, typename OutputType>
OutputType SafeStaticCast(const InputType value) {
  DCHECK_OR_RETURN(value >= std::numeric_limits<OutputType>::min(),
                   std::numeric_limits<OutputType>::min());
  DCHECK_OR_RETURN(value <= std::numeric_limits<OutputType>::max(),
                   std::numeric_limits<OutputType>::max());
  return static_cast<OutputType>(value);
}

/**
 * @brief Convert binary data to a string value
 * @param data raw binary data
 * @param data_size string length. Required to check whether the data is a
 * printable string.
 * @return conversion result
 */
std::string ConvertBinaryDataToString(const uint8_t* data,
                                      const size_t data_size);

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_CONVERT_UTILS_H_
