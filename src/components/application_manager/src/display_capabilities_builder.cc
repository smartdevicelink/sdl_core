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
#include "application_manager/display_capabilities_builder.h"

#include <algorithm>

#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
namespace application_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "DisplayCapabilitiesBuilder")

const WindowID kDefaultWindowID = 0;

DisplayCapabilitiesBuilder::DisplayCapabilitiesBuilder(Application& application)
    : owner_(application) {
  LOG4CXX_AUTO_TRACE(logger_);
}

void DisplayCapabilitiesBuilder::InitBuilder(
    DisplayCapabilitiesBuilder::ResumeCallback resume_callback,
    const smart_objects::SmartObject& windows_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(display_capabilities_lock_);
  resume_callback_ = resume_callback;
  window_ids_to_resume_.insert(kDefaultWindowID);
  for (size_t i = 0; i < windows_info.length(); ++i) {
    auto window_id = windows_info[i][strings::window_id].asInt();
    LOG4CXX_DEBUG(logger_,
                  "Inserting " << window_id << " to waiting container");
    window_ids_to_resume_.insert(window_id);
  }
}

void DisplayCapabilitiesBuilder::UpdateDisplayCapabilities(
    const smart_objects::SmartObject& incoming_display_capabilities) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace smart_objects;
  sync_primitives::AutoLock lock(display_capabilities_lock_);

  if (!display_capabilities_) {
    display_capabilities_ = std::make_shared<SmartObject>(SmartType_Array);
  }

  // As per v6.0, only single display is supported
  auto cur_window_caps =
      (*display_capabilities_)[0][strings::window_capabilities];

  const auto& inc_window_caps =
      incoming_display_capabilities[0][strings::window_capabilities];

  for (size_t i = 0; i < inc_window_caps.length(); ++i) {
    const WindowID window_id =
        inc_window_caps[i].keyExists(strings::window_id)
            ? inc_window_caps[i][strings::window_id].asInt()
            : kDefaultWindowID;
    if (window_ids_to_resume_.end() != window_ids_to_resume_.find(window_id)) {
      cur_window_caps[cur_window_caps.length()] = inc_window_caps[i];
      LOG4CXX_DEBUG(logger_, "Stop waiting for: " << window_id);
      window_ids_to_resume_.erase(window_id);
    }
  }

  *display_capabilities_ = incoming_display_capabilities;
  (*display_capabilities_)[0][strings::window_capabilities] = cur_window_caps;

  InvokeCallbackFunction();
}

const smart_objects::SmartObjectSPtr
DisplayCapabilitiesBuilder::display_capabilities() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return display_capabilities_;
}

void DisplayCapabilitiesBuilder::InvokeCallbackFunction() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!window_ids_to_resume_.empty()) {
    LOG4CXX_DEBUG(logger_, "Still waiting for another windows capabilities");
    return;
  }

  if (!display_capabilities_) {
    LOG4CXX_DEBUG(logger_, "Cached display capabilities are not available");
    return;
  }

  if (owner_.hmi_level(kDefaultWindowID) ==
      mobile_apis::HMILevel::INVALID_ENUM) {
    LOG4CXX_DEBUG(logger_, "Main window HMI level is not set yet");
    return;
  }

  LOG4CXX_TRACE(logger_, "Invoking resume callback");
  resume_callback_(owner_, *display_capabilities_);
  display_capabilities_.reset();
}

bool DisplayCapabilitiesBuilder::IsWaitingForWindowCapabilities(
    const smart_objects::SmartObject& incoming_display_capabilities) {
  const auto& inc_window_caps =
      incoming_display_capabilities[0][strings::window_capabilities];

  sync_primitives::AutoLock lock(display_capabilities_lock_);
  for (size_t i = 0; i < inc_window_caps.length(); ++i) {
    const WindowID window_id =
        inc_window_caps[i].keyExists(strings::window_id)
            ? inc_window_caps[i][strings::window_id].asInt()
            : kDefaultWindowID;
    if (helpers::in_range(window_ids_to_resume_, window_id)) {
      LOG4CXX_TRACE(
          logger_,
          "Application is waiting for capabilities for window " << window_id);
      return true;
    }
  }

  LOG4CXX_TRACE(
      logger_,
      "Application is not waiting for any of these windows capabilities");
  return false;
}

void DisplayCapabilitiesBuilder::ResetDisplayCapabilities() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(display_capabilities_lock_);
  for (auto& window_id : window_ids_to_resume_) {
    if (kDefaultWindowID != window_id) {
      window_ids_to_resume_.erase(window_id);
    }
  }

  if (display_capabilities_) {
    auto* cur_window_caps_ptr =
        (*display_capabilities_)[0][strings::window_capabilities].asArray();
    if (cur_window_caps_ptr) {
      std::remove_if(cur_window_caps_ptr->begin(),
                     cur_window_caps_ptr->end(),
                     [](const smart_objects::SmartObject& item) {
                       return item.keyExists(strings::window_id) &&
                              item[strings::window_id].asInt() !=
                                  kDefaultWindowID;
                     });
    }
  }
}

void DisplayCapabilitiesBuilder::StopWaitingForWindow(
    const WindowID window_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(display_capabilities_lock_);
  LOG4CXX_DEBUG(logger_,
                "Window id " << window_id << " will be erased due to failure");
  window_ids_to_resume_.erase(window_id);

  InvokeCallbackFunction();
}

}  // namespace application_manager
