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

#include <stdint.h>
#include <sys/time.h>
#include "utils/date_time.h"

namespace bpt = boost::posix_time;
using namespace boost::date_time;
namespace date_time {

/* Set of helper functions for the TimeDuration struct
 */

TimeDuration getCurrentTime() {
  return bpt::microsec_clock::local_time() - bpt::from_time_t(0);
}
TimeDuration TimeDurationZero() {
  return TimeDuration(0, 0, 0, 0);
}
int64_t getSecs(const TimeDuration& t) {
  return t.total_seconds();
}

int64_t getmSecs(const TimeDuration& t) {
  return t.total_milliseconds();
}

int64_t getuSecs(const TimeDuration& t) {
  return t.total_microseconds();
}

int64_t get_just_mSecs(const TimeDuration& t) {
  return t.total_milliseconds() % MILLISECONDS_IN_SECOND;
}

int64_t get_just_uSecs(const TimeDuration& t) {
  return t.total_microseconds() % MICROSECONDS_IN_SECOND;
}

int64_t calculateTimeSpan(const TimeDuration& sinceTime) {
  return calculateTimeDiff(getCurrentTime(), sinceTime);
}

int64_t calculateTimeDiff(const TimeDuration& time1,
                          const TimeDuration& time2) {
  return std::abs((time1 + -time2).total_milliseconds());
}

void AddMilliseconds(TimeDuration& t, uint32_t milliseconds) {
  t += bpt::milliseconds(milliseconds);
}

bool Greater(const TimeDuration& time1, const TimeDuration& time2) {
  return time1 > time2;
}

bool Less(const TimeDuration& time1, const TimeDuration& time2) {
  return time1 < time2;
}

bool Equal(const TimeDuration& time1, const TimeDuration& time2) {
  return time1 == time2;
}

TimeCompare compareTime(const TimeDuration& time1, const TimeDuration& time2) {
  if (Greater(time1, time2))
    return GREATER;
  if (Less(time1, time2))
    return LESS;
  return EQUAL;
}

}  // namespace date_time
