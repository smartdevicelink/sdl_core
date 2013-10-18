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

  public:
    /**
    * \brief process response.
    * \param method method name which has been called.
    */
    void onFullScreenChanged(bool isFullScreen);

    /**
    * \brief process response.
    * \param method method name which has been called.
    */
    void onAppStatusChanged(bool appBecameActive);

    /**
    * \brief navigation enabling notification.
    */
	void sendNavSettingsNotification(bool navIsEnabled);

    /**
    * \brief vehicle settings changes notification.
    * \param vehihcle that was set.
    */
	void sendVehicleSettingsNotification(std::string vehicle);
  private:
    /**
    * \brief Checks first start.
    * \param response container for response
    */
    void isFirstStart(Json::Value& response);

    /**
    * \brief Checks full screen.
    * \param response container for response
    */
    void isFullScreen(Json::Value& response);

    /**
    * \brief Returns window size.
    * \param response container for response
    */
    void getWindowSize(Json::Value& response);

    /**
    * \brief Returns window density.
    * \param response container for response
    */
    void getWindowDensity(Json::Value& response);

    /**
    * \brief Returns OS info.
    * \param response container for response
    */
    void getOSInfo(Json::Value& response);

    /**
    * \brief Returns OS info.
    * \param response container for response
    */
    void prepareOSInfo(Json::Value& response);

    /**
    * \brief Returns is navigation settings enabled.
    * \param response container for response
    */
    void hasMaps(Json::Value& response);
	
    /**
    * \brief Returns current vehicle model.
    * \param response container for response
    */
	void getVehicleModel(Json::Value& response);
	
    /**
    * \brief Returns "OK" if navigation enabling was successfuly set.
    * \param response container for response
    */
	void setHasMaps(Json::Value& response, bool value);
  
    /**
    * \brief Returns "OK" if vehicle model was successfuly set.
    * \param response container for response
    */
	void setVehicleModel(Json::Value& response, std::string value);
  
	/**
    * \brief Logs data to OS.
    * \param response container for response
    */
    void logToOS(Json::Value& params, Json::Value& response);

	/**
    * \brief invoke e-mail client to send message.
    * \param response container for response
    */
	void sendSupportEmail(Json::Value& response);

	/**
    * \brief open EULA text file.
    * \param response container for response
    */
	void openEula(Json::Value& response);

	std::wstring toStringW( const std::string& strText );
  };
}/* namespace NsMessageBroker */
