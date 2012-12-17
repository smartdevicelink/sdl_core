/**
 * \file AppMgrCore.h
 * \brief App manager core functionality
 * \author vsalo
 */

#ifndef APPMGR_H_
#define APPMGR_H_

#include <string>
#include "AppMgr/Application.h"
#include "AppMgr/ButtonMapping.h"
#include "AppMgr/MessageMapping.h"
#include "AppMgr/RequestMapping.h"
#include "AppMgr/CapabilitesContainer.h"
#include "AppMgr/SyncPManager.h"
#include "AppMgr/DeviceList.h"
#include "AppMgr/DeviceHandler.h"
#include "JSONHandler/ALRPCObjects/V2/DisplayCapabilities.h"
#include "JSONHandler/ALRPCObjects/V2/OnDriverDistraction.h"
#include "JSONHandler/ALRPCObjects/V2/VehicleType.h"
#include "JSONHandler/ALRPCObjects/V1/Language.h"
#include "JSONHandler/ALRPCObjects/V2/Language.h"

namespace NsAppLinkRPC
{
    class ALRPCMessage;
}
class JSONHandler;
class JSONRPC2Handler;

namespace NsAppLinkRPC
{
    class RegisterAppInterface_request;
    class OnDriverDistraction;
    class ALRPCRequest;
}

namespace NsRPC2Communication
{
    class RPC2Command;
}

namespace log4cplus
{
    class Logger;
}

namespace NsConnectionHandler
{
    class IDevicesDiscoveryStarter;
}

namespace NsAppManager
{

    class RegistryItem;
    template< class QueueType >
    class AppMgrCoreQueue;

    /**
     * \brief a connection between a mobile RPC message and a session
     */
    typedef std::pair<NsAppLinkRPC::ALRPCMessage*, ApplicationUniqueID> Message;

    /**
     * \brief a list of device names
     */
    typedef std::vector<std::string> DeviceNamesList;

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
         * \param connectionID id of a connection associated with application that sent the message
         * \param sessionID an id of a session associated with the application which pushes a message
         */
        void pushMobileRPCMessage(NsAppLinkRPC::ALRPCMessage * message , int connectionID, unsigned char sessionID);

        /**
         * \brief push HMI RPC2 message to a queue
         * \param message a message to be pushed
         */
        void pushRPC2CommunicationMessage( NsRPC2Communication::RPC2Command * message );

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

        /**
         * \brief Sets connection handler instance
         * \param handler connection handler
         */
        void setConnectionHandler(NsConnectionHandler::IDevicesDiscoveryStarter* handler);

        /**
         * \brief Gets connection handler instance
         * \return connection handler
         */
        NsConnectionHandler::IDevicesDiscoveryStarter* getConnectionHandler( ) const;

        /**
         * \brief set device list
         * \param deviceList device list
         */
        void setDeviceList( const NsConnectionHandler::tDeviceList& deviceList );

        /**
         * \brief get device list
         * \return device list
         */
        const NsConnectionHandler::tDeviceList& getDeviceList() const;

        /**
         * \brief add a device to a mapping
         * \param sessionKey session/connection key
         * \param device device handler
         */
        void addDevice( const int& sessionKey, const NsConnectionHandler::tDeviceHandle& device );

        /**
         * \brief remove a device from a mapping
         * \param sessionKey session/connection key
         */
        void removeDevice(const int& sessionKey);

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
        static void handleBusRPCMessageIncoming( NsRPC2Communication::RPC2Command* msg, void* pThis );

        /**
         * \brief Register an application
         * \param request a RegisterAppInterface request which is the source for application fields initial values
         * \param connectionID id of the connection which will be associated with the application
         * \param sessionID an id of the session which will be associated with the application
         * \return A instance of RegistryItem created for application
         */
        const RegistryItem* registerApplication(NsAppLinkRPC::ALRPCMessage *request , const unsigned int &connectionID, const unsigned char &sessionID);

        /**
         * \brief unregister an application associated with the given session
         * \param connectionID an id of the connection asociated with the application to be unregistered
         * \param sessionID an id of the session asociated with the application to be unregistered
         */
        void unregisterApplication(const unsigned int &connectionID, const unsigned char &sessionID);

        /**
         * \brief Remove all app components from HMI
         * \param currentApp app which components to be removed
         * \param connectionID connection id
         * \param sessionID session id
         */
        void removeAppFromHmi(Application* currentApp, const unsigned int& connectionID, const unsigned char &sessionID);

        /**
         * \brief retrieve an application instance from the RegistryItrem instance checking for non-null values
         * \param item a RegistryItem from which to retrieve an app pointer
         * \return Application instance retrieved from item
         */
        Application* getApplicationFromItemCheckNotNull( const RegistryItem* item ) const;

        /**
         * \brief serialize a string value to the text file
         * \param fileName name of the file to serialize to
         * \param value a value to serialize
         * \return success of an operation - true or false
         */
        bool serializeToFile(const std::string& fileName, const std::string &value) const;

        AppMgrCoreQueue<Message>* mQueueRPCAppLinkObjectsIncoming;
        AppMgrCoreQueue<NsRPC2Communication::RPC2Command*>* mQueueRPCBusObjectsIncoming;

        CapabilitiesContainer<NsAppLinkRPC::ButtonCapabilities> mButtonCapabilitiesV1;
        CapabilitiesContainer<NsAppLinkRPCV2::ButtonCapabilities> mButtonCapabilitiesV2;
        NsAppLinkRPC::DisplayCapabilities mDisplayCapabilitiesV1;
        NsAppLinkRPCV2::DisplayCapabilities mDisplayCapabilitiesV2;
        NsAppLinkRPCV2::PresetBankCapabilities mPresetBankCapabilities;
        CapabilitiesContainer<NsAppLinkRPC::HmiZoneCapabilities> mHmiZoneCapabilitiesV1;
        CapabilitiesContainer<NsAppLinkRPCV2::HmiZoneCapabilities> mHmiZoneCapabilitiesV2;
        CapabilitiesContainer<NsAppLinkRPC::VrCapabilities> mVrCapabilitiesV1;
        CapabilitiesContainer<NsAppLinkRPC::SpeechCapabilities> mSpeechCapabilitiesV1;
        CapabilitiesContainer<NsAppLinkRPCV2::VrCapabilities> mVrCapabilitiesV2;
        CapabilitiesContainer<NsAppLinkRPCV2::SpeechCapabilities> mSpeechCapabilitiesV2;
        CapabilitiesContainer<NsAppLinkRPCV2::SoftButtonCapabilities> mSoftButtonCapabilities;
        ButtonMapping    mButtonsMapping;
        MessageMapping   mMessageMapping;
        RequestMapping   mRequestMapping;
        DeviceList       mDeviceList;
        DeviceHandler    mDeviceHandler;

        NsAppLinkRPC::OnDriverDistraction* mDriverDistractionV1;
        NsAppLinkRPCV2::OnDriverDistraction* mDriverDistractionV2;

        NsAppLinkRPC::Language mUiLanguageV1;
        NsAppLinkRPC::Language mVrLanguageV1;
        NsAppLinkRPC::Language mTtsLanguageV1;
        NsAppLinkRPCV2::Language mUiLanguageV2;
        NsAppLinkRPCV2::Language mVrLanguageV2;
        NsAppLinkRPCV2::Language mTtsLanguageV2;

        NsAppLinkRPCV2::VehicleType mVehicleType;

        SyncPManager     mSyncPManager;

        static log4cplus::Logger mLogger;
    };

} // namespace NsAppManager

#endif /* APPMGR_H_ */
