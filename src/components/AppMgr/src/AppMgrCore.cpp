/*
 * AppMgr.cpp
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#include "AppMgr/AppMgrCore.h"
#include "AppMgr/Application.h"
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
#include "JSONHandler/RPC2Objects/UI/Marshaller.h"
#include "JSONHandler/RPC2Objects/VR/Marshaller.h"
#include "JSONHandler/RPC2Objects/TTS/Marshaller.h"
#include "JSONHandler/RPC2Objects/Buttons/Marshaller.h"
#include "JSONHandler/RPC2Objects/AppLinkCore/Marshaller.h"
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"
#include "JSONHandler/RPC2Notification.h"
#include <sys/socket.h>
#include "LoggerHelper.hpp"

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
    ,mQueueRPCBusObjectsIncoming(new AppMgrCoreQueue<RPC2Communication::RPC2Command*>(&AppMgrCore::handleBusRPCMessageIncoming, this))
{
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

	LOG4CPLUS_INFO_EXT(mLogger, " AppMgrCore destructed!");
}

/**
 * \brief push mobile RPC message to a queue
 * \param message a message to be pushed
 * \param sessionID an id of a session associated with the application which pushes a message
 */
void AppMgrCore::pushMobileRPCMessage( AppLinkRPC::ALRPCMessage * message, unsigned char sessionID )
{
	LOG4CPLUS_INFO_EXT(mLogger, " Pushing mobile RPC message...");
    if(!message)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Nothing to push! A null-ptr occured!");
        return;
    }
	
    mQueueRPCAppLinkObjectsIncoming->pushMessage(Message(message, sessionID));
	
	LOG4CPLUS_INFO_EXT(mLogger, " Pushed mobile RPC message");
}

/**
 * \brief push HMI RPC2 message to a queue
 * \param message a message to be pushed
 */
void AppMgrCore::pushRPC2CommunicationMessage( RPC2Communication::RPC2Command * message )
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
    unsigned char sessionID = message.second;
    AppLinkRPC::ALRPCMessage* mobileMsg = message.first;
    if(!mobileMsg)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " No message associated with the session "<<sessionID<<"!");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " A mobile RPC message "<< mobileMsg->getMethodId() <<" has been received for the session id "<<sessionID<<"!");
    if(!pThis)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " pThis should point to an instance of AppMgrCore class");
        return;
    }
    AppMgrCore* core = (AppMgrCore*)pThis;

    switch(mobileMsg->getMethodId())
	{
		case AppLinkRPC::Marshaller::METHOD_REGISTERAPPINTERFACE_REQUEST:
		{
			LOG4CPLUS_INFO_EXT(mLogger, " A RegisterAppInterface request has been invoked");
            AppLinkRPC::RegisterAppInterface_request * object = (AppLinkRPC::RegisterAppInterface_request*)mobileMsg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->registerApplication( object, sessionID ));
            AppLinkRPC::RegisterAppInterface_response* response = new AppLinkRPC::RegisterAppInterface_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Application "<< object->get_appName() <<" hasn't been registered!");
                response->set_success(false);
                response->set_resultCode(AppLinkRPC::Result::APPLICATION_NOT_REGISTERED);
                MobileHandler::getInstance().sendRPCMessage(response, sessionID);
                break;
            }

            if(object->get_autoActivateID())
            {
                response->set_autoActivateID(*object->get_autoActivateID());
            }
            response->set_buttonCapabilities(core->mButtonCapabilities.get());
            response->set_success(true);
            response->set_resultCode(AppLinkRPC::Result::SUCCESS);

            MobileHandler::getInstance().sendRPCMessage(response, sessionID);

            AppLinkRPC::OnHMIStatus* status = new AppLinkRPC::OnHMIStatus();
            status->set_hmiLevel(app->getApplicationHMIStatusLevel());
            MobileHandler::getInstance().sendRPCMessage(status, sessionID);
            RPC2Communication::AppLinkCore::OnAppRegistered* appRegistered = new RPC2Communication::AppLinkCore::OnAppRegistered();
            appRegistered->set_appName(app->getName());
            appRegistered->set_isMediaApplication(app->getIsMediaApplication());
            appRegistered->set_languageDesired(app->getLanguageDesired());
            appRegistered->set_vrSynonym(app->getVrSynonyms());
            HMIHandler::getInstance().sendNotification(appRegistered);

            break;
		}
        case AppLinkRPC::Marshaller::METHOD_UNREGISTERAPPINTERFACE_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An UnregisterAppInterface request has been invoked");

            AppLinkRPC::UnregisterAppInterface_request * object = (AppLinkRPC::UnregisterAppInterface_request*)mobileMsg;
            Application* app = core->getApplicationFromItemCheckNotNull(AppMgrRegistry::getInstance().getItem(sessionID));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " No application has been associated with this registry item!");
                break;
            }
            std::string appName = app->getName();
            core->unregisterApplication( sessionID );
            AppLinkRPC::UnregisterAppInterface_response* response = new AppLinkRPC::UnregisterAppInterface_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(AppLinkRPC::Result::SUCCESS);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);

            AppLinkRPC::OnAppInterfaceUnregistered* msgUnregistered = new AppLinkRPC::OnAppInterfaceUnregistered();
            msgUnregistered->set_reason(AppLinkRPC::AppInterfaceUnregisteredReason(AppLinkRPC::AppInterfaceUnregisteredReason::USER_EXIT));
            MobileHandler::getInstance().sendRPCMessage(msgUnregistered, sessionID);
            RPC2Communication::AppLinkCore::OnAppUnregistered* appUnregistered = new RPC2Communication::AppLinkCore::OnAppUnregistered();
            appUnregistered->set_appName(appName);
            appUnregistered->set_reason(AppLinkRPC::AppInterfaceUnregisteredReason(AppLinkRPC::AppInterfaceUnregisteredReason::USER_EXIT));
            HMIHandler::getInstance().sendNotification(appUnregistered);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_SUBSCRIBEBUTTON_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A SubscribeButton request has been invoked");

           /* RegistryItem* registeredApp = AppMgrRegistry::getInstance().getItem(sessionID);
            ButtonParams* params = new ButtonParams();
            params->mMessage = message;
            IAppCommand* command = new SubscribeButtonCmd(registeredApp, params);
            command->execute();
            delete command; */

            AppLinkRPC::SubscribeButton_request * object = (AppLinkRPC::SubscribeButton_request*)mobileMsg;
            RegistryItem* item = AppMgrRegistry::getInstance().getItem(sessionID);
            if(!item)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Session "<<sessionID<<" hasn't been associated with application!");
                break;
            }
            core->mButtonsMapping.addButton( object->get_buttonName(), item );
            AppLinkRPC::SubscribeButton_response* response = new AppLinkRPC::SubscribeButton_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(AppLinkRPC::Result::SUCCESS);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_UNSUBSCRIBEBUTTON_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An UnsubscribeButton request has been invoked");
            AppLinkRPC::UnsubscribeButton_request * object = (AppLinkRPC::UnsubscribeButton_request*)mobileMsg;
            core->mButtonsMapping.removeButton( object->get_buttonName() );
            AppLinkRPC::UnsubscribeButton_response* response = new AppLinkRPC::UnsubscribeButton_response();
            response->setCorrelationID(object->getCorrelationID());
            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(AppLinkRPC::Result::SUCCESS);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_SHOW_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Show request has been invoked");
            LOG4CPLUS_INFO_EXT(mLogger, "message " << mobileMsg->getMethodId() );
            AppLinkRPC::Show_request* object = (AppLinkRPC::Show_request*)mobileMsg;
            RPC2Communication::UI::Show* showRPC2Request = new RPC2Communication::UI::Show();
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
            core->mMessageMapping.addMessage(showRPC2Request->getId(), sessionID);
            HMIHandler::getInstance().sendRequest(showRPC2Request);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_SPEAK_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Speak request has been invoked");
            AppLinkRPC::Speak_request* object = (AppLinkRPC::Speak_request*)mobileMsg;
            RPC2Communication::TTS::Speak* speakRPC2Request = new RPC2Communication::TTS::Speak();
            speakRPC2Request->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            speakRPC2Request->set_ttsChunks(object->get_ttsChunks());
            core->mMessageMapping.addMessage(speakRPC2Request->getId(), sessionID);
            HMIHandler::getInstance().sendRequest(speakRPC2Request);
            AppLinkRPC::Speak_response * mobileResponse = new AppLinkRPC::Speak_response;
            mobileResponse->set_resultCode(AppLinkRPC::Result::SUCCESS);
            mobileResponse->set_success(true);
            MobileHandler::getInstance().sendRPCMessage(mobileResponse, sessionID);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_SETGLOBALPROPERTIES_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A SetGlobalProperties request has been invoked");
            AppLinkRPC::SetGlobalProperties_request* object = (AppLinkRPC::SetGlobalProperties_request*)mobileMsg;
            RPC2Communication::UI::SetGlobalProperties* setGPRPC2Request = new RPC2Communication::UI::SetGlobalProperties();
            setGPRPC2Request->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(setGPRPC2Request->getId(), sessionID);
            if(object->get_helpPrompt())
            {
                setGPRPC2Request->set_helpPrompt(*object->get_helpPrompt());
            }

            if(object->get_timeoutPrompt())
            {
                setGPRPC2Request->set_timeoutPrompt(*object->get_timeoutPrompt());
            }
            HMIHandler::getInstance().sendRequest(setGPRPC2Request);
            AppLinkRPC::SetGlobalProperties_response * mobileResponse = new AppLinkRPC::SetGlobalProperties_response;
            mobileResponse->set_success(true);
            mobileResponse->set_resultCode(AppLinkRPC::Result::SUCCESS);
            MobileHandler::getInstance().sendRPCMessage(mobileResponse, sessionID);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_RESETGLOBALPROPERTIES_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A ResetGlobalProperties request has been invoked");
            AppLinkRPC::ResetGlobalProperties_request* object = (AppLinkRPC::ResetGlobalProperties_request*)mobileMsg;
            RPC2Communication::UI::ResetGlobalProperties* resetGPRPC2Request = new RPC2Communication::UI::ResetGlobalProperties();
            resetGPRPC2Request->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(resetGPRPC2Request->getId(), sessionID);
            resetGPRPC2Request->set_properties(object->get_properties());

            HMIHandler::getInstance().sendRequest(resetGPRPC2Request);
            AppLinkRPC::ResetGlobalProperties_response * mobileResponse = new AppLinkRPC::ResetGlobalProperties_response;
            mobileResponse->set_success(true);
            mobileResponse->set_resultCode(AppLinkRPC::Result::SUCCESS);
            MobileHandler::getInstance().sendRPCMessage(mobileResponse, sessionID);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_ALERT_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An Alert request has been invoked");
            AppLinkRPC::Alert_request* object = (AppLinkRPC::Alert_request*)mobileMsg;
            RPC2Communication::UI::Alert* alert = new RPC2Communication::UI::Alert();
            alert->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(alert->getId(), sessionID);
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
        case AppLinkRPC::Marshaller::METHOD_ONBUTTONPRESS:
        {
            LOG4CPLUS_INFO_EXT(mLogger, "OnButtonPress Notification has been received.");
            MobileHandler::getInstance().sendRPCMessage(mobileMsg, sessionID);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_ONCOMMAND:
        {
            LOG4CPLUS_INFO_EXT(mLogger, "OnCommand Notification has been received.");
            MobileHandler::getInstance().sendRPCMessage(mobileMsg, sessionID);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_ADDCOMMAND_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand request has been invoked");
            RegistryItem* item = AppMgrRegistry::getInstance().getItem(sessionID);
            if(!item)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Session "<<sessionID<<" hasn't been associated with application!");
                break;
            }
            AppLinkRPC::AddCommand_request* object = (AppLinkRPC::AddCommand_request*)mobileMsg;

            const unsigned int& cmdId = object->get_cmdID();
            if(object->get_menuParams())
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand UI request has been invoked");
                RPC2Communication::UI::AddCommand * addCmd = new RPC2Communication::UI::AddCommand();
                addCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                CommandType cmdType = CommandType::UI;
                const AppLinkRPC::MenuParams* menuParams = object->get_menuParams();
                addCmd->set_menuParams(*menuParams);
                addCmd->set_cmdId(cmdId);
                if(menuParams->get_parentID())
                {
                    const unsigned int& menuId = *menuParams->get_parentID();
                    core->mMenuMapping.addCommand(cmdId, menuId);
                }
                core->mMessageMapping.addMessage(addCmd->getId(), sessionID);
                core->mCommandMapping.addCommand(cmdId, cmdType, item);
                core->mCommandMapping.incrementUnrespondedRequestCount(cmdId);
                core->mRequestMapping.addMessage(addCmd->getId(), cmdId);
                HMIHandler::getInstance().sendRequest(addCmd);

            }
            if(object->get_vrCommands())
            {
                LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand VR request has been invoked");
                RPC2Communication::VR::AddCommand * addCmd = new RPC2Communication::VR::AddCommand();
                addCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                CommandType cmdType = CommandType::VR;
                addCmd->set_vrCommands(*object->get_vrCommands());
                addCmd->set_cmdId(cmdId);
                core->mMessageMapping.addMessage(addCmd->getId(), sessionID);
                core->mCommandMapping.addCommand(cmdId, cmdType, item);
                core->mCommandMapping.incrementUnrespondedRequestCount(cmdId);
                core->mRequestMapping.addMessage(addCmd->getId(), cmdId);
                HMIHandler::getInstance().sendRequest(addCmd);
            }

            break;
        }
        case AppLinkRPC::Marshaller::METHOD_DELETECOMMAND_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand request has been invoked");
            AppLinkRPC::DeleteCommand_request* object = (AppLinkRPC::DeleteCommand_request*)mobileMsg;

            CommandTypes cmdTypes;
            core->mCommandMapping.getTypes(object->get_cmdID(), cmdTypes);
            const unsigned int& cmdId = object->get_cmdID();
            for(CommandTypes::iterator it = cmdTypes.begin(); it != cmdTypes.end(); it++)
            {
                CommandType cmdType = *it;
                if(cmdType == CommandType::UI)
                {
                    LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand UI request has been invoked");
                    RPC2Communication::UI::DeleteCommand* deleteCmd = new RPC2Communication::UI::DeleteCommand();
                    deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(deleteCmd->getId(), sessionID);
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
                    RPC2Communication::VR::DeleteCommand* deleteCmd = new RPC2Communication::VR::DeleteCommand();
                    deleteCmd->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
                    core->mMessageMapping.addMessage(deleteCmd->getId(), sessionID);
                    deleteCmd->set_cmdId(cmdId);
                    core->mCommandMapping.removeCommand(cmdId, cmdType);
                    core->mCommandMapping.incrementUnrespondedRequestCount(cmdId);
                    core->mRequestMapping.addMessage(deleteCmd->getId(), cmdId);
                    HMIHandler::getInstance().sendRequest(deleteCmd);
                }
            }
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_ADDSUBMENU_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An AddSubmenu request has been invoked");
            AppLinkRPC::AddSubMenu_request* object = (AppLinkRPC::AddSubMenu_request*)mobileMsg;
            RPC2Communication::UI::AddSubMenu* addSubMenu = new RPC2Communication::UI::AddSubMenu();
            addSubMenu->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(addSubMenu->getId(), sessionID);
            addSubMenu->set_menuId(object->get_menuID());
            addSubMenu->set_menuName(object->get_menuName());
            if(object->get_position())
            {
                addSubMenu->set_position(*object->get_position());
            }
            HMIHandler::getInstance().sendRequest(addSubMenu);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_DELETESUBMENU_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteSubmenu request has been invoked");
            AppLinkRPC::DeleteSubMenu_request* object = (AppLinkRPC::DeleteSubMenu_request*)mobileMsg;
            RPC2Communication::UI::DeleteSubMenu* delSubMenu = new RPC2Communication::UI::DeleteSubMenu();
            delSubMenu->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(delSubMenu->getId(), sessionID);
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
        case AppLinkRPC::Marshaller::METHOD_CREATEINTERACTIONCHOICESET_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A CreateInteractionChoiceSet request has been invoked");
            AppLinkRPC::CreateInteractionChoiceSet_request* object = (AppLinkRPC::CreateInteractionChoiceSet_request*)mobileMsg;
            RPC2Communication::UI::CreateInteractionChoiceSet* createInteractionChoiceSet = new RPC2Communication::UI::CreateInteractionChoiceSet();
            createInteractionChoiceSet->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(createInteractionChoiceSet->getId(), sessionID);
            createInteractionChoiceSet->set_choiceSet(object->get_choiceSet());
            createInteractionChoiceSet->set_interactionChoiceSetID(object->get_interactionChoiceSetID());
            HMIHandler::getInstance().sendRequest(createInteractionChoiceSet);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_DELETEINTERACTIONCHOICESET_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteInteractionChoiceSet request has been invoked");
            AppLinkRPC::DeleteInteractionChoiceSet_request* object = (AppLinkRPC::DeleteInteractionChoiceSet_request*)mobileMsg;
            RPC2Communication::UI::DeleteInteractionChoiceSet* deleteInteractionChoiceSet = new RPC2Communication::UI::DeleteInteractionChoiceSet();
            deleteInteractionChoiceSet->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(deleteInteractionChoiceSet->getId(), sessionID);
            deleteInteractionChoiceSet->set_interactionChoiceSetID(object->get_interactionChoiceSetID());
            HMIHandler::getInstance().sendRequest(deleteInteractionChoiceSet);
            break;
        }
        case AppLinkRPC::Marshaller::METHOD_PERFORMINTERACTION_REQUEST:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A PerformInteraction request has been invoked");
            AppLinkRPC::PerformInteraction_request* object = (AppLinkRPC::PerformInteraction_request*)mobileMsg;
            RPC2Communication::UI::PerformInteraction* performInteraction = new RPC2Communication::UI::PerformInteraction();
            performInteraction->setId(HMIHandler::getInstance().getJsonRPC2Handler()->getNextMessageId());
            core->mMessageMapping.addMessage(performInteraction->getId(), sessionID);
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
        case AppLinkRPC::Marshaller::METHOD_SHOW_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_SPEAK_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_SETGLOBALPROPERTIES_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_RESETGLOBALPROPERTIES_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_REGISTERAPPINTERFACE_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_SUBSCRIBEBUTTON_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_UNSUBSCRIBEBUTTON_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_ONAPPINTERFACEUNREGISTERED:
        case AppLinkRPC::Marshaller::METHOD_ALERT_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_ADDCOMMAND_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_ADDSUBMENU_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_CREATEINTERACTIONCHOICESET_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_DELETECOMMAND_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_DELETEINTERACTIONCHOICESET_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_DELETESUBMENU_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_ENCODEDSYNCPDATA_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_GENERICRESPONSE_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_PERFORMINTERACTION_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_SETMEDIACLOCKTIMER_RESPONSE:
        case AppLinkRPC::Marshaller::METHOD_UNREGISTERAPPINTERFACE_RESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A "<< mobileMsg->getMethodId() << " response or notification has been invoked");
            MobileHandler::getInstance().sendRPCMessage(mobileMsg, sessionID);
            break;
        }

        case AppLinkRPC::Marshaller::METHOD_INVALID:
		default:
            LOG4CPLUS_ERROR_EXT(mLogger, " An undefined or invalid RPC message "<< mobileMsg->getMethodId() <<" has been received!");
			break;
    }
}

/**
 * \brief push HMI RPC2 message to a queue
 * \param msg a message to be pushed
 * \param pThis a pointer to AppMgrCore class instance
 */
void AppMgrCore::handleBusRPCMessageIncoming(RPC2Communication::RPC2Command* msg , void *pThis)
{
    if(!msg)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " Incoming null pointer from HMI side!");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been incoming...");

    if(!pThis)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, " pThis should point to an instance of AppMgrCore class");
        return;
    }
    AppMgrCore* core = (AppMgrCore*)pThis;
	switch(msg->getMethod())
	{
        case RPC2Communication::Buttons::Marshaller::METHOD_ONBUTTONEVENT:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonEvent notification has been invoked");
            RPC2Communication::Buttons::OnButtonEvent * object = (RPC2Communication::Buttons::OnButtonEvent*)msg;
            const AppLinkRPC::ButtonName & name = object->get_name();
            Application* app = core->getApplicationFromItemCheckNotNull(core->mButtonsMapping.findRegistryItemSubscribedToButton(name));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::OnButtonEvent* event = new AppLinkRPC::OnButtonEvent();
            event->set_buttonEventMode(object->get_mode());

            event->set_buttonName(name);

            unsigned char sessionID = app->getSessionID();
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(event, sessionID);
            return;
        }
        case RPC2Communication::Buttons::Marshaller::METHOD_ONBUTTONPRESS:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnButtonPress notification has been invoked");
            RPC2Communication::Buttons::OnButtonPress * object = (RPC2Communication::Buttons::OnButtonPress*)msg;
            const AppLinkRPC::ButtonName & name = object->get_name();
            Application* app = core->getApplicationFromItemCheckNotNull(core->mButtonsMapping.findRegistryItemSubscribedToButton(name));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::OnButtonPress* event = new AppLinkRPC::OnButtonPress();

            event->set_buttonName(name);
            event->set_buttonPressMode(object->get_mode());
            LOG4CPLUS_INFO_EXT(mLogger, "before we find sessionID");

            unsigned char sessionID = app->getSessionID();
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(event, sessionID);
            return;
        }        
        case RPC2Communication::Buttons::Marshaller::METHOD_GETCAPABILITIESRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A GetButtonCapabilities response has been income");
            RPC2Communication::Buttons::GetCapabilitiesResponse * btnCaps = (RPC2Communication::Buttons::GetCapabilitiesResponse*)msg;
            core->mButtonCapabilities.set( btnCaps->get_capabilities() );
            RPC2Communication::UI::GetCapabilitiesResponse * uiCaps = (RPC2Communication::UI::GetCapabilitiesResponse*)msg;
            core->mDisplayCapabilities = uiCaps->get_displayCapabilities();
            core->mHmiZoneCapabilities.set( uiCaps->get_hmiZoneCapabilities() );
            return;
        }
		case RPC2Communication::Buttons::Marshaller::METHOD_INVALID:
		default:
			LOG4CPLUS_ERROR_EXT(mLogger, " Not Buttons RPC message "<< msg->getMethod() <<" has been received!");
	}

    switch(msg->getMethod())
    {
        case RPC2Communication::UI::Marshaller::METHOD_ONREADY:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnReady UI notification has been invoked");
            HMIHandler::getInstance().setReadyState(true);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_ONCOMMAND:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnCommand UI notification has been invoked");
            RPC2Communication::UI::OnCommand* object = (RPC2Communication::UI::OnCommand*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mCommandMapping.findRegistryItemAssignedToCommand(object->get_commandId(), CommandType::UI));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::OnCommand* event = new AppLinkRPC::OnCommand();
            event->set_cmdID(object->get_commandId());

            unsigned char sessionID = app->getSessionID();
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(event, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_SHOWRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Show response has been income");
            RPC2Communication::UI::ShowResponse* object = (RPC2Communication::UI::ShowResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::Show_response* response = new AppLinkRPC::Show_response();
            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));
            response->set_success(true);

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_SETGLOBALPROPERTIESRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A SetGlobalProperties response has been income");
            RPC2Communication::UI::SetGlobalPropertiesResponse* object = (RPC2Communication::UI::SetGlobalPropertiesResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::SetGlobalProperties_response* response = new AppLinkRPC::SetGlobalProperties_response();

            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));
            response->set_success(true);

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_RESETGLOBALPROPERTIESRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A ResetGlobalProperties response has been income");
            RPC2Communication::UI::ResetGlobalPropertiesResponse* object = (RPC2Communication::UI::ResetGlobalPropertiesResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::ResetGlobalProperties_response* response = new AppLinkRPC::ResetGlobalProperties_response();

            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            response->set_success(true);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_ALERTRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An Alert response has been income");
            RPC2Communication::UI::AlertResponse* object = (RPC2Communication::UI::AlertResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::Alert_response* response = new AppLinkRPC::Alert_response();
            response->set_success(true);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_ADDCOMMANDRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand UI response has been income");
            RPC2Communication::UI::AddCommandResponse* object = (RPC2Communication::UI::AddCommandResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            unsigned char sessionID = app->getSessionID();
            unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
            core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
            if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
            {
                AppLinkRPC::AddCommand_response* response = new AppLinkRPC::AddCommand_response();
                response->set_success(true);
                response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));
                core->mRequestMapping.removeRequest(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            }

            core->mMessageMapping.removeMessage(object->getId());

            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_DELETECOMMANDRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand UI response has been income");
            RPC2Communication::UI::DeleteCommandResponse* object = (RPC2Communication::UI::DeleteCommandResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }

            unsigned char sessionID = app->getSessionID();
            unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
            core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
            if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
            {
                AppLinkRPC::DeleteCommand_response* response = new AppLinkRPC::DeleteCommand_response();
                response->set_success(true);
                response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));
                core->mRequestMapping.removeRequest(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            }

            core->mMessageMapping.removeMessage(object->getId());

            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_ADDSUBMENURESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An AddSubMenu response has been income");
            RPC2Communication::UI::AddSubMenuResponse* object = (RPC2Communication::UI::AddSubMenuResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::AddSubMenu_response* response = new AppLinkRPC::AddSubMenu_response();
            response->set_success(true);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_DELETESUBMENURESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteSubMenu response has been income");
            RPC2Communication::UI::DeleteSubMenuResponse* object = (RPC2Communication::UI::DeleteSubMenuResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::DeleteSubMenu_response* response = new AppLinkRPC::DeleteSubMenu_response();
            response->set_success(true);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_CREATEINTERACTIONCHOICESETRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A CreateInteractionChoiceSet response has been income");
            RPC2Communication::UI::CreateInteractionChoiceSetResponse* object = (RPC2Communication::UI::CreateInteractionChoiceSetResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::CreateInteractionChoiceSet_response* response = new AppLinkRPC::CreateInteractionChoiceSet_response();
            response->set_success(true);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_DELETEINTERACTIONCHOICESETRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteInteractionChoiceSet response has been income");
            RPC2Communication::UI::DeleteInteractionChoiceSetResponse* object = (RPC2Communication::UI::DeleteInteractionChoiceSetResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::DeleteInteractionChoiceSet_response* response = new AppLinkRPC::DeleteInteractionChoiceSet_response();
            response->set_success(true);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_PERFORMINTERACTIONRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A PerformInteraction response has been income");
            RPC2Communication::UI::PerformInteractionResponse* object = (RPC2Communication::UI::PerformInteractionResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            unsigned char sessionID = app->getSessionID();

            AppLinkRPC::PerformInteraction_response* response = new AppLinkRPC::PerformInteraction_response();
            if(object->get_choiceID())
            {
                response->set_choiceID(*object->get_choiceID());
            }
            if(object->get_triggerSource())
            {
                response->set_triggerSource(*object->get_triggerSource());
            }
            response->set_success(true);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));         
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::UI::Marshaller::METHOD_INVALID:
        default:
            LOG4CPLUS_ERROR_EXT(mLogger, " Not UI RPC message "<< msg->getMethod() <<" has been received!");
    }

    switch(msg->getMethod())
    {
        case RPC2Communication::VR::Marshaller::METHOD_ADDCOMMANDRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An AddCommand VR response has been income");
            RPC2Communication::VR::AddCommandResponse* object = (RPC2Communication::VR::AddCommandResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            unsigned char sessionID = app->getSessionID();
            unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
            core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
            if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
            {
                AppLinkRPC::AddCommand_response* response = new AppLinkRPC::AddCommand_response();
                response->set_success(true);
                response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));
                core->mRequestMapping.removeRequest(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            }

            core->mMessageMapping.removeMessage(object->getId());

            return;
        }
        case RPC2Communication::VR::Marshaller::METHOD_DELETECOMMANDRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A DeleteCommand VR response has been income");
            RPC2Communication::VR::DeleteCommandResponse* object = (RPC2Communication::VR::DeleteCommandResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            unsigned char sessionID = app->getSessionID();
            unsigned int cmdId = core->mRequestMapping.findRequestIdAssignedToMessage(object->getId());
            core->mCommandMapping.decrementUnrespondedRequestCount(cmdId);
            if(core->mCommandMapping.getUnrespondedRequestCount(cmdId) == 0)
            {
                AppLinkRPC::DeleteCommand_response* response = new AppLinkRPC::DeleteCommand_response();
                response->set_success(true);
                response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));
                core->mRequestMapping.removeRequest(object->getId());
                LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
                MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            }

            core->mMessageMapping.removeMessage(object->getId());

            return;
        }
        case RPC2Communication::VR::Marshaller::METHOD_ONCOMMAND:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnCommand VR notification has been invoked");
            RPC2Communication::VR::OnCommand* object = (RPC2Communication::VR::OnCommand*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mCommandMapping.findRegistryItemAssignedToCommand(object->get_cmdID(), CommandType::VR));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            unsigned char sessionID = app->getSessionID();

            AppLinkRPC::OnCommand* event = new AppLinkRPC::OnCommand();
            event->set_cmdID(object->get_cmdID());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(event, sessionID);
            return;
        }
        case RPC2Communication::VR::Marshaller::METHOD_INVALID:
        default:
            LOG4CPLUS_ERROR_EXT(mLogger, " Not VR RPC message "<< msg->getMethod() <<" has been received!");
    }

    switch(msg->getMethod())
    {
        case RPC2Communication::TTS::Marshaller::METHOD_SPEAKRESPONSE:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " A Speak response has been income");
            RPC2Communication::TTS::SpeakResponse* object = (RPC2Communication::TTS::SpeakResponse*)msg;
            Application* app = core->getApplicationFromItemCheckNotNull(core->mMessageMapping.findRegistryItemAssignedToCommand(object->getId()));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::Speak_response* response = new AppLinkRPC::Speak_response();
            response->setMessageType(AppLinkRPC::ALRPCMessage::RESPONSE);
            response->set_resultCode(static_cast<AppLinkRPC::Result::ResultInternal>(object->getResult()));
            response->set_success(true);

            unsigned char sessionID = app->getSessionID();
            core->mMessageMapping.removeMessage(object->getId());
            LOG4CPLUS_INFO_EXT(mLogger, " A message will be sent to an app "<< app->getName()<<" session id "<<sessionID);
            MobileHandler::getInstance().sendRPCMessage(response, sessionID);
            return;
        }
        case RPC2Communication::TTS::Marshaller::METHOD_INVALID:
        default:
            LOG4CPLUS_ERROR_EXT(mLogger, " Not TTS RPC message "<< msg->getMethod() <<" has been received!");
    }

    switch(msg->getMethod())
    {
        case RPC2Communication::AppLinkCore::Marshaller::METHOD_ONAPPUNREGISTERED:
        {
            LOG4CPLUS_INFO_EXT(mLogger, " An OnAppUnregistered notification has been income");
            RPC2Communication::AppLinkCore::OnAppUnregistered * object = (RPC2Communication::AppLinkCore::OnAppUnregistered*)msg;
            AppLinkRPC::OnAppInterfaceUnregistered* event = new AppLinkRPC::OnAppInterfaceUnregistered();
            if ( object->get_reason() )
            {
                event->set_reason(*object->get_reason());
            }            
            MobileHandler::getInstance().sendRPCMessage(event, 1);//just temporarily!!!
            return;
        }        
        case RPC2Communication::AppLinkCore::Marshaller::METHOD_ACTIVATEAPP:
        {
            LOG4CPLUS_INFO_EXT(mLogger, "ActivateApp has been received!");
            RPC2Communication::AppLinkCore::ActivateApp* object = static_cast<RPC2Communication::AppLinkCore::ActivateApp*>(msg);
            if ( !object )
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "Couldn't cast object to ActivateApp type");
                return;
            }     
            const std::string& appName = object->get_appName();
            Application* app = core->getApplicationFromItemCheckNotNull(AppMgrRegistry::getInstance().getItem(appName));
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
                return;
            }
            AppLinkRPC::OnHMIStatus * hmiStatus = new AppLinkRPC::OnHMIStatus;
            hmiStatus->set_hmiLevel(AppLinkRPC::HMILevel::HMI_FULL);


            app->setApplicationHMIStatusLevel(AppLinkRPC::HMILevel::HMI_FULL);
            hmiStatus->set_audioStreamingState(app->getApplicationAudioStreamingState());
            hmiStatus->set_systemContext(AppLinkRPC::SystemContext::SYSCTXT_MENU);
            MobileHandler::getInstance().sendRPCMessage( hmiStatus, app->getSessionID() );
            RPC2Communication::AppLinkCore::ActivateAppResponse * response = new RPC2Communication::AppLinkCore::ActivateAppResponse;
            response->setId(object->getId());
            response->setResult(AppLinkRPC::Result::SUCCESS);
            HMIHandler::getInstance().sendResponse(response);
            return;
        }
        case RPC2Communication::AppLinkCore::Marshaller::METHOD_INVALID:
        default:
            LOG4CPLUS_ERROR_EXT(mLogger, " Not AppLinkCore RPC message "<< msg->getMethod() <<" has been received!");
    }

    LOG4CPLUS_INFO_EXT(mLogger, " A RPC2 bus message "<< msg->getMethod() <<" has been invoked!");
}

/**
 * \brief Register an application
 * \param request a RegisterAppInterface request which is the source for application fields initial values
 * \param sessionID an id of the session which will be associated with the application
 * \return A instance of RegistryItem created for application
 */
const RegistryItem* AppMgrCore::registerApplication( AppLinkRPC::RegisterAppInterface_request * request, const unsigned char& sessionID )
{
    if(!request)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "Null-request specified for session "<<sessionID<<"!");
        return 0;
    }

    LOG4CPLUS_INFO_EXT(mLogger, " Registering an application " << request->get_appName() << "!");

    const std::string& appName = request->get_appName();
    Application* application = new Application( appName, sessionID );

    bool isMediaApplication = request->get_isMediaApplication();
    const AppLinkRPC::Language& languageDesired = request->get_languageDesired();
    const AppLinkRPC::SyncMsgVersion& syncMsgVersion = request->get_syncMsgVersion();

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

	application->setApplicationHMIStatusLevel(AppLinkRPC::HMILevel::HMI_NONE);
    LOG4CPLUS_INFO_EXT(mLogger, "Application created." );
    return AppMgrRegistry::getInstance().registerApplication( application );
}

/**
 * \brief unregister an application associated with the given session
 * \param sessionID an id of the session asociated with the application to be unregistered
 */
void AppMgrCore::unregisterApplication(const unsigned char &sessionID)
{
    RegistryItem* item = AppMgrRegistry::getInstance().getItem(sessionID);
    Application* app = getApplicationFromItemCheckNotNull( item );
    if(!app)
    {
        LOG4CPLUS_ERROR_EXT(mLogger, "No application associated with this registry item!");
        return;
    }
    LOG4CPLUS_INFO_EXT(mLogger, " Unregistering an application " << app->getName() << " session id" << sessionID << "!");

    const std::string& appName = app->getName();
    LOG4CPLUS_INFO_EXT(mLogger, " Unregistering an application " << appName <<" session id " <<sessionID<< "!");
    mButtonsMapping.removeItem(item);
    mCommandMapping.removeItem(item);
    mMessageMapping.removeItem(item);
    AppMgrRegistry::getInstance().unregisterApplication(item);
    LOG4CPLUS_INFO_EXT(mLogger, " Unregistered an application " << appName << " session id " <<sessionID<< "!");
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
    MobileHandler::getInstance().getJsonHandler();
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
