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

#include <boost/thread/thread.hpp>
#include "gtest/gtest.h"
#include "utils/date_time.h"

namespace test {
namespace components {
namespace utils_test {

using namespace date_time;

namespace bpt = boost::posix_time;
TEST(DateTimeTest, GetCurrentTime) {
  // arrange
  const date_time::TimeDuration time1 = getCurrentTime();

  // assert
  ASSERT_NE(0, time1.total_seconds());
  ASSERT_GE(get_just_uSecs(time1), 0);

  // act
  const date_time::TimeDuration time2 = getCurrentTime();

  // assert
  ASSERT_NE(0, time2.total_seconds());
  ASSERT_GE(get_just_uSecs(time1), 0);
  ASSERT_GE(time2.total_seconds(), time1.total_seconds());
}

TEST(DateTimeTest, GetSecs) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(1) + bpt::milliseconds(2);

  // assert
  ASSERT_EQ(1, getSecs(time1));
}

TEST(DateTimeTest, GetmSecs) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(1) + bpt::milliseconds(2);
  int expected = 1 * MILLISECONDS_IN_SECOND + 2;

  // assert
  ASSERT_EQ(expected, getmSecs(time1));
}

TEST(DateTimeTest, GetuSecs) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(3) + bpt::microseconds(4);
  int expect_value =
      3 * MILLISECONDS_IN_SECOND * MICROSECONDS_IN_MILLISECOND + 4;

  // assert
  ASSERT_EQ(expect_value, getuSecs(time1));
}

TEST(DateTimeTest, GetuSecsmSecs) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(5) + bpt::microseconds(6);

  int64_t expect_value = getuSecs(time1) / MICROSECONDS_IN_MILLISECOND;

  // assert
  ASSERT_EQ(expect_value, getmSecs(time1));
}

TEST(DateTimeTest, CalculateTimeSpan) {
  // arrange
  const date_time::TimeDuration time1 = getCurrentTime();

  const uint32_t sleep_time_mSec = 10;
  // sleep current thread
  boost::this_thread::sleep(boost::posix_time::milliseconds(sleep_time_mSec));

  // assert
  ASSERT_GE(calculateTimeSpan(time1), sleep_time_mSec);
}

TEST(DateTimeTest, CalculateTimeDiff) {
  // arrange

  date_time::TimeDuration time1 = bpt::seconds(2) + bpt::milliseconds(2);
  date_time::TimeDuration time2 = bpt::seconds(3) + bpt::milliseconds(4);

  // time2 to time1
  date_time::TimeDuration diff1 = bpt::seconds(1) + bpt::milliseconds(2);

  // assert
  ASSERT_EQ(diff1.total_milliseconds(), calculateTimeDiff(time2, time1));

  // time1 to time2, but calculateTimeDiff returns an absolute value
  date_time::TimeDuration diff2 = -diff1.invert_sign();

  // assert
  ASSERT_EQ(diff2.total_milliseconds(), calculateTimeDiff(time1, time2));
}

TEST(DateTimeTest, CalculateEqualTimeDiff) {
  date_time::TimeDuration time1 = bpt::seconds(1) + bpt::milliseconds(2);

  date_time::TimeDuration time2 = bpt::seconds(1) + bpt::milliseconds(2);

  ASSERT_EQ(0, calculateTimeDiff(time2, time1));
  ASSERT_EQ(0, calculateTimeDiff(time1, time2));
}

TEST(DateTimeTest, compareTime) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(1) + bpt::milliseconds(2);

  date_time::TimeDuration time2 = bpt::seconds(2) + bpt::milliseconds(4);

  // assert
  ASSERT_EQ(LESS, compareTime(time1, time2));
  ASSERT_EQ(GREATER, compareTime(time2, time1));
  ASSERT_NE(EQUAL, compareTime(time2, time1));

  // act
  date_time::TimeDuration time3 = time2 - time1;

  // assert
  ASSERT_EQ(EQUAL, compareTime(time1, time3));
}

TEST(DateTimeTest, GetSecs_UsecConvertedInSec) {
  // arrange
  date_time::TimeDuration time1 = bpt::microseconds(MICROSECONDS_IN_SECOND);

  // assert
  ASSERT_EQ(1, getSecs(time1));
}

TEST(DateTimeTest, compareTime_UsecConvertedInSec) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(1);

  date_time::TimeDuration time2 = bpt::microseconds(MICROSECONDS_IN_SECOND);

  // assert
  ASSERT_EQ(1, getSecs(time1));
  ASSERT_EQ(1, getSecs(time2));
  ASSERT_EQ(EQUAL, compareTime(time1, time2));
  ASSERT_TRUE(Equal(time1, time2));
}

TEST(DateTimeTest, compareLessTime_UsecConvertedInSec) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(1);

  date_time::TimeDuration time2 = bpt::microseconds(2 * MICROSECONDS_IN_SECOND);

  // assert
  ASSERT_TRUE(Less(time1, time2));
}

TEST(DateTimeTest, compareGreaterTime_UsecConvertedInSec) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(1);

  date_time::TimeDuration time2 = bpt::milliseconds(2 * MICROSECONDS_IN_SECOND);

  // assert
  ASSERT_TRUE(Greater(time2, time1));
}

TEST(DateTimeTest, CalculateTimeSub_UsecConvertedInSec) {
  // arrange
  date_time::TimeDuration time1 = bpt::seconds(1);

  date_time::TimeDuration time2 = bpt::microseconds(2 * MICROSECONDS_IN_SECOND);

  date_time::TimeDuration time3 = time2 - time1;

  // assert
  ASSERT_EQ(EQUAL, compareTime(time1, time3));
}

TEST(DateTimeTest, CalculateTimeDiff_UsecConvertedInSec) {
  // arrange

  date_time::TimeDuration time1 =
      bpt::seconds(2) + bpt::microseconds(5 * MICROSECONDS_IN_SECOND);
  date_time::TimeDuration time2 =
      bpt::seconds(3) + bpt::microseconds(1 * MICROSECONDS_IN_SECOND);

  // assert
  ASSERT_EQ(3000, calculateTimeDiff(time2, time1));
  ASSERT_EQ(3000, calculateTimeDiff(time1, time2));
}

TEST(DateTimeTest, CalculateEqualTimeDiff_UsecConvertedInSec) {
  date_time::TimeDuration time1 =
      bpt::seconds(2) + bpt::microseconds(2 * MICROSECONDS_IN_SECOND);
  date_time::TimeDuration time2 =
      bpt::seconds(3) + bpt::microseconds(1 * MICROSECONDS_IN_SECOND);

  // assert
  ASSERT_EQ(0, calculateTimeDiff(time2, time1));
  ASSERT_EQ(0, calculateTimeDiff(time1, time2));

  date_time::TimeDuration time3 = time2 - time1;
  date_time::TimeDuration time4 = time1 - time2;

  date_time::TimeDuration time_expected(0, 0, 0, 0);

  ASSERT_EQ(EQUAL, compareTime(time_expected, time3));
  ASSERT_EQ(EQUAL, compareTime(time_expected, time4));
}

TEST(DateTimeTest,
     AddMilliseconds_SetMillisecondMultiplesSecond_ExpectChangeTime) {
  date_time::TimeDuration time1 = bpt::seconds(3);
  uint32_t milliseconds = 7000;
  AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(10, getSecs(time1));
  ASSERT_EQ(0, get_just_mSecs(time1));
}

TEST(DateTimeTest,
     AddMilliseconds_SetMillisecondNotMultiplesSecond_ExpectChangeTime) {
  date_time::TimeDuration time1 = bpt::seconds(3);
  uint32_t milliseconds = 7500;

  AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(10, time1.total_seconds());
  ASSERT_EQ(500000, get_just_uSecs(time1));
}

TEST(DateTimeTest,
     AddMilliseconds_SetMilliSecondLessThenSeconds_ExpectChangeTime) {
  date_time::TimeDuration time1 = bpt::seconds(3);
  uint32_t milliseconds = 500;

  AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(3, getSecs(time1));
  ASSERT_EQ(500000, get_just_uSecs(time1));
}

TEST(DateTimeTest,
     AddMilliseconds_SetMillisecondEqualNull_ExpectNotChangeTime) {
  date_time::TimeDuration time1 = bpt::seconds(3);
  uint32_t milliseconds = 0;

  AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(3, getSecs(time1));
  ASSERT_EQ(0, get_just_mSecs(time1));
}

TEST(DateTimeTest, AddMilliseconds_SetOverlowMicrosecond_ExpectChangeTime) {
  date_time::TimeDuration time1 = bpt::seconds(3);
  uint32_t milliseconds = 7600;

  AddMilliseconds(time1, milliseconds);
  ASSERT_EQ(10, getSecs(time1));
  ASSERT_EQ(600000, get_just_uSecs(time1));
}

TEST(DateTimeTest, Operator_minus_TimeDuration_positive) {
  date_time::TimeDuration time1 = bpt::seconds(3);
  date_time::TimeDuration time2 = bpt::seconds(3);
  date_time::TimeDuration time3 =
      bpt::seconds(2) + bpt::microseconds(9 * MICROSECONDS_IN_SECOND);
  date_time::TimeDuration diff1 = time1 - time2;
  ASSERT_EQ(0, getSecs(diff1));
  ASSERT_EQ(8000000, getuSecs(time3 - time1));
}

TEST(DateTimeTest, Operator_minus_TimeDuration_negative) {
  date_time::TimeDuration time1 = bpt::seconds(3);
  date_time::TimeDuration time2 =
      bpt::seconds(2) + bpt::microseconds(9 * MICROSECONDS_IN_SECOND);

  ASSERT_NE(1, getSecs(time1 - time2));
  ASSERT_NE(-8000000, getSecs(time2 - time1));
}

}  // namespace utils_test
}  // namespace components
}  // namespace test
