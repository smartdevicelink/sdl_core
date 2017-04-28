/*
 * Copyright (c) 2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_ADAPTER_H_
#define SRC_COMPONENTS_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_ADAPTER_H_

#include "hmi_message_handler/hmi_message_sender.h"

namespace hmi_message_handler {
/**
 * \class HMIMessageAdapter
 * \brief Interface class describing methods neccessary for exchanging message
 * between ApplicationManager and HMI. Adapter for concrete transport connection
 * SDL with HMI has to implement this interface.
 */
class HMIMessageAdapter : public HMIMessageSender {
 protected:
  /**
   * \brief Interface for subscriptions.
   * Each class implementing interface should use it according to
   * standarts of transport for which it is to be an adapter.
   * For example, Adapter for MessageBroker will use it to subscribe to
   * notifications
   * from HMI.
   */
  virtual void SubscribeTo() = 0;
};

}  // namespace hmi_message_handler

#endif  // SRC_COMPONENTS_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_ADAPTER_H_
