/**
* \file signals.cc
* \brief Signal (i.e. SIGINT) handling.
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

#ifndef SRC_APPMAIN_LIFE_CYCLE_H_
#define SRC_APPMAIN_LIFE_CYCLE_H_

#include "hmi_message_handler/hmi_message_handler_impl.h"
#ifdef DBUS_HMIADAPTER
#  include "hmi_message_handler/dbus_message_adapter.h"
#endif  // DBUS_HMIADAPTER
#ifdef MESSAGEBROKER_HMIADAPTER
#  include "hmi_message_handler/messagebroker_adapter.h"
#endif  // MESSAGEBROKER_HMIADAPTER
#ifdef MQUEUE_HMIADAPTER
#  include "hmi_message_handler/mqueue_adapter.h"
#endif  // MQUEUE_HMIADAPTER
#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_default.h"
#include "media_manager/media_manager_impl.h"
#include "policies/policy_manager_impl.h"
#include "utils/singleton.h"

#ifdef MESSAGEBROKER_HMIADAPTER
#  include "CMessageBroker.hpp"
#  include "mb_tcpserver.hpp"
#  include "networking.h"  // cpplint: Include the directory when naming .h files
#endif  // MESSAGEBROKER_HMIADAPTER
#include "system.h"      // cpplint: Include the directory when naming .h files

namespace main_namespace {
class LifeCycle : public utils::Singleton<LifeCycle> {
  public:
    bool StartComponents();

    /**
    * Initialize MessageBroker component
    * @return true if success otherwise false.
    */
    bool InitMessageSystem();
    void StopComponents();
    static void StopComponentsOnSignal(int32_t params);

  private:
    LifeCycle();
    transport_manager::TransportManager* transport_manager_;
    protocol_handler::ProtocolHandlerImpl* protocol_handler_;
    connection_handler::ConnectionHandlerImpl* connection_handler_;
    application_manager::ApplicationManagerImpl* app_manager_;
    hmi_message_handler::HMIMessageHandlerImpl* hmi_handler_;
#ifdef DBUS_HMIADAPTER
    hmi_message_handler::DBusMessageAdapter* dbus_adapter_;
#endif  // DBUS_HMIADAPTER
#ifdef MESSAGEBROKER_HMIADAPTER
    hmi_message_handler::MessageBrokerAdapter* mb_adapter_;
#endif  // MESSAGEBROKER_HMIADAPTER
    hmi_message_handler::HMIMessageAdapter* hmi_message_adapter_;
    media_manager::MediaManagerImpl* media_manager_;
    policies::PolicyManagerImpl* policy_manager_;

#ifdef MESSAGEBROKER_HMIADAPTER
    NsMessageBroker::CMessageBroker* message_broker_;
    NsMessageBroker::TcpServer* message_broker_server_;
    System::Thread* mb_thread_;
    System::Thread* mb_server_thread_;
    System::Thread* mb_adapter_thread_;
#endif  // MESSAGEBROKER_HMIADAPTER
#ifdef DBUS_HMIADAPTER
    System::Thread* dbus_adapter_thread_;
#endif  // DBUS_HMIADAPTER

#   ifdef ENABLE_LOG
    static log4cxx::LoggerPtr logger_;
#   endif // ENABLE_LOG

    DISALLOW_COPY_AND_ASSIGN(LifeCycle);

    FRIEND_BASE_SINGLETON_CLASS(LifeCycle);
};
}  //  namespace main_namespace

#endif  //  SRC_APPMAIN_LIFE_CYCLE_H_
