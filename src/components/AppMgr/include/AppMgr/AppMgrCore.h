/*
 * AppMgr.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPMGR_H_
#define APPMGR_H_

#include <string>
#include "AppMgr/ButtonMapping.h"
#include "AppMgr/CommandMapping.h"
#include "AppMgr/MessageMapping.h"
#include "AppMgr/MenuMapping.h"
#include "AppMgr/RequestMapping.h"
#include "AppMgr/CapabilitesContainer.h"

namespace AppLinkRPC{
    class ALRPCMessage;
}
class JSONHandler;
class JSONRPC2Handler;

namespace AppLinkRPC
{
    class RegisterAppInterface_request;
}

namespace RPC2Communication
{
	class RPC2Command;
}

namespace log4cplus
{
	class Logger;
}

namespace NsAppManager
{

class RegistryItem;
template< class QueueType >
class AppMgrCoreQueue;
class Application;

typedef std::pair<AppLinkRPC::ALRPCMessage*, unsigned char> Message;


/**
 * \brief Core app manager class which acts as a core for application manager
 */
class AppMgrCore
{
    friend class SubscribeButtonCmd;
public:

    /**
     * \brief Default class destructor
     */
	virtual ~AppMgrCore();

    /**
     * \brief Returning class instance
     * \return class instance
     */
	static AppMgrCore& getInstance();

    /**
     * \brief push mobile RPC message to a queue
     * \param message a message to be pushed
     * \param sessionID an id of a session associated with the application which pushes a message
     */
    void pushMobileRPCMessage(AppLinkRPC::ALRPCMessage * message , unsigned char sessionID);

    /**
     * \brief push HMI RPC2 message to a queue
     * \param message a message to be pushed
     */
	void pushRPC2CommunicationMessage( RPC2Communication::RPC2Command * message );

    /**
     * \brief method to execute threads.
     */
	void executeThreads();

    /**
     * \brief set Json mobile handler
     * \param handler a handler instance
     */
	void setJsonHandler(JSONHandler* handler);

    /**
     * \brief get Json mobile handler
     * \return JSONHandler instance
     */
	JSONHandler* getJsonHandler( ) const;

    /**
     * \brief set Json RPC2 handler
     * \param handler a handler instance
     */
    void setJsonRPC2Handler(JSONRPC2Handler* handler);

    /**
     * \brief get Json RPC2 handler
     * \return JSONRPC2Handler instance
     */
    JSONRPC2Handler* getJsonRPC2Handler( ) const;

private:

    /**
     * \brief Default class constructor
     */
	AppMgrCore();

    /**
     * \brief Copy constructor
     */
    AppMgrCore(const AppMgrCore&);

    /**
     * \brief mobile RPC message handler
     * \param mesage a message to be handled
     * \param pThis a pointer to AppMgrCore class instance
     */
    static void handleMobileRPCMessage(Message message, void* pThis);

    /**
     * \brief push HMI RPC2 message to a queue
     * \param msg a message to be pushed
     * \param pThis a pointer to AppMgrCore class instance
     */
    static void handleBusRPCMessageIncoming( RPC2Communication::RPC2Command* msg, void* pThis );

    /**
     * \brief Register an application
     * \param request a RegisterAppInterface request which is the source for application fields initial values
     * \param sessionID an id of the session which will be associated with the application
     * \return A instance of RegistryItem created for application
     */
    const RegistryItem* registerApplication(AppLinkRPC::RegisterAppInterface_request *request , const unsigned char &sessionID);

    /**
     * \brief unregister an application associated with the given session
     * \param sessionID an id of the session asociated with the application to be unregistered
     */
    void unregisterApplication(const unsigned char &sessionID);

    /**
     * \brief retrieve an application instance from the RegistryItrem instance checking for non-null values
     * \param item a RegistryItem from which to retrieve an app pointer
     * \return Application instance retrieved from item
     */
    Application* getApplicationFromItemCheckNotNull( const RegistryItem* item ) const;

    AppMgrCoreQueue<Message>* mQueueRPCAppLinkObjectsIncoming;
    AppMgrCoreQueue<RPC2Communication::RPC2Command*>* mQueueRPCBusObjectsIncoming;

    CapabilitiesContainer<AppLinkRPC::ButtonCapabilities> mButtonCapabilities;
    AppLinkRPC::DisplayCapabilities mDisplayCapabilities;
    CapabilitiesContainer<AppLinkRPC::HmiZoneCapabilities> mHmiZoneCapabilities;
    CapabilitiesContainer<AppLinkRPC::VrCapabilities> mVrCapabilities;
    ButtonMapping    mButtonsMapping;
    CommandMapping   mCommandMapping;
    MessageMapping   mMessageMapping;
    MenuMapping      mMenuMapping;
    RequestMapping   mRequestMapping;

	static log4cplus::Logger mLogger;
};

} // namespace NsAppManager

#endif /* APPMGR_H_ */
