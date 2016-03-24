#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SUBSCRIBE_WAY_POINTS_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SUBSCRIBE_WAY_POINTS_RESPONSE_H_

#include "application_manager/commands/hmi/response_from_hmi.h"

namespace application_manager {

namespace commands {

/**
 * @brief NaviSubscribeWaypoints command class
 **/
class NaviSubscribeWaypointsResponse
    : public ResponseFromHMI {
 public:
    /**
     * @brief NaviSubscribeWaypoints class constructor
     *
     * @param message Incoming SmartObject message
     **/
    explicit NaviSubscribeWaypointsResponse(const MessageSharedPtr& message);

    /**
     * @brief NaviSubscribeWaypoints class destructor
     **/
    virtual ~NaviSubscribeWaypointsResponse();

    /**
     * @brief Execute command
     **/
    virtual void Run();
 private:
  DISALLOW_COPY_AND_ASSIGN(NaviSubscribeWaypointsResponse);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SUBSCRIBE_WAY_POINTS_RESPONSE_H_
