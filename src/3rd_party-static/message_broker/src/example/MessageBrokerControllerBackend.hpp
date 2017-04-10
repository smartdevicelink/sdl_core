/**
 * \file MessageBrokerControllerBackend.hpp
 * \brief MessageBroker Controller Backend.
 * \author AKara
 */
#pragma once

#include <iostream>

#include "json/json.h"

#include "mb_controller.hpp"
#include "CMessageBroker.hpp"

/**
 * \namespace NsMessageBroker
 * \brief MessageBroker related functions.
 */ 
namespace NsMessageBroker
{
  /**
  * \class CMessageBrokerControllerBackend
  * \brief MessageBroker Controller.
  */

  class CMessageBrokerControllerBackend : public CMessageBrokerController
  {
  public:
    /**
    * \brief Constructor.
    * \param address remote network address or FQDN
    * \param port remote local port
    */
    CMessageBrokerControllerBackend(const std::string& address, unsigned short port);

    /**
    * \brief Destructor.
    */
    ~CMessageBrokerControllerBackend();

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
    /**
    * \brief Checks first start.
    * \param response container for response
    */
    void isFirstStart(Json::Value& response);

    /**
    * \brief Checks first start.
    * \param response container for response
    */
    void isFullScreen(Json::Value& response);

    /**
    * \brief Checks first start.
    * \param response container for response
    */
    void getWindowSize(Json::Value& response);

    /**
    * \brief Checks first start.
    * \param response container for response
    */
    void getWindowDensity(Json::Value& response);

    /**
    * \brief Checks first start.
    * \param response container for response
    */
    void getOSInfo(Json::Value& response);

    /**
    * \brief Checks first start.
    * \param response container for response
    */
    void logToOS(Json::Value& response);
  };
}/* namespace NsMessageBroker */
