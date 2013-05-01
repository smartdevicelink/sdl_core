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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APPLICATION_IMPL

#include "application_manager/message.h"
#include "application_manager/application.h"
#include "v4_protocol_v2_0_revT.h"

namespace application_manager {

namespace mobile_api = NsSmartDeviceLinkRPC::V2;

class ApplicationImpl : public Application {
public:
	explicit ApplicationImpl(int app_id);
	~ApplicationImpl();
	virtual void processMessage(Message * message);
	virtual Message * activeMessage() const ;
	const Version& version() const;
	int app_id() const;
	const std::string& name() const;
	bool is_media_application() const;
	const std::vector<mobile_api::AppType::eType> & app_types() const;
	const std::vector<std::string> & vr_synonyms() const;
	const std::string & mobile_app_id() const;
	void set_version(const Version& version);
	void set_app_id(int app_id);
	void set_name(const std::string& name);
	void set_is_media_application(bool is_media);
	void set_app_types(const std::vector<mobile_api::AppType::eType> & app_types);
	void set_vr_synonyms(const std::vector<std::string> & vr_synonyms);
	void set_mobile_app_id(const std::string & mobile_app_id);

private:
	Version version_;
	mobile_api::HMILevel::eType hmi_level_;
	mobile_api::SystemContext::eType system_context_;
	mobile_api::Language::eType language_;
	mobile_api::Language::eType ui_language_;
	std::vector<mobile_api::AppType::eType> app_types_;
	std::vector<std::string> vr_synonyms_;
	Message * active_message_;
	bool is_media_;
	int app_id_;
	std::string app_name_;
	std::string ngn_media_screen_app_name_;
	std::string mobile_app_id_;
	//TTSChunks  - ?
};

} // namespace application_manager

#endif