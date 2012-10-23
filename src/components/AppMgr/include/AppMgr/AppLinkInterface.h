/*
 * AppLinkInterface.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPLINKINTERFACE_H_
#define APPLINKINTERFACE_H_

#include "RPCBusObject.h"
#include "JSONHandler/RPC2Marshaller.h"
#include "JSONHandler/RPC2Command.h"
#include "mb_controller.hpp"

#include "system.h"
#include <queue>
#include <set>

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

	void executeThreads();
	void terminateThreads();

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

	void sendRPCCommand(const RPC2Communication::RPC2Command* rpcObject);
	void receiveRPCCommand(RPC2Communication::RPC2Command* rpcObject);
	
	void enqueueRPCCommandIncoming( RPC2Communication::RPC2Command * object );
	void enqueueRPCCommandOutgoing( RPC2Communication::RPC2Command * object );
	void sendMessageAwaitingExecution( RPC2Communication::RPC2Command* rpcObject, bool repeat );

	void addAwaitedResponseMethod( int method );
	void addRespondedMethod( int method );
	void removeAwaitedResponseMethod( int method );
	void removeRespondedMethod( int method );

	bool findAwaitedResponseMethod( int method ) const;
	bool findRespondedMethod( int method ) const;
	
	void getAllCapabilities();
	void getButtonCapabilities();
	void getVoiceCapabilities();
	void getVRCapabilities();

	void* handleQueueRPCBusObjectsIncoming( void* );
	void* handleQueueRPCBusObjectsOutgoing( void* );
	
	static std::string mAddress;
	static uint16_t mPort;
	static std::string mName;
	static bool m_bInitialized;
	bool m_bTerminate;

	bool m_bButtonCapsRetrieved;
	bool m_bVRCapsRetrieved;
	bool m_bVoiceCapsRetrieved;
	
	static log4cplus::Logger mLogger;

	std::queue< RPC2Communication::RPC2Command* > mQueueRPCBusObjectsIncoming;
	std::queue< RPC2Communication::RPC2Command* > mQueueRPCBusObjectsOutgoing;

	std::set<int> mAwaitedResponseMethods;
	std::set<int> mRespondedMethods;

	System::Mutex mMtxAwaitedResponseMethods;
	System::Mutex mMtxRespondedMethods;
	
	System::Mutex mMtxRPCBusObjectsIncoming;
	System::Mutex mMtxRPCBusObjectsOutgoing;
	
	System::Thread mThreadRPCBusObjectsIncoming;
	System::Thread mThreadRPCBusObjectsOutgoing;
};

}; // namespace NsAppManager

#endif /* APPLINKINTERFACE_H_ */
