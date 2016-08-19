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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_INTERFACES_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_INTERFACES_IMPL_H_
#include <map>
#include "application_manager/hmi_interfaces.h"
#include "utils/macro.h"
#include "utils/lock.h"
/**
 * @brief The HmiInterfacesImpl class handles
 *  hmi interfaces states
 */
namespace application_manager {

class HmiInterfacesImpl : public HmiInterfaces {
 public:
  HmiInterfacesImpl();

  /**
   * @brief GetInterfaceState return currecnt state of hmi interface
   * @param interface to get state
   * @return state of interface
   */
  InterfaceState GetInterfaceState(InterfaceID interface) const OVERRIDE;

  /**
   * @brief SetInterfaceState set interface to some state
   * @param interface interface to set state
   * @param state to setup
   */
  void SetInterfaceState(InterfaceID interface, InterfaceState state) OVERRIDE;

  /**
   * @brief GetInterfaceFromFunction extract interface name fron function id
   * @param function to extract interface name
   * @return extracted interface name
   */
  InterfaceID GetInterfaceFromFunction(
      hmi_apis::FunctionID::eType function) const OVERRIDE;

 private:
  typedef std::map<InterfaceID, InterfaceState> InterfaceStatesMap;
  InterfaceStatesMap interfaces_states_;
  mutable sync_primitives::Lock interfaces_states_lock_;
};
}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_INTERFACES_IMPL_H_
