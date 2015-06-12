/*
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

#include <sys/time.h>
#include <stdint.h>
#include "utils/date_time.h"


namespace date_time {

  TimevalStruct DateTime::getCurrentTime() {
    TimevalStruct currentTime;
    timezone timeZone;

    gettimeofday(&currentTime, &timeZone);

    return currentTime;
  }

int64_t date_time::DateTime::getSecs(const TimevalStruct &time) {
   return static_cast<int64_t>(time.tv_sec);
}

int64_t DateTime::getmSecs(const TimevalStruct &time) {
  return static_cast<int64_t>(time.tv_sec) * MILLISECONDS_IN_SECOND
      + time.tv_usec / MICROSECONDS_IN_MILLISECONDS;
}

int64_t DateTime::getuSecs(const TimevalStruct &time) {
  return static_cast<int64_t>(time.tv_sec) * MILLISECONDS_IN_SECOND
      * MICROSECONDS_IN_MILLISECONDS + time.tv_usec;
}

int64_t DateTime::calculateTimeSpan(const TimevalStruct& sinceTime) {
  return calculateTimeDiff(getCurrentTime(), sinceTime);
}

int64_t DateTime::calculateTimeDiff(const TimevalStruct &time1,
                                    const TimevalStruct &time2){
  TimevalStruct ret;
  if (Greater(time1, time2)) {
    ret = Sub(time1, time2);
  } else {
    ret = Sub(time2, time1);
  }
  return getmSecs(ret);
}

TimevalStruct DateTime::Sub(const TimevalStruct& time1,
                            const TimevalStruct& time2) {
  TimevalStruct ret;
  timersub(&time1, &time2, &ret);
  return ret;
}

bool DateTime::Greater(const TimevalStruct& time1, const TimevalStruct& time2) {
  return timercmp(&time1, &time2, >);
}

bool DateTime::Less(const TimevalStruct& time1, const TimevalStruct& time2) {
  return timercmp(&time1, &time2, <);
}

bool DateTime::Equal(const TimevalStruct& time1, const TimevalStruct& time2) {
  return !timercmp(&time1, &time2, !=);
}

TimeCompare date_time::DateTime::compareTime(const TimevalStruct &time1, const TimevalStruct &time2) {
  if (Greater(time1, time2)) return GREATER;
  if (Less(time1, time2)) return LESS;
  return EQUAL;
}

}  // namespace date_time

bool operator<(const TimevalStruct& time1, const TimevalStruct& time2) {
  return date_time::DateTime::Less(time1, time2);
}

bool operator==(const TimevalStruct& time1, const TimevalStruct& time2) {
  return date_time::DateTime::Equal(time1, time2);
}
