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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_DATE_TIME_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_DATE_TIME_H_

#include <stdint.h>
#include "boost/date_time/posix_time/posix_time_duration.hpp"

namespace date_time {

// skip boost namespacing for all this
// NOTE that it's called posix_time for its functionality, not for any
// platform-dependence
typedef boost::posix_time::time_duration TimeDuration;
// Capture from boost's namespaces
using boost::posix_time::microseconds;
using boost::posix_time::milliseconds;
using boost::posix_time::seconds;

enum TimeCompare { LESS, EQUAL, GREATER };

// public defines for external usage
const int32_t MILLISECONDS_IN_SECOND = 1000;
const int32_t MICROSECONDS_IN_MILLISECOND = 1000;
const int32_t NANOSECONDS_IN_MICROSECOND = 1000;
const int32_t SECONDS_IN_HOUR = 3600;
const int32_t MICROSECONDS_IN_SECOND =
    MILLISECONDS_IN_SECOND * MICROSECONDS_IN_MILLISECOND;
const int32_t NANOSECONDS_IN_MILLISECOND =
    MICROSECONDS_IN_MILLISECOND * NANOSECONDS_IN_MICROSECOND;

TimeDuration getCurrentTime();

// empty duration
TimeDuration TimeDurationZero();

// return SECONDS count
int64_t getSecs(const TimeDuration& t);

// return MILLISECONDS count
int64_t getmSecs(const TimeDuration& t);
// return MICROSECONDS count
int64_t getuSecs(const TimeDuration& t);

// get just the MILLISECONDS count (< 1000)
int64_t get_just_mSecs(const TimeDuration& t);

// get just the MICROSECONDS count (< 1000)
int64_t get_just_uSecs(const TimeDuration& t);

// return MILLISECONDS count between sinceTime value and current time
int64_t calculateTimeSpan(const TimeDuration& sinceTime);

// return MILLISECONDS count between time1 and time2
int64_t calculateTimeDiff(const TimeDuration& time1, const TimeDuration& time2);

/**
 * @brief Adds milliseconds to time struct
 * @param time contains time struct
 * @param milliseconds contains value which need to
 * add to time struct
 **/
void AddMilliseconds(TimeDuration& time, uint32_t milliseconds);

TimeCompare compareTime(const TimeDuration& time1, const TimeDuration& time2);

bool Greater(const TimeDuration& time1, const TimeDuration& time2);
bool Less(const TimeDuration& time1, const TimeDuration& time2);
bool Equal(const TimeDuration& time1, const TimeDuration& time2);

}  // namespace date_time
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_DATE_TIME_H_
