#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_WAY_POINTS_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_WAY_POINTS_RESPONSE_H_

#include "application_manager/commands/command_response_impl.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief SubscribeWayPointsResponse command class
 **/
class SubscribeWayPointsResponse : public CommandResponseImpl {
public:
  /**
   * \brief SubscribeWayPointsResponse class constructor
   **/
  explicit SubscribeWayPointsResponse(const MessageSharedPtr& message);

  /**
   * \brief SubscribeWayPointsResponse class destructor
   **/
  virtual ~SubscribeWayPointsResponse();

  /**
   * @brief Execute command
   **/
  virtual void Run();
private:
  DISALLOW_COPY_AND_ASSIGN(SubscribeWayPointsResponse);
};

} // commands

} // application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_WAY_POINTS_RESPONSE_H_
