#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_GET_WAY_POINTS_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_GET_WAY_POINTS_REQUEST_H_

#include "application_manager/commands/hmi/request_to_hmi.h"

namespace application_manager {

namespace commands {

/**
 * @brief NaviGetWayPointsRequest command class
 **/
class NaviGetWayPointsRequest
    : public RequestToHMI {
public:
    /**
     * @brief NaviGetWayPointsRequest class constructor
     *
     * @param message Incoming SmartObject message
     **/
    explicit NaviGetWayPointsRequest(const MessageSharedPtr& message);
    /**
     * @brief NaviGetWayPointsRequest class destructor
     **/
    virtual ~NaviGetWayPointsRequest();
    /**
     * @brief Execute command
     **/
    virtual void Run() OVERRIDE;

private:
  DISALLOW_COPY_AND_ASSIGN(NaviGetWayPointsRequest);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_NAVI_GET_WAY_POINTS_REQUEST_H_
