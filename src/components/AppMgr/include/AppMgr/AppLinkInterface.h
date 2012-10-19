/*
 * AppLinkInterface.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPLINKINTERFACE_H_
#define APPLINKINTERFACE_H_

#include "RPCBusObject.h"
#include "mb_controller.hpp"

namespace log4cplus
{
	class Logger;
};

namespace NsAppManager
{
	
class AppLinkInterface : public NsMessageBroker::CMessageBrokerController
{
public:
	static AppLinkInterface& getInstance( );
	~AppLinkInterface( );

	void sendRPCBusObject(const RPCBusObject* rpcObject);

	/**
	 * \brief needs to be called before getInstance.
	 * \param address address to bind to.
	 * \param port port to bind to.
	 * \param name name to bind to.
	 */
	static void setParams(const std::string& address, uint16_t port, std::string name);
	
	/**
	 * \brief pure virtual method to process response.
	 * \param method method name which has been called.
	 * \param root JSON message.
	 */
	virtual void processResponse(std::string method, Json::Value& root);
	
	/**
	 * \brief pure virtual method to process request.
	 * \param root JSON message.
	 */
	virtual void processRequest(Json::Value& root);
	
	/**
	 * \brief Process notification message.
	 * \brief Notify subscribers about property change.
	 * expected notification format example:
	 * \code
	 * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
	 * \endcode
	 * \param root JSON message.
	 */
	virtual void processNotification(Json::Value& root);

private:
	AppLinkInterface( const std::string& address, uint16_t port, const std::string& name );

	void sendMessage();
	
	void getButtonCapabilities();
	void getVoiceCapabilities();
	void getVRCapabilities();

	static std::string mAddress;
	static uint16_t mPort;
	static std::string mName;
	static bool m_bInitialized;

	const log4cplus::Logger& mLogger;
};

}; // namespace NsAppManager

#endif /* APPLINKINTERFACE_H_ */
