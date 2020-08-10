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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_MESSAGEMETER_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_MESSAGEMETER_H_

#include <cstddef>
#include <map>
#include <set>
#include "utils/date_time.h"
#include "utils/lock.h"

namespace utils {

CREATE_LOGGERPTR_GLOBAL(logger_, "MessageMeter")
/**
    @brief The MessageMeter class need to count message frequency
    Default time range value is 1 second
    IncomingDataHandler methods are reentrant and not thread-safe
    @tparam Id could be used for handling messages by session,
    connection or other identifier
 */
template <class Id>
class MessageMeter {
 public:
  MessageMeter();
  /**
     @brief Update frequency value for selected identifier
     @param Id - unique identifier
     @return frequency
   */
  size_t TrackMessage(const Id& id);
  /**
     @brief Update frequency value for selected identifier
     @param Id - unique identifier
     @param count - count of received messages
     @return frequency
   */
  size_t TrackMessages(const Id& id, const size_t count);
  /**
     @brief Frequency of messages for selected identifier
     @param Id - unique identifier
     @return frequency
   */
  size_t Frequency(const Id& id);

  /**
     @brief Remove all data refer to selected identifier
     @param Id - unique identifier
   */
  void RemoveIdentifier(const Id& id);

  /**
     @brief Remove all frequency data
   */
  void ClearIdentifiers();

  void set_time_range(const size_t time_range_msecs);
  void set_time_range(const date_time::TimeDuration& time_range);
  date_time::TimeDuration time_range() const;

 private:
  size_t FrequencyImpl(const Id& id);

  date_time::TimeDuration time_range_;
  typedef std::multiset<date_time::TimeDuration> Timings;
  typedef std::map<Id, Timings> TimingMap;
  TimingMap timing_map_;
  sync_primitives::Lock timing_map_lock_;
};

template <class Id>
MessageMeter<Id>::MessageMeter() {
  time_range_ = date_time::seconds(1);
}

template <class Id>
size_t MessageMeter<Id>::TrackMessage(const Id& id) {
  LOG4CXX_AUTO_TRACE(logger_);
  return TrackMessages(id, 1);
}

template <class Id>
size_t MessageMeter<Id>::TrackMessages(const Id& id, const size_t count) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(timing_map_lock_);
  Timings& timings = timing_map_[id];
  const date_time::TimeDuration current_time = date_time::getCurrentTime();
  for (size_t i = 0; i < count; ++i) {
    // Adding to the end is amortized constant
    timings.insert(timings.end(), current_time);
  }
  return FrequencyImpl(id);
}

template <class Id>
size_t MessageMeter<Id>::Frequency(const Id& id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(timing_map_lock_);
  return FrequencyImpl(id);
}

template <class Id>
size_t MessageMeter<Id>::FrequencyImpl(const Id& id) {
  LOG4CXX_AUTO_TRACE(logger_);
  typename TimingMap::iterator it = timing_map_.find(id);
  if (it == timing_map_.end()) {
    return 0u;
  }
  Timings& timings = it->second;
  if (timings.empty()) {
    return 0u;
  }
  const date_time::TimeDuration actual_begin_time =
      (date_time::getCurrentTime() - time_range_);
  timings.erase(timings.begin(), timings.upper_bound(actual_begin_time));
  return timings.size();
}

template <class Id>
void MessageMeter<Id>::RemoveIdentifier(const Id& id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(timing_map_lock_);
  timing_map_.erase(id);
}

template <class Id>
void MessageMeter<Id>::ClearIdentifiers() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(timing_map_lock_);
  timing_map_.clear();
}

template <class Id>
void MessageMeter<Id>::set_time_range(const size_t time_range_msecs) {
  LOG4CXX_AUTO_TRACE(logger_);
  time_range_ = date_time::milliseconds(time_range_msecs);
}
template <class Id>
void MessageMeter<Id>::set_time_range(
    const date_time::TimeDuration& time_range) {
  LOG4CXX_AUTO_TRACE(logger_);
  time_range_ = time_range;
}
template <class Id>
date_time::TimeDuration MessageMeter<Id>::time_range() const {
  return time_range_;
}
}  // namespace utils
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_MESSAGEMETER_H_
