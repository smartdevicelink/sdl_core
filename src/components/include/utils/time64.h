/*
 * Copyright (c) 2021, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_TIME64_H
#define SRC_COMPONENTS_INCLUDE_UTILS_TIME64_H

#include <chrono>

#if defined(__ANDROID__) && !defined(__LP64__)
#include <time64.h>
typedef time64_t TIME_TYPE;
#define GMTIME(time) gmtime64(time)
#define LOCALTIME(time) localtime64(time)
#define MKTIME(time) mktime64(time)
#else
#include <ctime>
typedef time_t TIME_TYPE;
#define GMTIME(time) gmtime(time)
#define LOCALTIME(time) localtime(time)
#define MKTIME(time) mktime(time)
#endif

template <typename DURATION_TYPE>
double get_duration_diff(TIME_TYPE from, TIME_TYPE to) {
    const DURATION_TYPE time_from(from);
    const DURATION_TYPE time_to(to);

    return std::chrono::duration_cast<DURATION_TYPE>(time_from - time_to).count();
}

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_TIME64_H
