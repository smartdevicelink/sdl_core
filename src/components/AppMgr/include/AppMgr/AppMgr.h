#ifndef APPMGR_H
#define APPMGR_H

#include "JSONHandler/IRPCMessagesObserver.h"
#include "AppMgr/AppLinkInterface.h"
#include "mb_controller.hpp"

class JSONHandler;
class ALRPCMessage;
class JSONRPC2Handler;

namespace log4cplus
{
	class Logger;
};

namespace RPC2Communication
{
    class RPC2Command;
};

namespace NsAppManager
{

class AppMgrRegistry;
class AppMgrCore;
class AppFactory;
	
class AppMgr: public IRPCMessagesObserver
{
public:

	/**
	 * \brief needs to be called before getInstance.
	 * \param address address to bind to.
	 * \param port port to bind to.
	 * \param name name to bind to.
	 */
	static void setParams(const std::string& address, uint16_t port, std::string name);
	
	static AppMgr& getInstance();
	
    virtual void onMessageReceivedCallback( ALRPCMessage * message, unsigned char sessionID );

	/**
	 * \brief pure virtual method to process response.
	 * \param method method name which has been called.
	 * \param root JSON message.
	 */
	void processResponse(std::string method, Json::Value& root);
	
	/**
	 * \brief pure virtual method to process request.
	 * \param root JSON message.
	 */
	void processRequest(Json::Value& root);
	
	/**
	 * \brief Process notification message.
	 * \brief Notify subscribers about property change.
	 * expected notification format example:
	 * \code
	 * {"jsonrpc": "2.0", "method": "<ComponentName>.<NotificationName>", "params": <list of params>}
	 * \endcode
	 * \param root JSON message.
	 */
	void processNotification(Json::Value& root);

	void setJsonHandler(JSONHandler* handler);

    void setJsonRPC2Handler(JSONRPC2Handler* handler);

	const AppLinkInterface& getAppLinkInterface() const;

	void startAppMgr();
	
private:
	virtual ~AppMgr();
    AppMgr(const AppMgr&);
	AppMgr();

	AppLinkInterface& mAppLinkInterface;
	AppMgrRegistry& mAppMgrRegistry;
	AppMgrCore& mAppMgrCore;
	AppFactory& mAppFactory;

	JSONHandler* mJSONHandler;
	static log4cplus::Logger mLogger;
};

}; // namespace NsAppManager

#endif // APPMGR_H
