/**
 * \file ButtonsRPC.hpp
 * \brief MessageBroker Controller ButtonsRPC.
 * \author AKara
 */
#pragma once

#include <iostream>

#include "json/json.h"

#include "mb_controller.hpp"
#include "CMessageBroker.hpp"

/**
 * \namespace NsHMIEmulator
 * \brief HMIEmulator related functions.
 */ 
namespace NsHMIEmulator
{
  /**
  * \class ButtonsRPC
  * \brief MessageBroker Controller.
  */

  class ButtonsRPC : public NsMessageBroker::CMessageBrokerController
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
    ~ButtonsRPC();

    /**
    * \brief process request.
    * \param root JSON message.
    */
    void processRequest(Json::Value& root);

    /**
    * \brief process notification.
    * \param root JSON message.
    */
    void processNotification(Json::Value& root);

    /**
    * \brief process response.
    * \param method method name which has been called.
    * \param root JSON message.
    */
    void processResponse(std::string method, Json::Value& root);
  private:
  };
}/* namespace NsHMIEmulator */
