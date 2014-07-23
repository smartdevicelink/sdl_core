/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_OBSERVER_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_OBSERVER_H_

#include "protocol/common.h"

/**
 *\namespace protocol_handlerHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {

/**
 * \class IProtocolObserver
 * \brief Interface for making a bridge between
 * ProtocolHandler and JSONHandler components.
 */
class ProtocolObserver {
 public:
  /**
   * \brief Callback function which is used by ProtocolHandler
   * when new message is received from Mobile Application.
   * \param message Message with supporting params received
   */
  virtual void OnMessageReceived(const RawMessagePtr message) = 0;

  virtual void OnMobileMessageSent(const RawMessagePtr message)  = 0;
 protected:
  /**
   * \brief Destructor
   */
  virtual ~ProtocolObserver() {
  }
};
}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_OBSERVER_H_
