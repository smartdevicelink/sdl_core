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
#include "application_manager/mobile_command_factory.h"
#include "application_manager/message_conversion.h"
#include "application_manager/message_chaining.h"
#include "application_manager/audio_pass_thru_thread_impl.h"
#include "utils/macro.h"
#include "utils/threads/thread.h"

namespace application_manager {

ApplicationManagerImpl::ApplicationManagerImpl()
  : hmi_deletes_commands_(false),
    audio_pass_thru_flag_(false),
    perform_audio_thread_(NULL) {
}

ApplicationManagerImpl::~ApplicationManagerImpl() {
  message_chaining_.clear();
  delete perform_audio_thread_;
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

Application* ApplicationManagerImpl::active_application() const {
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->IsFullscreen()) {
      return *it;
    }
  }
  return NULL;
}

std::vector<Application*> ApplicationManagerImpl::applications_by_button(
  unsigned int button) {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    if ((*it)->IsSubscribedToButton(static_cast<mobile_api::ButtonName::eType>(
                                      button))) {
      result.push_back(*it);
    }
  }
  return result;
}

std::vector<Application*> ApplicationManagerImpl::applications_by_ivi(
  unsigned int vehicle_info) {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
      application_list_.end() != it; ++it) {
    if ((*it)->IsSubscribedToIVI(vehicle_info)) {
      result.push_back(*it);
    }
  }
  return result;
}

std::vector<Application*> ApplicationManagerImpl::applications_with_navi() {
  std::vector<Application*> result;
  for (std::set<Application*>::iterator it = application_list_.begin();
      application_list_.end() != it;
      ++it) {
    if ((*it)->SupportsNavigation()) {
      result.push_back(*it);
    }
  }
  return result;
}

std::set<connection_handler::Device>& ApplicationManagerImpl::device_list() {
  std::set<connection_handler::Device> devices;
  return devices;
}

bool ApplicationManagerImpl::RegisterApplication(Application* application) {
  DCHECK(application);
  if (NULL == application) {
    return false;
  }
  std::map<int, Application*>::iterator it = applications_.find(
        application->app_id());
  if (applications_.end() != it) {
    return false;
  }
  applications_.insert(std::pair<int, Application*>(
                         application->app_id(), application));
  application_list_.insert(application);
  return true;
}

bool ApplicationManagerImpl::UnregisterApplication(Application* application) {
  DCHECK(application);
  if (NULL == application) {
    return false;
  }
  std::map<int, Application*>::iterator it = applications_.find(
        application->app_id());
  if (applications_.end() == it) {
    return false;
  }
  applications_.erase(it);
  application_list_.erase(application);
  return true;
}

void ApplicationManagerImpl::UnregisterAllApplications() {
  applications_.clear();
  for (std::set<Application*>::iterator it = application_list_.begin();
       application_list_.end() != it;
       ++it) {
    delete(*it);
  }
  application_list_.clear();
}

void ApplicationManagerImpl::ConnectToDevice(unsigned int id) {
  // TODO(VS): Call function from ConnectionHandler
}

const std::set<Application*>& ApplicationManagerImpl::applications() const {
  return application_list_;
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
    message_chaining_[correlation_id] = ptr;
    return chain;
  } else  {
    MessageChains::const_iterator it = message_chaining_.begin();
    for (; it != message_chaining_.end(); ++it) {
      if ((*it->second) == *chain) {
        it->second->IncrementCounter();
        message_chaining_[correlation_id] = it->second;
        break;
      }
    }
  }
  return chain;
}

bool ApplicationManagerImpl::DecreaseMessageChain(
    unsigned int correlation_id) {
  bool result = false;
  MessageChains::iterator it =
      message_chaining_.find(correlation_id);

  if (message_chaining_.end() != it) {
    (*it->second).DecrementCounter();
    if (!(*it->second).counter()) {
      result = true;
    }
    message_chaining_.erase(it);
  }
  return result;
}

MessageChaining* ApplicationManagerImpl::GetMessageChain(
  unsigned int correlation_id) const {
  MessageChains::const_iterator it =
      message_chaining_.find(correlation_id);
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

void ApplicationManagerImpl::StartAudioPassThruThread(int session_key,
    int correlation_id, int max_duration, int sampling_rate,
    int bits_per_sample, int audio_type) {
  AudioPassThruThreadImpl* thread_impl = new AudioPassThruThreadImpl(
      static_cast<unsigned int>(session_key),
      static_cast<unsigned int>(correlation_id),
      static_cast<unsigned int>(max_duration),
      static_cast<SamplingRate>(sampling_rate),
      static_cast<AudioCaptureQuality>(bits_per_sample),
      static_cast<AudioType>(audio_type));

  thread_impl->Init();
  perform_audio_thread_ = new threads::Thread("AudioPassThru thread",
      thread_impl);

  perform_audio_thread_->startWithOptions(
    threads::ThreadOptions(threads::Thread::kMinStackSize));
}

void ApplicationManagerImpl::StopAudioPassThruThread() {
  perform_audio_thread_->stop();
  delete perform_audio_thread_;
  perform_audio_thread_ = NULL;
}

void ApplicationManagerImpl::onMessageReceived(
  application_manager::Message* message) {
  NsSmartDeviceLink::NsSmartObjects::CSmartObject smart_object =
    MessageToSmartObject(*message);
  CommandSharedPtr command = MobileCommandFactory::CreateCommand(&smart_object);
  command->Init();
  command->Run();
  command->CleanUp();
}

}  // namespace application_manager
