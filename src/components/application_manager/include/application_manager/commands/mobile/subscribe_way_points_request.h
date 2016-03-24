#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_WAY_POINTS_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_WAY_POINTS_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"

namespace application_manager {

namespace commands {

/**
 * @brief SubsribeWayPointsRequest command class
 **/
class SubscribeWayPointsRequest : public CommandRequestImpl {
public:
  /**
   * \brief SubscribeWayPointsRequest class constructor
   **/
  explicit SubscribeWayPointsRequest(const MessageSharedPtr& message);

  /**
   * \brief SubscribeWayPointsRequest class destructor
   **/
  virtual ~SubscribeWayPointsRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();
  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  virtual void on_event(const event_engine::Event& event);
private:
  DISALLOW_COPY_AND_ASSIGN(SubscribeWayPointsRequest);
};

} // commands

} // application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_MOBILE_SUBSCRIBE_WAY_POINTS_REQUEST_H_
