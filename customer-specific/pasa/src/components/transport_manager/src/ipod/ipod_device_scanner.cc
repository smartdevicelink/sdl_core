/**
 * \file ipod_device_scanner.cc
 * \brief ipodDeviceScanner class header file.
 *
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

#include "transport_manager/ipod/ipod_device_scanner.h"

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#include <vector>
#include <string.h>
#include "transport_manager/ipod/ipod_transport_adapter.h"
#include "transport_manager/ipod/ipod_device.h"

namespace transport_manager {
namespace transport_adapter {

// Todd: Ipod support

IpodDeviceScanner::IpodDeviceScanner(
  TransportAdapterController* controller, bool auto_repeat_search,
  int auto_repeat_pause_sec)
  : controller_(controller),
    thread_(),
    thread_started_(false),
    shutdown_requested_(false),
    ready_(true),
    device_scan_requested_(false),
    device_scan_requested_lock_(),
    device_scan_requested_cv_(),
    auto_repeat_search_(auto_repeat_search),
    mPASAFWSendHandle(openMsgQ(PREFIX_STR_FROMSDLCOREUSBADAPTER_QUEUE, true, true)),
    mIncomingDevice(NULL),
    auto_repeat_pause_sec_(auto_repeat_pause_sec) {
}

IpodDeviceScanner::~IpodDeviceScanner() {
}

mqd_t IpodDeviceScanner::openMsgQ(const char *queue_name, bool sender, bool block)
{
    struct mq_attr attributes;
    attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
    attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;
    attributes.mq_flags = 0;
    int flags = 0;

    if (sender)
    {
    	flags = (block) ? (O_WRONLY|O_CREAT) : (O_WRONLY|O_CREAT|O_NONBLOCK);
    }
    else
    {
    	flags = (block) ? (O_RDONLY|O_CREAT) : (O_RDONLY|O_CREAT|O_NONBLOCK);
    }

   return mq_open(queue_name, flags, 0666, &attributes);
}

void* IpodDeviceScanner::handlePASAFrameworkIncomingMessages(void *data)
{
    LOG4CXX_TRACE_ENTER(logger_);
    IpodDeviceScanner* ipodDeviceScanner =
      static_cast<IpodDeviceScanner*>(data);

    mqd_t mq_ToSDL = ipodDeviceScanner->openMsgQ(PREFIX_STR_TOSDLCOREUSBADAPTER_QUEUE, false, true);

    char buffer[MAX_QUEUE_MSG_SIZE];
    ssize_t length=0;

    while (1)
    {
        if ((length = mq_receive(mq_ToSDL, buffer, sizeof(buffer), 0)) != -1)
        {
            LOG4CXX_INFO(logger_, "Received PASA FW Message");

        	switch (buffer[0])
        	{
				case SDL_MSG_IPOD_DEVICE_CONNECT:
				{
                    ipodDeviceScanner->connectIpodDevice(&buffer[1]);
				}
				break;
				case SDL_MSG_IPOD_DEVICE_DISCONNECT:
				{
					ipodDeviceScanner->disconnectIpodDevice(&buffer[1]);
				}
				break;
				default:
				break;
        	}
        }
    }
    LOG4CXX_TRACE_EXIT(logger_);

	return (NULL);
}



void* IpodDeviceScannerThread(void* data) {
  LOG4CXX_TRACE_ENTER(logger_);
  IpodDeviceScanner* ipodDeviceScanner =
    static_cast<IpodDeviceScanner*>(data);
  assert(ipodDeviceScanner != 0);
  ipodDeviceScanner->Thread();
  LOG4CXX_TRACE_EXIT(logger_);
  return 0;
}

bool IpodDeviceScanner::IsInitialised() const {
  return thread_started_;
}

void IpodDeviceScanner::UpdateTotalDeviceList() {
  DeviceVector devices;

  devices.insert(devices.end(), found_devices_with_sdl_.begin(),
                 found_devices_with_sdl_.end());
  controller_->SearchDeviceDone(devices);
}

void IpodDeviceScanner::DoInquiry() {
  LOG4CXX_TRACE_ENTER(logger_);

  UpdateTotalDeviceList();

}

int IpodDeviceScanner::sendMsg(mqd_t q_fd, uint8_t msgType, uint32_t length, const void *data)
{
	uint8_t sndBuf[MSGQ_MESSAGE_SIZE] = { 0 };

	if(length > 0 && NULL == data)
	{
		return -1;
	}

	sndBuf[0] = msgType;

	if ( length > 0 && data != NULL )
	{
		// copy the msg to send buffer
		memcpy(&sndBuf[0 + sizeof(msgType)], data, length);
	}

	return mq_send(q_fd, (const char *)&sndBuf[0], sizeof(sndBuf), 0);
}


void IpodDeviceScanner::connectIpodDevice(void *data)
{
	PUSBDeviceConnectInfo pDeviceInfo = static_cast<PUSBDeviceConnectInfo>(data);

    if(pDeviceInfo)
    {
        mIncomingDevice = new IpodDevice(pDeviceInfo->cDeviceName, pDeviceInfo->srcID);

        if (mIncomingDevice) {  
          LOG4CXX_INFO(logger_, "USB iPod device created successfully");
          found_devices_with_sdl_.push_back(mIncomingDevice);

          Scan();

        } else {
          LOG4CXX_WARN(logger_, "Can't create USB iPod device " << pDeviceInfo->cDeviceName);
        }

        sendMsg(mPASAFWSendHandle, SDL_MSG_IPOD_DEVICE_CONNECT_ACK, 0, NULL);
    }
}


void IpodDeviceScanner::disconnectIpodDevice(void *data)
{
	PUSBDeviceDisconnectInfo pDeviceInfo = static_cast<PUSBDeviceDisconnectInfo>(data);

    IpodDevice ipod(pDeviceInfo->cDeviceName, pDeviceInfo->srcID);

//	pthread_mutex_lock(&mDevicesMutex);
	for (DeviceVector::iterator deviceIterator = found_devices_with_sdl_.begin(); deviceIterator != found_devices_with_sdl_.end(); ++deviceIterator)
	{

		if (true == (*deviceIterator)->IsSameAs((const Device*)&ipod))
		{
            controller_->DisconnectDevice( (*deviceIterator)->unique_device_id() );
            found_devices_with_sdl_.erase(deviceIterator);
			break;
		}
	}
//	pthread_mutex_unlock(&mDevicesMutex);

    sendMsg(mPASAFWSendHandle, SDL_MSG_IPOD_DEVICE_DISCONNECT_ACK, 0, NULL);
}


void IpodDeviceScanner::Thread() {
  LOG4CXX_TRACE_ENTER(logger_);
  LOG4CXX_INFO(logger_, "ipod adapter main thread initialized");
  ready_ = true;
  if (auto_repeat_search_) {
    while (!shutdown_requested_) {
      DoInquiry();
      device_scan_requested_ = false;
      TimedWaitForDeviceScanRequest();
    }
  } else {  // search only on demand
    while (true) {
      {
        sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
      while (!(device_scan_requested_ || shutdown_requested_)) {
          device_scan_requested_cv_.Wait(auto_lock);
        }
      }
      if (shutdown_requested_) {
        break;
      }
      DoInquiry();
      device_scan_requested_ = false;
    }
  }

  LOG4CXX_INFO(logger_, "ipod device scanner thread finished");
  LOG4CXX_TRACE_EXIT(logger_);
}

void IpodDeviceScanner::TimedWaitForDeviceScanRequest() {
  LOG4CXX_TRACE_ENTER(logger_);

  if (auto_repeat_pause_sec_ == 0) {
    LOG4CXX_TRACE_EXIT(logger_);
    return;
  }

  {
    sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
    while (!(device_scan_requested_ || shutdown_requested_)) {
      const sync_primitives::ConditionalVariable::WaitStatus wait_status =
          device_scan_requested_cv_.WaitFor(auto_lock, auto_repeat_pause_sec_ * 1000);
      if (wait_status == sync_primitives::ConditionalVariable::kTimeout) {
        LOG4CXX_INFO(logger_, "ipod scanner timeout, performing scan");
        device_scan_requested_ = true;
      }
    }
  }

  LOG4CXX_TRACE_EXIT(logger_);
}

TransportAdapter::Error IpodDeviceScanner::Init() {
  LOG4CXX_TRACE_ENTER(logger_);

// Todd: Support BT 
  int thread_start_error;

  found_devices_with_sdl_.clear();

  thread_start_error = pthread_create(&mPASAFWMsgHandlerThread, 0, handlePASAFrameworkIncomingMessages, this);

  if (0 == thread_start_error) {

      pthread_setname_np(mPASAFWMsgHandlerThread, "PASAFW BTAdapter");

      thread_start_error = pthread_create(&thread_, 0,
                                     &IpodDeviceScannerThread,
                                     this);
  }

  if (0 == thread_start_error) {
    thread_started_ = true;
    LOG4CXX_INFO(logger_, "ipod device scanner thread started");
  } else {
    LOG4CXX_ERROR(
      logger_,
      "ipod device scanner thread start failed, error code "
      << thread_start_error);
    LOG4CXX_TRACE_EXIT(logger_);
    return TransportAdapter::FAIL;
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return TransportAdapter::OK;
}

void IpodDeviceScanner::Terminate() {
  LOG4CXX_TRACE_ENTER(logger_);
  shutdown_requested_ = true;

  if (true == thread_started_) {
    {
      sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
    device_scan_requested_ = false;
      device_scan_requested_cv_.NotifyOne();
    }
    LOG4CXX_INFO(logger_,
                 "Waiting for ipod device scanner thread termination");

    pthread_join(mPASAFWMsgHandlerThread, 0);

    pthread_join(thread_, 0);
    LOG4CXX_INFO(logger_, "ipod device scanner thread terminated");
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

TransportAdapter::Error IpodDeviceScanner::Scan() {
  LOG4CXX_TRACE_ENTER(logger_);

  if ((!thread_started_) || shutdown_requested_) {
    LOG4CXX_INFO(logger_, "bad state");
    return TransportAdapter::BAD_STATE;
  }

  TransportAdapter::Error ret = TransportAdapter::OK;

  {
    sync_primitives::AutoLock auto_lock(device_scan_requested_lock_);
  if (false == device_scan_requested_) {
    LOG4CXX_INFO(logger_, "Requesting device Scan");
    device_scan_requested_ = true;
      device_scan_requested_cv_.NotifyOne();
  } else {
    ret = TransportAdapter::BAD_STATE;
    LOG4CXX_INFO(logger_, "Device Scan is currently in progress");
  }
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return ret;
}

}  // namespace transport_adapter
}  // namespace transport_manager

