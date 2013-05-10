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
#include "get_device_list_response_command.h"
#include "JSONHandler/CSmartFactory.hpp"
#include "SmartObjects/CSmartObject.hpp"

namespace application_manager {
namespace commands {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
using namespace NsSmartDeviceLink::NsJSONHandler::strings;

GetDeviceListResponseCommand::GetDeviceListResponseCommand(
		int result_code,
		std::vector<std::string> * device_list)
: device_list_(NULL)
, result_code_(result_code) {
	device_list_ = device_list;
}

GetDeviceListResponseCommand::~GetDeviceListResponseCommand() {
	device_list_ = NULL;
	result_code_ = -1;
}
	
void GetDeviceListResponseCommand::run() {
	smart_objects::CSmartObject response;
	response[S_PARAMS][S_MESSAGE_TYPE] = "Response";
	
	if (device_list_) {
		for(int i = 0; i < device_list_->size(); ++i) {
			response[S_MSG_PARAMS]["deviceList"][i] = device_list_->at(i);
		}
	}
	response[S_MSG_PARAMS]["code"] = result_code_;
	//response.setSchema();
	//if (response.isValid() == smart_objcets::Errors::OK) ApplicationManger->sendMessage();
}


} // namespace commands
} // namespace application_manager