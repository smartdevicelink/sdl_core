#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_GET_WAY_POINTS_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_GET_WAY_POINTS_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"

namespace application_manager {

namespace commands {

/**
 * @brief GetWayPointsRequest command class
 **/
class GetWayPointsRequest : public CommandRequestImpl {
public:
  /**
   * \brief GetWayPointsRequest class constructor
   **/
  explicit GetWayPointsRequest(const MessageSharedPtr& message);

  /**
   * \brief GetWayPointsRequest class destructor
   **/
  virtual ~GetWayPointsRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run() OVERRIDE;
  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  virtual void on_event(const event_engine::Event& event);
private:
  DISALLOW_COPY_AND_ASSIGN(GetWayPointsRequest);
};

} // namespace commands

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_GET_WAY_POINTS_REQUEST_H_
