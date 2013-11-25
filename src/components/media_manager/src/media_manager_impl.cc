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
#include <stdio.h>
#include "config_profile/profile.h"
#include "media_manager/media_manager_impl.h"

namespace media_manager {

log4cxx::LoggerPtr MediaManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("MediaManagerImpl"));

MediaManagerImpl* MediaManagerImpl::sInstance_ = 0;

const std::string MediaManagerImpl::sA2DPSourcePrefix_ = "bluez_source.";

MediaManagerImpl* MediaManagerImpl::getMediaManager() {
  if (0 == sInstance_) {
    sInstance_ = new MediaManagerImpl;
  }

  return sInstance_;
}

MediaManagerImpl::MediaManagerImpl()
// Six hex-pairs + five underscores + '\n'
  : MAC_ADDRESS_LENGTH_(12 + 5 + 1),
    protocol_handler_(NULL),
    redecoder_(NULL),
    video_server_(NULL),
    senderThread_(NULL),
    recorderThread_(NULL) {
}

MediaManagerImpl::~MediaManagerImpl() {
  if (NULL  != video_server_) {
    delete video_server_;
    video_server_ = NULL;
  }

  if (NULL != redecoder_) {
    delete redecoder_;
    redecoder_ = NULL;
  }

  protocol_handler_ = NULL;
}

void MediaManagerImpl::SetProtocolHandler(
  protocol_handler::ProtocolHandler* protocol_hndlr) {
  DCHECK(protocol_hndlr);
  protocol_handler_ = protocol_hndlr;
  protocol_handler_->AddProtocolObserver(this);
}

void MediaManagerImpl::addA2DPSource(const std::string& device) {
  if (sources_.find(device) == sources_.end()) {
    sources_.insert(std::pair<std::string, threads::Thread*>(device, NULL));
  }
}

void MediaManagerImpl::removeA2DPSource(const std::string& device) {
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

void MediaManagerImpl::playA2DPSource(const std::string& device) {
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

void MediaManagerImpl::stopA2DPSource(const std::string& device) {
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

std::string MediaManagerImpl::sockAddr2SourceAddr(const sockaddr& device) {
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

void MediaManagerImpl::addA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  addA2DPSource(sockAddr2SourceAddr(device));
}

void MediaManagerImpl::removeA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  removeA2DPSource(sockAddr2SourceAddr(device));
}

void MediaManagerImpl::playA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  playA2DPSource(sockAddr2SourceAddr(device));
}

void MediaManagerImpl::stopA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  stopA2DPSource(sockAddr2SourceAddr(device));
}

void MediaManagerImpl::startMicrophoneRecording(const std::string& outputFileName,
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

void MediaManagerImpl::stopMicrophoneRecording() {
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


void MediaManagerImpl::startVideoStreaming() {
  if (video_server_) {
    video_server_->start();
  }
}

void MediaManagerImpl::stopVideoStreaming() {
  if (video_server_) {
    video_server_->stop();
  }
}

void MediaManagerImpl::OnMessageReceived(
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_TRACE_ENTER(logger_);

  if (!message) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to message.");
    NOTREACHED();
    return;
  }

  if (message->is_fully_binary()) {

    video_server_->sendMsg(message);

    // TODO(DK): only temporary
    static int messsages_for_session = 0;
    ++messsages_for_session;
    int app_connection_key = 0;
    LOG4CXX_INFO(logger_, "Handling map streaming message. This is "
                 << messsages_for_session << "th message for " << app_connection_key);

    if (10 == messsages_for_session) {
      protocol_handler_->SendFramesNumber(app_connection_key,
                                          messsages_for_session);
      messsages_for_session = 0;
    }
  }
}

void MediaManagerImpl::onRedecoded(const protocol_handler::RawMessagePtr&
                                        message) {
}

void MediaManagerImpl::setVideoRedecoder(redecoding::VideoRedecoder* redecoder) {
   redecoder_ = redecoder;
}

void MediaManagerImpl::setConsumer(video_stream_producer_consumer::VideoStreamConsumer* server) {
  LOG4CXX_TRACE_ENTER(logger_);
  video_server_ = server;

  startVideoStreaming();
}

}  // namespace media_manager
