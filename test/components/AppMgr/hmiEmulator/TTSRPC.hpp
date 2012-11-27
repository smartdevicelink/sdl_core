/**
 * \file TTSRPC.hpp
 * \brief MessageBroker Controller TTSRPC.
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
  * \class TTSRPC
  * \brief MessageBroker Controller.
  */

  class TTSRPC : public RPC
  {
  public:
    /**
    * \brief Constructor.
    * \param address remote network address or FQDN
    * \param port remote local port
    */
    TTSRPC(const std::string& address, unsigned short port);

    /**
    * \brief Destructor.
    */
    virtual ~TTSRPC();

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
     * \brief Callback function which is called by JSONRPC2Handler
     *  when new RPC2Bus Json message is received from HMI.
     * \param command RPC2Bus Json message
     */
    virtual void onCommandReceivedCallback( NsRPC2Communication::RPC2Command * command );
  };
}/* namespace NsHMIEmulator */
