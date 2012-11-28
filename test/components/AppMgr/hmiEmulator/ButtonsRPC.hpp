/**
 * \file ButtonsRPC.hpp
 * \brief MessageBroker Controller ButtonsRPC.
 * \author AKara
 */
#pragma once

#include <iostream>

#include "RPC.h"

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */ 
namespace NsHMIEmulator
{
  /**
  * \class ButtonsRPC
  * \brief MessageBroker Controller.
  */

  class ButtonsRPC : public RPC
  {
  public:
    /**
    * \brief Constructor.
    * \param address remote network address or FQDN
    * \param port remote local port
    */
    ButtonsRPC(const std::string& address, unsigned short port);

    /**
    * \brief Destructor.
    */
    virtual ~ButtonsRPC();

    /**
    * \brief process request.
    * \param root JSON message.
    */
    virtual void processRequest(Json::Value& root);

    /**
    * \brief process notification.
    * \param root JSON message.
    */
    virtual void processNotification(Json::Value& root);

    /**
    * \brief process response.
    * \param method method name which has been called.
    * \param root JSON message.
    */
    virtual void processResponse(std::string method, Json::Value& root);

    /**
     * \brief Callback function which is called upon a new message from mobile side arrival
     * \param command RPC2Bus Json message
     */
    virtual void messageReceivedFromDeviceCallback( NsRPC2Communication::RPC2Command * command );

  };
}/* namespace NsHMIEmulator */
