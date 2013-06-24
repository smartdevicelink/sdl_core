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
#include "transport_manager/transport_manager_listener.h"
#include "transport_manager/device_adapter_listener_impl.h"

namespace transport_manager {

log4cxx::LoggerPtr TransportManagerImpl::logger_ = log4cxx::LoggerPtr(
		log4cxx::Logger::getLogger("TransportManager"));

TransportManagerImpl::TransportManagerImpl() :
		messageQueue_mutex_(), all_thread_active_(false), device_adapters_(), messsage_queue_thread_(), device_listener_thread_(), device_listener_thread_wakeup_(), transport_manager_listener_(), device_adapter_listener_() {

	pthread_mutex_init(&messageQueue_mutex_, 0);
	pthread_mutex_init(&device_listener_thread_mutex_, 0);
	pthread_cond_init(&device_listener_thread_wakeup_, NULL);
	device_adapter_listener_ = new DeviceAdapterListenerImpl(this);
	DeviceAdapter *d = new BluetoothAdapter();
	d->init(device_adapter_listener_, NULL);
	addDeviceAdapter(d);
	//addDeviceAdapter(new TCPAdapter());

}

TransportManagerImpl::TransportManagerImpl(DeviceAdapter *device_adapter) :
		messageQueue_mutex_(), all_thread_active_(false), device_adapters_(), messsage_queue_thread_(), device_listener_thread_(), device_listener_thread_wakeup_(), transport_manager_listener_(), device_adapter_listener_() {

	pthread_mutex_init(&messageQueue_mutex_, 0);
	pthread_mutex_init(&device_listener_thread_mutex_, 0);
	pthread_cond_init(&device_listener_thread_wakeup_, NULL);
	device_adapter_listener_ = new DeviceAdapterListenerImpl();
	device_adapter->init(device_adapter_listener_, NULL);
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
 transport_manager_listener_(),
 device_adapter_listener_()
 {

 pthread_mutex_init(&queue_mutex_, 0);
 pthread_mutex_init(&event_thread_mutex_, 0);
 pthread_cond_init(&event_thread_wakeup_, NULL);
 device_adapters_ = device_adapter_list;
 }
 */

TransportManagerImpl::~TransportManagerImpl() {

	pthread_mutex_destroy(&messageQueue_mutex_);
	pthread_mutex_destroy(&device_listener_thread_mutex_);
	pthread_cond_destroy(&device_listener_thread_wakeup_);
	for (std::vector<DeviceAdapter*>::iterator device =
			device_adapters_.begin(); device != device_adapters_.end();
			++device) {
		delete (*device);
	}
	delete device_adapter_listener_;
}

TransportManagerImpl* TransportManagerImpl::instance() {
	static TransportManagerImpl instance;
	return &instance;
}

void TransportManagerImpl::init() {

}

void TransportManagerImpl::connectDevice(DeviceHandle device_id, ApplicationHandle app_id, SessionID session_id) {
	//find adapter
	device_adapter->connect(device_id, app_id, session_id);
}

void TransportManagerImpl::disconnectDevice(const SessionID session_id) {
	getDeviceAdapter(session_id)->disconnect(session_id);
}

void TransportManagerImpl::registerEventListener(
		TransportManagerListener *listener) {
	this->set_transport_manager_listener(listener);
}

void TransportManagerImpl::registerAdapterListener(
		DeviceAdapterListener *listener) {
	this->set_device_adapter_listener(listener);
}

void TransportManagerImpl::sendMessageToDevice(const void *message) {
	protocol_handler::RawMessage &msg =
			static_cast<protocol_handler::RawMessage>(*message);

	this->postMessage(msg);
}

void TransportManagerImpl::receiveEventFromDevice(const void *event) {
	transport_manager::DeviceAdapterListenerImpl::DeviceAdapterEvent &evt =
			static_cast<transport_manager::DeviceAdapterListenerImpl::DeviceAdapterEvent>(*event);
	this->postEvent(evt);
}

void TransportManagerImpl::postMessage(
		const protocol_handler::RawMessage &message) {
	//todo: check data copying
	protocol_handler::RawMessage msg(message.connection_key(),
			msg.protocol_version(), msg.data(), msg.data_size(),
			msg.serial_number());
	pthread_mutex_lock(&messageQueue_mutex_);
	messageQueue_.push_back(msg);
	pthread_mutex_unlock(&messageQueue_mutex_);
}

void TransportManagerImpl::updateMessage(
		const protocol_handler::RawMessage &message) {
	pthread_mutex_lock(&messageQueue_mutex_);
	pthread_mutex_unlock(&messageQueue_mutex_);

}

void TransportManagerImpl::removeMessage(
		const protocol_handler::RawMessage *message) {
	pthread_mutex_lock(&messageQueue_mutex_);
	messageQueue_.erase(
			std::remove(messageQueue_.begin(), messageQueue_.end(), message),
			messageQueue_.end());
	pthread_mutex_unlock(&messageQueue_mutex_);
}

void TransportManagerImpl::postEvent(
		const DeviceAdapterListenerImpl::DeviceAdapterEvent &event) {
	//todo: check data copying
	DeviceAdapterListenerImpl::DeviceAdapterEvent evt(event.event_type(),
			event.session_id(), event.device_adapter(), event.data(),
			DeviceAdapterError(event.error()));
	pthread_mutex_lock(&eventQueue_mutex_);
	eventQueue_.push_back(evt);
	pthread_mutex_unlock(&eventQueue_mutex_);
}

void TransportManagerImpl::set_device_adapter_listener(
		DeviceAdapterListener *listener) {
	device_adapter_listener_ = listener;
}

void TransportManagerImpl::set_transport_manager_listener(
		TransportManagerListener *listener) {
	transport_manager_listener_ = listener;
}

void TransportManagerImpl::initialize(void) {

	int error_code = pthread_create(&messsage_queue_thread_, 0,
			&messageQueueThread, 0);

	if (0 == error_code) {
	} else {
		return;
	}

	int error_code = pthread_create(&device_listener_thread_, 0,
			&deviceListenerThread, 0);

	if (0 == error_code) {
	} else {
		return;
	}
	all_thread_active_ = true;
}

void *TransportManagerImpl::deviceListenerThread(void *) {

	while (true == all_thread_active_) {

		pthread_mutex_lock(&device_listener_thread_mutex_);
		pthread_cond_wait(&device_listener_thread_wakeup_,
				&device_listener_thread_mutex_);

		for (std::vector<DeviceAdapterListenerImpl::DeviceAdapterEvent>::iterator evt =
				eventQueue_.begin(); evt != eventQueue_.end(); ++evt) {
			switch ((*evt).event_type()) {
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE:
				//todo: event must contain list of adapters, for each adapter - list of devices and for each device - list of apps
				transport_manager_listener_->onSearchDeviceDone();
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL:
				//error happened in real search process (external error)
				transport_manager_listener_->onSearchDeviceFailed(
						(*evt).device_adapter(), (*evt).error());
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE:
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL:
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE:
				this->removeConnection(this->getDeviceAdapter((*evt).session_id()));
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL:
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE:
				protocol_handler::RawMessage *msg = *((*evt).data());
				this->removeMessage(msg);
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL:
				//todo: start timer here to wait before notify caller and remove unsent messages
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE:
				transport_manager_listener_->onDataReceiveDone(
						(*evt).device_adapter(), (*evt).session_id(),
						(*evt).data());
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL:
				transport_manager_listener_->onSearchDeviceDone(NULL);
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR:
				break;
			}
			eventQueue_.erase(
					std::remove(eventQueue_.begin(), eventQueue_.end(), evt),
					eventQueue_.end());
		}
		pthread_mutex_unlock(&device_listener_thread_mutex_);

	}

	return 0;
}
void *TransportManagerImpl::messageQueueThread(void *) {

	while (true == all_thread_active_) {
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
		for (std::vector<protocol_handler::RawMessage>::iterator msg =
				messageQueue_.begin(); msg != messageQueue_.end(); ++msg) {
			if ((*msg).serial_number() > serial_number) {
				active_msg = (*msg);
				serial_number = (*msg).serial_number();
			}
		}
		//todo add serial number to raw msg constructor
		RawMessageSptr msg_to_send(active_msg.connection_key(),
				active_msg.protocol_version(), active_msg.data(),
				active_msg.data_size());

		DeviceAdapter *device_adapter = getDeviceAdapter(
				active_msg.connection_key());

		if (NULL == device_adapter) {
			//probably error no device adapters found
		} else {
			device_adapter->sendData(active_msg.connection_key(), msg_to_send);
		}

	} //while(true)
	return 0;
}

DeviceAdapter *TransportManagerImpl::getDeviceAdapter(SessionID session_id) {
	std::map<SessionID, DeviceAdapter *>::iterator da =
			session_to_device_map_.find(session_id);
	if (da != session_to_device_map_.begin()) {
		return da;
	}
	return NULL;
}

void TransportManagerImpl::addDeviceAdapter(DeviceAdapter *device_adapter) {
	device_adapters_.push_back(device_adapter);
}

void TransportManagerImpl::searchDevices(void) const {
	for (std::vector<DeviceAdapter *>::iterator da = device_adapters_.begin();
			da != device_adapters_.end(); ++da) {
		DeviceAdapter::Error err = (*da)->searchDevices();
		if (DeviceAdapter::Error::OK != err) {
			//some error happened during call to this function. This it not error during real search process
			//null as error class means that it is internal error in DA.
			transport_manager_listener_->onSearchDeviceFailed((*da), NULL);
		}
	}

}

pthread_cond_t TransportManagerImpl::device_listener_thread_wakeup(void) {
	return device_listener_thread_wakeup_;
}

} //namespace

