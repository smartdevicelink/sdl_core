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
#include "application_manager/commands/hmi/on_microphone_recording_notification.h"
#include "application_manager/application_manager_impl.h"
#include "utils/file_system.h"
#include "encryption/Base64.h"

namespace application_manager {

namespace commands {

OnMicrophoneRecordingNotification::OnMicrophoneRecordingNotification(
  const MessageSharedPtr& message): NotificationToHMI(message) {
}

OnMicrophoneRecordingNotification::~OnMicrophoneRecordingNotification() {
}

void OnMicrophoneRecordingNotification::Run() {
  LOG4CXX_INFO(logger_, "OnMicrophoneRecordingNotification::Run");
  // add by patrick
  // write wav file...
//  std::string strData = (*message_)[strings::msg_params][strings::binary_data].asString();
//	//std::vector<unsigned char> vecDataAfterDecode(strData.length());
//	//memcpy(vecDataAfterDecode.data(), (unsigned char *)strData.data(), strData.length());
//	//file_system::Write("receive_record_from_ale.wav", vecDataAfterDecode, std::ios_base::app);
//  std::string strDecodeData = base64_decode(strData);
//  std::vector<unsigned char> vecData(strDecodeData.length());
//  memcpy(vecData.data(), (unsigned char *)strDecodeData.data(), strDecodeData.length());
//#ifdef MODIFY_FUNCTION_SIGN
//  //file_system::Write("record_after_decode.wav", vecData, std::ios_base::app);
//	int app_id = (*message_)[strings::msg_params][strings::app_id].asInt();
//	application_manager::ApplicationManagerImpl::instance()->
//		SendAudioPassThroughNotification(app_id, vecData);
//#endif
//   SendNotification();
}

}  // namespace commands

}  // namespace application_manager

