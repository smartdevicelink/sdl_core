/**
 * \file transport_manager_impl.cpp
 * \brief TransportManager class source file.
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

#include <queue>
#include <pthread.h>
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/device_adapter_listener_impl.h"

namespace transport_manager {

log4cxx::LoggerPtr TransportManagerImpl::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger( "TransportManager"));

TransportManagerImpl::TransportManagerImpl():
		queue_mutex_(),
		all_thread_active_(false),
		device_adapters_(),
		messsage_queue_thread_(),
		event_thread_(),
		event_thread_wakeup_(),
		device_adapter_listener_()
{

    pthread_mutex_init(&queue_mutex_, 0);
    pthread_mutex_init(&event_thread_mutex_, 0);
    pthread_cond_init(&event_thread_wakeup_, NULL);
    device_adapter_listener_ = new DeviceAdapterListenerImpl(this);
    DeviceAdapter *d = new BluetoothAdapter();
    d->Init(device_adapter_listener_, NULL, NULL);
    addDeviceAdapter(d);
    //addDeviceAdapter(new TCPAdapter());

}

TransportManagerImpl::TransportManagerImpl(DeviceAdapter *device_adapter):
		queue_mutex_(),
		all_thread_active_(false),
		device_adapters_(),
		messsage_queue_thread_(),
		event_thread_(),
		event_thread_wakeup_(),
		device_adapter_listener_()
{

    pthread_mutex_init(&queue_mutex_, 0);
    pthread_mutex_init(&event_thread_mutex_, 0);
    pthread_cond_init(&event_thread_wakeup_, NULL);
    device_adapter_listener_ = new DeviceAdapterListenerImpl();
    device_adapter->Init(device_adapter_listener_, NULL, NULL);
    addDeviceAdapter(device_adapter);
}

/*todo:
TransportManagerImpl::TransportManagerImpl(std::vector<DeviceAdapter *> device_adapter_list):
		queue_mutex_(),
		all_thread_active_(false),
		device_adapters_(),
		messsage_queue_thread_(),
		event_thread_(),
		event_thread_wakeup_(),
		device_adapter_listener_()
{

    pthread_mutex_init(&queue_mutex_, 0);
    pthread_mutex_init(&event_thread_mutex_, 0);
    pthread_cond_init(&event_thread_wakeup_, NULL);
    device_adapters_ = device_adapter_list;
}
*/

TransportManagerImpl::~TransportManagerImpl(){

	pthread_mutex_destroy(&queue_mutex_);
	pthread_mutex_destroy(&event_thread_mutex_);
	pthread_cond_destroy(&event_thread_wakeup_);
	for (std::vector<DeviceAdapter*>::iterator device = device_adapters_.begin(); device != device_adapters_.end(); ++device){
		delete (*device);
	}
	delete device_adapter_listener_;
}

TransportManagerImpl* TransportManagerImpl::instance() {
  static TransportManagerImpl instance;
  return &instance;
}

void TransportManagerImpl::connectDevice(const int SessionID){

}
void TransportManagerImpl::disconnectDevice(const int SessionID){

}

void TransportManagerImpl::postMessage(const protocol_handler::RawMessage message){
	pthread_mutex_lock(&queue_mutex_);
	queue_.push_back(message);
	pthread_mutex_unlock(&queue_mutex_);
}

void TransportManagerImpl::set_device_adapter_listener(DeviceAdapterListener *listener){
	device_adapter_listener_ = listener;
}

void TransportManagerImpl::initialize(void){

	int error_code = pthread_create(&messsage_queue_thread_, 0, &processQueue, 0);

    if (0 == error_code) {
    }
    else {
    	return;
    }

	int error_code = pthread_create(&event_thread_, 0, &deviceListener, 0);

    if (0 == error_code) {
    }
    else {
    	return;
    }
    all_thread_active_ = true;
}

void *TransportManagerImpl::deviceListener(void *){

	while(true == all_thread_active_){

		pthread_mutex_lock(&event_thread_mutex_);
		pthread_cond_wait(&event_thread_wakeup_, &event_thread_mutex_);
		pthread_mutex_unlock(&event_thread_mutex_);
	}


	return 0;
}
void *TransportManagerImpl::processQueue(void *){

	while(true == all_thread_active_){
		/*
		//get higher priority messages
		u_int priority = 0;

		todo: add priority processing
		 * std::queue highPriorityMsgs;
		for(std::deque<int>::iterator msg = queue_.c.begin(); msg != queue_.c.end(); ++msg){
			if(msg.priority >= priority){
				highPriorityMsgs.push(msg);
				priority = msg.priority;
			}
		}
		std::queue highSerialNumberMsgs;
		*/

		u_int serial_number = 0;
		protocol_handler::RawMessage active_msg;
		for(std::vector<protocol_handler::RawMessage>::iterator msg = queue_.begin(); msg != queue_.end(); ++msg){
			if((*msg).serial_number() > serial_number){
				active_msg = (*msg);
				serial_number = (*msg).serial_number();
			}
		}
		DataContainer dc = new DataContainer(active_msg.data(), active_msg.data_size());
		ConnectionHandle *connection = getConnectionHandler(active_msg.connection_key());
		connection->device_adapter->SendData(connection->device_handle, dc);

		pthread_mutex_lock(&queue_mutex_);
		queue_.erase(std::remove(queue_.begin(), queue_.end(), active_msg), queue_.end());
		pthread_mutex_unlock(&queue_mutex_);

	}//while(true)
	return 0;
}

void TransportManagerImpl::addDeviceAdapter(DeviceAdapter *device_adapter){
	device_adapters_.push_back(device_adapter);
}

void TransportManagerImpl::searchDevices(void) const {
	for(std::vector<DeviceAdapter *>::iterator da = device_adapters_.begin(); da != device_adapters_.end(); ++da){
		DeviceAdapter::Error err = (*da)->SearchDevices();
		if(DeviceAdapter::Error::OK == err){
			;
		}
	}

}

pthread_cond_t TransportManagerImpl::event_thread_wakeup(void){
	return event_thread_wakeup_;
}

}//namespace

