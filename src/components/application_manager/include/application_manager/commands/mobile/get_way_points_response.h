#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_GET_WAY_POINTS_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_GET_WAY_POINTS_RESPONSE_H_

#include "application_manager/commands/command_response_impl.h"
#include "utils/macro.h"

namespace application_manager {

namespace commands {

/**
 * @brief GetWayPointsResponse command class
 **/
class GetWayPointsResponse : public CommandResponseImpl {
public:
  /**
   * \brief GetWayPointsResponse class constructor
   **/
  explicit GetWayPointsResponse(const MessageSharedPtr& message);

  /**
   * \brief GetWayPointsResponse class destructor
   **/
  virtual ~GetWayPointsResponse();

  /**
   * @brief Execute command
   **/
  virtual void Run() OVERRIDE;
private:
  DISALLOW_COPY_AND_ASSIGN(GetWayPointsResponse);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_GET_WAY_POINTS_RESPONSE_H_
