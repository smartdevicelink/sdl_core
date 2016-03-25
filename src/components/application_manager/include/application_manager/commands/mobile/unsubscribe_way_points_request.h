#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_UNSUBSCRIBE_WAY_POINTS_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_UNSUBSCRIBE_WAY_POINTS_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"

namespace application_manager {

namespace commands {

class UnSubscribeWayPointsRequest : public CommandRequestImpl {
public:
  /**
   * \brief UnSubscribeWayPointsRequest class constructor
   **/
  explicit UnSubscribeWayPointsRequest(const MessageSharedPtr& message);

  /**
   * \brief UnSubscribeWayPointsRequest class destructor
   **/
  virtual ~UnSubscribeWayPointsRequest();

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
  DISALLOW_COPY_AND_ASSIGN(UnSubscribeWayPointsRequest);
};

} // commands

} // application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_UNSUBSCRIBE_WAY_POINTS_REQUEST_H_
