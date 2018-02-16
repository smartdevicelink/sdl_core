/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMAND_FACTORY_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMAND_FACTORY_H

#include "application_manager/commands/command.h"
#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "utils/macro.h"

namespace application_manager {
using rpc_service::RPCService;
using policy::PolicyHandlerInterface;
typedef utils::SharedPtr<commands::Command> CommandSharedPtr;

/**
 * @brief Factory class for command creation
 **/
class CommandFactory {
 public:
  /**
   * @brief Create command object and return pointer to it
   *
   * @param  smartObject SmartObject shared pointer.
   * @return Pointer to created command object.
   **/
  virtual CommandSharedPtr CreateCommand(
      const commands::MessageSharedPtr& message,
      commands::Command::CommandSource source) = 0;
  virtual bool IsAbleToProcess(
      const int32_t,
      const application_manager::commands::Command::CommandSource) const = 0;
};

class ICommandCreator {
 public:
  virtual ~ICommandCreator() {}
  virtual bool isAble() const = 0;
  virtual CommandSharedPtr create(
      const commands::MessageSharedPtr& message) const = 0;
};

template <typename CommandType>
class CommandCreator : public ICommandCreator {
 public:
  CommandCreator(ApplicationManager& application_manager,
                 RPCService& rpc_service,
                 HMICapabilities& hmi_capabilities,
                 PolicyHandlerInterface& policy_handler)
      : application_manager_(application_manager)
      , rpc_service_(rpc_service)
      , hmi_capabilities_(hmi_capabilities)
      , policy_handler_(policy_handler)
      , able_(true) {}

 private:
  bool isAble() const override;
  CommandSharedPtr create(
      const commands::MessageSharedPtr& message) const override {
    CommandSharedPtr command(new CommandType(message,
                                             application_manager_,
                                             rpc_service_,
                                             hmi_capabilities_,
                                             policy_handler_));
    return command;
  }

  ApplicationManager& application_manager_;
  RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
  bool able_;
};

struct InvalidCommand {};

template <>
class CommandCreator<InvalidCommand> : public ICommandCreator {
 public:
  CommandCreator(ApplicationManager& application_manager,
                 RPCService& rpc_service,
                 HMICapabilities& hmi_capabilities,
                 PolicyHandlerInterface& policy_handler)
      : application_manager_(application_manager)
      , rpc_service_(rpc_service)
      , hmi_capabilities_(hmi_capabilities)
      , policy_handler_(policy_handler)
      , able_(false) {}

 private:
  bool isAble() const override;
  CommandSharedPtr create(
      const commands::MessageSharedPtr& message) const override {
    UNUSED(message);
    return CommandSharedPtr();
  }

  ApplicationManager& application_manager_;
  RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
  bool able_;
};

struct CommandCreatorFacotry {
  CommandCreatorFacotry(ApplicationManager& application_manager,
                        rpc_service::RPCService& rpc_service,
                        HMICapabilities& hmi_capabilities,
                        PolicyHandlerInterface& policy_handler)
      : application_manager_(application_manager)
      , rpc_service_(rpc_service)
      , hmi_capabilities_(hmi_capabilities)
      , policy_handler_(policy_handler) {}

  template <typename CommandType>
  ICommandCreator& GetCreator() {
    static CommandCreator<CommandType> res(
        application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
    return res;
  }
  ApplicationManager& application_manager_;
  RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
};

}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMAND_FACTORY_H
