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

#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#ifdef OS_WIN32
#include <time.h>
#include <assert.h>
#elif defined(OS_MAC)
#else
#include <sys/time.h>
#endif
#include <stdint.h>
#include "utils/date_time.h"
#ifdef OS_WIN32
#include "os/poll_windows.h"
#endif
#ifdef OS_WINCE
#include "utils/global.h"
#endif
#ifdef OS_WIN32
void clock_gettime(int i, timespec * tm)
{
	if (i == CLOCK_MONOTONIC)
	{
		unsigned _int64 cur = GetTickCount();
		tm->tv_sec = cur / 1000;
		tm->tv_nsec = (cur % 1000) * 1000;
	}
	else if (i == CLOCK_REALTIME)
	{
		time_t t;
		::time(&t);
		tm->tv_sec = t;
		tm->tv_nsec = 0;
	}
	else
		assert(false);
}
#elif defined(OS_MAC)
void clock_gettime(int i, timespec * tm)
{
	clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), i, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    tm->tv_sec = mts.tv_sec;
    tm->tv_nsec = mts.tv_nsec;
}
#endif

namespace date_time {

#ifdef OS_WIN32
#else
int32_t const DateTime::MILLISECONDS_IN_SECOND;
int32_t const DateTime::MICROSECONDS_IN_MILLISECONDS;
#endif

TimevalStruct DateTime::getCurrentTime() {
  TimevalStruct currentTime;
#ifdef OS_WIN32
  timespec tm;
  clock_gettime(CLOCK_REALTIME, &tm);
  currentTime.tv_sec = (long)tm.tv_sec;
  currentTime.tv_usec = tm.tv_nsec;
#else
#ifdef OS_MAC
    struct timezone timeZone;
#else
  timezone timeZone;
#endif
  gettimeofday(&currentTime, &timeZone);
#endif

    return currentTime;
  }

int64_t date_time::DateTime::getSecs(const TimevalStruct &time) {
   const TimevalStruct times = ConvertionUsecs(time);
   return static_cast<int64_t>(times.tv_sec);
}

int64_t DateTime::getmSecs(const TimevalStruct &time) {
  const TimevalStruct times = ConvertionUsecs(time);
  return static_cast<int64_t>(times.tv_sec) * MILLISECONDS_IN_SECOND
      + times.tv_usec / MICROSECONDS_IN_MILLISECONDS;
}

int64_t DateTime::getuSecs(const TimevalStruct &time) {
  const TimevalStruct times = ConvertionUsecs(time);
  return static_cast<int64_t>(times.tv_sec) * MILLISECONDS_IN_SECOND
      * MICROSECONDS_IN_MILLISECONDS + times.tv_usec;
}

int64_t DateTime::calculateTimeSpan(const TimevalStruct& sinceTime) {
  return calculateTimeDiff(getCurrentTime(), sinceTime);
}

int64_t DateTime::calculateTimeDiff(const TimevalStruct &time1,
                                    const TimevalStruct &time2){
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

TimevalStruct DateTime::Sub(const TimevalStruct& time1,
                            const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  TimevalStruct ret;
  timersub(&times1, &times2, &ret);
  return ret;
}

bool DateTime::Greater(const TimevalStruct& time1, const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  return timercmp(&times1, &times2, >);
}

bool DateTime::Less(const TimevalStruct& time1, const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  return timercmp(&times1, &times2, <);
}

bool DateTime::Equal(const TimevalStruct& time1, const TimevalStruct& time2) {
  const TimevalStruct times1 = ConvertionUsecs(time1);
  const TimevalStruct times2 = ConvertionUsecs(time2);
  return !timercmp(&times1, &times2, !=);
}

TimeCompare date_time::DateTime::compareTime(const TimevalStruct &time1, const TimevalStruct &time2) {
  if (Greater(time1, time2)) return GREATER;
  if (Less(time1, time2)) return LESS;
  return EQUAL;
}

TimevalStruct date_time::DateTime::ConvertionUsecs(const TimevalStruct &time){
  if (time.tv_usec >= MICROSECONDS_IN_SECOND) {
    TimevalStruct time1;
    time1.tv_sec = static_cast<int64_t>(time.tv_sec) + (time.tv_usec/MICROSECONDS_IN_SECOND);
    time1.tv_usec = static_cast<int64_t>(time.tv_usec) % MICROSECONDS_IN_SECOND;
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
