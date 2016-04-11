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

#include "gtest/gtest.h"
#include "utils/date_time.h"

namespace test {
namespace components {
namespace utils {

using namespace date_time;
TEST(DateTimeTest, GetCurrentTime) {

  //arrange
  const TimevalStruct time1 = date_time::DateTime::getCurrentTime();

  //assert
  ASSERT_NE(0, time1.tv_sec);
  ASSERT_GE(time1.tv_usec, 0);

  //act
  const TimevalStruct time2 = date_time::DateTime::getCurrentTime();

  //assert
  ASSERT_NE(0, time2.tv_sec);
  ASSERT_GE(time2.tv_usec, 0);
  ASSERT_GE(time2.tv_sec, time1.tv_sec);
}

TEST(DateTimeTest, GetSecs) {
  //arrange
  TimevalStruct time;
  time.tv_sec = 1;
  time.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  //assert
  ASSERT_EQ(1, date_time::DateTime::getSecs(time));
}

TEST(DateTimeTest, GetmSecs) {
  //arrange
  TimevalStruct time;
  time.tv_sec = 1;
  time.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  int64_t expect_value = time.tv_sec
      * date_time::DateTime::MILLISECONDS_IN_SECOND
      + time.tv_usec / date_time::DateTime::MICROSECONDS_IN_MILLISECOND;
  //assert
  ASSERT_EQ(expect_value, date_time::DateTime::getmSecs(time));
}

TEST(DateTimeTest, GetuSecs) {
  //arrange
  TimevalStruct time;
  time.tv_sec = 3;
  time.tv_usec = 4;

  int64_t expect_value = time.tv_sec
      * date_time::DateTime::MILLISECONDS_IN_SECOND
      * date_time::DateTime::MICROSECONDS_IN_MILLISECOND + time.tv_usec;
  //assert
  ASSERT_EQ(expect_value, date_time::DateTime::getuSecs(time));
}

TEST(DateTimeTest, GetuSecsmSecs) {
  //arrange
  TimevalStruct time;
  time.tv_sec = 5;
  time.tv_usec = 6;

  int64_t expect_value = date_time::DateTime::getuSecs(time)
      / date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  //assert
  ASSERT_EQ(expect_value, date_time::DateTime::getmSecs(time));
}

TEST(DateTimeTest, CalculateTimeSpan) {
  //arrange
  const TimevalStruct time = date_time::DateTime::getCurrentTime();

  const uint32_t sleep_time_mSec = 10;
  usleep(sleep_time_mSec * date_time::DateTime::MICROSECONDS_IN_MILLISECOND);

  //assert
  ASSERT_GE(date_time::DateTime::calculateTimeSpan(time), sleep_time_mSec);
}

TEST(DateTimeTest, CalculateTimeDiff) {

  //arrange
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  TimevalStruct time2;
  time2.tv_sec = 3;
  time2.tv_usec = 4 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  //time2 to time1
  TimevalStruct diff1;
  diff1.tv_sec = time2.tv_sec - time1.tv_sec;
  diff1.tv_usec = time2.tv_usec - time1.tv_usec;

  const int64_t mSecDiff = static_cast<int64_t>(diff1.tv_sec) * 1000
      + diff1.tv_usec / 1000;

  //assert
  ASSERT_EQ(mSecDiff, date_time::DateTime::calculateTimeDiff(time2, time1));

  //time1 to time2
  TimevalStruct diff2;
  diff2.tv_sec = time1.tv_sec - time2.tv_sec;
  diff2.tv_usec = time1.tv_usec - time2.tv_usec;

  const int64_t mSecDiff2 = -(static_cast<int64_t>(diff2.tv_sec) * 1000
      + diff2.tv_usec / 1000);

  //assert
  ASSERT_EQ(mSecDiff2, date_time::DateTime::calculateTimeDiff(time1, time2));
}

TEST(DateTimeTest, CalculateEqualTimeDiff) {
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  TimevalStruct time2;
  time2.tv_sec = 1;
  time2.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  ASSERT_EQ(0, date_time::DateTime::calculateTimeDiff(time2, time1));
  ASSERT_EQ(0, date_time::DateTime::calculateTimeDiff(time1, time2));
}

TEST(DateTimeTest, compareTime) {

  //arrange
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  TimevalStruct time2;
  time2.tv_sec = 2;
  time2.tv_usec = 4 * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;

  //assert
  ASSERT_EQ(LESS, date_time::DateTime::compareTime(time1, time2));
  ASSERT_EQ(GREATER, date_time::DateTime::compareTime(time2, time1));
  ASSERT_NE(EQUAL, date_time::DateTime::compareTime(time2, time1));

  //act
  TimevalStruct time3 = date_time::DateTime::Sub(time2, time1);

  //assert
  ASSERT_EQ(EQUAL, date_time::DateTime::compareTime(time1, time3));
}

TEST(DateTimeTest, GetSecs_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 0;
  time1.tv_usec = date_time::DateTime::MICROSECONDS_IN_SECOND;

  //assert
  ASSERT_EQ(1, date_time::DateTime::getSecs(time1));
}

TEST(DateTimeTest, compareTime_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 0;

  TimevalStruct time2;
  time2.tv_sec = 0;
  time2.tv_usec = date_time::DateTime::MICROSECONDS_IN_SECOND;

  //assert
  ASSERT_EQ(1, date_time::DateTime::getSecs(time1));
  ASSERT_EQ(1, date_time::DateTime::getSecs(time2));
  ASSERT_EQ(EQUAL, date_time::DateTime::compareTime(time1, time2));
}

TEST(DateTimeTest, compareEqualTime_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 0;

  TimevalStruct time2;
  time2.tv_sec = 0;
  time2.tv_usec = date_time::DateTime::MICROSECONDS_IN_SECOND;

  //assert
  ASSERT_TRUE(date_time::DateTime::Equal(time1, time2));
}

TEST(DateTimeTest, compareLessTime_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 0;

  TimevalStruct time2;
  time2.tv_sec = 0;
  time2.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  //assert
  ASSERT_TRUE(date_time::DateTime::Less(time1, time2));
}

TEST(DateTimeTest, compareGreaterTime_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 0;

  TimevalStruct time2;
  time2.tv_sec = 0;
  time2.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  //assert
  ASSERT_TRUE(date_time::DateTime::Greater(time2, time1));
}

TEST(DateTimeTest, CalculateTimeSub_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 1;
  time1.tv_usec = 0;

  TimevalStruct time2;
  time2.tv_sec = 0;
  time2.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  TimevalStruct time3 = date_time::DateTime::Sub(time2, time1);

  //assert
  ASSERT_EQ(EQUAL, date_time::DateTime::compareTime(time1, time3));
}

TEST(DateTimeTest, CalculateTimeDiff_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 2;
  time1.tv_usec = 5 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  TimevalStruct time2;
  time2.tv_sec = 3;
  time2.tv_usec = 1 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  //assert
  ASSERT_EQ(3000, date_time::DateTime::calculateTimeDiff(time2, time1));
  ASSERT_EQ(3000, date_time::DateTime::calculateTimeDiff(time1, time2));
}

TEST(DateTimeTest, CalculateEqualTimeDiff_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 2;
  time1.tv_usec = 2 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  TimevalStruct time2;
  time2.tv_sec = 3;
  time2.tv_usec = 1 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  //assert
  ASSERT_EQ(0, date_time::DateTime::calculateTimeDiff(time2, time1));
  ASSERT_EQ(0, date_time::DateTime::calculateTimeDiff(time1, time2));
}

TEST(DateTimeTest, CalculateEqualTimeSub_UsecConvertedInSec) {
  //arrange
  TimevalStruct time1;
  time1.tv_sec = 3;
  time1.tv_usec = 0;

  TimevalStruct time2;
  time2.tv_sec = 2;
  time2.tv_usec = 1 * date_time::DateTime::MICROSECONDS_IN_SECOND;

  TimevalStruct time3 = date_time::DateTime::Sub(time2, time1);
  TimevalStruct time4 = date_time::DateTime::Sub(time1, time2);

  TimevalStruct time_expected;
  time_expected.tv_sec = 0;
  time_expected.tv_usec = 0;
  //assert
  ASSERT_EQ(EQUAL, date_time::DateTime::compareTime(time_expected, time3));
  ASSERT_EQ(EQUAL, date_time::DateTime::compareTime(time_expected, time4));
}

TEST(DateTimeTest, AddMilliseconds_SetMillisecondMultiplesSecond_ExpectChangeTime) {
  TimevalStruct time1;
  time1.tv_sec = 3;
  time1.tv_usec = 0;
  uint32_t milliseconds = 7000;
  date_time::DateTime::AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(10, time1.tv_sec);
  ASSERT_EQ(0, time1.tv_usec);
}

TEST(DateTimeTest, AddMilliseconds_SetMillisecondNotMultiplesSecond_ExpectChangeTime) {
  TimevalStruct time1;
  uint32_t milliseconds = 7500;
  time1.tv_sec = 3;
  time1.tv_usec = 0;
  date_time::DateTime::AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(10, time1.tv_sec);
  ASSERT_EQ(500000, time1.tv_usec);
}

TEST(DateTimeTest, AddMilliseconds_SetMilliSecondLessThenSeconds_ExpectChangeTime) {
  TimevalStruct time1;
  uint32_t milliseconds = 500;
  time1.tv_sec = 3;
  time1.tv_usec = 0;
  date_time::DateTime::AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(3, time1.tv_sec);
  ASSERT_EQ(500000, time1.tv_usec);
}

TEST(DateTimeTest, AddMilliseconds_SetMillisecondEqualNull_ExpectNotChangeTime) {
  TimevalStruct time1;
  uint32_t milliseconds = 0;
  time1.tv_sec = 3;
  time1.tv_usec = 0;
  date_time::DateTime::AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(3, time1.tv_sec);
  ASSERT_EQ(0, time1.tv_usec);
}

TEST(DateTimeTest, AddMilliseconds_SetOverlowMicrosecond_ExpectChangeTime) {
  TimevalStruct time1;
  uint32_t milliseconds = 7600;
  time1.tv_sec = 3;
  time1.tv_usec = 500000;
  date_time::DateTime::AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(11, time1.tv_sec);
  ASSERT_EQ(100000, time1.tv_usec);
}

TEST(DateTimeTest, Operator_minus_TimevalStruct_positive){
    TimevalStruct time1;
    TimevalStruct time2;
    TimevalStruct time3;
    time1.tv_sec = 3;
    time1.tv_usec = 0;
    time2.tv_sec = 3;
    time2.tv_usec = 0;
    time3.tv_sec = 2;
    time3.tv_usec = 9000000;
    ASSERT_EQ(0, date_time::DateTime::getSecs(time1 - time2));
    ASSERT_EQ(8000000, date_time::DateTime::getuSecs(time3 - time1));
}

TEST(DateTimeTest, Operator_minus_TimevalStruct_negative){
    TimevalStruct time1;
    TimevalStruct time2;
    time1.tv_sec = 3;
    time1.tv_usec = 0;
    time2.tv_sec = 2;
    time2.tv_usec = 9000000;
    ASSERT_NE(1, date_time::DateTime::getSecs(time1 - time2));
    ASSERT_NE(-8000000, date_time::DateTime::getSecs(time2 - time1));
}


}  // namespace utils
}  // namespace components
}  // namespace test
