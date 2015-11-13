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

#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "application_manager/commands/hmi/on_ui_perform_audio_pass_thru_start_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

OnUIPerformAudioPassThruStartNotification::OnUIPerformAudioPassThruStartNotification(
    const MessageSharedPtr& message)
    : NotificationFromHMI(message) {
}

OnUIPerformAudioPassThruStartNotification::~OnUIPerformAudioPassThruStartNotification() {
}

void OnUIPerformAudioPassThruStartNotification::Run() {
  LOG4CXX_INFO(logger_, "OnUIPerformAudioPassThruStartNotification::Run");

	LOG4CXX_INFO(logger_, "audio_pass_thru_type=" << (*message_)[strings::msg_params][strings::audio_pass_thru_type].asInt()
		<< " max_duration=" << (*message_)[strings::msg_params][strings::max_duration].asInt()
		<< " sampling_rate=" << (*message_)[strings::msg_params][strings::sampling_rate].asInt()
		<< " bits_per_sample=" << (*message_)[strings::msg_params][strings::bits_per_sample].asInt()
		<< " audio_type=" << (*message_)[strings::msg_params][strings::audio_type].asInt()
		<< " send_audio_pass_thru_file=" << (*message_)[strings::msg_params][strings::send_audio_pass_thru_file].asString()
		<< " save_audio_pass_thru_file=" << (*message_)[strings::msg_params][strings::save_audio_pass_thru_file].asString());

	switch((*message_)[strings::msg_params][strings::audio_pass_thru_type].asInt()){
		case hmi_apis::Common_AudioPassThruType::RECORD_SEND:
			{
				ApplicationManagerImpl::instance()->StartAudioPassThruThread(
						0,
						0,
						(*message_)[strings::msg_params][strings::max_duration].asInt(),
						(*message_)[strings::msg_params][strings::sampling_rate].asInt(),
						(*message_)[strings::msg_params][strings::bits_per_sample].asInt(),
						(*message_)[strings::msg_params][strings::audio_type].asInt());
			}
			break;
		case hmi_apis::Common_AudioPassThruType::READ_FILE_SEND:
			{
				ApplicationManagerImpl::instance()->StartAudioPassThruReadFileThread(
					(*message_)[strings::msg_params][strings::send_audio_pass_thru_file].asString());
			}
			break;
		case hmi_apis::Common_AudioPassThruType::RECORD_SAVE_SEND:
			{
				ApplicationManagerImpl::instance()->StartAudioPassThruThread(
						0,
						0,
						(*message_)[strings::msg_params][strings::max_duration].asInt(),
						(*message_)[strings::msg_params][strings::sampling_rate].asInt(),
						(*message_)[strings::msg_params][strings::bits_per_sample].asInt(),
						(*message_)[strings::msg_params][strings::audio_type].asInt(),
						true, 
						true, 
						(*message_)[strings::msg_params][strings::save_audio_pass_thru_file].asString());
			}
			break;
		case hmi_apis::Common_AudioPassThruType::SAVE_ONLY:
			{
				ApplicationManagerImpl::instance()->StartAudioPassThruThread(
						0,
						0,
						(*message_)[strings::msg_params][strings::max_duration].asInt(),
						(*message_)[strings::msg_params][strings::sampling_rate].asInt(),
						(*message_)[strings::msg_params][strings::bits_per_sample].asInt(),
						(*message_)[strings::msg_params][strings::audio_type].asInt(),
						true, 
						false, 
						(*message_)[strings::msg_params][strings::save_audio_pass_thru_file].asString());
			}
			break;
		default:
			break;
	}
}
	
}  // namespace commands

}  // namespace application_manager

