#include "AppMgr/AppMgr.h"

namespace NsAppManager
{
	
std::string AppMgr::mAddress = "";
uint16_t AppMgr::mPort = 0;
std::string AppMgr::mName = "";
bool AppMgr::m_bInitialized = false;
	
AppMgr& AppMgr::getInstance( )
{
	if(m_bInitialized)
	{
		static AppMgr appMgr(mAddress, mPort, mName);
		return appMgr;
	}
	//here to log error
}

void AppMgr::setParams(const std::string& address, uint16_t port, std::string name)
{
	mAddress = address;
	mPort = port;
	mName = name;
	m_bInitialized = true;
}

AppMgr::AppMgr(const std::string& address, uint16_t port, std::string name)
	:NsMessageBroker::CMessageBrokerController::CMessageBrokerController(address, port, name)
	,mAppLinkInterface(AppLinkInterface::getInstance())
	,mAppMgrRegistry(AppMgrRegistry::getInstance())
	,mAppMgrCore(AppMgrCore::getInstance())
	,mRPCAppLinkFactory(RPCAppLinkFactory::getInstance())
	,mRPCBusFactory(RPCBusFactory::getInstance())
	,mAppFactory(AppFactory::getInstance())
{
}

AppMgr::~AppMgr()
{
}

void AppMgr::onMessageReceivedCallback( MobileRPCMessage * message )
{
	mAppMgrCore.pushMobileRPCMessage( message );
}

/**
 * \brief pure virtual method to process response.
 * \param method method name which has been called.
 * \param root JSON message.
 */
void AppMgr::processResponse(std::string method, Json::Value& root)
{
}

/**
 * \brief pure virtual method to process request.
 * \param root JSON message.
 */
void AppMgr::processRequest(Json::Value& root)
{
}

/**
 * \brief Process notification message.
 * \brief Notify subscribers about property change.
 * expected notification format example:
 * \code
 * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
 * \endcode
 * \param root JSON message.
 */
void AppMgr::processNotification(Json::Value& root)
{
}

};
