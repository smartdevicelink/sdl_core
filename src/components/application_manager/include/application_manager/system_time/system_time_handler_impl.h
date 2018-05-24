/*
 * Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SYSTEM_TIME_SYSTEM_TIME_HANDLER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SYSTEM_TIME_SYSTEM_TIME_HANDLER_IMPL_H_

#include <time.h>
#include <vector>

#include "utils/lock.h"
#include "utils/macro.h"
#include "utils/system_time_handler.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/event_engine/event_observer.h"

namespace application_manager {

/**
 * @brief The SystemTimeHandlerImpl class.
 * Responsible for the system time notification. It keeps the list
 * of system time listeners, sends appropriate request to the system
 * and notifies the listeners with new time right after response has appeared.
 */
class SystemTimeHandlerImpl : public utils::SystemTimeHandler,
                              public event_engine::EventObserver {
 public:
  /**
   * @brief SystemTimeHandlerImpl creates the instance of the class.
   */
  explicit SystemTimeHandlerImpl(ApplicationManager& application_manager);

  /**
   * @brief ~SystemTimeHandlerImpl
   */
  ~SystemTimeHandlerImpl();

 private:
  /**
   * @brief on_event allows to process certain events from the system.
   * Event which are handles within this methods  are: OnSystemTimeReady
   * in order to send system time query and GetSystemTimeResponse in order
   * to retrieve utc time and notify all the listeners with new time value.
   */
  void on_event(const application_manager::event_engine::Event& event) FINAL;

  /**
   * @brief DoSystemTimeQuery sends the appropriate request to the system
   * and subscribes the class to the response. This is asynchronous request
   * thus it won't block until the system respond and returns immediately.
   */
  void DoSystemTimeQuery() FINAL;

  /**
   * @brief DoSubscribe allows to subscribe certain listener for
   * SystemTime. This certain implementation does not maintain
   * listeners collection instead it save the pointer to listener.
   * @note the class is not reponsible for the pointer's lifecycle.
   * So consider to explicitly unsibsscribe from event when listener
   * is going to be destroyed.
   * @param listener the listener which will be notified
   * in case of SystemTime appeared.
   */
  void DoSubscribe(utils::SystemTimeListener* listener) FINAL;

  /**
   * @brief DoUnsubscribe assigns the pointer to listener to NULL.
   * This certain implementation ignores passed parameter.
   * @param listener pointer to the object which should be
   * unsubscribed from events.
   */
  void DoUnsubscribe(utils::SystemTimeListener* listener) FINAL;

  /**
   * @brief FetchSystemTime this method allows to obtain
   * recently received utc time.
   * @note it is up to user to check whether the returned
   * argument is valid
   * @return the time value returned by system.
   */
  time_t FetchSystemTime() FINAL;

  /**
   * @brief SendTimeRequest sends the request to the system
   * and subscribes for response.
   */
  void SendTimeRequest();

  /**
   * @brief ProcessSystemTimeResponse allows to process GetSystemTimeResponse
   * @param event contains response parameters aka "systemTime".
   */
  void ProcessSystemTimeResponse(
      const application_manager::event_engine::Event& event);

  /**
   * @brief ProcessSystemTimeReadyNotification allows to process
   * OnSystemTimeready notification
   * from HMI. It unsubscribes from the mentioned notification and sends
   * GetSystemTimeRequest to HMI in order to obtain system time
   */
  void ProcessSystemTimeReadyNotification();

  /**
   * @brief Checks if UTC time is ready to provided by HMI
   * and can be requested by GetSystemTime request
   * @return True if HMI is ready to provide UTC time
   * otherwise False
   */
  bool utc_time_can_be_received() const FINAL;

  mutable sync_primitives::Lock state_lock_;
  // Variable means HMI readiness to provide system time by request
  volatile bool utc_time_can_be_received_;
  // Varible used to schedule next GetSystemTime request
  // if at the moment of sending first GetSystemTime request
  // HMI is not ready to provide system time
  volatile bool get_system_time_pending_;
  // Varible used to store result for GetSystemTime request
  time_t last_time_;

  sync_primitives::Lock system_time_listener_lock_;
  utils::SystemTimeListener* system_time_listener_;
  ApplicationManager& app_manager_;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SYSTEM_TIME_SYSTEM_TIME_HANDLER_IMPL_H_
