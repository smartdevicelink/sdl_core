/**
 * \file transport_adapter_event.cc
 * \brief TransportAdapterEvent class source file.
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
#include "transport_manager/transport_adapter/transport_adapter_event.h"

namespace transport_manager {

TransportAdapterEvent::TransportAdapterEvent(int type, TransportAdapterSptr transport_adapter,
                                       const DeviceUID &device_handle,
                                       const ApplicationHandle& application_id,
                                       RawMessageSptr data, BaseError *error)
    : event_type_(type),
      application_id_(application_id),
      transport_adapter_(transport_adapter),
      device_uid_(device_handle),
      event_data_(data) {
  set_error(error);
}

TransportAdapterEvent::~TransportAdapterEvent() {
}

void TransportAdapterEvent::set_event_type(int type) {
  event_type_ = type;
}

void TransportAdapterEvent::set_transport_adapter(
    transport_adapter::TransportAdapterSptr transport_adapter) {
  transport_adapter_ = transport_adapter;
}
void TransportAdapterEvent::set_data(RawMessageSptr data) {
  event_data_ = data;
}

void TransportAdapterEvent::set_error(BaseError *error) {
  event_error_ = error;
}

int TransportAdapterEvent::event_type(void) const {
  return event_type_;
}

ApplicationHandle TransportAdapterEvent::application_id(void) const {
  return application_id_;
}

TransportAdapterSptr TransportAdapterEvent::transport_adapter(void) const {
  return transport_adapter_;
}

RawMessageSptr TransportAdapterEvent::data(void) const {
  return event_data_;
}

const DeviceUID &TransportAdapterEvent::device_uid() const {
  return device_uid_;
}

BaseError *TransportAdapterEvent::event_error(void) const {
  return event_error_;
}

bool TransportAdapterEvent::operator ==(const TransportAdapterEvent &other) {
  if (this->event_type_ == other.event_type_
      && this->application_id_ == other.application_id_
      && this->transport_adapter_ == other.transport_adapter_) {

    if (!this->event_data_.valid() && !other.event_data_.valid())
      return true;

    if (this->event_data_.valid() && other.event_data_.valid()
        && this->event_data_.get()
            == other.event_data_.get()) {
      return true;
    }
    return false;
  } else {
    return false;
  }
}

}  // namespace transport_manager

