#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_UNSUBSCRIBE_WAY_POINTS_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_UNSUBSCRIBE_WAY_POINTS_RESPONSE_H_

#include "application_manager/commands/hmi/response_from_hmi.h"

namespace application_manager {

namespace commands {

/**
 * @brief NaviUnSubscribeWaypoints command class
 **/
class NaviUnSubscribeWaypointsResponse
    : public ResponseFromHMI {
 public:
    /**
     * @brief NaviUnSubscribeWaypoints class constructor
     *
     * @param message Incoming SmartObject message
     **/
    explicit NaviUnSubscribeWaypointsResponse(const MessageSharedPtr& message);

    /**
     * @brief NaviUnSubscribeWaypoints class destructor
     **/
    virtual ~NaviUnSubscribeWaypointsResponse();

    /**
     * @brief Execute command
     **/
    virtual void Run();
 private:
  DISALLOW_COPY_AND_ASSIGN(NaviUnSubscribeWaypointsResponse);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_UNSUBSCRIBE_WAY_POINTS_RESPONSE_H_
