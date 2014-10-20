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
#ifndef SRC_COMPONENTS_INCLUDE_UTILS_DATE_TIME_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_DATE_TIME_H_

#if defined(OS_POSIX)
#include <sys/time.h>
typedef struct timeval TimevalStruct;
#endif
#include <stdint.h>

namespace date_time {

enum TimeCompare {
  LESS,
  EQUAL,
  GREATER
};

class DateTime {
 public:
  static const int32_t MILLISECONDS_IN_SECOND = 1000;
  static const int32_t MICROSECONDS_IN_MILLISECONDS = 1000;

  static TimevalStruct getCurrentTime();

  // return SECONDS count
  static int64_t getSecs(const TimevalStruct& time);

  // return MILLISECONDS count
  static int64_t getmSecs(const TimevalStruct& time);
  // return MICROSECONDS count
  static int64_t getuSecs(const TimevalStruct& time);

  // return MILLISECONDS count between sinceTime value and current time
  static int64_t calculateTimeSpan(const TimevalStruct& sinceTime);

  // return MILLISECONDS count between time1 and time2
  static int64_t calculateTimeDiff(const TimevalStruct& time1,
                                   const TimevalStruct& time2);

  static TimevalStruct Sub(const TimevalStruct& time1,
                           const TimevalStruct& time2);

  static TimeCompare compareTime(const TimevalStruct& time1,
                                 const TimevalStruct& time2);

  static bool Greater(const TimevalStruct& time1, const TimevalStruct& time2);
  static bool Less(const TimevalStruct& time1, const TimevalStruct& time2);
  static bool Equal(const TimevalStruct& time1, const TimevalStruct& time2);
};

}  // namespace date_time

#endif  // SRC_COMPONENTS_INCLUDE_UTILS_DATE_TIME_H_
