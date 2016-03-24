#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_UNSUBSCRIBE_WAY_POINTS_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_UNSUBSCRIBE_WAY_POINTS_REQUEST_H_

#include "application_manager/commands/hmi/request_to_hmi.h"

namespace application_manager {

namespace commands {

/**
 * @brief NaviSubscribeWayPointsRequest command class
 **/
class NaviUnSubscribeWayPointsRequest
    : public RequestToHMI {
public:
    /**
     * @brief NaviUnSubscribeWayPointsRequest class constructor
     *
     * @param message Incoming SmartObject message
     **/
    explicit NaviUnSubscribeWayPointsRequest(const MessageSharedPtr &);
    /**
     * @brief NaviUnSubscribeWayPointsRequest class destructor
     **/
    virtual ~NaviUnSubscribeWayPointsRequest();
    /**
     * @brief Execute command
     **/
    virtual void Run();

private:
  DISALLOW_COPY_AND_ASSIGN(NaviUnSubscribeWayPointsRequest);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_UNSUBSCRIBE_WAY_POINTS_REQUEST_H_
