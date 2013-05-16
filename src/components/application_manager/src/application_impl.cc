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

#include "application_manager/application_impl.h"

namespace application_manager {

ApplicationImpl::ApplicationImpl(int app_id)
    : app_id_(app_id),
      active_message_(NULL),
      is_media_(false),
      app_types_(NULL),
      vr_synonyms_(NULL),
      ngn_media_screen_name_(NULL),
      mobile_app_id_(NULL),
      tts_name_(NULL) {
}

ApplicationImpl::~ApplicationImpl() {
  // TODO(AK): check if this is correct assimption
  if (active_message_) {
    delete active_message_;
    active_message_ = NULL;
  }

  if (app_types_) {
    delete app_types_;
    app_types_ = NULL;
  }

  if (vr_synonyms_) {
    delete vr_synonyms_;
    vr_synonyms_ = NULL;
  }

  if (ngn_media_screen_name_) {
    delete ngn_media_screen_name_;
    ngn_media_screen_name_ = NULL;
  }

  if (mobile_app_id_) {
    delete mobile_app_id_;
    mobile_app_id_ = NULL;
  }

  if (tts_name_) {
    delete tts_name_;
    tts_name_ = NULL;
  }
}

void ApplicationImpl::processMessage(smart_objects::CSmartObject * message) {
}

void ApplicationImpl::reportError(smart_objects::CSmartObject * message,
                                  ErrorCode error_code) {
}

const smart_objects::CSmartObject * ApplicationImpl::activeMessage() const {
  return active_message_;
}

void ApplicationImpl::clearActiveMessage() {
  delete active_message_;
  active_message_ = NULL;
}

const Version& ApplicationImpl::version() const {
  return version_;
}

int ApplicationImpl::app_id() const {
  return app_id_;
}

const std::string& ApplicationImpl::name() const {
  return app_name_;
}

bool ApplicationImpl::is_media_application() const {
  return is_media_;
}

const smart_objects::CSmartObject* ApplicationImpl::app_types() const {
  return app_types_;
}

const smart_objects::CSmartObject* ApplicationImpl::vr_synonyms() const {
  return vr_synonyms_;
}

const smart_objects::CSmartObject* ApplicationImpl::mobile_app_id() const {
  return mobile_app_id_;
}

const smart_objects::CSmartObject* ApplicationImpl::tts_name() const {
  return tts_name_;
}

const smart_objects::CSmartObject*
ApplicationImpl::ngn_media_screen_name() const {
  return ngn_media_screen_name_;
}

const smart_objects::CSmartObject& ApplicationImpl::hmi_level() const {
  return hmi_level_;
}

const smart_objects::CSmartObject& ApplicationImpl::system_context() const {
  return system_context_;
}

const smart_objects::CSmartObject& ApplicationImpl::language() const {
  return language_;
}

const smart_objects::CSmartObject& ApplicationImpl::ui_language() const {
  return ui_language_;
}

void ApplicationImpl::set_version(const Version& version) {
  version_ = version;
}

void ApplicationImpl::set_name(const std::string& name) {
  app_name_ = name;
}

void ApplicationImpl::set_is_media_application(bool is_media) {
  is_media_ = is_media;
}

void ApplicationImpl::set_hmi_level(
    const smart_objects::CSmartObject& hmi_level) {
  hmi_level_ = hmi_level;
}

void ApplicationImpl::set_system_context(
    const smart_objects::CSmartObject& system_context) {
  system_context_ = system_context;
}

void ApplicationImpl::set_language(
    const smart_objects::CSmartObject& language) {
  language_ = language;
}

void ApplicationImpl::set_ui_language(
    const smart_objects::CSmartObject& ui_language) {
  ui_language_ = ui_language;
}

void ApplicationImpl::set_tts_name(
    const smart_objects::CSmartObject& tts_name) {
  if (tts_name_) {
    delete tts_name_;
  }

  tts_name_ = new smart_objects::CSmartObject(tts_name);
}

void ApplicationImpl::set_ngn_media_screen_name(
    const smart_objects::CSmartObject& ngn_name) {
  if (ngn_media_screen_name_)
    delete ngn_media_screen_name_;

  ngn_media_screen_name_ = new smart_objects::CSmartObject(ngn_name);
}

void ApplicationImpl::set_app_types(
    const smart_objects::CSmartObject& app_types) {
  if (app_types_)
    delete app_types_;

  app_types_ = new smart_objects::CSmartObject(app_types);
}

void ApplicationImpl::set_vr_synonyms(
    const smart_objects::CSmartObject& vr_synonyms) {
  if (vr_synonyms_)
    delete vr_synonyms_;
  vr_synonyms_ = new smart_objects::CSmartObject(vr_synonyms);
}

void ApplicationImpl::set_mobile_app_id(
    const smart_objects::CSmartObject& mobile_app_id) {
  mobile_app_id_ = new smart_objects::CSmartObject(mobile_app_id);
}

}  // namespace application_manager
