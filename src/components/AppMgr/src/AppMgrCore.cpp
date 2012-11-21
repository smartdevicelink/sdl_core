/**
 * \file AppMgrCore.cpp
 * \brief App manager core functionality
 * \author vsalo
 */

#include "AppMgr/AppMgrCore.h"
#include "AppMgr/AppMgrRegistry.h"
#include "AppMgr/AppPolicy.h"
#include "AppMgr/RegistryItem.h"
#include "AppMgr/AppMgrCoreQueues.h"
#include "AppMgr/SubscribeButton.h"
#include "AppMgr/HMIHandler.h"
#include "AppMgr/MobileHandler.h"
#include "JSONHandler/ALRPCMessage.h"
#include "JSONHandler/ALRPCRequest.h"
#include "JSONHandler/ALRPCResponse.h"
#include "JSONHandler/ALRPCNotification.h"
#include "JSONHandler/ALRPCObjects/Marshaller.h"
#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/JSONRPC2Handler.h"
#include "JSONHandler/RPC2Objects/Marshaller.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Notification.h"
#include <sys/socket.h>
#include "LoggerHelper.hpp"
#include <iostream>
#include <fstream>

namespace NsAppManager
{

    log4cplus::Logger AppMgrCore::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrCore"));
    const std::string AppMgrCore::mAutoActivateIdFileName = "autoActivateId";

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
    {
        std::ifstream file(mAutoActivateIdFileName);
        if( file.is_open() )
        {
            if( file.good() )
            {
                std::getline( file, mLastAutoActivateId );
            }
            file.close();
        }
        else
        {
            LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore cannot deserialize a file: probably file doesn't exist!");
        }

        LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore constructed!");
    }

    /**
     * \brief Copy constructor
     */
    AppMgrCore::AppMgrCore(const AppMgrCore &)
        :mQueueRPCAppLinkObjectsIncoming(0)
        ,mQueueRPCBusObjectsIncoming(0)
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

        std::ofstream(mAutoActivateIdFileName, std::ios::trunc);

        LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore destructed!");
    }

    /**
     * \brief push mobile RPC message to a queue
     * \param message a message to be pushed
     * \param connectionID id of a connection associated with application that sent the message
     * \param sessionID an id of a session associated with the application which pushes a message
     */
    void AppMgrCore::pushMobileRPCMessage( NsAppLinkRPC::ALRPCMessage * message, unsigned int connectionID, unsigned char sessionID )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Pushing mobile RPC message...");
        if(!message)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Nothing to push! A null-ptr occured!");
            return;
        }

        mQueueRPCAppLinkObjectsIncoming->pushMessage(Message(message, ApplicationUniqueID(connectionID, sessionID)));

        LOG4CPLUS_INFO_EXT(mLogger, " Pushed mobile RPC message");
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
        unsigned int connectionID = std::get<0>(message.second);
        unsigned char sessionID = std::get<1>(message.second);
        NsAppLinkRPC::ALRPCMessage* mobileMsg = message.first;
        if(!mobileMsg)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " No message associated with the connection " << connectionID << " session " << (uint)sessionID << " !");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC message " << mobileMsg->getMethodId() << " has been received for the connection " << connectionID << " session id " << (uint)sessionID << "!");
        if(!pThis)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " pThis should point to an instance of AppMgrCore class");
            return;
        }
        AppMgrCore* core = (AppMgrCore*)pThis;

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

                Application* app = core->getApplicationFromItemCheckNotNull(core->registerApplication( object, connectionID, sessionID ));
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
                LOG4CPLUS_INFO_EXT(mLogger, " About to find auto-activate id in a map...");
                std::string autoActivateIdFound = core->mAutoActivateIds.findAutoActivateIdAssignedToName(appName);
                LOG4CPLUS_INFO_EXT(mLogger, " An application " << appName << " is found in a map with auto activate id " << autoActivateIdFound);
                if(!autoActivateIdFound.empty())
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "Found already registered AutoActivateId" << (autoActivateIdFound.empty() ? "EMPTY" : autoActivateIdFound) << " assigned to app name " << appName);
                    if(object->get_autoActivateID())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "There is an AutoActivateId supplied withtin this RegisterAppInterface request: " << *object->get_autoActivateID());
                    }
                    else
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " Application " << object->get_appName() << " hasn't been registered because its autoActivateId NULL differs from the one specified before - " << autoActivateIdFound);
                    }
                }
                else
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "No AutoActivateId has previously been assigned to app name " << appName);
                    if(!object->get_autoActivateID())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "No AutoActivateId supplied within this RegisterAppInterface request!");
                    }
                    else
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " Application " << object->get_appName() << " specified an autoActivateId " << *object->get_autoActivateID() << " while id hasn't yet been registered!");
                    }
                }

                if(!object->get_autoActivateID())
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "No AutoActivateId supplied within this RegisterAppInterface request - about to register an application " << appName << " with the generated one");
                    const std::string& autoActivateId = core->mAutoActivateIds.addApplicationName(object->get_appName());
                    response->set_autoActivateID(autoActivateId);
                }
                else
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " Application " << object->get_appName() << " specified an autoActivateId " << *object->get_autoActivateID() << " while id hasn't yet been registered:");
                    LOG4CPLUS_INFO_EXT(mLogger, " about to register with the supplies auto-activate id!");
                    response->set_autoActivateID(*object->get_autoActivateID());
                }

                response->set_buttonCapabilities(core->mButtonCapabilities.get());
                response->set_displayCapabilities(core->mDisplayCapabilities);
                response->set_hmiZoneCapabilities(core->mHmiZoneCapabilities.get());
                response->set_speechCapabilities(core->mSpeechCapabilities.get());
                response->set_vrCapabilities(core->mVrCapabilities.get());
                response->set_success(true);
                response->set_resultCode(NsAppLinkRPC::Result::SUCCESS);

                LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface response for the app "  << app->getName() << " gets sent to a mobile side... ");
                MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);

                NsAppLinkRPC::OnHMIStatus* status = new NsAppLinkRPC::OnHMIStatus();
                status->set_hmiLevel(app->getApplicationHMIStatusLevel());
                MobileHandler::getInstance().sendRPCMessage(status, connectionID, sessionID);
                NsRPC2Communication::AppLinkCore::OnAppRegistered* appRegistered = new NsRPC2Communication::AppLinkCore::OnAppRegistered();
                appRegistered->set_appName(app->getName());
                appRegistered->set_isMediaApplication(app->getIsMediaApplication());
                appRegistered->set_languageDesired(app->getLanguageDesired());
                appRegistered->set_vrSynonym(app->getVrSynonyms());
                HMIHandler::getInstance().sendNotification(appRegistered);
                LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request was successful: registered an app " << app->getName());
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
                    LOG4CPLUS_ERROR_EXT(mLogger, " No application has been associated with this registry item!");
                    response->set_success(false);
                    response->set_resultCode(NsAppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                    break;
                }
                std::string appName = app->getName();
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
                appUnregistered->set_reason(NsAppLinkRPC::AppInterfaceUnregisteredReason(NsAppLinkRPC::AppInterfaceUnregisteredReason::USER_EXIT));
                HMIHandler::getInstance().sendNotification(appUnregistered);
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
                    LOG4CPLUS_ERROR_EXT(mLogger, " Session " << sessionID << " hasn't been associated with application!");
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

                RegistryItem* item = AppMgrRegistry::getInstance().getItem(connectionID, sessionID);
                if(!item)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, " Session " << sessionID << " hasn't been associated with application!");
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
                    if(menuParams->get_parentID())
                    {
                        const unsigned int& menuId = *menuParams->get_parentID();
                        core->mMenuMapping.addCommand(cmdId, menuId);
                    }
                    core->mMessageMapping.addMessage(addCmd->getId(), connectionID, sessionID);
                    core->mCommandMapping.addCommand(cmdId, cmdType, item);
                    core->mCommandMapping.incrementUnrespondedRequestCount(cmdId);
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
                    core->mMessageMapping.addMessage(addCmd->getId(), connectionID, sessionID);
                    core->mCommandMapping.addCommand(cmdId, cmdType, item);
                    core->mCommandMapping.incrementUnrespondedRequestCount(cmdId);
                    core->mRequestMapping.addMessage(addCmd->getId(), cmdId);
                    HMIHandler::getInstance().sendRequest(addCmd);
                }

                break;
            }
            case NsAppLinkRPC::Marshaller::METHOD_DELETECOMMAND_REQUEST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand request has been invoked");
                NsAppLinkRPC::DeleteCommand_request* object = (NsAppLinkRPC::DeleteCommand_request*)mobileMsg;

                CommandTypes cmdTypes;
                core->mCommandMapping.getTypes(object->get_cmdID(), cmdTypes);
                const unsigned int& cmdId = object->get_cmdID();
                for(CommandTypes::iterator it = cmdTypes.begin(); it != cmdTypes.end(); it++)
                {
                    CommandType cmdType = *it;
                    if(cmdType == CommandType::UI)
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand UI request has been invoked");
                        NsRPC2Communication::UI::DeleteCommand* deleteCmd = new NsRPC2Communication::UI::DeleteCommand();
                        deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                        core->mMessageMapping.addMessage(deleteCmd->getId(), connectionID, sessionID);
                        deleteCmd->set_cmdId(cmdId);
                        core->mCommandMapping.removeCommand(cmdId, cmdType);
                        core->mCommandMapping.incrementUnrespondedRequestCount(cmdId);
                        core->mMenuMapping.removeCommand(cmdId);
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
                        core->mCommandMapping.removeCommand(cmdId, cmdType);
                        core->mCommandMapping.incrementUnrespondedRequestCount(cmdId);
                        core->mRequestMapping.addMessage(deleteCmd->getId(), cmdId);
                        HMIHandler::getInstance().sendRequest(deleteCmd);
                    }
                }
                break;
            }
            case NsAppLinkRPC::Marshaller::METHOD_ADDSUBMENU_REQUEST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An AddSubmenu request has been invoked");
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
                HMIHandler::getInstance().sendRequest(addSubMenu);
                break;
            }
            case NsAppLinkRPC::Marshaller::METHOD_DELETESUBMENU_REQUEST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A DeleteSubmenu request has been invoked");
                NsAppLinkRPC::DeleteSubMenu_request* object = (NsAppLinkRPC::DeleteSubMenu_request*)mobileMsg;
                NsRPC2Communication::UI::DeleteSubMenu* delSubMenu = new NsRPC2Communication::UI::DeleteSubMenu();
                delSubMenu->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                core->mMessageMapping.addMessage(delSubMenu->getId(), connectionID, sessionID);
                const unsigned int& menuId = object->get_menuID();
                delSubMenu->set_menuId(menuId);
                MenuCommands menuCommands;
                core->mMenuMapping.findCommandsAssignedToMenu(menuId, menuCommands);
                for(MenuCommands::iterator it = menuCommands.begin(); it != menuCommands.end(); it++)
                {
                    core->mCommandMapping.removeCommand(*it, CommandType::UI);
                    core->mMenuMapping.removeCommand(*it);
                }
                HMIHandler::getInstance().sendRequest(delSubMenu);
                break;
            }
            case NsAppLinkRPC::Marshaller::METHOD_CREATEINTERACTIONCHOICESET_REQUEST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A CreateInteractionChoiceSet request has been invoked");
                NsAppLinkRPC::CreateInteractionChoiceSet_request* object = (NsAppLinkRPC::CreateInteractionChoiceSet_request*)mobileMsg;
                NsRPC2Communication::UI::CreateInteractionChoiceSet* createInteractionChoiceSet = new NsRPC2Communication::UI::CreateInteractionChoiceSet();
                createInteractionChoiceSet->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                core->mMessageMapping.addMessage(createInteractionChoiceSet->getId(), connectionID, sessionID);
                createInteractionChoiceSet->set_choiceSet(object->get_choiceSet());
                createInteractionChoiceSet->set_interactionChoiceSetID(object->get_interactionChoiceSetID());
                HMIHandler::getInstance().sendRequest(createInteractionChoiceSet);
                break;
            }
            case NsAppLinkRPC::Marshaller::METHOD_DELETEINTERACTIONCHOICESET_REQUEST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A DeleteInteractionChoiceSet request has been invoked");
                NsAppLinkRPC::DeleteInteractionChoiceSet_request* object = (NsAppLinkRPC::DeleteInteractionChoiceSet_request*)mobileMsg;
                NsRPC2Communication::UI::DeleteInteractionChoiceSet* deleteInteractionChoiceSet = new NsRPC2Communication::UI::DeleteInteractionChoiceSet();
                deleteInteractionChoiceSet->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                core->mMessageMapping.addMessage(deleteInteractionChoiceSet->getId(), connectionID, sessionID);
                deleteInteractionChoiceSet->set_interactionChoiceSetID(object->get_interactionChoiceSetID());
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
                HMIHandler::getInstance().sendRequest(performInteraction);
                break;
            }
            case NsAppLinkRPC::Marshaller::METHOD_SETMEDIACLOCKTIMER_REQUEST:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A SetMediaClockTimer request has been invoked");
                NsAppLinkRPC::SetMediaClockTimer_request* object = (NsAppLinkRPC::SetMediaClockTimer_request*)mobileMsg;
                NsRPC2Communication::UI::SetMediaClockTimer* setTimer = new NsRPC2Communication::UI::SetMediaClockTimer();
                setTimer->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                core->mMessageMapping.addMessage(setTimer->getId(), connectionID, sessionID);
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
                response->set_resultCode(NsAppLinkRPC::Result::UNSUPPORTED_REQUEST);
                MobileHandler::getInstance().sendRPCMessage(response, connectionID, sessionID);
                break;
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
                MobileHandler::getInstance().sendRPCMessage(event, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_BUTTONS__GETCAPABILITIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A GetButtonCapabilities response has been income");
                NsRPC2Communication::Buttons::GetCapabilitiesResponse * btnCaps = (NsRPC2Communication::Buttons::GetCapabilitiesResponse*)msg;
                core->mButtonCapabilities.set( btnCaps->get_capabilities() );
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

                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETCAPABILITIESRESPONSE:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " A GetUICapabilities response has been income");
                NsRPC2Communication::UI::GetCapabilitiesResponse * uiCaps = (NsRPC2Communication::UI::GetCapabilitiesResponse*)msg;
                core->mDisplayCapabilities = uiCaps->get_displayCapabilities();
                core->mHmiZoneCapabilities.set( uiCaps->get_hmiZoneCapabilities() );
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONCOMMAND:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnCommand UI notification has been invoked");
                NsRPC2Communication::UI::OnCommand* object = (NsRPC2Communication::UI::OnCommand*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mCommandMapping.findRegistryItemAssignedToCommand(object->get_commandId(), CommandType::UI));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::OnCommand* event = new NsAppLinkRPC::OnCommand();
                event->set_cmdID(object->get_commandId());

                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
                if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::AddCommand_response* response = new NsAppLinkRPC::AddCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
                if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::DeleteCommand_response* response = new NsAppLinkRPC::DeleteCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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

                NsAppLinkRPC::SetMediaClockTimer_response* response = new NsAppLinkRPC::SetMediaClockTimer_response();
                response->set_success(true);
                response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                core->mMessageMapping.removeMessage(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_UI__ONDRIVERDISTRACTION:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnDriverDistraction UI notification has been invoked");
                NsRPC2Communication::UI::OnDriverDistraction* object = (NsRPC2Communication::UI::OnDriverDistraction*)msg;

                NsAppLinkRPC::OnDriverDistraction* event = new NsAppLinkRPC::OnDriverDistraction();
                event->set_state(object->get_state());

                MobileHandler::getInstance().sendRPCMessage(event, 0, 1);//0-temp! Specify unsigned int connectionID instead!!!! 1-also temp! Just no way to deduct an app here
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
                core->mVrCapabilities.set(vrCaps->get_capabilities());
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
                core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
                if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::AddCommand_response* response = new NsAppLinkRPC::AddCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
                if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
                {
                    NsAppLinkRPC::DeleteCommand_response* response = new NsAppLinkRPC::DeleteCommand_response();
                    response->set_success(true);
                    response->set_resultCode(static_cast<NsAppLinkRPC::Result::ResultInternal>(object->getResult()));
                    core->mRequestMapping.removeRequest(object->getId());
                    LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
                    MobileHandler::getInstance().sendRPCMessage(response, connectionId, sessionID);
                }

                core->mMessageMapping.removeMessage(object->getId());

                return;
            }
            case NsRPC2Communication::Marshaller::METHOD_NSRPC2COMMUNICATION_VR__ONCOMMAND:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An OnCommand VR notification has been invoked");
                NsRPC2Communication::VR::OnCommand* object = (NsRPC2Communication::VR::OnCommand*)msg;
                Application* app = core->getApplicationFromItemCheckNotNull(core->mCommandMapping.findRegistryItemAssignedToCommand(object->get_cmdID(), CommandType::VR));
                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                unsigned char sessionID = app->getSessionID();
                unsigned int connectionId = app->getConnectionID();

                NsAppLinkRPC::OnCommand* event = new NsAppLinkRPC::OnCommand();
                event->set_cmdID(object->get_cmdID());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                core->mSpeechCapabilities.set(ttsCaps->get_capabilities());
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
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app " << app->getName() << " session id " << sessionID);
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
                Application* app = core->getApplicationFromItemCheckNotNull(items[0]);

                if(!app)
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                    return;
                }
                NsAppLinkRPC::OnHMIStatus * hmiStatus = new NsAppLinkRPC::OnHMIStatus;
                hmiStatus->set_hmiLevel(NsAppLinkRPC::HMILevel::HMI_FULL);

                core->mLastAutoActivateId = app->getAutoActivateID();
                if(!core->serializeToFile(core->mAutoActivateIdFileName, core->mLastAutoActivateId))
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, "Cannot serialize auto-activate id!");
                }

                AppMgrRegistry::getInstance().activateApp(app);
                hmiStatus->set_audioStreamingState(app->getApplicationAudioStreamingState());
                hmiStatus->set_systemContext(NsAppLinkRPC::SystemContext::SYSCTXT_MENU);
                MobileHandler::getInstance().sendRPCMessage( hmiStatus, app->getConnectionID(), app->getSessionID() );
                NsRPC2Communication::AppLinkCore::ActivateAppResponse * response = new NsRPC2Communication::AppLinkCore::ActivateAppResponse;
                response->setId(object->getId());
                response->setResult(NsAppLinkRPC::Result::SUCCESS);
                HMIHandler::getInstance().sendResponse(response);
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
                    hmiApp.set_appName(core->getApplicationFromItemCheckNotNull(it->second)->getName());
                    hmiApp.set_ngnMediaScreenAppName(core->getApplicationFromItemCheckNotNull(it->second)->getNgnMediaScreenAppName());
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
    const RegistryItem* AppMgrCore::registerApplication( NsAppLinkRPC::RegisterAppInterface_request * request, const unsigned int& connectionID, const unsigned char& sessionID )
    {
        if(!request)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Null-request specified for session " << sessionID << "!");
            return 0;
        }

        LOG4CPLUS_INFO_EXT(mLogger, " Registering an application " << request->get_appName() << "!");

        const std::string& appName = request->get_appName();
        Application* application = new Application( appName, connectionID, sessionID );

        bool isMediaApplication = request->get_isMediaApplication();
        const NsAppLinkRPC::Language& languageDesired = request->get_languageDesired();
        const NsAppLinkRPC::SyncMsgVersion& syncMsgVersion = request->get_syncMsgVersion();

        if ( request -> get_ngnMediaScreenAppName() )
        {
            const std::string& ngnMediaScreenAppName = *request->get_ngnMediaScreenAppName();
            application->setNgnMediaScreenAppName(ngnMediaScreenAppName);
        }

        if ( request -> get_vrSynonyms() )
        {
            const std::vector<std::string>& vrSynonyms = *request->get_vrSynonyms();
            application->setVrSynonyms(vrSynonyms);
        }

        if ( request -> get_usesVehicleData() )
        {
            bool usesVehicleData = request->get_usesVehicleData();
            application->setUsesVehicleData(usesVehicleData);
        }

        if ( request-> get_autoActivateID() )
        {
            const std::string& autoActivateID = *request->get_autoActivateID();
            application->setAutoActivateID(autoActivateID);
        }

        application->setIsMediaApplication(isMediaApplication);
        application->setLanguageDesired(languageDesired);
        application->setSyncMsgVersion(syncMsgVersion);

        application->setApplicationHMIStatusLevel(NsAppLinkRPC::HMILevel::HMI_NONE);
        LOG4CPLUS_INFO_EXT(mLogger, "Application created." );
        return AppMgrRegistry::getInstance().registerApplication( application );
    }

    /**
     * \brief unregister an application associated with the given session
     * \param connectionID an id of the connection asociated with the application to be unregistered
     * \param sessionID an id of the session asociated with the application to be unregistered
     */
    void AppMgrCore::unregisterApplication(const unsigned int& connectionID, const unsigned char &sessionID)
    {
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
        mCommandMapping.removeItem(item);
        mMessageMapping.removeItem(item);
        AppMgrRegistry::getInstance().unregisterApplication(item);
        LOG4CPLUS_INFO_EXT(mLogger, " Unregistered an application " << appName << " connection id " << connectionID << " session id " << (uint)sessionID << "!");
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
                return true;
            }
            else
            {
                LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore cannot serialize to a file: error creating file!");
                return false;
            }
        }
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

}
