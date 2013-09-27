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

#include "application_manager/message_chaining.h"

namespace application_manager {

MessageChaining::MessageChaining(unsigned int connection_key,
                                 unsigned int correlation_id)
    : correlation_id_(correlation_id),
      connection_key_(connection_key),
      success_(true),
      counter_(1),
      data_(),
      ui_response_result_(hmi_apis::Common_Result::INVALID_ENUM),
      vr_response_result_(hmi_apis::Common_Result::INVALID_ENUM),
      tts_response_result_(hmi_apis::Common_Result::INVALID_ENUM) {
}

MessageChaining::~MessageChaining() {
}

bool MessageChaining::operator==(const MessageChaining& other) const {
  return ((correlation_id_ == other.correlation_id_)
      && (connection_key_ == other.connection_key_));
}

const unsigned int MessageChaining::correlation_id() const {
  return correlation_id_;
}

const unsigned int MessageChaining::connection_key() const {
  return connection_key_;
}

void MessageChaining::IncrementCounter() {
  ++counter_;
}

void MessageChaining::DecrementCounter() {
  --counter_;
}

int MessageChaining::counter() const {
  return counter_;
}

void MessageChaining::set_counter(const unsigned int& counter) {
  counter_ = counter;
}

void MessageChaining::set_data(const smart_objects::SmartObject& data) {
  data_ = data;
}

const smart_objects::SmartObject& MessageChaining::data() const {
  return data_;
}

void MessageChaining::set_vr_response_result(
    const hmi_apis::Common_Result::eType& result) {
  vr_response_result_ = result;
}

const hmi_apis::Common_Result::eType&
MessageChaining::vr_response_result() const {
  return vr_response_result_;
}

void MessageChaining::set_ui_response_result(
    const hmi_apis::Common_Result::eType& result) {
  ui_response_result_ = result;
}

const hmi_apis::Common_Result::eType&
MessageChaining::ui_response_result() const {
  return ui_response_result_;
}

void MessageChaining::set_tts_response_result(
    const hmi_apis::Common_Result::eType& result) {
  tts_response_result_ = result;
}

const hmi_apis::Common_Result::eType&
MessageChaining::tts_response_result() const {
  return tts_response_result_;
}

}  // namespace application_manager
