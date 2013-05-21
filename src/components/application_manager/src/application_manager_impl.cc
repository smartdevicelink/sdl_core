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

#include "application_manager/application.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/basic_command_factory.h"
#include "application_manager/message_conversion.h"

namespace application_manager {

ApplicationManagerImpl * ApplicationManagerImpl::self_ = NULL;

ApplicationManagerImpl::~ApplicationManagerImpl() {

}

ApplicationManagerImpl * ApplicationManagerImpl::GetInstance() {
	if (NULL == self_) {
		self_ = new ApplicationManagerImpl();
	}
	return self_;
}

Application * ApplicationManagerImpl::application(int app_id) {
	std::map<int, Application *>::iterator it = applications_.find(app_id);
	if (applications_.end() != it) {
		return it->second;
	}
	else {
		return NULL;
	}
}

bool ApplicationManagerImpl::RegisterApplication(Application * application)
{
  return true;
}

bool ApplicationManagerImpl::UnregisterApplication(Application * application)
{

  return true;
}

std::vector<Application *>
ApplicationManagerImpl::applications() const {
	std::vector<Application*> result;
	for(std::map<int, Application*>::const_iterator it = applications_.begin();
		applications_.end() != it;
		++it) {

		if(it->second->app_id() == it->first) {
			result.push_back(it->second);
		}
	}
	return result;
}

void ApplicationManagerImpl::onMessageReceived(application_manager::Message * message)
{
  NsSmartDeviceLink::NsSmartObjects::CSmartObject smart_object = MessageToSmartObject(*message);
  CommandSharedPtr command = BasicCommandFactory::CreateCommand(&smart_object);
  command->Init();
  command->Run();
  command->CleanUp();
}

} // namespace application_manager
