/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SYSTEM_TIME_HANDLER_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SYSTEM_TIME_HANDLER_H_

#include <time.h>

namespace utils {

/**
 * @brief The SystemTimeListener interface.
 * This interface allows to get notifications whenever
 * system time appears or fails to appear.
 */
class SystemTimeListener {
 public:
  /**
   * @brief OnSystemTimeArrived Notify about system time
   * in utc format
   * @param utc_time current system time.
   */
  virtual void OnSystemTimeArrived(const time_t utc_time) = 0;

  /**
   * @brief OnSystemTimeFails notifies when system
   * fails to respond with proper time.
   */
  virtual void OnSystemTimeFails() = 0;
};

/**
 * @brief SystemTimeHandler the interface which provides the necessary
 * public API to work with system time. The class does not implement
 * any logic it's public api forwards call to the private virtual
 * methods which are suppose to be defined within specific implementation.
 */
class SystemTimeHandler {
 public:
  /**
   * @brief SystemTimeHandler creates an instance
   * for this class.
   */
  SystemTimeHandler();

  /**
   * @brief QuerySystemTime provides the public interface
   * to retrieve the system time. Interface uses private implementation
   * hence the logic will be defined within descendant class.
   */
  void QuerySystemTime();

  /**
   * @brief SubscribeOnSystemTime allows to subscribe listener
   * to the certain event. This class does not provide such storage.
   * It rather uses private pure virtual function. So the final behaviour
   * should be defined within the descendant class
   */
  void SubscribeOnSystemTime(SystemTimeListener* listener);

  /**
   * @brief UnSubscribeFromSystemTime allows to unsubscribe listener
   * from the certain event. This class does not manipulate with storage.
   * It rather uses private pure virtual function. So the final behaviour
   * should be defined within the descendant class
   */
  void UnSubscribeFromSystemTime(SystemTimeListener* listener);

  /**
   * @brief GetUTCTime allows to obtain cached result for the
   * GetSystemTime request
   * @return utc time.
   */
  time_t GetUTCTime();

  /**
   * @brief ~SystemTimeHandler destroys the object
   */
  virtual ~SystemTimeHandler();

 private:
  /**
   * @brief DoSystemTimeQuery responsible for the system time querying.
   * It is up to implementator how exactly system is going to receive this time.
   */
  virtual void DoSystemTimeQuery() = 0;

  /**
   * @brief DoSubscribe implements the logic which allows to handle
   * subscription process. The handling logic should be defined within
   * descendant class.
   */
  virtual void DoSubscribe(SystemTimeListener* listener) = 0;

  /**
   * @brief DoUnsubscribe implements the logic which allows to handle
   * unsubscription process. The handling logic should be defined within
   * descendant class.
   */
  virtual void DoUnsubscribe(SystemTimeListener* listener) = 0;

  /**
   * @brief FetchSystemTime allows to obtain the cached result
   * for the GetSystemTime request.
   * @return utc time.
   */
  virtual time_t FetchSystemTime() = 0;
};

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SYSTEM_TIME_HANDLER_H_
