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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_GEN_HASH_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_GEN_HASH_H_

#include <string>

namespace utils {

namespace custom_string {
class CustomString;
}

/**
 * @brief generate random alphanumeric string of specified length
 * @param size length of random string
 * @return random string
 */

const std::string gen_hash(size_t size);

/**
 * @brief Allows to generate hash from the specified string.
 * The djb2 algorithm uses for hash generation.
 * @param str_to_hash - the string from which hash should be generated.
 * @return integer hash for the specified string.
 */
int32_t Djb2HashFromString(const std::string& str_to_hash);

/**
 * @brief Generates hash.
 * The faq6 algorithm uses for hash generation.
 * @param str_to_hash - the CustomSting contains ASCII or UTF8 string from which
 * hash should be generated.
 * @return uint32_t hash for the specified string.
 */
uint32_t CaseInsensitiveFaq6HashFromString(
    const custom_string::CustomString& str_to_hash);

/**
 * @brief Transforms input string to lower case and then generates hash.
 * The faq6 algorithm uses for hash generation.
 * @param str_to_hash - the string from which hash should be generated.
 * @return uint32_t hash for the specified string.
 */
uint32_t CaseInsensitiveFaq6HashFromString(const char* cstr);

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_GEN_HASH_H_
