/*
 * Copyright (c) 2020, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_EXTENSION_PENDING_RESUMPTION_HANDLER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_EXTENSION_PENDING_RESUMPTION_HANDLER_H
#include "application_manager/app_extension.h"
#include "application_manager/application_manager.h"
#include "application_manager/event_engine/event_observer.h"

namespace resumption {

namespace app_mngr = application_manager;

class ExtensionPendingResumptionHandler
    : public application_manager::event_engine::EventObserver {
 public:
  ExtensionPendingResumptionHandler(
      app_mngr::ApplicationManager& application_manager);

  virtual ~ExtensionPendingResumptionHandler() {}

  // EventObserver interface
  virtual void on_event(
      const application_manager::event_engine::Event& event) = 0;

  virtual void HandleResumptionSubscriptionRequest(
      app_mngr::AppExtension& extension,
      Subscriber& subscriber,
      application_manager::Application& app) = 0;

  virtual void ClearPendingResumptionRequests() = 0;

 protected:
  virtual ResumptionRequest MakeResumptionRequest(
      const uint32_t corr_id,
      const hmi_apis::FunctionID::eType function_id,
      const smart_objects::SmartObject& message);

  app_mngr::ApplicationManager& application_manager_;
};
}  //  namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_EXTENSION_PENDING_RESUMPTION_HANDLER_H