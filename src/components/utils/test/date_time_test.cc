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

#include <unistd.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "utils/date_time.h"

namespace test  {
namespace components  {
namespace utils  {

TEST(DateTimeTest, GetCurrentTime) {
  const TimevalStruct time1 = date_time::DateTime::getCurrentTime();
  ASSERT_NE(0, time1.tv_sec);
  ASSERT_GE(time1.tv_usec, 0);

  const TimevalStruct time2 = date_time::DateTime::getCurrentTime();
  ASSERT_NE(0, time2.tv_sec);
  ASSERT_GE(time2.tv_usec, 0);
  ASSERT_GE(time2.tv_sec,  time1.tv_sec);
}

TEST(DateTimeTest, GetmSecs) {
  TimevalStruct time;
  time.tv_sec  = 1;
  time.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECONDS;

  ASSERT_EQ(time.tv_sec  * date_time::DateTime::MILLISECONDS_IN_SECOND +
            time.tv_usec / date_time::DateTime::MICROSECONDS_IN_MILLISECONDS,
            date_time::DateTime::getmSecs(time));
}

TEST(DateTimeTest, GetuSecs) {
  TimevalStruct time;
  time.tv_sec  = 3;
  time.tv_usec = 4;

  ASSERT_EQ(time.tv_sec  * date_time::DateTime::MILLISECONDS_IN_SECOND *
            date_time::DateTime::MICROSECONDS_IN_MILLISECONDS + time.tv_usec,
            date_time::DateTime::getuSecs(time));
}

TEST(DateTimeTest, GetuSecsmSecs) {
  TimevalStruct time;
  time.tv_sec  = 5;
  time.tv_usec = 6;

  ASSERT_EQ( date_time::DateTime::getmSecs(time),
             date_time::DateTime::getuSecs(time) / date_time::DateTime::MICROSECONDS_IN_MILLISECONDS);
}

TEST(DateTimeTest, CalculateTimeSpan) {
  const TimevalStruct time = date_time::DateTime::getCurrentTime();

  const uint32_t sleep_time_mSec = 10;
  usleep(sleep_time_mSec * date_time::DateTime::MICROSECONDS_IN_MILLISECONDS);

  ASSERT_GE(date_time::DateTime::calculateTimeSpan(time),
            sleep_time_mSec);
}

TEST(DateTimeTest, CalculateTimeDiff) {
  TimevalStruct time1;
  time1.tv_sec  = 1;
  time1.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECONDS;

  TimevalStruct time2;
  time2.tv_sec  = 3;
  time2.tv_usec = 4 * date_time::DateTime::MICROSECONDS_IN_MILLISECONDS;

  //time2 to time1
  TimevalStruct diff1;
  diff1.tv_sec  = time2.tv_sec  - time1.tv_sec;
  diff1.tv_usec = time2.tv_usec - time1.tv_usec;

  const int64_t mSecDiff = static_cast<int64_t>(diff1.tv_sec) * 1000
      + diff1.tv_usec / 1000;

  ASSERT_EQ(mSecDiff,
            date_time::DateTime::calculateTimeDiff(time2, time1));

  //time1 to time2
  TimevalStruct diff2;
  diff2.tv_sec  = time1.tv_sec  - time2.tv_sec;
  diff2.tv_usec = time1.tv_usec - time2.tv_usec;

  const int64_t mSecDiff2 = -(static_cast<int64_t>(diff2.tv_sec) * 1000
      + diff2.tv_usec / 1000);

  ASSERT_EQ(mSecDiff2,
            date_time::DateTime::calculateTimeDiff(time1, time2));
}

}  // namespace utils
}  // namespace components
}  // namespace test
