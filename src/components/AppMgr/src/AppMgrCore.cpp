/**
 * \file AppMgrCore.cpp
 * \brief App manager core functionality
 * \author vsalo
 */

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>

#include "AppMgr/AppMgrCore.h"
#include "AppMgr/AppMgrRegistry.h"
#include "AppMgr/AppPolicy.h"
#include "AppMgr/RegistryItem.h"
#include "AppMgr/AppMgrCoreQueues.h"
#include "AppMgr/SubscribeButton.h"
#include "AppMgr/HMIHandler.h"
#include "AppMgr/MobileHandler.h"
#include "AppMgr/ConnectionHandler.h"
#include "JSONHandler/ALRPCMessage.h"
#include "JSONHandler/ALRPCRequest.h"
#include "JSONHandler/ALRPCResponse.h"
#include "JSONHandler/ALRPCNotification.h"
#include "JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "JSONHandler/ALRPCObjects/V2/Marshaller.h"
#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"
#include "JSONHandler/RPC2Objects/Marshaller.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/ALRPCObjects/V2/AppType.h"
#include <sys/socket.h>
#include "LoggerHelper.hpp"
#include <iostream>
#include <fstream>
#include <cstddef>

namespace {
    unsigned long int getAvailableSpace() {
        char currentAppPath[256];
        memset((void*)currentAppPath, 0, 256);
        getcwd(currentAppPath, 255);

        struct statvfs fsInfo;
        memset((void*)&fsInfo, 0, sizeof(fsInfo));
        statvfs(currentAppPath, &fsInfo);
        return fsInfo.f_bsize * fsInfo.f_bfree;
    }
}

namespace NsAppManager
{
    log4cplus::Logger AppMgrCore::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));

    /**
     * \brief Returning class instance
     * \return class instance
     */
    AppMgrCore& AppMgrCore::getInstance( )
    {
        static AppMgrCore appMgr;
        return appMgr;
    }

    /**
     * \brief Default class constructor
     */
    AppMgrCore::AppMgrCore()
        :mQueueRPCAppLinkObjectsIncoming(new AppMgrCoreQueue<Message>(&AppMgrCore::handleMobileRPCMessage, this))
        ,mQueueRPCBusObjectsIncoming(new AppMgrCoreQueue<NsRPC2Communication::RPC2Command*>(&AppMgrCore::handleBusRPCMessageIncoming, this))
        ,mDriverDistraction(0)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore constructed!");
    }

    /**
     * \brief Copy constructor
     */
    AppMgrCore::AppMgrCore(const AppMgrCore &)
        :mQueueRPCAppLinkObjectsIncoming(0)
        ,mQueueRPCBusObjectsIncoming(0)
        ,mDriverDistraction(0)
    {
    }

    /**
     * \brief Default class destructor
     */
    AppMgrCore::~AppMgrCore()
    {
        if(mQueueRPCAppLinkObjectsIncoming)
            delete mQueueRPCAppLinkObjectsIncoming;
        if(mQueueRPCBusObjectsIncoming)
            delete mQueueRPCBusObjectsIncoming;
        if(mDriverDistraction)
            delete mDriverDistraction;

        LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore destructed!");
    }

    /**
     * \brief push mobile RPC message to a queue
     * \param message a message to be pushed
     * \param connectionID id of a connection associated with application that sent the message
     * \param sessionID an id of a session associated with the application which pushes a message
     */
    void AppMgrCore::pushMobileRPCMessage( NsAppLinkRPC::ALRPCMessage * message, int connectionID, unsigned char sessionID )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Pushing mobile RPC message for connection id " << connectionID << " session id " << (uint)sessionID << "...");
        if(!message)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Nothing to push! A null-ptr occured!");
            return;
        }

        mQueueRPCAppLinkObjectsIncoming->pushMessage(Message(message, ApplicationUniqueID(connectionID, sessionID)));

        LOG4CPLUS_INFO_EXT(mLogger, " Pushed mobile RPC message for connection id " << connectionID << " session id " << (uint)sessionID);
    }

    /**
     * \brief push HMI RPC2 message to a queue
     * \param message a message to be pushed
     */
    void AppMgrCore::pushRPC2CommunicationMessage( NsRPC2Communication::RPC2Command * message )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Returning a message from HMI...");
        if(!message)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Nothing to push! A null-ptr occured!");
            return;
        }

        mQueueRPCBusObjectsIncoming->pushMessage(message);

        LOG4CPLUS_INFO_EXT(mLogger, " Returned a message from HMI");
    }

    /**
     * \brief method to execute threads.
     */
    void AppMgrCore::executeThreads()
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Threads are being started!");

        mQueueRPCAppLinkObjectsIncoming->executeThreads();
        mQueueRPCBusObjectsIncoming->executeThreads();

        LOG4CPLUS_INFO_EXT(mLogger, " Threads have been started!");
    }

    /**
     * \brief mobile RPC message handler
     * \param mesage a message to be handled
     * \param pThis a pointer to AppMgrCore class instance
     */
    void AppMgrCore::handleMobileRPCMessage(Message message , void *pThis)
    {
        const unsigned int& connectionID = std::get<0>(message.second);
        const unsigned char& sessionID = std::get<1>(message.second);
        int appId = connectionID|(sessionID << 16);

        NsAppLinkRPC::ALRPCMessage* mobileMsg = message.first;
        if(!mobileMsg)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " No message associated with the connection " << connectionID << " session " << (uint)sessionID << " !");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC message " << mobileMsg->getMethodId() << " has been received for the connection " << connectionID << " session id " << (uint)sessionID << " !");
        LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC message " << mobileMsg->getMethodId() << " has been received for the app id == connection key == session key ==  " << appId << " !");
        if(!pThis)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " pThis should point to an instance of AppMgrCore class");
            return;
        }
        AppMgrCore* core = (AppMgrCore*)pThis;
        const unsigned int& protocolVersion = mobileMsg->getProtocolVersion();
        const NsConnectionHandler::tDeviceHandle& currentDeviceHandle = core->mDeviceHandler.findDeviceAssignedToSession(appId);
        const NsConnectionHandler::CDevice* currentDevice = core->mDeviceList.findDeviceByHandle(currentDeviceHandle);
        if(!currentDevice)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Cannot retreive current device name for the message with connection " << connectionID << " session " << (uint)sessionID << " !");
            return;
        }
        const std::string& currentDeviceName = currentDevice->getUserFriendlyName();

        LOG4CPLUS_INFO_EXT(mLogger, "Message received is from protocol " << protocolVersion);
        if ( 1 == mobileMsg->getProtocolVersion() )
        {
            switch(mobileMsg->getMethodId())
            {
                case NsAppLinkRPC::Marshaller::METHOD_REGISTERAPPINTERFACE_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request has been invoked");

                    NsAppLinkRPC::RegisterAppInterface_request * object = (NsAppLinkRPC::RegisterAppInterface_request*)mobileMsg;
                    NsAppLinkRPC::RegisterAppInterface_response* response = new NsAppLinkRPC::RegisterAppInterface_response();
                    const std::string& appName = object->get_appName();

                    if(AppMgrRegistry::getInstance().getItem(connectionID, sessionID))
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Application " << appName << " is already registered!");
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_REGISTERED_ALREADY);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }

                    Application_v1* app = (Application_v1*)core->getApplicationFromItemCheckNotNull(core->registerApplication( object, connectionID, sessionID ));
                    response->setCorrelationID(object->getCorrelationID());
                    response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Application " << appName << " hasn't been registered!");
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }

                    NsAppLinkRPC::OnHMIStatus* status = new NsAppLinkRPC::OnHMIStatus();
                    status->set_hmiLevel(app->getApplicationHMIStatusLevel());
                    status->set_audioStreamingState(app->getApplicationAudioStreamingState());
                    status->set_systemContext(app->getSystemContext());
                    MobileHandler::getInstance().sendRPCMessage(status, connectionID, sessionID);
                    LOG4CPLUS_INFO_EXT(mLogger, " An OnHMIStatus notification for the app "  << app->getName() << " connection/session key " << app->getAppID() << " gets sent to a mobile side... ");

                    response->set_buttonCapabilities(core->mButtonCapabilitiesV1.get());
                    response->set_displayCapabilities(core->mDisplayCapabilitiesV1);
                    response->set_hmiZoneCapabilities(core->mHmiZoneCapabilitiesV1.get());
                    response->set_speechCapabilities(core->mSpeechCapabilitiesV1.get());
                    response->set_vrCapabilities(core->mVrCapabilitiesV1.get());
                    response->set_language(app->getLanguageDesired());
                    response->set_syncMsgVersion(app->getSyncMsgVersion());
                    response->set_success(true);
                    response->set_resultCode(NsAppLinkRPC::Result::SUCCESS);

                    LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface response for the app "  << app->getName() << " connection/session key " << app->getAppID() << " gets sent to a mobile side... ");
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);

                    NsRPC2Communication::AppLinkCore::OnAppRegistered* appRegistered = new NsRPC2Communication::AppLinkCore::OnAppRegistered();
                    appRegistered->set_appName(app->getName());
                    appRegistered->set_appId(app->getAppID());
                    appRegistered->set_isMediaApplication(app->getIsMediaApplication());
                    appRegistered->set_languageDesired(app->getLanguageDesired());
                    appRegistered->set_vrSynonym(app->getVrSynonyms());
                    appRegistered->set_hmiDisplayLanguageDesired(app->getHMIDisplayLanguageDesired());
                    appRegistered->set_deviceName(currentDeviceName);
                    HMIHandler::getInstance().sendNotification(appRegistered);
                    LOG4CPLUS_INFO_EXT(mLogger, " An AppLinkCore::OnAppRegistered notofocation for the app "  << app->getName() << " connection/session key " << app->getAppID() << " gets sent to an HMI side... ");
                    LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request was successful: registered an app " << app->getName() << " connection/session key " << app->getAppID());
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_UNREGISTERAPPINTERFACE_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An UnregisterAppInterface request has been invoked");

                    NsAppLinkRPC::UnregisterAppInterface_request * object = (NsAppLinkRPC::UnregisterAppInterface_request*)mobileMsg;
                    Application* app = core->getApplicationFromItemCheckNotNull(AppMgrRegistry::getInstance().getItem(connectionID, sessionID));
                    NsAppLinkRPC::UnregisterAppInterface_response* response = new NsAppLinkRPC::UnregisterAppInterface_response();
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }
                    std::string appName = app->getName();

                    core->removeAppFromHmi(app, connectionID, sessionID);
                    core->unregisterApplication( connectionID, sessionID );

                    response->setCorrelationID(object->getCorrelationID());
                    response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                    response->set_success(true);
                    response->set_resultCode(NsAppLinkRPC::Result::SUCCESS);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);

                    NsAppLinkRPC::OnAppInterfaceUnregistered* msgUnregistered = new NsAppLinkRPC::OnAppInterfaceUnregistered();
                    msgUnregistered->set_reason(NsAppLinkRPC::AppInterfaceUnregisteredReason(NsAppLinkRPC::AppInterfaceUnregisteredReason::USER_EXIT));
                    MobileHandler::getInstance().sendRPCMessage(msgUnregistered, connectionID, sessionID);
                    NsRPC2Communication::AppLinkCore::OnAppUnregistered* appUnregistered = new NsRPC2Communication::AppLinkCore::OnAppUnregistered();
                    appUnregistered->set_appName(appName);
                    appUnregistered->set_appId(app->getAppID());
                    appUnregistered->set_reason(NsAppLinkRPC::AppInterfaceUnregisteredReason(NsAppLinkRPC::AppInterfaceUnregisteredReason::USER_EXIT));
                    HMIHandler::getInstance().sendNotification(appUnregistered);

                    LOG4CPLUS_INFO_EXT(mLogger, " An application " << appName << " has been unregistered successfully ");
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_SUBSCRIBEBUTTON_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A SubscribeButton request has been invoked");

                    /* RegistryItem* registeredApp = AppMgrRegistry::getInstance().getItem(sessionID);
                ButtonParams* params = new ButtonParams();
                params->mMessage = message;
                IAppCommand* command = new SubscribeButtonCmd(registeredApp, params);
                command->execute();
                delete command; */

                    NsAppLinkRPC::SubscribeButton_request * object = (NsAppLinkRPC::SubscribeButton_request*)mobileMsg;
                    NsAppLinkRPC::SubscribeButton_response* response = new NsAppLinkRPC::SubscribeButton_response();
                    RegistryItem* item = AppMgrRegistry::getInstance().getItem(connectionID, sessionID);
                    if(!item)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }
                    core->mButtonsMapping.addButton( object->get_buttonName(), item );

                    response->setCorrelationID(object->getCorrelationID());
                    response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                    response->set_success(true);
                    response->set_resultCode(NsAppLinkRPC::Result::SUCCESS);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_UNSUBSCRIBEBUTTON_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An UnsubscribeButton request has been invoked");
                    NsAppLinkRPC::UnsubscribeButton_request * object = (NsAppLinkRPC::UnsubscribeButton_request*)mobileMsg;
                    core->mButtonsMapping.removeButton( object->get_buttonName() );
                    NsAppLinkRPC::UnsubscribeButton_response* response = new NsAppLinkRPC::UnsubscribeButton_response();
                    response->setCorrelationID(object->getCorrelationID());
                    response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                    response->set_success(true);
                    response->set_resultCode(NsAppLinkRPC::Result::SUCCESS);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_SHOW_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A Show request has been invoked");
                    LOG4CPLUS_INFO_EXT(mLogger, "message " << mobileMsg->getMethodId() );
                    NsAppLinkRPC::Show_request* object = (NsAppLinkRPC::Show_request*)mobileMsg;
                    NsRPC2Communication::UI::Show* showRPC2Request = new NsRPC2Communication::UI::Show();
                    showRPC2Request->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    LOG4CPLUS_INFO_EXT(mLogger, "showrpc2request created");
                    if(object->get_mainField1())
                    {
                        showRPC2Request->set_mainField1(*object->get_mainField1());
                    }
                    LOG4CPLUS_INFO_EXT(mLogger, "setMainField1 was called");
                    if(object->get_mediaClock())
                    {
                        showRPC2Request->set_mainField2(*object->get_mainField2());
                    }
                    if(object->get_mediaClock())
                    {
                        showRPC2Request->set_mediaClock(*object->get_mediaClock());
                    }
                    if(object->get_statusBar())
                    {
                        showRPC2Request->set_statusBar(*object->get_statusBar());
                    }
                    if(object->get_alignment())
                    {
                        showRPC2Request->set_alignment(*object->get_alignment());
                    }
                    showRPC2Request->set_appId(appId);
                    LOG4CPLUS_INFO_EXT(mLogger, "Show request almost handled" );
                    core->mMessageMapping.addMessage(showRPC2Request->getId(), connectionID, sessionID);
                    HMIHandler::getInstance().sendRequest(showRPC2Request);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_SPEAK_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A Speak request has been invoked");
                    NsAppLinkRPC::Speak_request* object = (NsAppLinkRPC::Speak_request*)mobileMsg;
                    NsRPC2Communication::TTS::Speak* speakRPC2Request = new NsRPC2Communication::TTS::Speak();
                    speakRPC2Request->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    speakRPC2Request->set_ttsChunks(object->get_ttsChunks());
                    speakRPC2Request->set_appId(appId);
                    core->mMessageMapping.addMessage(speakRPC2Request->getId(), connectionID, sessionID);
                    HMIHandler::getInstance().sendRequest(speakRPC2Request);
                    NsAppLinkRPC::Speak_response * mobileResponse = new NsAppLinkRPC::Speak_response;
                    mobileResponse->set_resultCode(NsAppLinkRPC::Result::SUCCESS);
                    mobileResponse->set_success(true);
                    MobileHandler::getInstance().sendRPCMessage(mobileResponse, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_SETGLOBALPROPERTIES_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A SetGlobalProperties request has been invoked");
                    NsAppLinkRPC::SetGlobalProperties_request* object = (NsAppLinkRPC::SetGlobalProperties_request*)mobileMsg;
                    NsRPC2Communication::UI::SetGlobalProperties* setGPRPC2Request = new NsRPC2Communication::UI::SetGlobalProperties();
                    setGPRPC2Request->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(setGPRPC2Request->getId(), connectionID, sessionID);
                    if(object->get_helpPrompt())
                    {
                        setGPRPC2Request->set_helpPrompt(*object->get_helpPrompt());
                    }

                    if(object->get_timeoutPrompt())
                    {
                        setGPRPC2Request->set_timeoutPrompt(*object->get_timeoutPrompt());
                    }

                    setGPRPC2Request->set_appId(appId);
                    HMIHandler::getInstance().sendRequest(setGPRPC2Request);
                    NsAppLinkRPC::SetGlobalProperties_response * mobileResponse = new NsAppLinkRPC::SetGlobalProperties_response;
                    mobileResponse->set_success(true);
                    mobileResponse->set_resultCode(NsAppLinkRPC::Result::SUCCESS);
                    MobileHandler::getInstance().sendRPCMessage(mobileResponse, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_RESETGLOBALPROPERTIES_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A ResetGlobalProperties request has been invoked");
                    NsAppLinkRPC::ResetGlobalProperties_request* object = (NsAppLinkRPC::ResetGlobalProperties_request*)mobileMsg;
                    NsRPC2Communication::UI::ResetGlobalProperties* resetGPRPC2Request = new NsRPC2Communication::UI::ResetGlobalProperties();
                    resetGPRPC2Request->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(resetGPRPC2Request->getId(), connectionID, sessionID);
                    resetGPRPC2Request->set_properties(object->get_properties());
                    resetGPRPC2Request->set_appId(appId);
                    HMIHandler::getInstance().sendRequest(resetGPRPC2Request);
                    NsAppLinkRPC::ResetGlobalProperties_response * mobileResponse = new NsAppLinkRPC::ResetGlobalProperties_response;
                    mobileResponse->set_success(true);
                    mobileResponse->set_resultCode(NsAppLinkRPC::Result::SUCCESS);
                    MobileHandler::getInstance().sendRPCMessage(mobileResponse, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_ALERT_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An Alert request has been invoked");
                    NsAppLinkRPC::Alert_request* object = (NsAppLinkRPC::Alert_request*)mobileMsg;
                    NsRPC2Communication::UI::Alert* alert = new NsRPC2Communication::UI::Alert();
                    alert->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(alert->getId(), connectionID, sessionID);
                    if(object->get_alertText1())
                    {
                        alert->set_AlertText1(*object->get_alertText1());
                    }
                    if(object->get_alertText2())
                    {
                        alert->set_AlertText2(*object->get_alertText2());
                    }
                    if(object->get_duration())
                    {
                        alert->set_duration(*object->get_duration());
                    }
                    if(object->get_playTone())
                    {
                        alert->set_playTone(*object->get_playTone());
                    }
                    alert->set_appId(appId);
                    HMIHandler::getInstance().sendRequest(alert);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_ONBUTTONPRESS:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "OnButtonPress Notification has been received.");
                    MobileHandler::getInstance().sendRPCMessage(mobileMsg, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_ONCOMMAND:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "OnCommand Notification has been received.");
                    MobileHandler::getInstance().sendRPCMessage(mobileMsg, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_ADDCOMMAND_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand request has been invoked");

                    Application* app = AppMgrRegistry::getInstance().getApplication(connectionID, sessionID);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        NsAppLinkRPC::AddCommand_response* response = new NsAppLinkRPC::AddCommand_response();
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }
                    NsAppLinkRPC::AddCommand_request* object = (NsAppLinkRPC::AddCommand_request*)mobileMsg;

                    const unsigned int& cmdId = object->get_cmdID();

                    if(object->get_menuParams())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand UI request has been invoked");
                        NsRPC2Communication::UI::AddCommand * addCmd = new NsRPC2Communication::UI::AddCommand();
                        addCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                        CommandType cmdType = CommandType::UI;
                        const NsAppLinkRPC::MenuParams* menuParams = object->get_menuParams();
                        addCmd->set_menuParams(*menuParams);
                        addCmd->set_cmdId(cmdId);
                        addCmd->set_appId(app->getAppID());
                        if(menuParams->get_parentID())
                        {
                            const unsigned int& menuId = *menuParams->get_parentID();
                            app->addMenuCommand(cmdId, menuId);
                        }
                        core->mMessageMapping.addMessage(addCmd->getId(), connectionID, sessionID);

                        CommandParams params;
                        params.menuParams = menuParams;
                        app->addCommand(cmdId, cmdType, params);
                        app->incrementUnrespondedRequestCount(cmdId);
                        core->mRequestMapping.addMessage(addCmd->getId(), cmdId);
                        HMIHandler::getInstance().sendRequest(addCmd);

                    }
                    if(object->get_vrCommands())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand VR request has been invoked");
                        NsRPC2Communication::VR::AddCommand * addCmd = new NsRPC2Communication::VR::AddCommand();
                        addCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                        CommandType cmdType = CommandType::VR;
                        addCmd->set_vrCommands(*object->get_vrCommands());
                        addCmd->set_cmdId(cmdId);
                        addCmd->set_appId(app->getAppID());
                        core->mMessageMapping.addMessage(addCmd->getId(), connectionID, sessionID);
                        CommandParams params;
                        params.vrCommands = object->get_vrCommands();
                        app->addCommand(cmdId, cmdType, params);
                        app->incrementUnrespondedRequestCount(cmdId);
                        core->mRequestMapping.addMessage(addCmd->getId(), cmdId);
                        HMIHandler::getInstance().sendRequest(addCmd);
                    }

                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_DELETECOMMAND_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand request has been invoked");
                    Application* app = AppMgrRegistry::getInstance().getApplication(connectionID, sessionID);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        NsAppLinkRPC::DeleteCommand_response* response = new NsAppLinkRPC::DeleteCommand_response();
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }

                    NsAppLinkRPC::DeleteCommand_request* object = (NsAppLinkRPC::DeleteCommand_request*)mobileMsg;

                    CommandTypes cmdTypes = app->getCommandTypes(object->get_cmdID());
                    const unsigned int& cmdId = object->get_cmdID();
                    for(CommandTypes::iterator it = cmdTypes.begin(); it != cmdTypes.end(); it++)
                    {
                        CommandType cmdType = *it;
                        if(cmdType == CommandType::UI)
                        {
                            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand UI request has been invoked");
                            NsRPC2Communication::UI::DeleteCommand* deleteCmd = new NsRPC2Communication::UI::DeleteCommand();
                            deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                            deleteCmd->set_appId(app->getAppID());
                            core->mMessageMapping.addMessage(deleteCmd->getId(), connectionID, sessionID);
                            deleteCmd->set_cmdId(cmdId);
                            app->removeCommand(cmdId, cmdType);
                            app->incrementUnrespondedRequestCount(cmdId);
                            app->removeMenuCommand(cmdId);
                            core->mRequestMapping.addMessage(deleteCmd->getId(), cmdId);
                            HMIHandler::getInstance().sendRequest(deleteCmd);
                        }
                        else if(cmdType == CommandType::VR)
                        {
                            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand VR request has been invoked");
                            NsRPC2Communication::VR::DeleteCommand* deleteCmd = new NsRPC2Communication::VR::DeleteCommand();
                            deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                            core->mMessageMapping.addMessage(deleteCmd->getId(), connectionID, sessionID);
                            deleteCmd->set_cmdId(cmdId);
                            deleteCmd->set_appId(app->getAppID());
                            app->removeCommand(cmdId, cmdType);
                            app->incrementUnrespondedRequestCount(cmdId);
                            core->mRequestMapping.addMessage(deleteCmd->getId(), cmdId);
                            HMIHandler::getInstance().sendRequest(deleteCmd);
                        }
                    }
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_ADDSUBMENU_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An AddSubmenu request has been invoked");
                    Application* app = AppMgrRegistry::getInstance().getApplication(connectionID, sessionID);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        NsAppLinkRPC::AddSubMenu_response* response = new NsAppLinkRPC::AddSubMenu_response();
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }

                    NsAppLinkRPC::AddSubMenu_request* object = (NsAppLinkRPC::AddSubMenu_request*)mobileMsg;
                    NsRPC2Communication::UI::AddSubMenu* addSubMenu = new NsRPC2Communication::UI::AddSubMenu();
                    addSubMenu->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(addSubMenu->getId(), connectionID, sessionID);
                    addSubMenu->set_menuId(object->get_menuID());
                    addSubMenu->set_menuName(object->get_menuName());
                    if(object->get_position())
                    {
                        addSubMenu->set_position(*object->get_position());
                    }
                    addSubMenu->set_appId(app->getAppID());
                    app->addMenu(object->get_menuID(), object->get_menuName(), object->get_position());
                    HMIHandler::getInstance().sendRequest(addSubMenu);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_DELETESUBMENU_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A DeleteSubmenu request has been invoked");
                    NsAppLinkRPC::DeleteSubMenu_request* object = (NsAppLinkRPC::DeleteSubMenu_request*)mobileMsg;
                    Application* app = AppMgrRegistry::getInstance().getApplication(connectionID, sessionID);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        NsAppLinkRPC::DeleteSubMenu_response* response = new NsAppLinkRPC::DeleteSubMenu_response;
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }
                    NsRPC2Communication::UI::DeleteSubMenu* delSubMenu = new NsRPC2Communication::UI::DeleteSubMenu();
                    delSubMenu->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(delSubMenu->getId(), connectionID, sessionID);
                    const unsigned int& menuId = object->get_menuID();
                    delSubMenu->set_menuId(menuId);
                    delSubMenu->set_appId(app->getAppID());
                    const MenuCommands& menuCommands = app->findMenuCommands(menuId);
                    LOG4CPLUS_INFO_EXT(mLogger, " A given menu has " << menuCommands.size() << " UI commands - about to delete 'em!");
                    for(MenuCommands::const_iterator it = menuCommands.begin(); it != menuCommands.end(); it++)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " Deleting command with id " << *it);
                        NsRPC2Communication::UI::DeleteCommand* delUiCmd = new NsRPC2Communication::UI::DeleteCommand();
                        delUiCmd->set_cmdId(*it);
                        delUiCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                        delUiCmd->set_appId(app->getAppID());
                        core->mMessageMapping.addMessage(delUiCmd->getId(), connectionID, sessionID);
                        core->mRequestMapping.addMessage(delUiCmd->getId(), *it);
                        HMIHandler::getInstance().sendRequest(delUiCmd);
                        const CommandTypes& types = app->getCommandTypes(*it);
                        for(CommandTypes::const_iterator it2 = types.begin(); it2 != types.end(); it2++)
                        {
                            const CommandType& type = *it2;
                            if(type == CommandType::VR)
                            {
                                LOG4CPLUS_INFO_EXT(mLogger, " A given command id " << *it << " has VR counterpart attached to: deleting it also!");
                                NsRPC2Communication::VR::DeleteCommand* delVrCmd = new NsRPC2Communication::VR::DeleteCommand();
                                delVrCmd->set_cmdId(*it);
                                delVrCmd->set_appId(app->getAppID());
                                core->mMessageMapping.addMessage(delVrCmd->getId(), connectionID, sessionID);
                                core->mRequestMapping.addMessage(delVrCmd->getId(), *it);
                                HMIHandler::getInstance().sendRequest(delVrCmd);
                            }
                        }
                        app->removeCommand(*it, CommandType::UI);
                        app->removeMenuCommand(*it);
                    }
                    app->removeMenu(menuId);
                    HMIHandler::getInstance().sendRequest(delSubMenu);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_CREATEINTERACTIONCHOICESET_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A CreateInteractionChoiceSet request has been invoked");
                    NsAppLinkRPC::CreateInteractionChoiceSet_request* object = (NsAppLinkRPC::CreateInteractionChoiceSet_request*)mobileMsg;
                    Application* app = AppMgrRegistry::getInstance().getApplication(connectionID, sessionID);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        NsAppLinkRPC::CreateInteractionChoiceSet_response* response = new NsAppLinkRPC::CreateInteractionChoiceSet_response;
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }
                    NsRPC2Communication::UI::CreateInteractionChoiceSet* createInteractionChoiceSet = new NsRPC2Communication::UI::CreateInteractionChoiceSet();
                    createInteractionChoiceSet->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(createInteractionChoiceSet->getId(), connectionID, sessionID);
                    createInteractionChoiceSet->set_choiceSet(object->get_choiceSet());
                    createInteractionChoiceSet->set_interactionChoiceSetID(object->get_interactionChoiceSetID());
                    createInteractionChoiceSet->set_appId(app->getAppID());
                    app->addChoiceSet(object->get_interactionChoiceSetID(), object->get_choiceSet());
                    HMIHandler::getInstance().sendRequest(createInteractionChoiceSet);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_DELETEINTERACTIONCHOICESET_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A DeleteInteractionChoiceSet request has been invoked");
                    NsAppLinkRPC::DeleteInteractionChoiceSet_request* object = (NsAppLinkRPC::DeleteInteractionChoiceSet_request*)mobileMsg;
                    Application* app = AppMgrRegistry::getInstance().getApplication(connectionID, sessionID);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        NsAppLinkRPC::DeleteInteractionChoiceSet_response* response = new NsAppLinkRPC::DeleteInteractionChoiceSet_response;
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }
                    NsRPC2Communication::UI::DeleteInteractionChoiceSet* deleteInteractionChoiceSet = new NsRPC2Communication::UI::DeleteInteractionChoiceSet();
                    deleteInteractionChoiceSet->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(deleteInteractionChoiceSet->getId(), connectionID, sessionID);
                    deleteInteractionChoiceSet->set_interactionChoiceSetID(object->get_interactionChoiceSetID());
                    deleteInteractionChoiceSet->set_appId(app->getAppID());
                    app->removeChoiceSet(object->get_interactionChoiceSetID());
                    HMIHandler::getInstance().sendRequest(deleteInteractionChoiceSet);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_PERFORMINTERACTION_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A PerformInteraction request has been invoked");
                    NsAppLinkRPC::PerformInteraction_request* object = (NsAppLinkRPC::PerformInteraction_request*)mobileMsg;
                    NsRPC2Communication::UI::PerformInteraction* performInteraction = new NsRPC2Communication::UI::PerformInteraction();
                    performInteraction->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(performInteraction->getId(), connectionID, sessionID);
                    if(object->get_helpPrompt())
                    {
                        performInteraction->set_helpPrompt(*object->get_helpPrompt());
                    }
                    performInteraction->set_initialPrompt(object->get_initialPrompt());
                    performInteraction->set_initialText(object->get_initialText());
                    performInteraction->set_interactionChoiceSetIDList(object->get_interactionChoiceSetIDList());
                    performInteraction->set_interactionMode(object->get_interactionMode());
                    if(object->get_timeout())
                    {
                        performInteraction->set_timeout(*object->get_timeout());
                    }
                    if(object->get_timeoutPrompt())
                    {
                        performInteraction->set_timeoutPrompt(*object->get_timeoutPrompt());
                    }
                    performInteraction->set_appId(appId);
                    HMIHandler::getInstance().sendRequest(performInteraction);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_SETMEDIACLOCKTIMER_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A SetMediaClockTimer request has been invoked");
                    NsRPC2Communication::UI::SetMediaClockTimer* setTimer = new NsRPC2Communication::UI::SetMediaClockTimer();
                    setTimer->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    setTimer->set_appId(appId);
                    core->mMessageMapping.addMessage(setTimer->getId(), connectionID, sessionID);

                    NsAppLinkRPC::SetMediaClockTimer_request* object = (NsAppLinkRPC::SetMediaClockTimer_request*)mobileMsg;
                    if(object->get_startTime())
                    {
                        setTimer->set_startTime(*object->get_startTime());
                    }
                    setTimer->set_updateMode(object->get_updateMode());
                    HMIHandler::getInstance().sendRequest(setTimer);

                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_ENCODEDSYNCPDATA_REQUEST:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An EncodedSyncPData request has been invoked");

                    NsAppLinkRPC::EncodedSyncPData_request* object = (NsAppLinkRPC::EncodedSyncPData_request*)mobileMsg;
                    NsAppLinkRPC::EncodedSyncPData_response* response = new NsAppLinkRPC::EncodedSyncPData_response;
                    if(object->get_data())
                    {
                        Application* app = core->getApplicationFromItemCheckNotNull( AppMgrRegistry::getInstance().getItem(connectionID, sessionID) );
                        const std::string& name = app->getName();
                        core->mSyncPManager.setPData(*object->get_data(), name, object->getMethodId());
                        response->set_success(true);
                        response->set_resultCode(NsAppLinkRPC::Result::SUCCESS);
                    }
                    else
                    {
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPC::Result::INVALID_DATA);
                    }

                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_SHOW_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_SPEAK_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_SETGLOBALPROPERTIES_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_RESETGLOBALPROPERTIES_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_REGISTERAPPINTERFACE_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_SUBSCRIBEBUTTON_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_UNSUBSCRIBEBUTTON_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_ONAPPINTERFACEUNREGISTERED:
                case NsAppLinkRPC::Marshaller::METHOD_ALERT_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_ADDCOMMAND_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_ADDSUBMENU_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_CREATEINTERACTIONCHOICESET_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_DELETECOMMAND_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_DELETEINTERACTIONCHOICESET_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_DELETESUBMENU_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_ENCODEDSYNCPDATA_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_GENERICRESPONSE_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_PERFORMINTERACTION_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_SETMEDIACLOCKTIMER_RESPONSE:
                case NsAppLinkRPC::Marshaller::METHOD_UNREGISTERAPPINTERFACE_RESPONSE:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A " << mobileMsg->getMethodId() << " response or notification has been invoked");
                    MobileHandler::getInstance().sendRPCMessage(mobileMsg, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPC::Marshaller::METHOD_INVALID:
                default:
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, " An undefined or invalid RPC message " << mobileMsg->getMethodId() << " has been received!");

                    NsAppLinkRPC::GenericResponse_response* response = new NsAppLinkRPC::GenericResponse_response();
                    response->set_success(false);
                    response->set_resultCode(NsAppLinkRPC::Result::INVALID_DATA);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
            }
        }
        else if ( 2 == mobileMsg->getProtocolVersion() )
        {
            LOG4CPLUS_INFO_EXT(mLogger,"Received message of version 2.");
            switch(mobileMsg->getMethodId())
            {
                case NsAppLinkRPCV2::FunctionID::RegisterAppInterfaceID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "Message id is NsAppLinkRPCV2::FunctionID::RegisterAppInterfaceID");
                    NsAppLinkRPCV2::RegisterAppInterface_request * object = (NsAppLinkRPCV2::RegisterAppInterface_request*)mobileMsg;
                    NsAppLinkRPCV2::RegisterAppInterface_response* response = new NsAppLinkRPCV2::RegisterAppInterface_response();
                    const std::string& appName = object->get_appName();

                    if(AppMgrRegistry::getInstance().getItem(connectionID, sessionID))
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Application " << appName << " is already registered!");
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPCV2::Result::APPLICATION_REGISTERED_ALREADY);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }

                    Application_v2* app = (Application_v2*)core->getApplicationFromItemCheckNotNull(core->registerApplication( object, connectionID, sessionID ));
                    response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Application " << appName << " hasn't been registered!");
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPCV2::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }

                    NsAppLinkRPCV2::OnHMIStatus* status = new NsAppLinkRPCV2::OnHMIStatus();
                    status->set_hmiLevel(app->getApplicationHMIStatusLevel());
                    status->set_audioStreamingState(app->getApplicationAudioStreamingState());
                    status->set_systemContext(app->getSystemContext());
                    MobileHandler::getInstance().sendRPCMessage(status, connectionID, sessionID);

                    response->set_buttonCapabilities(core->mButtonCapabilitiesV2.get());
                    response->set_displayCapabilities(core->mDisplayCapabilitiesV2);
                    response->set_hmiZoneCapabilities(core->mHmiZoneCapabilitiesV2.get());
                    response->set_hmiDisplayLanguage(app->getHMIDisplayLanguageDesired());
                    response->set_language(app->getLanguageDesired());
                    response->set_speechCapabilities(core->mSpeechCapabilitiesV2.get());
                    response->set_vrCapabilities(core->mVrCapabilitiesV2.get());
                    response->set_syncMsgVersion(app->getSyncMsgVersion());
                    response->set_softButtonCapabilities(core->mSoftButtonCapabilities.get());
                    response->set_presetBankCapabilities(core->mPresetBankCapabilities);
                    response->set_success(true);
                    response->set_resultCode(NsAppLinkRPCV2::Result::SUCCESS);

                    LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface response for the app "  << app->getName() << " gets sent to a mobile side... ");
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);

                    NsRPC2Communication::AppLinkCore::OnAppRegistered* appRegistered = new NsRPC2Communication::AppLinkCore::OnAppRegistered();
                    appRegistered->set_appName(app->getName());
                    appRegistered->set_isMediaApplication(app->getIsMediaApplication());
                    const NsAppLinkRPCV2::Language& languageDesired = app->getLanguageDesired();
                    NsAppLinkRPC::Language languageDesiredV1;
                    languageDesiredV1.set((NsAppLinkRPC::Language::LanguageInternal)languageDesired.get());
                    appRegistered->set_languageDesired(languageDesiredV1);
                    appRegistered->set_vrSynonym(app->getVrSynonyms());
                    appRegistered->set_appId(app->getAppID());
                    appRegistered->set_appType(app->getAppType());
                    const NsAppLinkRPCV2::Language& hmiLanguageDesired = app->getHMIDisplayLanguageDesired();
                    NsAppLinkRPC::Language hmiLanguageDesiredV1;
                    hmiLanguageDesiredV1.set((NsAppLinkRPC::Language::LanguageInternal)hmiLanguageDesired.get());
                    appRegistered->set_hmiDisplayLanguageDesired(hmiLanguageDesiredV1);
                    appRegistered->set_vrSynonym(app->getVrSynonyms());
                    appRegistered->set_deviceName(currentDeviceName);
                    appRegistered->set_ttsName(*(std::vector<NsAppLinkRPC::TTSChunk>*)&app->getTtsName());
                    HMIHandler::getInstance().sendNotification(appRegistered);
                    LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request was successful: registered an app " << app->getName());
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::UnregisterAppInterfaceID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An UnregisterAppInterface request has been invoked");
                    NsAppLinkRPCV2::UnregisterAppInterface_request * object = (NsAppLinkRPCV2::UnregisterAppInterface_request*)mobileMsg;
                    Application* app = core->getApplicationFromItemCheckNotNull(AppMgrRegistry::getInstance().getItem(connectionID, sessionID));
                    NsAppLinkRPCV2::UnregisterAppInterface_response* response = new NsAppLinkRPCV2::UnregisterAppInterface_response();
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " Connection " << connectionID << " and session " << (uint)sessionID << " haven't been associated with any application!");
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPCV2::Result::APPLICATION_NOT_REGISTERED);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                        break;
                    }
                    std::string appName = app->getName();

                    core->removeAppFromHmi(app, connectionID, sessionID);
                    core->unregisterApplication( connectionID, sessionID );

                    response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                    response->set_success(true);
                    response->set_resultCode(NsAppLinkRPCV2::Result::SUCCESS);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);

                    NsAppLinkRPCV2::OnAppInterfaceUnregistered* msgUnregistered = new NsAppLinkRPCV2::OnAppInterfaceUnregistered();
                    msgUnregistered->set_reason(NsAppLinkRPCV2::AppInterfaceUnregisteredReason(NsAppLinkRPCV2::AppInterfaceUnregisteredReason::USER_EXIT));
                    MobileHandler::getInstance().sendRPCMessage(msgUnregistered, connectionID, sessionID);
                    NsRPC2Communication::AppLinkCore::OnAppUnregistered* appUnregistered = new NsRPC2Communication::AppLinkCore::OnAppUnregistered();
                    appUnregistered->set_appName(appName);
                    appUnregistered->set_appId(app->getAppID());
                    appUnregistered->set_reason(NsAppLinkRPC::AppInterfaceUnregisteredReason((NsAppLinkRPC::AppInterfaceUnregisteredReason::AppInterfaceUnregisteredReasonInternal)NsAppLinkRPCV2::AppInterfaceUnregisteredReason::USER_EXIT));
                    HMIHandler::getInstance().sendNotification(appUnregistered);
                    LOG4CPLUS_INFO_EXT(mLogger, " An application " << appName << " has been unregistered successfully ");
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::SetMediaClockTimerID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A SetMediaClockTimer request has been invoked");
                    NsRPC2Communication::UI::SetMediaClockTimer* setTimer = new NsRPC2Communication::UI::SetMediaClockTimer();
                    setTimer->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    setTimer->set_appId(appId);
                    core->mMessageMapping.addMessage(setTimer->getId(), connectionID, sessionID);

                    NsAppLinkRPCV2::SetMediaClockTimer_request* object = (NsAppLinkRPCV2::SetMediaClockTimer_request*)mobileMsg;
                    if(object->get_startTime())
                    {
                        NsAppLinkRPC::StartTime startTime;
                        const NsAppLinkRPCV2::StartTime& startTimeV2 = *object->get_startTime();
                        startTime.set_hours(startTimeV2.get_hours());
                        startTime.set_minutes(startTimeV2.get_minutes());
                        startTime.set_seconds(startTimeV2.get_seconds());
                        setTimer->set_startTime(startTime);
                    }

                    const NsAppLinkRPCV2::UpdateMode& updateModeV2 = object->get_updateMode();
                    NsAppLinkRPC::UpdateMode updateMode;
                    updateMode.set((NsAppLinkRPC::UpdateMode::UpdateModeInternal)updateModeV2.get());
                    setTimer->set_updateMode(updateMode);
                    HMIHandler::getInstance().sendRequest(setTimer);
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::PutFileID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An PutFile request has been invoked");
                    NsAppLinkRPCV2::PutFile_request* object = (NsAppLinkRPCV2::PutFile_request*)mobileMsg;
                    NsAppLinkRPCV2::PutFile_response* response = new NsAppLinkRPCV2::PutFile_response;

                    unsigned long int freeSpace = getAvailableSpace();
                    const std::string& syncFileName = object->get_syncFileName();
                    const NsAppLinkRPCV2::FileType& fileType = object->get_fileType();
                    bool persistentFile = object->get_persistentFile();
                    const std::vector<unsigned char>* fileData = object->getBinaryData();

                    bool isSyncFileName = !syncFileName.empty();
                    bool isFileData = fileData && !fileData->empty();
                    if (isSyncFileName && isFileData)
                    {
                        bool flag = false;
                        if (freeSpace > fileData->size())
                        {
                            struct stat status;
                            memset(&status, 0, sizeof(status));
                            if (stat(syncFileName.c_str(), &status) == -1) // File doesn't exist
                            {
                                Application* app = core->getApplicationFromItemCheckNotNull(
                                                        AppMgrRegistry::getInstance().getItem(connectionID, sessionID));

                                const std::string& name = app->getName();
                                const int& id = app->getAppID();

                                char path[FILENAME_MAX];
                                memset(path, 0, FILENAME_MAX);
                                snprintf(path, FILENAME_MAX - 1, "%s_%d/%s", name.c_str(), id, syncFileName.c_str());
                                std::ofstream file(path, std::ios_base::binary);
                                if (file.is_open())
                                {
                                    for (int i = 0; i < fileData->size(); ++i)
                                    {
                                        file << fileData->operator[](i);
                                    }
                                    file.close();
                                    flag = true;
                                }
                            }
                        }

                        if (flag)
                        {
                            response->set_success(true);
                            response->set_resultCode(NsAppLinkRPCV2::Result::SUCCESS);
                            response->set_spaceAvailable(freeSpace);
                        }
                        else
                        {
                            response->set_success(false);
                            response->set_resultCode(NsAppLinkRPCV2::Result::GENERIC_ERROR);
                            response->set_spaceAvailable(freeSpace);
                        }
                    }
                    else
                    {
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPCV2::Result::INVALID_DATA);
                        response->set_spaceAvailable(freeSpace);
                    }

                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::DeleteFileID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An DeleteFile request has been invoked");
                    NsAppLinkRPCV2::DeleteFile_request* object = (NsAppLinkRPCV2::DeleteFile_request*)mobileMsg;
                    NsAppLinkRPCV2::DeleteFile_response* response = new NsAppLinkRPCV2::DeleteFile_response;

                    unsigned long int freeSpace = getAvailableSpace();
                    const std::string& syncFileName = object->get_syncFileName();
                    if(!syncFileName.empty())
                    {
                        Application* app = core->getApplicationFromItemCheckNotNull(
                                    AppMgrRegistry::getInstance().getItem(connectionID, sessionID));

                        const std::string& name = app->getName();
                        const int& id = app->getAppID();

                        char path[FILENAME_MAX];
                        memset(path, 0, FILENAME_MAX);
                        snprintf(path, FILENAME_MAX - 1, "%s_%d/%s", name.c_str(), id, syncFileName.c_str());
                        if(remove(path) != 0)
                        {
                            response->set_success(false);
                            response->set_resultCode(NsAppLinkRPCV2::Result::GENERIC_ERROR);
                            response->set_spaceAvailable(freeSpace);
                        }
                        else
                        {
                            response->set_success(true);
                            response->set_resultCode(NsAppLinkRPCV2::Result::SUCCESS);
                            response->set_spaceAvailable(freeSpace);
                        }
                    }
                    else
                    {
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPCV2::Result::INVALID_DATA);
                        response->set_spaceAvailable(freeSpace);
                    }

                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::ListFilesID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An ListFiles request has been invoked");
                    NsAppLinkRPCV2::ListFiles_request* object = (NsAppLinkRPCV2::ListFiles_request*)mobileMsg;
                    NsAppLinkRPCV2::ListFiles_response* response = new NsAppLinkRPCV2::ListFiles_response;

                    std::vector<std::string> listFiles;
                    unsigned long int freeSpace = getAvailableSpace();

                    Application* app = core->getApplicationFromItemCheckNotNull(
                                    AppMgrRegistry::getInstance().getItem(connectionID, sessionID));

                    const std::string& name = app->getName();
                    const int& id = app->getAppID();

                    char path[FILENAME_MAX];
                    memset(path, 0, FILENAME_MAX);
                    snprintf(path, FILENAME_MAX - 1, "%s_%d/", name.c_str(), id);

                    DIR* dir = NULL;
                    struct dirent* dirElement = NULL;
                    memset(dirElement, 0, sizeof(dirent));
                    dir = opendir(path);
                    if (dir != NULL)
                    {
                        while (dirElement = readdir(dir))
                        {
                            LOG4CPLUS_INFO_EXT(mLogger, " file: " << dirElement->d_name);
                            listFiles.push_back(std::string(dirElement->d_name));
                        }
                        closedir(dir);

                        response->set_filenames(listFiles);
                        response->set_success(true);
                        response->set_resultCode(NsAppLinkRPCV2::Result::SUCCESS);
                        response->set_spaceAvailable(freeSpace);
                    }
                    else
                    {
                        response->set_success(false);
                        response->set_resultCode(NsAppLinkRPCV2::Result::GENERIC_ERROR);
                        response->set_spaceAvailable(freeSpace);
                    }

                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::SliderID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A Slider request has been invoked");
                    NsAppLinkRPCV2::Slider_request* request = (NsAppLinkRPCV2::Slider_request*)mobileMsg;
                    NsRPC2Communication::UI::Slider* slider = new NsRPC2Communication::UI::Slider();

                    slider->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(slider->getId(), connectionID, sessionID);

                    slider->set_numTicks(request->get_numTicks());

                    slider->set_sliderHeader(request->get_sliderHeader());

                    if (request->get_sliderFooter())
                    {
                        slider->set_sliderFooter(*(request->get_sliderFooter()));
                    }

                    slider->set_position(request->get_position());
                    slider->set_timeout(request->get_timeout());
                    HMIHandler::getInstance().sendRequest(slider);
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::EncodedSyncPDataID:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " An EncodedSyncPData request has been invoked");
                    NsAppLinkRPCV2::EncodedSyncPData_request* object = (NsAppLinkRPCV2::EncodedSyncPData_request*)mobileMsg;
                    NsAppLinkRPCV2::EncodedSyncPData_response* response = new NsAppLinkRPCV2::EncodedSyncPData_response;
                    Application* app = core->getApplicationFromItemCheckNotNull( AppMgrRegistry::getInstance().getItem(connectionID, sessionID) );
                    const std::string& name = app->getName();
                    core->mSyncPManager.setPData(object->get_data(), name, object->getMethodId());
                    response->set_success(true);
                    response->set_resultCode(NsAppLinkRPCV2::Result::SUCCESS);

                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                case NsAppLinkRPCV2::FunctionID::INVALID_ENUM:
                default:
                {
                    NsAppLinkRPCV2::GenericResponse_response* response = new NsAppLinkRPCV2::GenericResponse_response();
                    response->setMethodId(NsAppLinkRPCV2::FunctionID::GenericResponseID);
                    response->set_success(false);
                    response->set_resultCode(NsAppLinkRPCV2::Result::INVALID_DATA);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
            }
        }
    }

    /**
     * \brief push HMI RPC2 message to a queue
     * \param msg a message to be pushed
     * \param pThis a pointer to AppMgrCore class instance
     */
    void AppMgrCore::handleBusRPCMessageIncoming(NsRPC2Communication::RPC2Command* msg , void *pThis)
    {
        if(!msg)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Incoming null pointer from HMI side!");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message " << msg->getMethod() << " has been incoming...");

        if(!pThis)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " pThis should point to an instance of AppMgrCore class");
            return;
        }
        AppMgrCore* core = (AppMgrCore*)pThis;
        switch(msg->getMethod())
        {
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONEVENT:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonEvent notification has been invoked");
                NsRPC2Communication::Buttons::OnButtonEvent * object = (NsRPC2Communication::Buttons::OnButtonEvent*)msg;
                const NsAppLinkRPC::ButtonName & name = object->get_name();
                Application* app = core->getApplicationFromItemCheckNotNull(core->mButtonsMapping.findRegistryItemSubscribedToButton(name));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::OnButtonEvent* event = new NsAppLinkRPC::OnButtonEvent();
                event->set_buttonEventMode(object->get_mode());

                event->set_buttonName(name);

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__ONBUTTONPRESS:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonPress notification has been invoked");
                NsRPC2Communication::Buttons::OnButtonPress * object = (NsRPC2Communication::Buttons::OnButtonPress*)msg;
                const NsAppLinkRPC::ButtonName & name = object->get_name();
                Application* app = core->getApplicationFromItemCheckNotNull(core->mButtonsMapping.findRegistryItemSubscribedToButton(name));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::OnButtonPress* event = new NsAppLinkRPC::OnButtonPress();

                event->set_buttonName(name);
                event->set_buttonPressMode(object->get_mode());
                LOG4CPLUS_INFO_EXT(mLogger, "before we find sessionID");

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A GetButtonCapabilities response has been income");
                NsRPC2Communication::Buttons::GetCapabilitiesResponse * btnCaps = (NsRPC2Communication::Buttons::GetCapabilitiesResponse*)msg;
                core->mButtonCapabilitiesV1.set( btnCaps->get_capabilities() );
                core->mButtonCapabilitiesV2.set( *((std::vector< NsAppLinkRPCV2::ButtonCapabilities>*)&btnCaps->get_capabilities()) );
                if(btnCaps->get_presetBankCapabilities())
                {
                    core->mPresetBankCapabilities = *btnCaps->get_presetBankCapabilities();
                }
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_INVALID:
            default:
                LOG4CPLUS_ERROR_EXT(mLogger, " Not Buttons RPC message " << msg->getMethod() << " has been received!");
        }

        switch(msg->getMethod())
        {
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONREADY:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnReady UI notification has been invoked");
                HMIHandler::getInstance().setReadyState(true);

                NsRPC2Communication::UI::GetCapabilities* getUiCapsRequest = new NsRPC2Communication::UI::GetCapabilities();
                HMIHandler::getInstance().sendRequest(getUiCapsRequest);
                NsRPC2Communication::VR::GetCapabilities* getVrCapsRequest = new NsRPC2Communication::VR::GetCapabilities();
                HMIHandler::getInstance().sendRequest(getVrCapsRequest);
                NsRPC2Communication::TTS::GetCapabilities* getTtsCapsRequest = new NsRPC2Communication::TTS::GetCapabilities();
                HMIHandler::getInstance().sendRequest(getTtsCapsRequest);
                NsRPC2Communication::Buttons::GetCapabilities* getButtonsCapsRequest = new NsRPC2Communication::Buttons::GetCapabilities();
                HMIHandler::getInstance().sendRequest(getButtonsCapsRequest);

                ConnectionHandler::getInstance().startDevicesDiscovery();

                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A GetUICapabilities response has been income");
                NsRPC2Communication::UI::GetCapabilitiesResponse * uiCaps = (NsRPC2Communication::UI::GetCapabilitiesResponse*)msg;
                core->mDisplayCapabilitiesV1 = uiCaps->get_displayCapabilities();
                core->mDisplayCapabilitiesV2 = (*(NsAppLinkRPCV2::DisplayCapabilities*)&uiCaps->get_displayCapabilities());
                core->mHmiZoneCapabilitiesV1.set( uiCaps->get_hmiZoneCapabilities() );
                core->mHmiZoneCapabilitiesV2.set( *(std::vector<NsAppLinkRPCV2::HmiZoneCapabilities>*)&uiCaps->get_hmiZoneCapabilities() );
                if(uiCaps->get_softButtonCapabilities())
                {
                    core->mSoftButtonCapabilities.set(*uiCaps->get_softButtonCapabilities());
                }
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnCommand UI notification has been invoked");
                NsRPC2Communication::UI::OnCommand* object = (NsRPC2Communication::UI::OnCommand*)msg;
                Application* app = AppMgrRegistry::getInstance().getApplicationByCommand(object->get_commandId());
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::OnCommand* event = new NsAppLinkRPC::OnCommand();
                event->set_cmdID(object->get_commandId());

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SHOWRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A Show response has been income");
                NsRPC2Communication::UI::ShowResponse* object = (NsRPC2Communication::UI::ShowResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::Show_response* response = new NsAppLinkRPC::Show_response();
                response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                response->set_success(true);

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETGLOBALPROPERTIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A SetGlobalProperties response has been income");
                NsRPC2Communication::UI::SetGlobalPropertiesResponse* object = (NsRPC2Communication::UI::SetGlobalPropertiesResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::SetGlobalProperties_response* response = new NsAppLinkRPC::SetGlobalProperties_response();

                response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                response->set_success(true);

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__RESETGLOBALPROPERTIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A ResetGlobalProperties response has been income");
                NsRPC2Communication::UI::ResetGlobalPropertiesResponse* object = (NsRPC2Communication::UI::ResetGlobalPropertiesResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::ResetGlobalProperties_response* response = new NsAppLinkRPC::ResetGlobalProperties_response();

                response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ALERTRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An Alert response has been income");
                NsRPC2Communication::UI::AlertResponse* object = (NsRPC2Communication::UI::AlertResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::Alert_response* response = new NsAppLinkRPC::Alert_response();
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDCOMMANDRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand UI response has been income");
                NsRPC2Communication::UI::AddCommandResponse* object = (NsRPC2Communication::UI::AddCommandResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
                app->decrementUnrespondedRequestCount(cmdId);
                if(app->getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::AddCommand_response* response = new NsAppLinkRPC::AddCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                }

                core->mMessageMapping.removeMessage(object->getId());

                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETECOMMANDRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand UI response has been income");
                NsRPC2Communication::UI::DeleteCommandResponse* object = (NsRPC2Communication::UI::DeleteCommandResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
                app->decrementUnrespondedRequestCount(cmdId);
                if(app->getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::DeleteCommand_response* response = new NsAppLinkRPC::DeleteCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                }

                core->mMessageMapping.removeMessage(object->getId());

                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ADDSUBMENURESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An AddSubMenu response has been income");
                NsRPC2Communication::UI::AddSubMenuResponse* object = (NsRPC2Communication::UI::AddSubMenuResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::AddSubMenu_response* response = new NsAppLinkRPC::AddSubMenu_response();
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETESUBMENURESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A DeleteSubMenu response has been income");
                NsRPC2Communication::UI::DeleteSubMenuResponse* object = (NsRPC2Communication::UI::DeleteSubMenuResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::DeleteSubMenu_response* response = new NsAppLinkRPC::DeleteSubMenu_response();
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__CREATEINTERACTIONCHOICESETRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A CreateInteractionChoiceSet response has been income");
                NsRPC2Communication::UI::CreateInteractionChoiceSetResponse* object = (NsRPC2Communication::UI::CreateInteractionChoiceSetResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::CreateInteractionChoiceSet_response* response = new NsAppLinkRPC::CreateInteractionChoiceSet_response();
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DELETEINTERACTIONCHOICESETRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A DeleteInteractionChoiceSet response has been income");
                NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse* object = (NsRPC2Communication::UI::DeleteInteractionChoiceSetResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::DeleteInteractionChoiceSet_response* response = new NsAppLinkRPC::DeleteInteractionChoiceSet_response();
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__PERFORMINTERACTIONRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A PerformInteraction response has been income");
                NsRPC2Communication::UI::PerformInteractionResponse* object = (NsRPC2Communication::UI::PerformInteractionResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();

                NsAppLinkRPC::PerformInteraction_response* response = new NsAppLinkRPC::PerformInteraction_response();
                if(object->get_choiceID())
                {
                    response->set_choiceID(*object->get_choiceID());
                }
                if(object->get_triggerSource())
                {
                    response->set_triggerSource(*object->get_triggerSource());
                }
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SETMEDIACLOCKTIMERRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A SetMediaClockTimer response has been income");
                NsRPC2Communication::UI::SetMediaClockTimerResponse* object = (NsRPC2Communication::UI::SetMediaClockTimerResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();

                switch(app->getProtocolVersion())
                {
                    case 1:
                    {
                        NsAppLinkRPC::SetMediaClockTimer_response* response = new NsAppLinkRPC::SetMediaClockTimer_response();
                        response->set_success(true);
                        response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                        core->mMessageMapping.removeMessage(object->getId());
                        LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                        break;
                    }
                    case 2:
                    {
                        NsAppLinkRPCV2::SetMediaClockTimer_response* response = new NsAppLinkRPCV2::SetMediaClockTimer_response();
                        response->set_success(true);
                        response->set_resultCode(static_cast<NsAppLinkRPCV2::Result::ResultInternal>(object->getResult()));
                        core->mMessageMapping.removeMessage(object->getId());
                        LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                        MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                        break;
                    }
                }
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnDriverDistraction UI notification has been invoked");
                NsRPC2Communication::UI::OnDriverDistraction* object = (NsRPC2Communication::UI::OnDriverDistraction*)msg;
                Application* app = AppMgrRegistry::getInstance().getActiveItem();
                if(!app)
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "No currently active application found");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
            //    switch(app->getProtocolVersion())
            //    {
                //    case 1:
                //    {
                        NsAppLinkRPC::OnDriverDistraction* event = new NsAppLinkRPC::OnDriverDistraction();
                        event->set_state(object->get_state());
                        core->mDriverDistraction = event;
                        MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                        break;
                //    }
                //    case 2:
                //    {
                //        NsAppLinkRPC::OnDriverDistraction* event = new NsAppLinkRPC::OnDriverDistraction();
                //        event->set_state(object->get_state());
                //        core->mDriverDistraction = event;
                //        MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                //        break;
                //    }
            //    }
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONSYSTEMCONTEXT:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnSystemContext UI notification has been invoked");
                NsRPC2Communication::UI::OnSystemContext* object = (NsRPC2Communication::UI::OnSystemContext*)msg;

                Application* app = AppMgrRegistry::getInstance().getActiveItem();
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, " null-application found as an active item!");
                    return;
                }
                switch(app->getProtocolVersion())
                {
                    case 1:
                    {
                        Application_v1* appv1 = (Application_v1*)app;
                        appv1->setSystemContext(object->get_systemContext());
                        break;
                    }
                    case 2:
                    {
                        Application_v2* appv2 = (Application_v2*)app;
                        NsAppLinkRPCV2::SystemContext ctx2;
                        const NsAppLinkRPC::SystemContext& ctx = object->get_systemContext();
                        ctx2.set((NsAppLinkRPCV2::SystemContext::SystemContextInternal)ctx.get());
                        appv2->setSystemContext(ctx2);
                        break;
                    }
                }

                NsAppLinkRPC::OnHMIStatus* event = new NsAppLinkRPC::OnHMIStatus;
                event->set_systemContext(object->get_systemContext());

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                LOG4CPLUS_INFO_EXT(mLogger, " An NsAppLinkRPC::OnHMIStatus UI notification has been sent to a mobile side!");
                MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__SLIDERRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A Slider response has been income");
                NsRPC2Communication::UI::SliderResponse* uiResponse = (NsRPC2Communication::UI::SliderResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(
                    core->mMessageMapping.findRegistryItemAssignedToCommand(uiResponse->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();

                NsAppLinkRPCV2::Slider_response* response = new NsAppLinkRPCV2::Slider_response();
                
                response->set_success(true);
                response->set_sliderPosition(uiResponse->get_sliderPosition());
                response->set_resultCode(static_cast<NsAppLinkRPCV2::Result::ResultInternal>(uiResponse->getResult()));
                core->mMessageMapping.removeMessage(uiResponse->getId());

                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDEVICECHOSEN:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnDeviceChosen notification has been income");
                NsRPC2Communication::UI::OnDeviceChosen* chosen = (NsRPC2Communication::UI::OnDeviceChosen*)msg;
                const std::string& deviceName = chosen->get_deviceName();
                const NsConnectionHandler::CDevice* device = core->mDeviceList.findDeviceByName(deviceName);
                if (device) {
                    const NsConnectionHandler::tDeviceHandle& handle = device->getDeviceHandle();
                    ConnectionHandler::getInstance().connectToDevice(handle);
                }
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_INVALID:
            default:
                LOG4CPLUS_ERROR_EXT(mLogger, " Not UI RPC message " << msg->getMethod() << " has been received!");
        }

        switch(msg->getMethod())
        {
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETCAPABILITIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A GetVRCapabilities response has been income");
                NsRPC2Communication::VR::GetCapabilitiesResponse * vrCaps = (NsRPC2Communication::VR::GetCapabilitiesResponse*)msg;
                core->mVrCapabilitiesV1.set(vrCaps->get_capabilities());
                core->mVrCapabilitiesV2.set(*((std::vector< NsAppLinkRPCV2::VrCapabilities>*)&vrCaps->get_capabilities()));
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ADDCOMMANDRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand VR response has been income");
                NsRPC2Communication::VR::AddCommandResponse* object = (NsRPC2Communication::VR::AddCommandResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
                app->decrementUnrespondedRequestCount(cmdId);
                if(app->getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::AddCommand_response* response = new NsAppLinkRPC::AddCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                }

                core->mMessageMapping.removeMessage(object->getId());

                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__DELETECOMMANDRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand VR response has been income");
                NsRPC2Communication::VR::DeleteCommandResponse* object = (NsRPC2Communication::VR::DeleteCommandResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
                app->decrementUnrespondedRequestCount(cmdId);
                if(app->getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::DeleteCommand_response* response = new NsAppLinkRPC::DeleteCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                }

                core->mMessageMapping.removeMessage(object->getId());

                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnCommand VR notification has been invoked");
                NsRPC2Communication::VR::OnCommand* object = (NsRPC2Communication::VR::OnCommand*)msg;
                Application* app = AppMgrRegistry::getInstance().getApplicationByCommand(object->get_cmdID());
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();

                NsAppLinkRPC::OnCommand* event = new NsAppLinkRPC::OnCommand();
                event->set_cmdID(object->get_cmdID());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_INVALID:
            default:
                LOG4CPLUS_ERROR_EXT(mLogger, " Not VR RPC message " << msg->getMethod() << " has been received!");
        }

        switch(msg->getMethod())
        {
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETCAPABILITIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A GetTTSCapabilities response has been income");
                NsRPC2Communication::TTS::GetCapabilitiesResponse * ttsCaps = (NsRPC2Communication::TTS::GetCapabilitiesResponse*)msg;
                core->mSpeechCapabilitiesV1.set(ttsCaps->get_capabilities());
                core->mSpeechCapabilitiesV2.set(*((std::vector< NsAppLinkRPCV2::SpeechCapabilities>*)&ttsCaps->get_capabilities()));
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__SPEAKRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A Speak response has been income");
                NsRPC2Communication::TTS::SpeakResponse* object = (NsRPC2Communication::TTS::SpeakResponse*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::Speak_response* response = new NsAppLinkRPC::Speak_response();
                response->setMessageType(NsAppLinkRPC::ALRPCMessage::RESPONSE);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                response->set_success(true);

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " connection id " << connectionId << " session id " << (uint)sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_INVALID:
            default:
                LOG4CPLUS_ERROR_EXT(mLogger, " Not TTS RPC message " << msg->getMethod() << " has been received!");
        }

        switch(msg->getMethod())
        {
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ONAPPUNREGISTERED:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnAppUnregistered notification has been income");
                NsRPC2Communication::AppLinkCore::OnAppUnregistered * object = (NsRPC2Communication::AppLinkCore::OnAppUnregistered*)msg;
                NsAppLinkRPC::OnAppInterfaceUnregistered* event = new NsAppLinkRPC::OnAppInterfaceUnregistered();
                if ( object->get_reason() )
                {
                    event->set_reason(*object->get_reason());
                }
                MobileHandler::getInstance().sendRPCMessage(event, 0, 1);//just temporarily!!! //0-temp! Specify unsigned int connectionID instead!!!! 1-also temporarily, until an app can be deducted
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__ACTIVATEAPP:
            {
                LOG4CPLUS_INFO_EXT(mLogger, "ActivateApp has been received!");
                NsRPC2Communication::AppLinkCore::ActivateApp* object = static_cast<NsRPC2Communication::AppLinkCore::ActivateApp*>(msg);
                if ( !object )
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Couldn't cast object to ActivateApp type");
                    return;
                }

                //a silly workaround!!! Until the object starts supplying some sort of connection id + session id instead of just a name (there may me MORE than one app of the same name registered on HMI simultaneously)
                const std::string& appName = object->get_appName();
                AppMgrRegistry::Items items = AppMgrRegistry::getInstance().getItems(appName);
                if(items.empty())
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application with the name " << appName << " found!");
                    return;
                }

                Application* app = core->getApplicationFromItemCheckNotNull(items[0]);
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }

                Application* currentApp = AppMgrRegistry::getInstance().getActiveItem();
                if(currentApp)
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "There is a currently active application - about to remove it from HMI first");
                    core->removeAppFromHmi(currentApp, app->getConnectionID(), app->getSessionID());
                }
                else
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "No application is currently active");
                }

                AppMgrRegistry::getInstance().activateApp(app);
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionID = app->getConnectionID();

                const ChoiceSetItems& newChoiceSets = app->getAllChoiceSets();
                LOG4CPLUS_INFO_EXT(mLogger, "Adding new application's interaction choice sets to HMI due to a new application activation");
                for(ChoiceSetItems::const_iterator it = newChoiceSets.begin(); it != newChoiceSets.end(); it++)
                {
                    const unsigned int& choiceSetId = it->first;
                    const ChoiceSet& choiceSet = it->second;
                    NsRPC2Communication::UI::CreateInteractionChoiceSet* addCmd = new NsRPC2Communication::UI::CreateInteractionChoiceSet();
                    addCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    addCmd->set_interactionChoiceSetID(choiceSetId);
                    addCmd->set_choiceSet(choiceSet);
                    addCmd->set_appId(app->getAppID());
                    core->mMessageMapping.addMessage(addCmd->getId(), connectionID, sessionID);

                    HMIHandler::getInstance().sendRequest(addCmd);
                }
                LOG4CPLUS_INFO_EXT(mLogger, "New app's interaction choice sets added!");

                const MenuItems& newMenus = app->getAllMenus();
                LOG4CPLUS_INFO_EXT(mLogger, "Adding new application's menus to HMI due to a new application activation");
                for(MenuItems::const_iterator it = newMenus.begin(); it != newMenus.end(); it++)
                {
                    const unsigned int& menuId = it->first;
                    const MenuValue& menuVal = it->second;
                    const std::string& menuName = menuVal.first;
                    const unsigned int* position = menuVal.second;
                    NsRPC2Communication::UI::AddSubMenu* addCmd = new NsRPC2Communication::UI::AddSubMenu();
                    addCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    addCmd->set_menuId(menuId);
                    addCmd->set_menuName(menuName);
                    if(position)
                    {
                        addCmd->set_position(*position);
                    }
                    addCmd->set_appId(app->getAppID());
                    core->mMessageMapping.addMessage(addCmd->getId(), connectionID, sessionID);

                    HMIHandler::getInstance().sendRequest(addCmd);
                }
                LOG4CPLUS_INFO_EXT(mLogger, "New app's menus added!");

                const Commands& newCommands = app->getAllCommands();
                LOG4CPLUS_INFO_EXT(mLogger, "Adding a new application's commands to HMI due to a new application activation");
                for(Commands::const_iterator it = newCommands.begin(); it != newCommands.end(); it++)
                {
                    const Command& key = *it;
                    const CommandParams& params = key.second;
                    const NsAppLinkRPC::MenuParams* menuParams = params.menuParams;
                    const CommandBase& base = key.first;
                    const CommandType& type = std::get<1>(base);
                    unsigned int cmdId = std::get<0>(base);

                    NsRPC2Communication::RPC2Request* addCmd = 0;
                    if(type == CommandType::UI)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "Adding UI command");
                        addCmd = new NsRPC2Communication::UI::AddCommand();
                    }
                    else if(type == CommandType::VR)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "Adding VR command");
                        addCmd = new NsRPC2Communication::VR::AddCommand();
                    }
                    else
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, "An unindentified command type - " << type.getType());
                        continue;
                    }
                    addCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    ((NsRPC2Communication::UI::AddCommand*)addCmd)->set_cmdId(cmdId); //doesn't matter, of which type- VR or UI is thye cmd = eather has the set_cmdId method within
                    ((NsRPC2Communication::UI::AddCommand*)addCmd)->set_menuParams(*menuParams);
                    ((NsRPC2Communication::UI::AddCommand*)addCmd)->set_appId(app->getAppID());
                    core->mMessageMapping.addMessage(addCmd->getId(), connectionID, sessionID);

                    HMIHandler::getInstance().sendRequest(addCmd);
                }
                LOG4CPLUS_INFO_EXT(mLogger, "New app's commands added!");

                if(core->mDriverDistraction)
                {
                    MobileHandler::getInstance().sendRPCMessage(core->mDriverDistraction, connectionID, sessionID);
                }

                switch(app->getProtocolVersion())
                {
                    case 1:
                    {
                        Application_v1* appv1 = (Application_v1*)app;
                        NsAppLinkRPC::OnHMIStatus * hmiStatus = new NsAppLinkRPC::OnHMIStatus;
                        hmiStatus->set_hmiLevel(NsAppLinkRPC::HMILevel::HMI_FULL);
                        hmiStatus->set_audioStreamingState(appv1->getApplicationAudioStreamingState());
                        hmiStatus->set_systemContext(appv1->getSystemContext());
                        MobileHandler::getInstance().sendRPCMessage( hmiStatus, connectionID, sessionID );
                        NsRPC2Communication::AppLinkCore::ActivateAppResponse * response = new NsRPC2Communication::AppLinkCore::ActivateAppResponse;
                        response->setId(object->getId());
                        response->setResult(NsAppLinkRPC::Result::SUCCESS);
                        HMIHandler::getInstance().sendResponse(response);
                        break;
                    }
                    case 2:
                    {
                        Application_v2* appv2 = (Application_v2*)app;
                        NsAppLinkRPCV2::OnHMIStatus * hmiStatus = new NsAppLinkRPCV2::OnHMIStatus;
                        hmiStatus->set_hmiLevel(NsAppLinkRPCV2::HMILevel::HMI_FULL);
                        hmiStatus->set_audioStreamingState(appv2->getApplicationAudioStreamingState());
                        hmiStatus->set_systemContext(appv2->getSystemContext());
                        MobileHandler::getInstance().sendRPCMessage( hmiStatus, connectionID, sessionID );
                        NsRPC2Communication::AppLinkCore::ActivateAppResponse * response = new NsRPC2Communication::AppLinkCore::ActivateAppResponse;
                        response->setId(object->getId());
                        response->setResult(NsAppLinkRPCV2::Result::SUCCESS);
                        HMIHandler::getInstance().sendResponse(response);
                        break;
                    }
                }
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA:
            {
                LOG4CPLUS_INFO_EXT(mLogger, "SendData request has been received!");
                NsRPC2Communication::AppLinkCore::SendData* object = static_cast<NsRPC2Communication::AppLinkCore::SendData*>(msg);
                core->mSyncPManager.setRawData( object->get_data() );
                NsAppLinkRPC::OnEncodedSyncPData* encodedNotification = new NsAppLinkRPC::OnEncodedSyncPData;
                encodedNotification->set_data(core->mSyncPManager.getPData());
                MobileHandler::getInstance().sendRPCMessage( encodedNotification, 0, 1 );//0-temp! Specify unsigned int connectionID instead!!!! 1-also temp, at least until we deduct an app here
                NsRPC2Communication::AppLinkCore::SendDataResponse* response = new NsRPC2Communication::AppLinkCore::SendDataResponse;
                response->setId(object->getId());
                response->setResult(NsAppLinkRPC::Result::SUCCESS);
                HMIHandler::getInstance().sendResponse(response);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETAPPLIST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, "GetAppList request has been received!");
                NsRPC2Communication::AppLinkCore::GetAppList* object = static_cast<NsRPC2Communication::AppLinkCore::GetAppList*>(msg);
                NsRPC2Communication::AppLinkCore::GetAppListResponse* response = new NsRPC2Communication::AppLinkCore::GetAppListResponse;
                response->setId(object->getId());
                const AppMgrRegistry::ItemsMap& registeredApps = AppMgrRegistry::getInstance().getItems();
                std::vector< NsAppLinkRPC::HMIApplication> hmiApps;
                for(AppMgrRegistry::ItemsMap::const_iterator it = registeredApps.begin(); it != registeredApps.end(); it++)
                {
                    NsAppLinkRPC::HMIApplication hmiApp;
                    Application* app = core->getApplicationFromItemCheckNotNull(it->second);
                    if(!app)
                    {
                        LOG4CPLUS_ERROR_EXT(mLogger, " null-application found!");
                        continue;
                    }

                    LOG4CPLUS_INFO_EXT(mLogger, "Adding an application " << app->getName() << " connection/session key " << app->getAppID() << " is media? " << app->getIsMediaApplication() );
                    hmiApp.set_appName(app->getName());
                    hmiApp.set_ngnMediaScreenAppName(app->getNgnMediaScreenAppName());
                    hmiApp.set_appId(app->getAppID());
                    hmiApp.set_isMediaApplication(app->getIsMediaApplication());
                    hmiApps.push_back(hmiApp);
                }
                if(!hmiApps.empty())
                {
                    response->set_appList(hmiApps);
                    response->setResult(NsAppLinkRPC::Result::SUCCESS);
                }
                else
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, " Application list is empty!");
                    response->setResult(NsAppLinkRPC::Result::GENERIC_ERROR);
                }
                HMIHandler::getInstance().sendResponse(response);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_APPLINKCORE__GETDEVICELIST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A GetDeviceList request has been income");
                NsRPC2Communication::AppLinkCore::GetDeviceList* getDevList = (NsRPC2Communication::AppLinkCore::GetDeviceList*)msg;
                NsRPC2Communication::AppLinkCore::GetDeviceListResponse* response = new NsRPC2Communication::AppLinkCore::GetDeviceListResponse;
                response->setId(getDevList->getId());
                DeviceNamesList list;
                const NsConnectionHandler::tDeviceList& devList = core->mDeviceList.getDeviceList();
                for(NsConnectionHandler::tDeviceList::const_iterator it = devList.begin(); it != devList.end(); it++)
                {
                    const NsConnectionHandler::CDevice& device = it->second;
                    list.push_back(device.getUserFriendlyName());
                }
                if ( list.empty() )
                {
                    list.push_back("");
                    response->setResult(NsAppLinkRPC::Result::GENERIC_ERROR);
                }
                else
                {
                    response->setResult(NsAppLinkRPC::Result::SUCCESS);
                }
                response->set_deviceList(list);
                HMIHandler::getInstance().sendResponse(response);
                return;
            }

            case NsRPC2Communication::Marshaller::METHOD_INVALID:
            default:
                LOG4CPLUS_ERROR_EXT(mLogger, " Unknown RPC message " << msg->getMethod() << " has been received!");
        }

        LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message " << msg->getMethod() << " has been invoked!");
    }

    /**
     * \brief Register an application
     * \param request a RegisterAppInterface request which is the source for application fields initial values
     * \param connectionID id of the connection which will be associated with the application
     * \param sessionID an id of the session which will be associated with the application
     * \return A instance of RegistryItem created for application
     */
    const RegistryItem* AppMgrCore::registerApplication(NsAppLinkRPC::ALRPCMessage * request, const unsigned int& connectionID, const unsigned char& sessionID)
    {
        if(!request)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Null-request specified for connection id " << connectionID << " session id " << (uint)sessionID << "!");
            return 0;
        }

        const unsigned int& protocolVersion = request->getProtocolVersion();
        const std::string& appName = ((NsAppLinkRPC::RegisterAppInterface_request*)request)->get_appName();
        int appId = connectionID|(sessionID << 16);
        LOG4CPLUS_INFO_EXT(mLogger, " Registering an application " << appName << " app id " << appId << " !");

        switch(protocolVersion)
        {
            case 2:
            {
                Application_v2* application = new Application_v2( appName, connectionID, sessionID, appId );
                if(!application)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Cannot register application " << appName << " connection " << connectionID << " session " << (uint)sessionID << " protocol version " << protocolVersion << " !");
                    return 0;
                }

                NsAppLinkRPCV2::RegisterAppInterface_request* registerRequest = (NsAppLinkRPCV2::RegisterAppInterface_request*) request;
       /*         if(registerRequest->get_appID())
                {
                    const std::string& appId = *registerRequest->get_appID();
                    application->setAppID(appId);
                }
        */        if( registerRequest->get_appType() )
                {
                    const std::vector<NsAppLinkRPCV2::AppType>& appType = *registerRequest->get_appType();
                    application->setAppType(appType);
                }
                bool isMediaApplication = registerRequest->get_isMediaApplication();
                const NsAppLinkRPCV2::Language& languageDesired = registerRequest->get_languageDesired();
                application->setLanguageDesired(languageDesired);

                const NsAppLinkRPCV2::SyncMsgVersion& syncMsgVersion = registerRequest->get_syncMsgVersion();

                if ( registerRequest -> get_ngnMediaScreenAppName() )
                {
                    const std::string& ngnMediaScreenAppName = *registerRequest->get_ngnMediaScreenAppName();
                    application->setNgnMediaScreenAppName(ngnMediaScreenAppName);
                }

                if ( registerRequest -> get_vrSynonyms() )
                {
                    const std::vector<std::string>& vrSynonyms = *registerRequest->get_vrSynonyms();
                    application->setVrSynonyms(vrSynonyms);
                }

                application->setIsMediaApplication(isMediaApplication);
                application->setSyncMsgVersion(syncMsgVersion);
                application->setSystemContext(NsAppLinkRPCV2::SystemContext::SYSCTXT_MAIN);

                if(registerRequest->get_ttsName())
                {
                    application->setTtsName(*registerRequest->get_ttsName());
                }

                application->setApplicationHMIStatusLevel(NsAppLinkRPCV2::HMILevel::HMI_NONE);

                LOG4CPLUS_INFO_EXT(mLogger, "Application created." );
                return AppMgrRegistry::getInstance().registerApplication( application );
            }
            case 1:
            {
                Application_v1* application = new Application_v1( appName, connectionID, sessionID, appId );
                if(!application)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Cannot register application " << appName << " connection " << connectionID << " session " << (uint)sessionID << " protocol version " << protocolVersion << " !");
                    return 0;
                }

                NsAppLinkRPC::RegisterAppInterface_request* registerRequest = (NsAppLinkRPC::RegisterAppInterface_request*) request;
                bool isMediaApplication = registerRequest->get_isMediaApplication();
                const NsAppLinkRPC::Language& languageDesired = registerRequest->get_languageDesired();
                const NsAppLinkRPC::SyncMsgVersion& syncMsgVersion = registerRequest->get_syncMsgVersion();

                if ( registerRequest -> get_ngnMediaScreenAppName() )
                {
                    const std::string& ngnMediaScreenAppName = *registerRequest->get_ngnMediaScreenAppName();
                    application->setNgnMediaScreenAppName(ngnMediaScreenAppName);
                }

                if ( registerRequest -> get_vrSynonyms() )
                {
                    const std::vector<std::string>& vrSynonyms = *registerRequest->get_vrSynonyms();
                    application->setVrSynonyms(vrSynonyms);
                }

                if ( registerRequest -> get_usesVehicleData() )
                {
                    bool usesVehicleData = registerRequest->get_usesVehicleData();
                    application->setUsesVehicleData(usesVehicleData);
                }

                application->setIsMediaApplication(isMediaApplication);
                application->setLanguageDesired(languageDesired);
                application->setSyncMsgVersion(syncMsgVersion);
                application->setSystemContext(NsAppLinkRPC::SystemContext::SYSCTXT_MAIN);

                application->setApplicationHMIStatusLevel(NsAppLinkRPC::HMILevel::HMI_NONE);

                LOG4CPLUS_INFO_EXT(mLogger, "Application created." );
                return AppMgrRegistry::getInstance().registerApplication( application );
            }
            default:
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Unsupported protocol version number " << protocolVersion << " !");
                return 0;
            }
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Application " << appName << " app id " << appId << " registered successfully !");
    }

    /**
     * \brief unregister an application associated with the given session
     * \param connectionID an id of the connection asociated with the application to be unregistered
     * \param sessionID an id of the session asociated with the application to be unregistered
     */
    void AppMgrCore::unregisterApplication(const unsigned int& connectionID, const unsigned char &sessionID)
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Trying to unregister an application for connection id " << connectionID << " session id " << (uint)sessionID);
        RegistryItem* item = AppMgrRegistry::getInstance().getItem(connectionID, sessionID);
        Application* app = getApplicationFromItemCheckNotNull( item );
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
            return;
        }

        const std::string& appName = app->getName();
        LOG4CPLUS_INFO_EXT(mLogger, " Unregistering an application " << appName << " connection id " << connectionID << " session id " << (uint)sessionID << "!");
        mButtonsMapping.removeItem(item);
        mMessageMapping.removeItem(item);
        AppMgrRegistry::getInstance().unregisterApplication(item);
        LOG4CPLUS_INFO_EXT(mLogger, " Unregistered an application " << appName << " connection id " << connectionID << " session id " << (uint)sessionID << "!");
    }

    /**
     * \brief Remove all app components from HMI
     * \param currentApp app which components to be removed
     * \param connectionID connection id
     * \param sessionID session id
     */
    void AppMgrCore::removeAppFromHmi(Application* currentApp, const unsigned int &connectionID, const unsigned char &sessionID)
    {
        const Commands& currentCommands = currentApp->getAllCommands();
        LOG4CPLUS_INFO_EXT(mLogger, "Removing current application's commands from HMI");
        for(Commands::const_iterator it = currentCommands.begin(); it != currentCommands.end(); it++)
        {
            const Command& key = *it;
            const CommandParams& params = key.second;
            const CommandBase& base = key.first;
            const CommandType& type = std::get<1>(base);
            unsigned int cmdId = std::get<0>(base);
            NsRPC2Communication::RPC2Request* deleteCmd = 0;
            if(type == CommandType::UI)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Removing UI command");
                deleteCmd = new NsRPC2Communication::UI::DeleteCommand();
            }
            else if(type == CommandType::VR)
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Removing VR command");
                deleteCmd = new NsRPC2Communication::VR::DeleteCommand();
            }
            else
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "An unindentified command type - " << type.getType());
                continue;
            }
            deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            ((NsRPC2Communication::UI::DeleteCommand*)deleteCmd)->set_cmdId(cmdId); //doesn't matter, of which type- VR or UI is thye cmd = eather has the set_cmdId method within
            ((NsRPC2Communication::UI::DeleteCommand*)deleteCmd)->set_appId(currentApp->getAppID());
            mMessageMapping.addMessage(deleteCmd->getId(), connectionID, sessionID);

            HMIHandler::getInstance().sendRequest(deleteCmd);
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Current app's commands removed!");

        const MenuItems& currentMenus = currentApp->getAllMenus();
        LOG4CPLUS_INFO_EXT(mLogger, "Removing current application's menus from HMI");
        for(MenuItems::const_iterator it = currentMenus.begin(); it != currentMenus.end(); it++)
        {
            const unsigned int& menuId = it->first;
            NsRPC2Communication::UI::DeleteSubMenu* deleteCmd = new NsRPC2Communication::UI::DeleteSubMenu();
            deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            deleteCmd->set_menuId(menuId);
            deleteCmd->set_appId(currentApp->getAppID());
            mMessageMapping.addMessage(deleteCmd->getId(), connectionID, sessionID);

            HMIHandler::getInstance().sendRequest(deleteCmd);
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Current app's menus removed!");

        const ChoiceSetItems& currentChoiceSets = currentApp->getAllChoiceSets();
        LOG4CPLUS_INFO_EXT(mLogger, "Removing current application's interaction choice sets from HMI");
        for(ChoiceSetItems::const_iterator it = currentChoiceSets.begin(); it != currentChoiceSets.end(); it++)
        {
            const unsigned int& choiceSetId = it->first;
            NsRPC2Communication::UI::DeleteInteractionChoiceSet* deleteCmd = new NsRPC2Communication::UI::DeleteInteractionChoiceSet();
            deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            deleteCmd->set_interactionChoiceSetID(choiceSetId);
            deleteCmd->set_appId(currentApp->getAppID());
            mMessageMapping.addMessage(deleteCmd->getId(), connectionID, sessionID);

            HMIHandler::getInstance().sendRequest(deleteCmd);
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Current app's interaction choice sets removed!");
    }

    /**
     * \brief retrieve an application instance from the RegistryItrem instance checking for non-null values
     * \param item a RegistryItem from which to retrieve an app pointer
     * \return Application instance retrieved from item
     */
    Application *AppMgrCore::getApplicationFromItemCheckNotNull(const RegistryItem *item) const
    {
        if(!item)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "No registry item found!");
            return 0;
        }
        Application* app = item->getApplication();
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
            return 0;
        }
        return app;
    }

    /**
     * \brief serialize a string value to the text file
     * \param fileName name of the file to serialize to
     * \param value a value to serialize
     * \return success of an operation - true or false
     */
    bool AppMgrCore::serializeToFile(const std::string &fileName, const std::string& value) const
    {
        if(!value.empty())
        {
            std::ofstream file(fileName, std::ios::out | std::ios::trunc);
            if(file.is_open())
            {
                file << value;
                file.close();
                LOG4CPLUS_INFO_EXT(mLogger, " Serialized a value " << value << " to a file " << fileName);
                return true;
            }
            else
            {
                LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore cannot serialize a value " << value << " to a file " << fileName << ": error creating file!");
                return false;
            }
        }
        LOG4CPLUS_ERROR_EXT(mLogger, " Cannot serialize an empty value to a file " << fileName << " !");
        return false;
    }

    /**
     * \brief set Json mobile handler
     * \param handler a handler instance
     */
    void AppMgrCore::setJsonHandler(JSONHandler* handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "A null pointer is being assigned - is this the intent?");
            return;
        }
        MobileHandler::getInstance().setJsonHandler(handler);
    }

    /**
     * \brief get Json mobile handler
     * \return JSONHandler instance
     */
    JSONHandler* AppMgrCore::getJsonHandler( ) const
    {
        return MobileHandler::getInstance().getJsonHandler();
    }

    /**
     * \brief set Json RPC2 handler
     * \param handler a handler instance
     */
    void AppMgrCore::setJsonRPC2Handler(JSONRPC2Handler *handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "A null pointer is being assigned - is this the intent?");
            return;
        }
        HMIHandler::getInstance().setJsonRPC2Handler(handler);
    }

    /**
     * \brief get Json RPC2 handler
     * \return JSONRPC2Handler instance
     */
    JSONRPC2Handler *AppMgrCore::getJsonRPC2Handler() const
    {
        return HMIHandler::getInstance().getJsonRPC2Handler();
    }

    /**
     * \brief Sets connection handler instance
     * \param handler connection handler
     */
    void AppMgrCore::setConnectionHandler(NsConnectionHandler::IDevicesDiscoveryStarter *handler)
    {
        if(!handler)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "A null pointer is being assigned - is this the intent?");
            return;
        }
        ConnectionHandler::getInstance().setConnectionHandler(handler);
    }

    /**
     * \brief Gets connection handler instance
     * \return connection handler
     */
    NsConnectionHandler::IDevicesDiscoveryStarter *AppMgrCore::getConnectionHandler() const
    {
        return ConnectionHandler::getInstance().getConnectionHandler();
    }

    /**
     * \brief set device list
     * \param deviceList device list
     */
    void AppMgrCore::setDeviceList(const NsConnectionHandler::tDeviceList &deviceList)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Updating device list: " << deviceList.size() << " devices");
        mDeviceList.setDeviceList(deviceList);
        NsRPC2Communication::AppLinkCore::OnDeviceListUpdated* deviceListUpdated = new NsRPC2Communication::AppLinkCore::OnDeviceListUpdated;
        DeviceNamesList list;
        const NsConnectionHandler::tDeviceList& devList = mDeviceList.getDeviceList();
        for(NsConnectionHandler::tDeviceList::const_iterator it = devList.begin(); it != devList.end(); it++)
        {
            const NsConnectionHandler::CDevice& device = it->second;
            list.push_back(device.getUserFriendlyName());
        }
        if ( list.empty() )
        {
            list.push_back("");
        }

        deviceListUpdated->set_deviceList(list);
        HMIHandler::getInstance().sendNotification(deviceListUpdated);
    }

    /**
     * \brief get device list
     * \return device list
     */
    const NsConnectionHandler::tDeviceList &AppMgrCore::getDeviceList() const
    {
        return mDeviceList.getDeviceList();
    }

    /**
     * \brief add a device to a mapping
     * \param sessionKey session/connection key
     * \param device device handler
     */
    void AppMgrCore::addDevice(const int &sessionKey, const NsConnectionHandler::tDeviceHandle &device)
    {
        mDeviceHandler.addDevice(sessionKey, device);
    }

    /**
     * \brief remove a device from a mapping
     * \param sessionKey session/connection key
     */
    void AppMgrCore::removeDevice(const int &sessionKey)
    {
        mDeviceHandler.removeDevice(sessionKey);
    }

}
