/**
 * \file device_adapter_listener_impl.cc
 * \brief Listener class source file.
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

#include <pthread.h>
#include "transport_manager/device_adapter.h"
#include "transport_manager/device_adapter_listener.h"
#include "transport_manager/transport_manager_impl.h"

namespace transport_manager {

log4cxx::LoggerPtr DeviceAdapterListenerImpl::logger_ = log4cxx::LoggerPtr(
		log4cxx::Logger::getLogger("DeviceAdapterListener"));

DeviceAdapterListenerImpl::DeviceAdapterEvent::DeviceAdapterEvent(int type,
		transport_manager::SessionID session_id, DeviceAdapter *device_adapter, RawMessageSptr *data,
		DeviceAdapterError &error) :
		event_type_(type), session_id_(session_id), device_adapter_(
				device_adapter), data_(data), error_(error) {

}

DeviceAdapterListenerImpl::DeviceAdapterEvent::~DeviceAdapterEvent() {

}

void DeviceAdapterListenerImpl::DeviceAdapterEvent::set_event_type(int type) {
	event_type_ = type;
}

void DeviceAdapterListenerImpl::DeviceAdapterEvent::set_session_id(transport_manager::SessionID id) {
	session_id_ = id;
}

void DeviceAdapterListenerImpl::DeviceAdapterEvent::set_device_adapter(
		DeviceAdapter *device_adapter) {
	device_adapter_ = device_adapter;
}
void DeviceAdapterListenerImpl::DeviceAdapterEvent::set_data(
		RawMessageSptr *data) {
	data_ = data;
}

void DeviceAdapterListenerImpl::DeviceAdapterEvent::set_error(
		DeviceAdapterError *error) {
	error_ = error;
}

int DeviceAdapterListenerImpl::DeviceAdapterEvent::event_type(void) const {
	return event_type_;
}

transport_manager::SessionID DeviceAdapterListenerImpl::DeviceAdapterEvent::session_id(void) const {
	return session_id_;
}

DeviceAdapter *DeviceAdapterListenerImpl::DeviceAdapterEvent::device_adapter(
		void) const {
	return device_adapter_;
}

RawMessageSptr *DeviceAdapterListenerImpl::DeviceAdapterEvent::data(
		void) const {
	return data_;
}

DeviceAdapterError *DeviceAdapterListenerImpl::DeviceAdapterEvent::error(
		void) const {
	return error_;
}

DeviceAdapterListenerImpl::DeviceAdapterListenerImpl(
		transport_manager::TransportManagerImpl *tm) {
	transport_manager_impl_ = tm;
}

void DeviceAdapterListenerImpl::onSearchDeviceDone(
		const DeviceAdapter *device_adapter) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE, 0,
			device_adapter, NULL, NULL);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onSearchDeviceFailed(
		const DeviceAdapter* device_adapter, const SearchDeviceError& error) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL, 0,
			device_adapter, NULL, error);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onConnectDone(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE,
			session_id, device_adapter, NULL, NULL);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onConnectFailed(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id,
		const ConnectError& error) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL,
			session_id, device_adapter, NULL, error);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onDisconnectDone(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE,
			session_id, device_adapter, NULL, NULL);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onDisconnectFailed(const DeviceAdapter* device_adapter,
                                  const SessionID session_id,
                                  const DisconnectError& error){
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL,
			session_id, device_adapter, NULL, error);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onDisconnectDeviceDone(
		const DeviceAdapter* device_adapter, const SessionID session_id) {

}

void DeviceAdapterListenerImpl::onDisconnectDeviceFailed(
		const DeviceAdapter* device_adapter, const SessionID session_id,
		const DisconnectDeviceError& error) {

}

void DeviceAdapterListenerImpl::onDataReceiveDone(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id,
		const RawMessageSptr data_container) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
			session_id, device_adapter, data_container, NULL);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onDataReceiveFailed(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id,
		const DataReceiveError& error) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE,
			session_id, device_adapter, NULL, error);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onDataSendDone(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id,
		const RawMessageSptr data_container) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE, session_id,
			device_adapter, data_container, NULL);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onDataSendFailed(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id,
		const DataSendError& error) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL, session_id,
			device_adapter, NULL, error);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

void DeviceAdapterListenerImpl::onCommunicationError(
		const DeviceAdapter* device_adapter, const transport_manager::SessionID session_id) {
	DeviceAdapterListenerImpl::DeviceAdapterEvent event(
			DeviceAdapterListenerImpl::EventTypeEnum::ON_COMMUNICATION_ERROR,
			session_id, device_adapter, NULL, NULL);

	transport_manager_impl_->receiveEventFromDevice(&event);

	pthread_cond_signal(
			&(transport_manager_impl_->device_listener_thread_wakeup()));
}

} //namespace
