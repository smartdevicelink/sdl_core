/*
 * Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DISPLAY_CAPABILITIES_BUILDER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_DISPLAY_CAPABILITIES_BUILDER_H_
#include "application_manager/application.h"

namespace application_manager {
/**
 * @brief DisplayCapabilitiesBuilder utility class used for building cached
 * notification, which is sent to mobile on resumption finish
 */
class DisplayCapabilitiesBuilder {
  typedef std::function<void(Application&, const smart_objects::SmartObject)>
      ResumeCallback;

 public:
  /**
   * @brief DisplayCapabilitiesBuilder class constructor
   * @param Application reference to application owner
   */
  DisplayCapabilitiesBuilder(Application& application);

  /**
   * @brief InitBuilder initialize builder with callback and windows to be
   * restored
   * @param resume_callback callback to be called when notifications for all
   * windows have been received
   * @param windows_info SO containing data of windows to be restored
   */
  void InitBuilder(ResumeCallback resume_callback,
                   const smart_objects::SmartObject& windows_info);

  /**
   * @brief UpdateDisplayCapabilities update cached notification
   * @param display_capabilities to update cached notification with
   */
  void UpdateDisplayCapabilities(
      const smart_objects::SmartObject& display_capabilities);

  /**
   * @brief IsWaitingForWindowCapabilities checks wheter builder is waiting for
   * a capabilities notification for at least one window or not
   * @param incoming_display_capabilities capabilities to analyze
   * @return true if builder is waiting for capabilities for at least one
   * window, otherwise returns false
   */
  bool IsWaitingForWindowCapabilities(
      const smart_objects::SmartObject& incoming_display_capabilities);

  /**
   * @brief ResetDisplayCapabilities resets stored notification
   */
  void ResetDisplayCapabilities();

  /**
   * @brief StopWaitingForWindow stop waiting for window resumption by removing
   * window_id from windows pending resumption
   * @param window_id window id to be removed
   */
  void StopWaitingForWindow(const WindowID window_id);

  /**
   * @brief retreives stored notification data
   */
  const smart_objects::SmartObjectSPtr display_capabilities() const;

 private:
  /**
   * @brief InvokeCallbackFunction invokes callback function if all required
   * criterias met
   */
  void InvokeCallbackFunction();

  smart_objects::SmartObjectSPtr display_capabilities_;
  typedef std::set<WindowID> WindowIDsToResume;
  WindowIDsToResume window_ids_to_resume_;
  Application& owner_;
  ResumeCallback resume_callback_;
  sync_primitives::Lock display_capabilities_lock_;
};
}  // namespace application_manager
#endif
