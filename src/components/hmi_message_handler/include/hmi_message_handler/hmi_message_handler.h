/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_HANDLER_H_
#define SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_HANDLER_H_

#include "hmi_message_handler/hmi_message_sender.h"
#include "hmi_message_handler/hmi_message_observer.h"
#include "hmi_message_handler/hmi_message_handler_settings.h"

namespace hmi_message_handler {

class HMIMessageAdapter;
/**
 * \class HMIMessageHandler
 * \brief Abstract class for handling different HMI adapters;
 * establishing interface for message exchange between SDL core and HMI.
 */
class HMIMessageHandler : public HMIMessageObserver, public HMIMessageSender {
 public:
  virtual ~HMIMessageHandler() {}
  virtual void AddHMIMessageAdapter(HMIMessageAdapter* adapter) = 0;
  virtual void RemoveHMIMessageAdapter(HMIMessageAdapter* adapter) = 0;

  /**
   * \brief Hmi message handler settings getter
   * \return pointer to hmi message handler settings class
   */
  virtual const HMIMessageHandlerSettings& get_settings() const = 0;
};

}  // namespace hmi_message_handler

#endif  // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_HANDLER_H_
