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
using policy::PolicyHandlerInterface;
typedef std::shared_ptr<commands::Command> CommandSharedPtr;

/**
 * @brief Factory class for command creation
 **/
class CommandFactory {
 public:
  /**
   * @brief Create command object and return pointer to it
   *
   * @param  message SmartObject shared pointer.
   * @return Pointer to created command object.
   **/
  virtual CommandSharedPtr CreateCommand(
      const commands::MessageSharedPtr& message,
      commands::Command::CommandSource source) = 0;
  /**
  * @param int32_t command id
  * @param CommandSource source
  * @return return true if command can be create, else return false
  **/
  virtual bool IsAbleToProcess(
      const int32_t,
      const application_manager::commands::Command::CommandSource source)
      const = 0;
};

/**
 * @brief Command creator interface for create commands
 **/
class CommandCreator {
 public:
  /**
   * @brief ~CommandCreator destructor
   **/
  virtual ~CommandCreator() {}
  /**
   * @return return true if command can be create, else return false
   **/
  virtual bool CanBeCreated() const = 0;
  /**
   * @brief Create command object and return pointer to it
   * @param  message SmartObject shared pointer.
   * @return Pointer to created command object.
   **/
  virtual CommandSharedPtr create(
      const commands::MessageSharedPtr& message) const = 0;
};

/**
 * @brief DefaultCommandCreator concrete command creator
 **/
template <typename CommandType>
class DefaultCommandCreator : public CommandCreator {
 public:
  /**
   * @brief DefaultCommandCreator constructor
   * @param  application_manager ApplicationManager.
   * @param  rpc_service RPCService.
   * @param  hmi_capabilities HMICapabilities.
   * @param  policy_handler PolicyHandlerInterface.
   */
  DefaultCommandCreator(ApplicationManager& application_manager,
                        rpc_service::RPCService& rpc_service,
                        HMICapabilities& hmi_capabilities,
                        PolicyHandlerInterface& policy_handler)
      : application_manager_(application_manager)
      , rpc_service_(rpc_service)
      , hmi_capabilities_(hmi_capabilities)
      , policy_handler_(policy_handler) {}

 private:
  /**
   * @return return true
   **/
  bool CanBeCreated() const override {
    return true;
  }

  /**
   * @brief Create command object and return pointer to it
   * @param  message SmartObject shared pointer.
   * @return Pointer to created command object.
   **/
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
  rpc_service::RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
};

struct InvalidCommand {};

/**
 * @brief DefaultCommandCreator<InvalidCommand> creator for invalid commands
 **/
template <>
class DefaultCommandCreator<InvalidCommand> : public CommandCreator {
 public:
  /**
   * @brief DefaultCommandCreator constructor
   * @param  application_manager ApplicationManager.
   * @param  rpc_service RPCService.
   * @param  hmi_capabilities HMICapabilities.
   * @param  policy_handler PolicyHandlerInterface.
   */
  DefaultCommandCreator(ApplicationManager& application_manager,
                        rpc_service::RPCService& rpc_service,
                        HMICapabilities& hmi_capabilities,
                        PolicyHandlerInterface& policy_handler) {
    UNUSED(application_manager);
    UNUSED(rpc_service);
    UNUSED(hmi_capabilities);
    UNUSED(policy_handler);
  }

 private:
  /**
   * @return return false
   **/
  bool CanBeCreated() const override {
    return false;
  }
  /**
   * @brief Create command object and return pointer to it
   * @param  message SmartObject shared pointer.
   * @return Pointer to created empty command object.
   **/
  CommandSharedPtr create(
      const commands::MessageSharedPtr& message) const override {
    UNUSED(message);
    return CommandSharedPtr();
  }
};

struct CommandCreatorFactory {
  CommandCreatorFactory(ApplicationManager& application_manager,
                        rpc_service::RPCService& rpc_service,
                        HMICapabilities& hmi_capabilities,
                        PolicyHandlerInterface& policy_handler)
      : application_manager_(application_manager)
      , rpc_service_(rpc_service)
      , hmi_capabilities_(hmi_capabilities)
      , policy_handler_(policy_handler) {}

  template <typename CommandType>
  CommandCreator& GetCreator() {
    static DefaultCommandCreator<CommandType> res(
        application_manager_, rpc_service_, hmi_capabilities_, policy_handler_);
    return res;
  }
  ApplicationManager& application_manager_;
  rpc_service::RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  PolicyHandlerInterface& policy_handler_;
};

}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMAND_FACTORY_H
