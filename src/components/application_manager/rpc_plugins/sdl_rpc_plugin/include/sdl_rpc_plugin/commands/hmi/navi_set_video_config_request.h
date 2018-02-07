/*
 * Copyright (c) 2017 Xevo Inc.
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
 * Neither the names of the copyright holders nor the names of its contributors
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SET_VIDEO_CONFIG_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SET_VIDEO_CONFIG_REQUEST_H_

#include "sdl_rpc_plugin/commands/hmi/request_to_hmi.h"

namespace application_manager {

namespace commands {

/**
 * @brief NaviSetVideoConfigRequest command class
 **/
class NaviSetVideoConfigRequest : public RequestToHMI,
                                  public event_engine::EventObserver {
 public:
  /**
   * @brief NaviSetVideoConfigRequest class constructor
   *
   * @param message Incoming SmartObject message
   * @param application_manager Reference of application manager
   **/
  NaviSetVideoConfigRequest(const MessageSharedPtr& message,
                            ApplicationManager& application_manager);

  /**
   * @brief NaviSetVideoConfigRequest class destructor
   **/
  virtual ~NaviSetVideoConfigRequest();

  /**
   * @brief Execute command
   **/
  void Run() OVERRIDE;

  /**
   * @brief On event callback
   **/
  void on_event(const event_engine::Event& event) OVERRIDE;

  /**
   * @brief onTimeOut callback
   */
  void onTimeOut() OVERRIDE;

 private:
  DISALLOW_COPY_AND_ASSIGN(NaviSetVideoConfigRequest);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SET_VIDEO_CONFIG_REQUEST_H_
