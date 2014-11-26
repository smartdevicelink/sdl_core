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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_H_

#include "functional_module/generic_module.h"
#include "can_cooperation/can_connection.h"
#include "utils/threads/message_loop_thread.h"


namespace threads {
    class Thread;
}

namespace can_cooperation {
class CANModule : public functional_modules::GenericModule,
	public utils::Singleton<CANModule>,
    public threads::MessageLoopThread<std::queue<MessageFromCAN>>::Handler,
    public threads::MessageLoopThread<std::queue<std::string>>::Handler {
  public:
    ~CANModule();
    functional_modules::PluginInfo GetPluginInfo() const;
    virtual functional_modules::ProcessResult ProcessMessage(
        application_manager::MessagePtr msg);
    virtual functional_modules::ProcessResult ProcessHMIMessage(
        application_manager::MessagePtr msg);
    void ProcessCANMessage(const MessageFromCAN& can_msg);
    void Handle(const std::string message);
    void Handle(const MessageFromCAN message);

    /**
     * @brief Remove extension created for specified application
     * @param app_id application id
     */
    virtual void RemoveAppExtension(uint32_t app_id);
protected:
    /**
     * @brief Remove extension for all applications
     */
    virtual void RemoveAppExtensions();
private:
 DISALLOW_COPY_AND_ASSIGN(CANModule);
 FRIEND_BASE_SINGLETON_CLASS(CANModule);
 CANModule();

 void SubscribeOnFunctions() ;

 struct HMIResponseSubscriberInfo {
   int32_t connection_key_;
   int32_t correlation_id_;
   int32_t function_id_;
 };

 static const functional_modules::ModuleID kCANModuleID = 153;
 static uint32_t next_correlation_id_;
 std::map<uint32_t, HMIResponseSubscriberInfo> hmi_response_subscribers_;
 utils::SharedPtr<CANConnection> can_connection;
 functional_modules::PluginInfo plugin_info_;
 threads::MessageLoopThread<std::queue<MessageFromCAN>> from_can_;
 threads::MessageLoopThread<std::queue<std::string>> from_mobile_;
 threads::Thread* thread_;
 friend class TCPClientDelegate;
};

EXPORT_FUNCTION(CANModule);

}

#endif  //  SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_MODULE_H_
