#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SUBSCRIBE_WAY_POINTS_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SUBSCRIBE_WAY_POINTS_REQUEST_H_

#include "application_manager/commands/hmi/request_to_hmi.h"

namespace application_manager {

namespace commands {

/**
 * @brief NaviSubscribeWayPointsRequest command class
 **/
class NaviSubscribeWayPointsRequest
    : public RequestToHMI {
public:
    /**
     * @brief NaviSubscribeWayPointsRequest class constructor
     *
     * @param message Incoming SmartObject message
     **/
    explicit NaviSubscribeWayPointsRequest(const MessageSharedPtr& message);
    /**
     * @brief NaviSubscribeWayPointsRequest class destructor
     **/
    virtual ~NaviSubscribeWayPointsRequest();
    /**
     * @brief Execute command
     **/
    virtual void Run() OVERRIDE;

private:
  DISALLOW_COPY_AND_ASSIGN(NaviSubscribeWayPointsRequest);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_SUBSCRIBE_WAY_POINTS_REQUEST_H_
