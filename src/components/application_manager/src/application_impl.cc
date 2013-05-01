// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 'A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "application_manager/application_impl.h"

namespace application_manager {

ApplicationImpl::ApplicationImpl(int app_id)
	: hmi_level_(mobile_api::HMILevel::INVALID_ENUM)
	, system_context_(mobile_api::SystemContext::INVALID_ENUM)
	, language_(mobile_api::Language::INVALID_ENUM)
	, ui_language_(mobile_api::Language::INVALID_ENUM)
	, active_message_(NULL)
	, is_media_(false)
	, app_id_(app_id) {

}

ApplicationImpl::~ApplicationImpl() {
	// TODO(AK): check if this is correct assimption
	if (active_message_) {
		delete active_message_;
		active_message_ = NULL;
	}
}
	
void ApplicationImpl::processMessage(Message * message) {

}
	
Message * ApplicationImpl::activeMessage() const {
	return active_message_;
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
	
const std::vector<mobile_api::AppType::eType> &
	ApplicationImpl::app_types() const {
	return app_types_;
}

const std::vector<std::string> & ApplicationImpl::vr_synonyms() const {
	return vr_synonyms_;
}
	
const std::string & ApplicationImpl::mobile_app_id() const {
	return mobile_app_id_;
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

void ApplicationImpl::set_app_types(
		const std::vector<mobile_api::AppType::eType> & app_types) {
	app_types_ = app_types;
}
	
void ApplicationImpl::set_vr_synonyms(
	const std::vector<std::string> & vr_synonyms) {
	vr_synonyms_ = vr_synonyms;
}
	
void ApplicationImpl::set_mobile_app_id(const std::string & mobile_app_id) {
	mobile_app_id_ = mobile_app_id;
}

} // namespace application_manager