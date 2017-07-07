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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_APP_EXTENSION_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_APP_EXTENSION_H_

#include <string>
#include <set>
#include "application_manager/service.h"
#include "application_manager/app_extension.h"
#include "can_cooperation/can_module.h"
#include "json/json.h"

namespace can_cooperation {

class CANAppExtension : public application_manager::AppExtension {
 public:
  explicit CANAppExtension(application_manager::AppExtensionUID uid);
  ~CANAppExtension();

  /**
   * @brief Checks is application has access to a radio tune control
   * @return true if control given
   */
  bool IsControlGiven() const;

  /**
   * @brief Give radio tune control to application
   * @param is_control_given true - give control, false - cancel control
   */
  void GiveControl(bool is_control_given);

  bool is_on_driver_device() const {
    return is_on_driver_device_;
  }

  void set_is_on_driver_device(bool is_driver_dev) {
    is_on_driver_device_ = is_driver_dev;
  }

  /**
   * @brief Subscribe to OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  void SubscribeToInteriorVehicleData(const Json::Value& moduleDescription);

  /**
   * @brief Unsubscribe from OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  void UnsubscribeFromInteriorVehicleData(const Json::Value& moduleDescription);

  /**
   * @brief Check if application subscribed to OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  bool IsSubscibedToInteriorVehicleData(const Json::Value& moduleDescription);

 private:
  bool is_control_given_;
  bool is_on_driver_device_;
  std::set<Json::Value> subscribed_interior_vehicle_data_;
};

typedef utils::SharedPtr<CANAppExtension> CANAppExtensionPtr;

}  //  namespace can_cooperation

#endif  //  SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_CAN_APP_EXTENSION_H_
