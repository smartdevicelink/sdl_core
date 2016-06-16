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
#if defined(OS_WINDOWS)

#include <time.h>
#include <stdint.h>

#include "utils/winhdr.h"
#include "utils/date_time.h"

namespace date_time {

TimevalStruct DateTime::getCurrentTime() {
  FILETIME ft;
  uint64_t tmpres = 0;
  TimevalStruct tv;

  GetSystemTimeAsFileTime(&ft);

  // The GetSystemTimeAsFileTime returns the number of 100 nanosecond
  // intervals since Jan 1, 1601 in a structure. Copy the high bits to
  // the 64 bit tmpres, shift it left by 32 then or in the low 32 bits.
  tmpres |= ft.dwHighDateTime;
  tmpres <<= 32;
  tmpres |= ft.dwLowDateTime;

  // Convert to microseconds by dividing by 10
  tmpres /= 10;

  // The Unix epoch starts on Jan 1 1970.  Need to subtract the difference
  // in seconds from Jan 1 1601.
  tmpres -= kDeltaEpochInMicrosecs;

  // Finally change microseconds to seconds and place in the seconds value.
  // The modulus picks up the microseconds.
  tv.tv_sec = static_cast<long>(tmpres / kMicrosecondsInSecond);
  tv.tv_usec = static_cast<long>(tmpres % kMicrosecondsInSecond);

  return tv;
}

int64_t date_time::DateTime::getSecs(const TimevalStruct& time) {
  const TimevalStruct times = ConvertionUsecs(time);
  return static_cast<int64_t>(times.tv_sec);
}

int64_t DateTime::getmSecs(const TimevalStruct& time) {
  const TimevalStruct times = ConvertionUsecs(time);
  return static_cast<int64_t>(times.tv_sec) * kMillisecondsInSecond +
         times.tv_usec / kMicrosecondsInMillisecond;
}

int64_t DateTime::getuSecs(const TimevalStruct& time) {
  const TimevalStruct times = ConvertionUsecs(time);
  return static_cast<int64_t>(times.tv_sec) * kMillisecondsInSecond *
             kMicrosecondsInMillisecond +
         times.tv_usec;
}

int64_t DateTime::calculateTimeSpan(const TimevalStruct& sinceTime) {
  return calculateTimeDiff(getCurrentTime(), sinceTime);
}

int64_t DateTime::calculateTimeDiff(const TimevalStruct& time1,
                                    const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  TimevalStruct ret;
  if (Greater(times1, times2)) {
    ret = Sub(times1, times2);
  } else {
    ret = Sub(times2, times1);
  }
  return getmSecs(ret);
}

void DateTime::AddMilliseconds(TimevalStruct& time, uint32_t milliseconds) {
  const uint32_t sec = milliseconds / kMillisecondsInSecond;
  const uint32_t usec =
      (milliseconds % kMillisecondsInSecond) * kMicrosecondsInMillisecond;
  time.tv_sec += sec;
  time.tv_usec += usec;
  time = ConvertionUsecs(time);
}

TimevalStruct DateTime::Sub(const TimevalStruct& time1,
                            const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  TimevalStruct ret;

  ret.tv_sec = times1.tv_sec - times2.tv_sec;
  ret.tv_usec = times1.tv_usec - times2.tv_usec;
  if (ret.tv_usec < 0) {
    --ret.tv_sec;
    ret.tv_usec += 1000000;
  }
  return ret;
}

bool DateTime::Greater(const TimevalStruct& time1, const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  return timercmp(&times1, &times2, > );
}

bool DateTime::Less(const TimevalStruct& time1, const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  return timercmp(&times1, &times2, < );
}

bool DateTime::Equal(const TimevalStruct& time1, const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  return !timercmp(&times1, &times2, != );
}

TimeCompare date_time::DateTime::compareTime(const TimevalStruct& time1,
                                             const TimevalStruct& time2) {
  if (Greater(time1, time2))
    return GREATER;
  if (Less(time1, time2))
    return LESS;
  return EQUAL;
}

TimevalStruct date_time::DateTime::ConvertionUsecs(const TimevalStruct& time) {
  if (time.tv_usec >= kMicrosecondsInSecond) {
    TimevalStruct time1;
    time1.tv_sec = static_cast<int64_t>(time.tv_sec) +
                   (time.tv_usec / kMicrosecondsInSecond);
    time1.tv_usec = static_cast<int64_t>(time.tv_usec) % kMicrosecondsInSecond;
    return time1;
  }
  return time;
}

}  // namespace date_time

bool operator<(const TimevalStruct& time1, const TimevalStruct& time2) {
  return date_time::DateTime::Less(time1, time2);
}

bool operator==(const TimevalStruct& time1, const TimevalStruct& time2) {
  return date_time::DateTime::Equal(time1, time2);
}

const TimevalStruct operator-(const TimevalStruct& time1,
                              const TimevalStruct& time2) {
  return date_time::DateTime::Sub(time1, time2);
}

#endif  // OS_WINDOWS
