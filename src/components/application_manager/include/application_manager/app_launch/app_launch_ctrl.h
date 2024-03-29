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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_CTRL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_CTRL_H_
#include <memory>
#include <string>

namespace application_manager {
class Application;
typedef std::shared_ptr<const Application> ApplicationConstSharedPtr;
}  // namespace application_manager

namespace app_launch {

/**
 * @brief The AppLaunchCtrl class manage logic of AppLaunch feature
 * It launches all known applications on newly connected device
 */
class AppLaunchCtrl {
 public:
  /**
   * @brief OnAppRegistered should be called when application registered
   * Save application parameters to database
   * @param app application to save
   */
  virtual void OnAppRegistered(const application_manager::Application& app) = 0;

  /**
   * @brief OnDeviceConnected shoudl be called on device connected event
   * Start launching saaved applications on ios device
   * @param device_mac
   */
  virtual void OnDeviceConnected(const std::string& device_mac) = 0;

  /**
   * @brief OnMasterReset clear database of saved applications
   */
  virtual void OnMasterReset() = 0;

  /**
   * @brief Stop - allows to stop app launcher.
   */
  virtual void Stop() = 0;

  virtual ~AppLaunchCtrl() {}
};

}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_CTRL_H_
