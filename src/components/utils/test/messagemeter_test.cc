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

#include <unistd.h>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "utils/macro.h"

#include "utils/messagemeter.h"
#include "utils/date_time.h"

namespace test  {
namespace components  {
namespace utils  {

// Pair of values <second, msecond>
typedef std::pair<int, int> TimePair;
const TimePair testing_time_pairs[] = { TimePair(0,  50),
                                        TimePair(0, 100),
                                        TimePair(0, 200),
                                        TimePair(0, 500),
                                        TimePair(0, 900),
                                        TimePair(1,   0),
                                        TimePair(1, 500) };

class MessageMeterTest: public ::testing::TestWithParam<TimePair> {
 protected:
  void SetUp() OVERRIDE {
    usecs = date_time::DateTime::MICROSECONDS_IN_MILLISECOND;
    id1 = 0x0;
    id2 = 0xABCDEF;
    id3 = 0xFEBCDA;

    const TimePair time_pair = GetParam();
    EXPECT_GT(usecs,
              time_pair.second) << "Wrong time (msecs) value";

    time_range.tv_sec = time_pair.first;
    time_range.tv_usec = time_pair.second * usecs;
    EXPECT_LT(0,
              date_time::DateTime::getuSecs(time_range))
        << "Wrong test case with null range value";

    meter.set_time_range(time_range);
    time_range_msecs = date_time::DateTime::getmSecs(time_range);
  }
  void TearDown() OVERRIDE {
  }
  ::utils::MessageMeter<int> meter;
  TimevalStruct time_range = {0, 0};
  int64_t time_range_msecs;
  int usecs;
  int id1, id2, id3;
};

TEST(MessageMeterTest, DefaultTimeRange) {
  const ::utils::MessageMeter<int> default_meter;
  const TimevalStruct time_second {1, 0};
  EXPECT_EQ(time_second, default_meter.time_range());
}

TEST(MessageMeterTest, TimeRangeSetter) {
  ::utils::MessageMeter<int> meter;
  TimevalStruct time_range {0, 0};
  const int test_count_secs = 1000;
  // Skip 1000th msec value as wrong for TimevalStruct
  const int test_count_msecs = 999;
  for (int sec = test_count_secs; sec >= 0; --sec) {
    for (int msec = test_count_msecs; msec >= 0; --msec) {
      time_range.tv_sec = sec;
      time_range.tv_usec = msec * date_time::DateTime::MICROSECONDS_IN_MILLISECOND;
      // Setter TimevalStruct
      meter.set_time_range(time_range);
      EXPECT_EQ(time_range,
                meter.time_range()) << sec << "." << msec << " sec";
      // Setter mSecs
      meter.set_time_range(sec * date_time::DateTime::MILLISECONDS_IN_SECOND +
                           msec);
      EXPECT_EQ(time_range,
                meter.time_range()) << sec << "." << msec << " sec";
    }
  }
}

TEST(MessageMeterTest, AddingWithNullTimeRange) {
  ::utils::MessageMeter<int> meter;
  const int id1 = 1;
  const int id2 = 2;
  const TimevalStruct null_time_range {0, 0};
  meter.set_time_range(null_time_range);
  for (int i = 0; i < 10000; ++i) {
    // 1st Connection
    EXPECT_EQ(0u,
              meter.TrackMessage(id1));
    EXPECT_EQ(0u,
              meter.Frequency(id1));

    // 2d Connection
    EXPECT_EQ(0u,
              meter.TrackMessage(id2));
    EXPECT_EQ(0u,
              meter.Frequency(id2));
  }
}

TEST_P(MessageMeterTest, TrackMessage_AddingOverPeriod_CorrectCountOfMessages) {
  size_t messages = 0;
  const TimevalStruct start_time = date_time::DateTime::getCurrentTime();
  // Add messages for less range period
  int64_t time_span;
  while ((time_span = date_time::DateTime::calculateTimeSpan(start_time))
         < time_range_msecs) {
    ++messages;

    size_t tracked_frequency = meter.TrackMessage(id1);
    size_t frequency = meter.Frequency(id1);

    EXPECT_GE(messages, tracked_frequency)
        << "Tracked messages can`t be over cycles.";

    if (messages > frequency) {
      EXPECT_GE(time_range_msecs, time_span);
      break;
    }
    EXPECT_EQ(tracked_frequency, frequency);
  }
}

TEST_P(MessageMeterTest,
    TrackMessage_AddingOverPeriodMultiIds_CorrectCountOfMessages) {
  size_t messages = 0;
  const TimevalStruct start_time = date_time::DateTime::getCurrentTime();
  // Add messages for less range period
  int64_t time_span;
  while ((time_span = date_time::DateTime::calculateTimeSpan(start_time))
         < time_range_msecs) {
    ++messages;

    size_t tracked_frequency_id1 = meter.TrackMessage(id1);
    size_t tracked_frequency_id2 = meter.TrackMessage(id2);
    size_t tracked_frequency_id3 = meter.TrackMessage(id3);
    size_t frequency_id1 = meter.Frequency(id1);
    size_t frequency_id2 = meter.Frequency(id2);
    size_t frequency_id3 = meter.Frequency(id3);

    EXPECT_GE(messages, tracked_frequency_id1)
        << "Tracked messages can`t be over cycles.";
    EXPECT_GE(messages, tracked_frequency_id2)
        << "Tracked messages can`t be over cycles.";
    EXPECT_GE(messages, tracked_frequency_id3)
        << "Tracked messages can`t be over cycles.";

    if (messages > frequency_id1 ||
       messages > frequency_id2 ||
       messages > frequency_id3) {
      EXPECT_GE(time_range_msecs, time_span);
      break;
    }
    EXPECT_EQ(tracked_frequency_id1, frequency_id1);
    EXPECT_EQ(tracked_frequency_id2, frequency_id2);
    EXPECT_EQ(tracked_frequency_id3, frequency_id3);
  }
}

TEST_P(MessageMeterTest, Frequency_CountingOverPeriod_CorrectCountOfMessages) {
  const size_t one_message = 1;
  const TimevalStruct start_time = date_time::DateTime::getCurrentTime();
  EXPECT_EQ(one_message,
            meter.TrackMessage(id1));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id2));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id3));

  // Check messages count over period
  int64_t time_span;
  while ((time_span = date_time::DateTime::calculateTimeSpan(start_time))
         < time_range_msecs) {
    usleep(time_range_msecs);

    if (one_message != meter.Frequency(id1) ||
       one_message != meter.Frequency(id2) ||
       one_message != meter.Frequency(id3)) {
      EXPECT_GE(time_range_msecs, time_span);
      break;
    }
  }
}

TEST_P(MessageMeterTest, CountingOutOfPeriod) {
  const size_t one_message = 1;
  EXPECT_EQ(one_message,
            meter.TrackMessage(id1));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id2));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id3));

  // sleep more than time range
  usleep(time_range_msecs * usecs * 1.1);
  EXPECT_EQ(0u,
            meter.Frequency(id1));
  EXPECT_EQ(0u,
            meter.Frequency(id2));
  EXPECT_EQ(0u,
            meter.Frequency(id3));
}

TEST_P(MessageMeterTest, ClearId) {
  const size_t one_message = 1;
  EXPECT_EQ(one_message,
            meter.TrackMessage(id1));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id2));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id3));

  meter.RemoveIdentifier(id1);

  EXPECT_EQ(0u,
            meter.Frequency(id1));
  EXPECT_EQ(one_message,
            meter.Frequency(id2));
  EXPECT_EQ(one_message,
            meter.Frequency(id3));

  meter.RemoveIdentifier(id2);

  EXPECT_EQ(0u,
            meter.Frequency(id1));
  EXPECT_EQ(0u,
            meter.Frequency(id2));
  EXPECT_EQ(one_message,
            meter.Frequency(id3));

  meter.RemoveIdentifier(id3);

  EXPECT_EQ(0u,
            meter.Frequency(id1));
  EXPECT_EQ(0u,
            meter.Frequency(id2));
  EXPECT_EQ(0u,
            meter.Frequency(id3));
}

TEST_P(MessageMeterTest, ClearIds) {
  const size_t one_message = 1;
  EXPECT_EQ(one_message,
            meter.TrackMessage(id1));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id2));
  EXPECT_EQ(one_message,
            meter.TrackMessage(id3));

  meter.ClearIdentifiers();

  EXPECT_EQ(0u,
            meter.Frequency(id2));
  EXPECT_EQ(0u,
            meter.Frequency(id2));
  EXPECT_EQ(0u,
            meter.Frequency(id3));
}

INSTANTIATE_TEST_CASE_P(MessageMeterTestCase,
                        MessageMeterTest,
                        ::testing::ValuesIn(testing_time_pairs));

}  // namespace utils
}  // namespace components
}  // namespace test
