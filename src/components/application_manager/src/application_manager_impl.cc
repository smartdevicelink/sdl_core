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

#include <vector>
#include <map>
#include "application_manager/application.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/basic_command_factory.h"
#include "application_manager/message_conversion.h"
#include "application_manager/message_chaining.h"
#include "utils/macro.h"

namespace application_manager {

ApplicationManagerImpl::ApplicationManagerImpl()
: hmi_deletes_commands_(false),
  audio_pass_thru_flag_(false) {
}

ApplicationManagerImpl::~ApplicationManagerImpl() {
  message_chaining_.clear();
}

ApplicationManagerImpl* ApplicationManagerImpl::instance() {
  static ApplicationManagerImpl instance;
  return &instance;
}

Application* ApplicationManagerImpl::application(int app_id) {
  std::map<int, Application*>::iterator it = applications_.find(app_id);
  if (applications_.end() != it) {
    return it->second;
  } else {
    return NULL;
  }
}

bool ApplicationManagerImpl::RegisterApplication(Application* application) {
  return true;
}

bool ApplicationManagerImpl::UnregisterApplication(Application* application) {
  return true;
}

std::vector<Application*>
ApplicationManagerImpl::applications() const {
  std::vector<Application*> result;
  for (std::map<int, Application*>::const_iterator it = applications_.begin();
       applications_.end() != it;
       ++it) {
    if (it->second->app_id() == it->first) {
      result.push_back(it->second);
    }
  }
  return result;
}

MessageChaining* ApplicationManagerImpl::AddMessageChain(MessageChaining* chain,
    unsigned int connection_key,
    unsigned int correlation_id,
    unsigned int function_id,
    const NsSmartDeviceLink::NsSmartObjects::CSmartObject* data) {
  if (!chain) {
      chain = new MessageChaining(
          connection_key,
          correlation_id);
      if (data) {
        chain->set_data(*data);
      }

      MessageChainPtr ptr(chain);
      message_chaining_[function_id] = ptr;
      return chain;
  } else  {
      chain->increment_counter();
      MessageChains::const_iterator it = message_chaining_.begin();
      for (; it != message_chaining_.end(); ++it) {
        if ((*it->second) == *chain) {
          message_chaining_[function_id] = it->second;
          break;
        }
      }
      return chain;
  }
}

bool ApplicationManagerImpl::DecreaseMessageChain(unsigned int function_id) {
  bool result = false;
  MessageChains::iterator it = message_chaining_.find(function_id);

  if (message_chaining_.end() != it) {
    (*it->second).decrement_counter();
    if (!(*it->second).counter()) {
      result = true;
    }
    message_chaining_.erase(it);
  }
  return result;
}

const MessageChaining* ApplicationManagerImpl::GetMessageChain(
    unsigned int function_id) const {
  MessageChains::const_iterator it = message_chaining_.find(function_id);
  if (message_chaining_.end() != it) {
    return &(*it->second);
  }

  return NULL;
}

bool ApplicationManagerImpl::audio_pass_thru_flag() const {
  return audio_pass_thru_flag_;
}

void ApplicationManagerImpl::set_audio_pass_thru_flag(bool flag) {
  audio_pass_thru_flag_ = flag;
}

void ApplicationManagerImpl::onMessageReceived(
  application_manager::Message* message) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject smart_object =
    MessageToSmartObject(*message);
  CommandSharedPtr command = BasicCommandFactory::CreateCommand(&smart_object);
  command->Init();
  command->Run();
  command->CleanUp();
}

}  // namespace application_manager
