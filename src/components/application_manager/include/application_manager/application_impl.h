/**
* Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_

#include <string>

#include "SmartObjects/CSmartObject.hpp"

#include "application_manager/message.h"
#include "application_manager/application.h"

// TODO(AK): Include the directory when naming .h files
#include "v4_protocol_v2_0_revT.h"

namespace application_manager {

namespace mobile_api = NsSmartDeviceLinkRPC::V2;

class ApplicationImpl : public Application {
 public:
  explicit ApplicationImpl(int app_id);
  ~ApplicationImpl();

  void processMessage(smart_objects::CSmartObject* message);
  void reportError(smart_objects::CSmartObject* message, ErrorCode error_code);
  const smart_objects::CSmartObject* activeMessage() const;
  void clearActiveMessage();
  const Version& version() const;
  int app_id() const;
  const std::string& name() const;

  bool is_media_application() const;
  const smart_objects::CSmartObject * app_types() const;
  const smart_objects::CSmartObject * vr_synonyms() const;
  const smart_objects::CSmartObject * mobile_app_id() const;
  const smart_objects::CSmartObject * tts_name() const;
  const smart_objects::CSmartObject * ngn_media_screen_name() const;
  const smart_objects::CSmartObject & hmi_level() const;
  const smart_objects::CSmartObject & system_context() const;
  const smart_objects::CSmartObject & language() const;
  const smart_objects::CSmartObject & ui_language() const;
  const smart_objects::CSmartObject * help_promt() const;
  const smart_objects::CSmartObject * timeout_promt() const;
  const smart_objects::CSmartObject * vr_help_title() const;
  const smart_objects::CSmartObject * vr_help() const;

  void set_version(const Version& version);
  void set_name(const std::string& name);
  void set_is_media_application(bool is_media);
  void set_hmi_level(const smart_objects::CSmartObject & hmi_level);
  void set_system_context(const smart_objects::CSmartObject & system_context);
  void set_language(const smart_objects::CSmartObject & language);
  void set_ui_language(const smart_objects::CSmartObject & ui_language);
  void set_app_types(const smart_objects::CSmartObject & app_types);
  void set_vr_synonyms(const smart_objects::CSmartObject & vr_synonyms);
  void set_mobile_app_id(const smart_objects::CSmartObject & mobile_app_id);
  void set_tts_name(const smart_objects::CSmartObject & tts_name);
  void set_ngn_media_screen_name(const smart_objects::CSmartObject & ngn_name);
  void set_help_prompt(const smart_objects::CSmartObject & help_promt);
  void set_timeout_prompt(const smart_objects::CSmartObject & timeout_promt);
  void set_vr_help_title(const smart_objects::CSmartObject & vr_help_title);
  void set_vr_help(const smart_objects::CSmartObject & vr_help);

 private:
  Version version_;
  smart_objects::CSmartObject * active_message_;
  int app_id_;
  std::string app_name_;

  bool is_media_;
  smart_objects::CSmartObject hmi_level_;
  smart_objects::CSmartObject system_context_;

  smart_objects::CSmartObject language_;
  smart_objects::CSmartObject ui_language_;
  smart_objects::CSmartObject * app_types_;
  smart_objects::CSmartObject * vr_synonyms_;
  smart_objects::CSmartObject * ngn_media_screen_name_;
  smart_objects::CSmartObject * mobile_app_id_;
  smart_objects::CSmartObject * tts_name_;
  smart_objects::CSmartObject * help_promt_;
  smart_objects::CSmartObject * timeout_promt_;
  smart_objects::CSmartObject * vr_help_title_;
  smart_objects::CSmartObject * vr_help_;
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL_H_
