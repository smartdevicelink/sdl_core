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

namespace transport_manager {

class BluetoothAdapter;
class TCPAdapter;

log4cxx::LoggerPtr TransportManagerImpl::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger( "TransportManager"));

TransportManagerImpl::TransportManagerImpl():
		queue_mutex_(),
		process_queue_terminate_(false),
		device_adapters_(),
		messsage_queue_thread_()
{

    pthread_mutex_init(&queue_mutex_, 0);
    addDeviceAdapter(new BluetoothAdapter());
    addDeviceAdapter(new TCPAdapter());
}

TransportManagerImpl::~TransportManagerImpl(){

	pthread_mutex_destroy(&queue_mutex_);
	for (std::vector<DeviceAdapter*>::iterator device = device_adapters_.begin(); device != device_adapters_.end(); ++device){
		delete (*device);
	}
}

TransportManagerImpl* TransportManagerImpl::instance() {
  static TransportManagerImpl instance;
  return &instance;
}

void TransportManagerImpl::connectDevice(const int SessionID){

}
void TransportManagerImpl::disconnectDevice(const int SessionID){

}

void TransportManagerImpl::postMessage(const application_manager::Message message){
	pthread_mutex_lock(&queue_mutex_);
	queue_.push(message);
	pthread_mutex_unlock(&queue_mutex_);
}

void TransportManagerImpl::addEventListener(const int EventType, const int *(Callback)(int *Data)){

}

void TransportManagerImpl::set_error_handler(const int ErrorHandler){

}

void TransportManagerImpl::set_message_container(const int MessageContainer){

}

void TransportManagerImpl::set_data_transmitter(const int DataTransmitter){

}

void TransportManagerImpl::initialize(void){

	int error_code = pthread_create(&messsage_queue_thread_, 0, &processQueue, 0);

    if (0 == error_code) {
    }
    else {
    }
}

void *TransportManagerImpl::processQueue(void *){

	while(true == process_queue_terminate_){
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
		for(std::deque<application_manager::Message>::iterator msg = queue_.c.begin(); msg != queue_.c.end(); ++msg){
			if(msg.serial_number > serial_number){
				active_msg = msg;
				serial_number = msg.serial_number;
			}
		}
		ConnectionHandle *connection = getConnectionHandler(active_msg.session_id);
		connection->device_adapter->SendData(connection->device_handle, active_msg.data);

		pthread_mutex_lock(&queue_mutex_);
		queue_.remove(active_msg);
		pthread_mutex_unlock(&queue_mutex_);

	}//while(true)
	return 0;
}
void TransportManagerImpl::addDeviceAdapter(DeviceAdapter *device_adapter){
	device_adapters_.push_back(device_adapter);
}
}
