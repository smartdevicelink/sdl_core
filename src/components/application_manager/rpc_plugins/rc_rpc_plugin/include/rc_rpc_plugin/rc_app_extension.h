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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_APP_EXTENSION_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_APP_EXTENSION_H_

#include <memory>
#include <set>
#include <string>
#include "application_manager/app_extension.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {

typedef std::pair<std::string, std::string> ModuleUid;

struct Grid {
  int32_t col_;
  int32_t row_;
  int32_t level_;
  int32_t colspan_;
  int32_t rowspan_;
  int32_t levelspan_;

  Grid()
      : col_(0), row_(0), level_(0), colspan_(0), rowspan_(0), levelspan_(0) {}

  Grid(int32_t col,
       int32_t row,
       int32_t level,
       int32_t colspan,
       int32_t rowspan,
       int32_t levelspan)
      : col_(col)
      , row_(row)
      , level_(level)
      , colspan_(colspan)
      , rowspan_(rowspan)
      , levelspan_(levelspan) {}

  Grid& operator=(const Grid& grid) {
    col_ = grid.col_;
    row_ = grid.row_;
    level_ = grid.level_;
    colspan_ = grid.colspan_;
    rowspan_ = grid.rowspan_;
    levelspan_ = grid.levelspan_;

    return *this;
  }

  bool operator==(const Grid& grid) const {
    return col_ == grid.col_ && row_ == grid.row_ && level_ == grid.level_ &&
           colspan_ == grid.colspan_ && rowspan_ == grid.rowspan_ &&
           levelspan_ == grid.levelspan_;
  }

  /**
   * @brief LevelIntersectionExists checks if the grids have an
   * intersection by levels.
   * @param grid with which to check intersection
   * @return true if intersection exists, otherwise - false
   */
  bool LevelIntersectionExists(const Grid& grid) const {
    const int32_t top_level = (level_ + levelspan_) - 1;
    const int32_t grid_top_level = (grid.level_ + grid.levelspan_) - 1;

    const int32_t min_level = std::max(level_, grid.level_);
    const int32_t max_level = std::min(top_level, grid_top_level);
    if ((max_level - min_level) < 0) {
      return false;
    }
    return true;
  }

  /**
   * @brief IntersectionExists checks if the grids have an
   * intersection. Grid can be represented by its bottom-left
   * and top-right coordinates like a rectangle. First, the
   * coordinates of the intersection are calculated, then checked
   * that the rectangle which is formed by the intersections of two
   * grids has non-zero width and height.
   * @param grid with which to check intersection
   * @return true if intersection exists, otherwise - false
   */
  bool IntersectionExists(const Grid& grid) const {
    if (!LevelIntersectionExists(grid)) {
      return false;
    }

    const int32_t right_top_col = (col_ + colspan_) - 1;
    const int32_t right_top_row = (row_ + rowspan_) - 1;
    const int32_t grid_right_top_col = (grid.col_ + grid.colspan_) - 1;
    const int32_t grid_right_top_row = (grid.row_ + grid.rowspan_) - 1;

    const int32_t left = std::max(col_, grid.col_);
    const int32_t bottom = std::max(row_, grid.row_);
    const int32_t right = std::min(right_top_col, grid_right_top_col);
    const int32_t top = std::min(right_top_row, grid_right_top_row);

    const int32_t width = right - left;
    const int32_t height = top - bottom;

    if ((width < 0) || (height < 0)) {
      return false;
    }
    return true;
  }
};

class RCAppExtension : public application_manager::AppExtension {
 public:
  explicit RCAppExtension(application_manager::AppExtensionUID uid);
  ~RCAppExtension();

  /**
   * @brief Subscribe to OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  void SubscribeToInteriorVehicleData(const ModuleUid& module);

  /**
   * @brief Unsubscribe from OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  void UnsubscribeFromInteriorVehicleData(const ModuleUid& module);

  void UnsubscribeFromInteriorVehicleDataOfType(const std::string& module_type);

  /**
   * @brief UnsubscribeFromInteriorVehicleData removes all subscriptions for
   * interior data
   */
  void UnsubscribeFromInteriorVehicleData();

  /**
   * @brief Check if application subscribed to OnInteriorVehicleDataNotification
   * @param module interior data specification(zone, data type)
   */
  bool IsSubscribedToInteriorVehicleData(const ModuleUid& module);

  bool IsSubscribedToInteriorVehicleDataOfType(const std::string& module_type);

  /**
   * @brief get list of subscriptions of application
   * @return list of subscriptions of application
   */
  std::set<ModuleUid> InteriorVehicleDataSubscriptions() const;

  /**
   * @brief GetUserLocation
   * @return grid of user location
   */
  Grid GetUserLocation() const;

  /**
   * @brief SetUserLocation sets user location
   * from object to the grid
   * @param user_location smart object for user_location
   */
  void SetUserLocation(
      const ns_smart_device_link::ns_smart_objects::SmartObject& user_location);

  /**
   * @brief SetUserLocation sets user location from grid
   * @param grid grid of user_location
   */
  void SetUserLocation(const Grid& grid);

 private:
  std::set<ModuleUid> subscribed_interior_vehicle_data_;

  Grid user_location_;

  // AppExtension interface
 public:
  void SaveResumptionData(ns_smart_device_link::ns_smart_objects::SmartObject&
                              resumption_data) OVERRIDE;
  void ProcessResumption(
      const ns_smart_device_link::ns_smart_objects::SmartObject&
          resumption_data) OVERRIDE;
};

typedef std::shared_ptr<RCAppExtension> RCAppExtensionPtr;

}  //  namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_APP_EXTENSION_H_
