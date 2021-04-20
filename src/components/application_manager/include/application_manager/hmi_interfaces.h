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

/**
 * @brief The class contains information about state HMI's interfaces
 * (Buttons, BasicCommunication, VR, TTS, UI, Navigation,VehicleInfo,
 *  SDL) and provides this information through public interfaces.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_INTERFACES_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_INTERFACES_H_

#include "interfaces/HMI_API.h"

namespace application_manager {

/**
 * @brief The class contains information about state HMI's interfaces
 * (Buttons, BasicCommunication, VR, TTS, UI, Navigation,VehicleInfo,
 *  SDL) and provides this information through public interfaces.
 */
class HmiInterfaces {
 public:
  enum InterfaceID {
    HMI_INTERFACE_INVALID_ENUM,
    HMI_INTERFACE_Buttons,
    HMI_INTERFACE_BasicCommunication,
    HMI_INTERFACE_VR,
    HMI_INTERFACE_TTS,
    HMI_INTERFACE_UI,
    HMI_INTERFACE_Navigation,
    HMI_INTERFACE_VehicleInfo,
    HMI_INTERFACE_SDL,
    HMI_INTERFACE_RC,
    HMI_INTERFACE_AppService
  };

  /**
   * @brief The InterfaceState enum handle possible states of HMI interfaces
   * STATE_NOT_RESPONSE - HMI didn't not responsed IsReady on Inerface
   * STATE_AVAILABLE - Hmi responsed IsReady(avaliable = true)
   * STATE_NOT_AVAILABLE - Hmi responsed IsReady(avaliable = false)
   */
  enum InterfaceState {
    STATE_NOT_RESPONSE,
    STATE_AVAILABLE,
    STATE_NOT_AVAILABLE
  };

  /**
   * @brief GetInterfaceState return currecnt state of hmi interface
   * @param interface to get state
   * @return state of interface
   */
  virtual InterfaceState GetInterfaceState(InterfaceID interface) const = 0;

  /**
   * @brief SetInterfaceState set interface to some state
   * @param interface interface to set state
   * @param state to setup
   */
  virtual InterfaceID GetInterfaceFromFunction(
      hmi_apis::FunctionID::eType function) const = 0;

  /**
   * @brief GetInterfaceFromFunction extract interface name fron function id
   * @param function to extract interface name
   * @return extracted interface name
   */
  virtual void SetInterfaceState(InterfaceID interface,
                                 InterfaceState state) = 0;
  virtual ~HmiInterfaces() {}
};
}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_INTERFACES_H_
