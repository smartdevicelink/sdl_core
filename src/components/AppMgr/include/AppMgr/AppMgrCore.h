//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef APPMGR_H_
#define APPMGR_H_

#include <string>
#include "AppMgr/Application.h"
#include "AppMgr/ButtonMapping.h"
#include "AppMgr/VehicleDataMapping.h"
#include "AppMgr/CapabilitesContainer.h"
#include "AppMgr/SyncPManager.h"
#include "AppMgr/DeviceList.h"
#include "AppMgr/DeviceHandler.h"
#include "JSONHandler/SDLRPCObjects/V2/DisplayCapabilities.h"
#include "JSONHandler/SDLRPCObjects/V2/OnDriverDistraction.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonEvent.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/Buttons/OnButtonPress.h"
#include "JSONHandler/SDLRPCObjects/V2/VehicleType.h"
#include "JSONHandler/SDLRPCObjects/V1/Language.h"
#include "JSONHandler/SDLRPCObjects/V2/Language.h"
#include "AppMgr/MessageChaining.hpp"
#include "AppMgr/DeviceStorage.hpp"

#include "utils/threads/thread.h"

namespace NsSmartDeviceLinkRPC
{
    class SDLRPCMessage;
}
class JSONHandler;
class JSONRPC2Handler;

namespace NsSmartDeviceLinkRPC
{
    class RegisterAppInterface_request;
    class OnDriverDistraction;
    class SDLRPCRequest;
}

namespace NsRPC2Communication
{
    class RPC2Command;
}

namespace log4cplus
{
    class Logger;
}

namespace connection_handler
{
    class DevicesDiscoveryStarter;
}

namespace NsAppManager
{
    class RegistryItem;
    template< class QueueType >
    class AppMgrCoreQueue;

    /**
     * \brief a connection between a mobile RPC message and a session
     */
    typedef std::pair<NsSmartDeviceLinkRPC::SDLRPCMessage*, int> Message;

    /**
     * \brief a list of device names
     */
    typedef std::vector<std::string> DeviceNamesList;

    /**
      *\brief Map of messages between mobile app and hmi
    */
    typedef std::map<int, MessageChaining*> MessageChains;

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
         */
        void pushMobileRPCMessage(NsSmartDeviceLinkRPC::SDLRPCMessage * message , int appId);

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
        void setConnectionHandler(connection_handler::DevicesDiscoveryStarter* handler);

        /**
         * \brief Gets connection handler instance
         * \return connection handler
         */
        connection_handler::DevicesDiscoveryStarter* getConnectionHandler( ) const;

        /**
         * \brief set device list
         * \param deviceList device list
         */
        void setDeviceList( const connection_handler::DeviceList& deviceList );

        /**
         * \brief get device list
         * \return device list
         */
        const connection_handler::DeviceList& getDeviceList() const;

        /**
         * \brief add a device to a mapping
         * \param sessionKey session/connection key
         * \param device device handler
         */
        void addDevice( const connection_handler::DeviceHandle &device,
            const int &sessionKey, int firstSessionKey );

        /**
         * \brief remove a device from a mapping
         * \param sessionKey session/connection key
         */
        void removeDevice(const int &sessionKey, int firstSessionKey);

        bool getAudioPassThruFlag() const;
        void setAudioPassThruFlag(bool flag);

        //const MessageMapping& getMessageMapping() const;

        /**
         * \brief retrieve an application instance from the RegistryItrem instance checking for non-null values
         * \param item a RegistryItem from which to retrieve an app pointer
         * \return Application instance retrieved from item
         */
        Application* getApplicationFromItemCheckNotNull( const RegistryItem* item ) const;

        /**
         * \brief Sends notification to Mobile Application about changes in its HMI status
         * ie in system context/hmi level/audio streaming state.
         * \param application Mobile app to be notified about changes
         */
        void sendHMINotificationToMobile( Application * application );

        /**
         * \brief Activates app and deactivates others.
         * \param app Application to be activated.
         * \return bool Success of operation
         */
        bool performActivitiesForActivatingApp( Application * app );

        Application * getItem( int applicationId );

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
        const Application* registerApplication(NsSmartDeviceLinkRPC::SDLRPCMessage *request , int sessionKey);

        /**
         * \brief unregister an application associated with the given session
         * \param connectionID an id of the connection asociated with the application to be unregistered
         * \param sessionID an id of the session asociated with the application to be unregistered
         */
        void unregisterApplication(int appId);

        /**
         * \brief Remove all app components from HMI
         * \param currentApp app which components to be removed
         * \param connectionID connection id
         * \param sessionID session id
         */
        void removeAppFromHmi(Application* currentApp, int appId);

        /**
         * \brief serialize a string value to the text file
         * \param fileName name of the file to serialize to
         * \param value a value to serialize
         * \return bool success of an operation - true or false
         */
        bool serializeToFile(const std::string& fileName, const std::string &value) const;

        /**
         * \brief Sends notification to mobile app about button event.
         * \param app Application to receive notification
         * \param object Notification received from HMI.
         */
        void sendButtonEvent( Application * app, NsRPC2Communication::Buttons::OnButtonEvent * object );

        /**
         * \brief Send notification to mobile app about button press event
         * \param app Application to receive notification
         * \param object Notification received from HMI.
         */
        void sendButtonPress( Application * app, NsRPC2Communication::Buttons::OnButtonPress * object );

        /**
         * \brief Remove message from message chain between mobile app and hmi.
         * If counter == 0 then remove message completely.
         * \param chain Iterator in message map
         * \return bool True if chain was deleted otherwise false
         */
        bool decreaseMessageChain(const MessageChains::iterator & chain);

        /**
         * \brief Inserts message chain
         * \param chain Pointer to @MessageChain
         * \param connectionKey Id of connection for Mobile side
         * \param correlationID Correlation id for response for Mobile side
         * \return @MessageChaining* pointer to result chain
         */
        MessageChaining * addChain(MessageChaining * chain, int connectionKey, unsigned int correlationID);

        void differenceBetweenLists( const connection_handler::DeviceList &deviceList );

        Application * getActiveItem();

        Application * getApplicationByCommand(const unsigned int &cmdId, int appId);

        bool activateApp( Application * appToBeActivated );

        AppMgrCoreQueue<Message>* mQueueRPCSmartDeviceLinkObjectsIncoming;
        AppMgrCoreQueue<NsRPC2Communication::RPC2Command*>* mQueueRPCBusObjectsIncoming;

        //CapabilitiesContainer<NsSmartDeviceLinkRPC::ButtonCapabilities> mButtonCapabilitiesV1;
        CapabilitiesContainer<NsSmartDeviceLinkRPCV2::ButtonCapabilities> mButtonCapabilitiesV2;
        NsSmartDeviceLinkRPC::DisplayCapabilities mDisplayCapabilitiesV1;
        NsSmartDeviceLinkRPCV2::DisplayCapabilities mDisplayCapabilitiesV2;
        NsSmartDeviceLinkRPCV2::PresetBankCapabilities mPresetBankCapabilities;
        //CapabilitiesContainer<NsSmartDeviceLinkRPC::HmiZoneCapabilities> mHmiZoneCapabilitiesV1;
        CapabilitiesContainer<NsSmartDeviceLinkRPCV2::HmiZoneCapabilities> mHmiZoneCapabilitiesV2;
        //CapabilitiesContainer<NsSmartDeviceLinkRPC::VrCapabilities> mVrCapabilitiesV1;
        //CapabilitiesContainer<NsSmartDeviceLinkRPC::SpeechCapabilities> mSpeechCapabilitiesV1;
        CapabilitiesContainer<NsSmartDeviceLinkRPCV2::VrCapabilities> mVrCapabilitiesV2;
        CapabilitiesContainer<NsSmartDeviceLinkRPCV2::SpeechCapabilities> mSpeechCapabilitiesV2;
        CapabilitiesContainer<NsSmartDeviceLinkRPCV2::SoftButtonCapabilities> mSoftButtonCapabilities;
        ButtonMapping       mButtonsMapping;
        VehicleDataMapping  mVehicleDataMapping;
        //MessageMapping      mMessageMapping;
        //RequestMapping      mRequestMapping;
        //DeviceList          mDeviceList;
        DeviceHandler       mDeviceHandler;

        MessageChains mMessageChaining;

        NsSmartDeviceLinkRPC::OnDriverDistraction* mDriverDistractionV1;
        NsSmartDeviceLinkRPCV2::OnDriverDistraction* mDriverDistractionV2;

        NsSmartDeviceLinkRPC::Language mUiLanguageV1;
        NsSmartDeviceLinkRPC::Language mVrLanguageV1;
        NsSmartDeviceLinkRPC::Language mTtsLanguageV1;
        NsSmartDeviceLinkRPCV2::Language mUiLanguageV2;
        NsSmartDeviceLinkRPCV2::Language mVrLanguageV2;
        NsSmartDeviceLinkRPCV2::Language mTtsLanguageV2;
        std::vector<NsSmartDeviceLinkRPCV2::Language> mUISupportedLanguages;
        std::vector<NsSmartDeviceLinkRPCV2::Language> mVRSupportedLanguages;
        std::vector<NsSmartDeviceLinkRPCV2::Language> mTTSSupportedLanguages;

        NsSmartDeviceLinkRPCV2::VehicleType mVehicleType;
        bool mAudioPassThruFlag;
        int mPerformInteractionFlag;
        int mHMIStartupFlag;

        std::map<int, int> menuMapping;  // map<request_id, menu_id>

        SyncPManager     mSyncPManager;

        static log4cplus::Logger mLogger;

        std::map<int, Application*> mApplications;
        std::map<int, DeviceStorage> mDevices;
        //connection_handler::DeviceList mDevices;

        threads::Thread* perform_audio_thread_;
    };

} // namespace NsAppManager

#endif /* APPMGR_H_ */
