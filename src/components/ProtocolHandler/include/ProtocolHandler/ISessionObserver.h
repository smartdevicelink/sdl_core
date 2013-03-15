/**
* \file ISessionObserver.h
* \brief ISessionObserver class header file.
* 
* Copyright (c) 2013 Ford Motor Company
*/

#ifndef ISESSIONOBSERVER_CLASS
#define ISESSIONOBSERVER_CLASS

/**
  *\namespace NsProtocolHandler
  *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
*/
namespace NsProtocolHandler
{
    /**
     * \class ISessionObserver
     * \brief Interface for making a bridge between ProtocolHandler and
     * ConnectionHandler components.
    */
    class ISessionObserver
    {
    public:
        /**
         * \brief Callback function used by ProtocolHandler
         * when Mobile Application initiates start of new session.
         * \param connectionHandle Connection identifier whithin which session has to be started.
         * \return int Id (number) of new session if successful otherwise -1.
         */
        virtual int onSessionStartedCallback(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle) = 0;

        /**
         * \brief Callback function used by ProtocolHandler
         * when Mobile Application initiates session ending.
         * \param connectionHandle Connection identifier whithin which session exists
         * \param sessionId Identifier of the session to be ended
         * \param hashCode Hash used only in second version of SmartDeviceLink protocol.
         * If not equal to hash assigned to session on start then operation fails.
         * \return int -1 if operation fails session key otherwise
         */
        virtual int onSessionEndedCallback(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                                               unsigned char sessionId,
                                               unsigned int hashCode) = 0;

        /**
         * \brief Creates unique identifier of session (can be used as hash) 
         * from given connection identifier
         * whithin which session exists and session number.
         * \param  connectionHandle Connection identifier whithin which session exists
         * \param sessionId Identifier of the session
         * \return int Unique key for session
         */
        virtual int keyFromPair(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                                               unsigned char sessionId) = 0;

        /**
         * \brief Returns connection identifier and session number from given session key
         * \param key Unique key used by other components as session identifier
         * \param connectionHandle Returned: Connection identifier whithin which session exists
         * \param sessionId Returned: Number of session
         */
        virtual void pairFromKey(int key, NsSmartDeviceLink::NsTransportManager::tConnectionHandle & connectionHandle,
                                               unsigned char & sessionId) = 0;
    protected:

        /**
         * \brief Destructor
        */
        virtual ~ISessionObserver() {};
    };
}

#endif  //  ISESSIONOBSERVER_CLASS
