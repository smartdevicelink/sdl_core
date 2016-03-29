#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_GET_WAY_POINTS_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_GET_WAY_POINTS_RESPONSE_H_

#include "application_manager/commands/hmi/response_from_hmi.h"

namespace application_manager {

namespace commands {

/**
 * @brief NaviGetWaypoints command class
 **/
class NaviGetWaypointsResponse
    : public ResponseFromHMI {
 public:
    /**
     * @brief NaviGetWaypoints class constructor
     *
     * @param message Incoming SmartObject message
     **/
    explicit NaviGetWaypointsResponse(const MessageSharedPtr& message);

    /**
     * @brief NaviGetWaypoints class destructor
     **/
    virtual ~NaviGetWaypointsResponse();

    /**
     * @brief Execute command
     **/
    virtual void Run();
 private:
  DISALLOW_COPY_AND_ASSIGN(NaviGetWaypointsResponse);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_GET_WAY_POINTS_RESPONSE_H_
