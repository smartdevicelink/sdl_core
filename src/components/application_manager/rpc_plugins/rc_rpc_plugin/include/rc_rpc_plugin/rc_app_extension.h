/*
 * Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RC_APP_EXTENSION_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RC_APP_EXTENSION_H_

#include <string>
#include <set>
#include "application_manager/app_extension.h"

namespace rc_rpc_plugin {
class RCAppExtension : public application_manager::AppExtension {
 public:
  explicit RCAppExtension(application_manager::AppExtensionUID uid);
  ~RCAppExtension();

  /**
   * @brief Subscribe to OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  void SubscribeToInteriorVehicleData(const std::string& module_type);

  /**
   * @brief Unsubscribe from OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  void UnsubscribeFromInteriorVehicleData(const std::string& module_type);

  /**
   * @brief UnsubscribeFromInteriorVehicleData removes all subscriptions for
   * interior data
   */
  void UnsubscribeFromInteriorVehicleData();

  /**
   * @brief Check if application subscribed to OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  bool IsSubscibedToInteriorVehicleData(const std::string& module_type);

  /**
   * @brief get list of subscriptions of application
   * @return list of subscriptions of application
   */
  std::set<std::string> InteriorVehicleDataSubscriptions() const;

 private:
  std::set<std::string> subscribed_interior_vehicle_data_;
};

typedef utils::SharedPtr<RCAppExtension> RCAppExtensionPtr;

}  //  namespace rc_rpc_plugin

#endif  //  SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_RC_APP_EXTENSION_H_
