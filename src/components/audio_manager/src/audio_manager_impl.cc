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

#include <algorithm>
#include <iterator>
#include <vector>
#include "application_manager/message_helper.h"
#include "audio_manager/audio_manager_impl.h"
#include "utils/file_system.h"

namespace audio_manager {

log4cxx::LoggerPtr AudioManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("AudioManagerImpl"));

AudioManagerImpl* AudioManagerImpl::sInstance_ = 0;

const std::string AudioManagerImpl::sA2DPSourcePrefix_ = "bluez_source.";

AudioManager* AudioManagerImpl::getAudioManager() {
  if (0 == sInstance_) {
    sInstance_ = new AudioManagerImpl;
  }

  return sInstance_;
}

AudioManagerImpl::AudioManagerImpl()
// Six hex-pairs + five underscores + '\n'
  : MAC_ADDRESS_LENGTH_(12 + 5 + 1),
    kH264FileName("h264file.mp4"),
    protocol_handler_(NULL),
    videoStreamerThread_(NULL),
    is_stream_running_(false),
    app_connection_key(0),
    timer_(this, &AudioManagerImpl::onTimer),
    video_serever_() {
}

AudioManagerImpl::~AudioManagerImpl() {
  protocol_handler_ = NULL;
  videoStreamerThread_ = NULL;
}

void AudioManagerImpl::SetProtocolHandler(
  protocol_handler::ProtocolHandler* protocol_hndlr) {
  DCHECK(protocol_hndlr);
  protocol_handler_ = protocol_hndlr;
  protocol_handler_->AddProtocolObserver(this);
}

void AudioManagerImpl::addA2DPSource(const std::string& device) {
  if (sources_.find(device) == sources_.end()) {
    sources_.insert(std::pair<std::string, threads::Thread*>(
                      device, NULL));
  }
}

void AudioManagerImpl::removeA2DPSource(const std::string& device) {
  std::map<std::string, threads::Thread*>::iterator it =
    sources_.find(device);

  if (it != sources_.end()) {
    // Source exists
    LOG4CXX_DEBUG(logger_, "Source exists");
    if (NULL != (*it).second) {
      // Sources thread was allocated
      LOG4CXX_DEBUG(logger_, "Sources thread was allocated");
      if ((*it).second->is_running()) {
        // Sources thread was started - stop it
        LOG4CXX_DEBUG(logger_, "Sources thread was started - stop it");
        (*it).second->stop();
      }
      // Delete allocated thread
      LOG4CXX_DEBUG(logger_, "Delete allocated thread");
      delete(*it).second;
    }

    sources_.erase(it);
  }
}

void AudioManagerImpl::playA2DPSource(const std::string& device) {
  std::map<std::string, threads::Thread*>::iterator it =
    sources_.find(device);

  if (it != sources_.end()) {
    // Source exists - allocate thread for the source
    (*it).second = new threads::Thread((*it).first.c_str(),
                                       new A2DPSourcePlayerThread((*it).first));

    if (NULL != (*it).second) {
      // Thread was successfully allocated - start thread
      (*it).second->start();
    }
  }
}

void AudioManagerImpl::stopA2DPSource(const std::string& device) {
  std::map<std::string, threads::Thread*>::iterator it =
    sources_.find(device);

  if (it != sources_.end()) {
    // Source exists
    if (NULL != (*it).second) {
      // Sources thread was allocated
      if ((*it).second->is_running()) {
        // Sources thread was started - stop it
        (*it).second->stop();
      }
    }
  }
}

std::string AudioManagerImpl::sockAddr2SourceAddr(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  char mac[MAC_ADDRESS_LENGTH_];

  snprintf(mac, MAC_ADDRESS_LENGTH_, "%02x_%02x_%02x_%02x_%02x_%02x",
           (unsigned char)device.sa_data[0], (unsigned char)device.sa_data[1],
           (unsigned char)device.sa_data[2], (unsigned char)device.sa_data[3],
           (unsigned char)device.sa_data[4], (unsigned char)device.sa_data[5]);

  std::string ret = std::string(mac);

  std::transform(ret.begin(), ret.end(), ret.begin(), toupper);

  ret = sA2DPSourcePrefix_ + ret;

  LOG4CXX_TRACE(logger_, "Mac : " << ret);

  return ret;
}

void AudioManagerImpl::addA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  addA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::removeA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  removeA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::playA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  playA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::stopA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  stopA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::startMicrophoneRecording(const std::string& outputFileName,
    mobile_apis::SamplingRate::eType samplingRate,
    int duration,
    mobile_apis::BitsPerSample::eType bitsPerSample,
    unsigned int session_key, unsigned int correlation_id) {
  LOG4CXX_TRACE_ENTER(logger_);

  FromMicToFileRecorderThread* recordThreadDelegate =
    new FromMicToFileRecorderThread();

  if (NULL != recordThreadDelegate) {
    recordThreadDelegate->setOutputFileName(outputFileName);
    recordThreadDelegate->setRecordDuration(duration);

    recorderThread_ = new threads::Thread("MicrophoneRecorder"
                                          , recordThreadDelegate);

    if (NULL != recorderThread_) {
      recorderThread_->start();
    }
  }

  AudioStreamSenderThread* senderThreadDelegate =
    new AudioStreamSenderThread(outputFileName,
                                static_cast<unsigned int>(session_key),
                                static_cast<unsigned int>(correlation_id));

  if (NULL != recordThreadDelegate) {
    senderThread_ = new threads::Thread("AudioStreamSender"
                                        , senderThreadDelegate);

    if (NULL != senderThread_) {
      senderThread_->start();
    }
  }
}

void AudioManagerImpl::stopMicrophoneRecording() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (NULL != recorderThread_) {
    recorderThread_->stop();
    delete recorderThread_;
    recorderThread_ = NULL;
  }

  if (NULL != senderThread_) {
    senderThread_->stop();
    delete senderThread_;
    senderThread_ = NULL;
  }
}

void AudioManagerImpl::startVideoStreaming(const std::string& fileName) {
  LOG4CXX_TRACE_ENTER(logger_);

  VideoStreamingThread* videoStreamingThreadDelegate =
    new VideoStreamingThread();

  if (NULL != videoStreamingThreadDelegate) {
    videoStreamingThreadDelegate->setVideoFileName(fileName);

    videoStreamerThread_ = new threads::Thread("VideoStreamer"
        , videoStreamingThreadDelegate);

    if (NULL != videoStreamerThread_) {
      videoStreamerThread_->start();
    }
  }
}

void AudioManagerImpl::stopVideoStreaming() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (NULL != videoStreamerThread_) {
    is_stream_running_ = false;
    recorderThread_->stop();
    delete videoStreamerThread_;
    videoStreamerThread_ = NULL;
  }
}

void AudioManagerImpl::OnMessageReceived(
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_TRACE_ENTER(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to message.");
    NOTREACHED();
    return;
  }

  if (message->is_fully_binary()) {

    if (false == is_stream_running_) {

      if (file_system::FileExists(kH264FileName)) {
        file_system::DeleteFile(kH264FileName);
      }

      LOG4CXX_ERROR(logger_, "Start streaming timer");
      is_stream_running_ = true;
      app_connection_key = (*message).connection_key();
      // FIXME
      //timer_.start(10);
      video_serever_.start();
      const std::string url = "http://localhost:5050";
      application_manager::MessageHelper::SendNaviStartStream(
        url, app_connection_key);
    }

    // TODO(DK): only temporary
    static int messsages_for_session = 0;
    ++messsages_for_session;
    LOG4CXX_INFO(logger_, "Handling map streaming message. This is "
                 << messsages_for_session << "th message for " << app_connection_key);

    if (10 == messsages_for_session) {
      protocol_handler_->SendFramesNumber(app_connection_key,
                                          messsages_for_session);
      messsages_for_session = 0;
    }

    video_serever_.sendMsg(message);
  }
}

void AudioManagerImpl::onTimer() const {
  // start streaming
  LOG4CXX_ERROR(logger_, "Start streaming");
  const std::string url = "http://localhost:8000/live";
  AudioManagerImpl::getAudioManager()->startVideoStreaming(kH264FileName);
  application_manager::MessageHelper::SendNaviStartStream(
    url, app_connection_key);
}

}  // namespace audio_manager
