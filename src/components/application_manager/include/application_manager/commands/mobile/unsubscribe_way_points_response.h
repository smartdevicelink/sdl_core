#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_UNSUBSCRIBE_WAY_POINTS_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_UNSUBSCRIBE_WAY_POINTS_RESPONSE_H_

#include "application_manager/commands/command_response_impl.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief UnSubscribeWayPointsResponse command class
 **/
class UnSubscribeWayPointsResponse : public CommandResponseImpl {
public:
  /**
   * \brief UnSubscribeWayPointsResponse class constructor
   **/
  explicit UnSubscribeWayPointsResponse(const MessageSharedPtr& message);

  /**
   * \brief UnSubscribeWayPointsResponse class destructor
   **/
  virtual ~UnSubscribeWayPointsResponse();

  /**
   * @brief Execute command
   **/
  virtual void Run();
private:
  DISALLOW_COPY_AND_ASSIGN(UnSubscribeWayPointsResponse);
};

} // commands

} // application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_UNSUBSCRIBE_WAY_POINTS_RESPONSE_H_
