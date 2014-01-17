#include "application_manager/commands/hmi/audio_stop_stream_request.h"

namespace application_manager {

namespace commands {

AudioStopStreamRequest::AudioStopStreamRequest(
    const MessageSharedPtr& message)
    : RequestToHMI(message) {
}

AudioStopStreamRequest::~AudioStopStreamRequest() {
}

void AudioStopStreamRequest::Run() {
  LOG4CXX_INFO(logger_, "AudioStopStreamRequest::Run");

  SendRequest();
}

}  // namespace commands

}  // namespace application_manager
