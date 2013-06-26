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
		messageQueue_mutex_(), all_thread_active_(false), adapter_handler_(), messsage_queue_thread_(), device_listener_thread_(), device_listener_thread_wakeup_(), transport_manager_listener_(), device_adapter_listener_() {

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
		messageQueue_mutex_(), all_thread_active_(false), adapter_handler_(), messsage_queue_thread_(), device_listener_thread_(), device_listener_thread_wakeup_(), transport_manager_listener_(), device_adapter_listener_() {

	pthread_mutex_init(&messageQueue_mutex_, 0);
	pthread_mutex_init(&device_listener_thread_mutex_, 0);
	pthread_cond_init(&device_listener_thread_wakeup_, NULL);
	device_adapter_listener_ = new DeviceAdapterListenerImpl();
	device_adapter->init(device_adapter_listener_, NULL);
	addDeviceAdapter(device_adapter);
}

//todo: more constructors

TransportManagerImpl::~TransportManagerImpl() {

	pthread_mutex_destroy(&messageQueue_mutex_);
	pthread_mutex_destroy(&device_listener_thread_mutex_);
	pthread_cond_destroy(&device_listener_thread_wakeup_);
	delete device_adapter_listener_;
}

TransportManagerImpl* TransportManagerImpl::instance() {
	static TransportManagerImpl instance;
	return &instance;
}

void TransportManagerImpl::init() {

}

void TransportManagerImpl::connectDevice(DeviceHandle device_id,
		ApplicationHandle app_id, SessionID session_id) {
	DeviceAdapter *da = adapter_handler_.getAdapterByDevice(device_id);
	if (NULL == da) {
		//error case
		return;
	}
	if (DeviceAdapter::Error::OK
			!= da->connect(device_id, app_id, session_id)) {
		//error case
		return;
	}
}

void TransportManagerImpl::disconnectDevice(const SessionID session_id) {
	adapter_handler_.getAdapterBySession(session_id)->disconnect(session_id);
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
void TransportManagerImpl::removeDevice(DeviceHandle device) {
	adapter_handler_.removeDevice(device);
}

void TransportManagerImpl::addDeviceAdapter(DeviceAdapter *device_adapter) {
	adapter_handler_.addAdapter(device_adapter);
}

void TransportManagerImpl::searchDevices(void) {
	for (AdapterHandler::AdapterList::iterator da =
			adapter_handler_.device_adapters().begin();
			da != adapter_handler_.device_adapters().end(); ++da) {
		DeviceAdapter::Error err = (*da)->searchDevices();
		if (DeviceAdapter::Error::OK != err) {
			//todo: notify error
		}
	}
}

void TransportManagerImpl::init(void) {

	int error_code = pthread_create(&messsage_queue_thread_, 0,
			&messageQueueThread, 0);

	if (0 == error_code) {
	} else {
		return;
	}

	int error_code = pthread_create(&device_listener_thread_, 0,
			&eventListenerThread, 0);

	if (0 == error_code) {
	} else {
		return;
	}
	all_thread_active_ = true;
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

void *TransportManagerImpl::eventListenerThread(void *) {

	while (true == all_thread_active_) {

		pthread_mutex_lock(&device_listener_thread_mutex_);
		pthread_cond_wait(&device_listener_thread_wakeup_,
				&device_listener_thread_mutex_);

		for (std::vector<DeviceAdapterListenerImpl::DeviceAdapterEvent>::iterator evt =
				eventQueue_.begin(); evt != eventQueue_.end(); ++evt) {

			const DeviceAdapter *da = (*evt).device_adapter();
			const SessionID sid = (*evt).session_id;

			switch ((*evt).event_type()) {
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE:
				DeviceAdapter *da = (*evt).device_adapter();
				DeviceList list = da->getDeviceList();
				for (DeviceList::iterator item = list.begin();
						item != list.end(); ++item) {
					adapter_handler_.addDevice(da, (*item));
					transport_manager_listener_->onSearchDeviceDone((*item),
							da->getApplicationList((*item)));
				}
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL:
				//error happened in real search process (external error)
				const SearchDeviceError err = (*evt).error();
				transport_manager_listener_->onSearchDeviceFailed(da, err);
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE:
				adapter_handler_.addSession((*evt).device_adapter(),
						(*evt).session_id());
				transport_manager_listener_->onConnectDone(da, sid);
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL:
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE:
				adapter_handler_.removeSession((*evt).device_adapter(),
						(*evt).session_id());
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
				const RawMessageSptr data = (*evt).data();
				transport_manager_listener_->onDataReceiveDone(da, sid, data);
				break;
			case DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL:
				const DataReceiveError d_err = (*evt).error();
				transport_manager_listener_->onDataReceiveFailed(da, sid,
						d_err);
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
		//todo: add priority processing

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

		DeviceAdapter *device_adapter = adapter_handler_.getAdapterBySession(
				active_msg.connection_key());

		if (NULL == device_adapter) {
			//probably error no device adapters found
		} else {
			device_adapter->sendData(active_msg.connection_key(), msg_to_send);
		}

	} //while(true)
	return 0;
}

pthread_cond_t TransportManagerImpl::device_listener_thread_wakeup(void) {
	return device_listener_thread_wakeup_;
}

DeviceAdapter *TransportManagerImpl::AdapterHandler::getAdapterBySession(
		SessionID session_id) {
	std::map<SessionID, DeviceAdapter *>::iterator da =
			session_to_adapter_map_.find(session_id);
	if (da != session_to_adapter_map_.begin()) {
		return da;
	}
	return NULL;
}

DeviceAdapter *TransportManagerImpl::AdapterHandler::getAdapterByDevice(
		DeviceHandle device_id) {
	std::map<DeviceHandle, DeviceAdapter *>::iterator da =
			device_to_adapter_multimap_.find(device_id);
	if (da != device_to_adapter_multimap_.end()) {
		return da;
	}
	return NULL;
}
void TransportManagerImpl::AdapterHandler::addAdapter(
		DeviceAdapter *device_adapter) {
	device_adapters_.push_back(device_adapter);
}

TransportManagerImpl::AdapterHandler::AdapterList TransportManagerImpl::AdapterHandler::device_adapters(
		void) {
	return device_adapters_;
}
void TransportManagerImpl::AdapterHandler::addSession(DeviceAdapter *da,
		SessionID sid) {
	AdapterList::iterator item = std::find(device_adapters_.begin(),
			device_adapters_.end(), da);
	if (item == device_adapters_.end()) {
		//todo: error case: unknown adapter
		return;
	}
	session_to_adapter_map_.insert(std::make_pair(sid, da));
}

void TransportManagerImpl::AdapterHandler::removeSession(DeviceAdapter *da,
		SessionID sid) {
	session_to_adapter_map_.erase(sid);
}

void TransportManagerImpl::AdapterHandler::addDevice(DeviceAdapter *da,
		DeviceHandle did) {
	AdapterList::iterator item = std::find(device_adapters_.begin(),
			device_adapters_.end(), da);
	if (item == device_adapters_.end()) {
		//todo: error case: unknown adapter
		return;
	}
	device_to_adapter_multimap_.insert(std::make_pair(did, da));
}

void TransportManagerImpl::AdapterHandler::removeDevice(DeviceHandle device) {
	device_to_adapter_multimap_.erase(device);
}

} //namespace

