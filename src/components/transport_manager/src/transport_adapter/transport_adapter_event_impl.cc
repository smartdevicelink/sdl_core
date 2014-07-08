/*
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

#include "transport_manager/error.h"
#include "transport_manager/transport_adapter/transport_adapter_event_impl.h"

namespace transport_manager {

TransportAdapterEventImpl::TransportAdapterEventImpl(
  int type, transport_adapter::TransportAdapter* transport_adapter,
  const DeviceUID& device_handle, const ApplicationHandle& application_id,
  RawMessagePtr data, BaseError* error)
  : event_type_(type),
    application_id_(application_id),
    device_uid_(device_handle),
    transport_adapter_(transport_adapter),
    event_data_(data) {
  set_error(error);
}

void TransportAdapterEventImpl::set_event_type(int type) {
  event_type_ = type;
}

void TransportAdapterEventImpl::set_application_id(const ApplicationHandle id) {
  application_id_ = id;
}

void TransportAdapterEventImpl::set_transport_adapter(
  transport_adapter::TransportAdapter* transport_adapter) {
  transport_adapter_ = transport_adapter;
}
void TransportAdapterEventImpl::set_data(RawMessagePtr message) {
  event_data_ = message;
}

void TransportAdapterEventImpl::set_error(BaseError* error) {
  event_error_ = error;
}

void TransportAdapterEventImpl::set_device_handle(const DeviceUID& device_handle) {
  device_uid_ = device_handle;
}

int TransportAdapterEventImpl::event_type() const {
  return event_type_;
}

ApplicationHandle TransportAdapterEventImpl::application_id() const {
  return application_id_;
}

transport_adapter::TransportAdapter* TransportAdapterEventImpl::transport_adapter() const {
  return transport_adapter_;
}

RawMessagePtr TransportAdapterEventImpl::data() const {
  return event_data_;
}

const DeviceUID& TransportAdapterEventImpl::device_uid() const {
  return device_uid_;
}

BaseError* TransportAdapterEventImpl::event_error() const {
  return event_error_;
}

}  // namespace transport_manager
