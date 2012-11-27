/**
 * \file Application.cpp
 * \brief Application metaphor
 * \author vsalo
 */

#include "AppMgr/Application.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger Application::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("Application"));

    /**
     * \brief Class constructor
     * \param name application name
     * \param connectionId id of the connection associated with this application
     * \param sessionId id of the session associated with this application
     */
    Application::Application(const std::string& name , unsigned int connectionId, unsigned char sessionId)
        :mNgnMediaScreenAppName("")
        ,m_bUsesVehicleData(false)
        ,m_bIsMediaApplication(false)
        ,mAutoActivateID("")
        ,mSessionID(sessionId)
        ,mConnectionID(connectionId)
        ,mName(name)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Created an application " << name << " for the connection id " << connectionId << " session id " << (uint)sessionId);
    }

    /**
     * \brief Copy constructor
     */
    Application::Application( const Application& app )
        :mNgnMediaScreenAppName(app.getNgnMediaScreenAppName())
        ,mName(app.getName())
        ,m_bUsesVehicleData(app.getUsesVehicleData())
        ,m_bIsMediaApplication(app.getIsMediaApplication())
        ,mAutoActivateID(app.getAutoActivateID())
        ,mLanguageDesired(app.getLanguageDesired())
        ,mHMIStatusLevel(app.getApplicationHMIStatusLevel())
        ,mSyncMsgVersion(app.getSyncMsgVersion())
        ,mConnectionID(app.getConnectionID())
        ,mSessionID(app.getSessionID())
    {
        mVrSynonyms = app.getVrSynonyms();
    }

    /**
     * \brief Default class destructor
     */
    Application::~Application( )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Deleted an application " << mName << " connection id " << mConnectionID << " session id " << mSessionID);
    }

    /**
     * \brief operator ==
     * \param item the item to compare with
     * \return comparison result
     */
    bool Application::operator ==(const Application &item) const
    {
        return this->getSessionID() == item.getSessionID()
            && this->getConnectionID() == item.getConnectionID();
    }

    /**
     * \brief retrieve aplication HMI status level
     * \return HMI status level of application
     */
    const NsAppLinkRPC::HMILevel::HMILevelInternal& Application::getApplicationHMIStatusLevel( ) const
    {
        return mHMIStatusLevel;
    }

    /**
     * \brief Set application HMI status level
     * \param hmiLevel HMI status level of application
     */
    void Application::setApplicationHMIStatusLevel( const NsAppLinkRPC::HMILevel::HMILevelInternal& hmiLevel )
    {
        mHMIStatusLevel = hmiLevel;
    }

    /**
     * \brief retrieve application audio streaming state
     * \return application audio streaming state
     */
    const NsAppLinkRPC::AudioStreamingState& Application::getApplicationAudioStreamingState( ) const
    {
        return mAudioStreamingState;
    }

    /**
     * \brief Set application audio streaming state
     * \param streamingState audio streaming state of application
     */
    void Application::setApplicationAudioStreamingState( const NsAppLinkRPC::AudioStreamingState& streamingState )
    {
        mAudioStreamingState = streamingState;
    }

    /**
     * \brief Set application NGN media screen app name
     * \param value application NGN media screen app name
     */
    void Application::setNgnMediaScreenAppName(const std::string& value)
    {
        mNgnMediaScreenAppName = value;
    }

    /**
     * \brief Set application voice recognition synonyms
     * \param value application voice recognition synonyms
     */
    void Application::setVrSynonyms(const std::vector<std::string>& value)
    {
        mVrSynonyms = value;
    }

    /**
     * \brief Set application usage of vehicle data
     * \param value does the application use vehicle data
     */
    void Application::setUsesVehicleData(bool value)
    {
        m_bUsesVehicleData = value;
    }

    /**
     * \brief Set if the application is a media application
     * \param value is the application a media application
     */
    void Application::setIsMediaApplication(bool value)
    {
        m_bIsMediaApplication = value;
    }

    /**
     * \brief Set application desired languuage
     * \param value application desired language
     */
    void Application::setLanguageDesired(NsAppLinkRPC::Language value)
    {
        mLanguageDesired = value;
    }

    /**
     * \brief Set application autoactivate ID
     * \param value application autoactivate ID
     */
    void Application::setAutoActivateID(const std::string& value)
    {
        mAutoActivateID = value;
    }

    /**
     * \brief Set application sync message version
     * \param value application sync message version
     */
    void Application::setSyncMsgVersion(NsAppLinkRPC::SyncMsgVersion value)
    {
        mSyncMsgVersion = value;
    }

    /**
     * \brief Set application ID
     * \param value application ID
     */
    void Application::setAppID(const std::string &value)
    {
        mAppID = value;
    }

    /**
     * \brief Set application HMI desired display language
     * \param value application HMI desired display language
     */
    void Application::setHMIDisplayLanguageDesired(NsAppLinkRPC::Language value)
    {
        mHMIDisplayLanguageDesired = value;
    }

    /**
     * \brief set application system context
     * \param application system context
     */
    void Application::setSystemContext(NsAppLinkRPC::SystemContext value)
    {
        mSystemContext = value;
    }

    /**
     * \brief retrieve application NGN media screen application name
     * \return application NGN media screen application name
     */
    const std::string& Application::getNgnMediaScreenAppName( ) const
    {
        return mNgnMediaScreenAppName;
    }

    /**
     * \brief retrieve application voice-recognition synonyms
     * \return application voice-recognition synonyms
     */
    const std::vector<std::string>& Application::getVrSynonyms( ) const
    {
        return mVrSynonyms;
    }

    /**
     * \brief retrieve does the application use vehicle data
     * \return does the application use vehicle data
     */
    bool Application::getUsesVehicleData( ) const
    {
        return m_bUsesVehicleData;
    }

    /**
     * \brief retrieve is the application a media application
     * \return is the application a media application
     */
    bool Application::getIsMediaApplication( ) const
    {
        return m_bIsMediaApplication;
    }

    /**
     * \brief retrieve application desired language
     * \return application desired language
     */
    const NsAppLinkRPC::Language& Application::getLanguageDesired( ) const
    {
        return mLanguageDesired;
    }

    /**
     * \brief retrieve application auto-activate ID
     * \return application auto-activate ID
     */
    const std::string& Application::getAutoActivateID( ) const
    {
        return mAutoActivateID;
    }

    /**
     * \brief retrieve application sync message version
     * \return application sync msg version
     */
    const NsAppLinkRPC::SyncMsgVersion& Application::getSyncMsgVersion( ) const
    {
        return mSyncMsgVersion;
    }

    /**
     * \brief retrieve application ID
     * \return application ID
     */
    const std::string &Application::getAppID() const
    {
        return mAppID;
    }

    /**
     * \brief retrieve application HMI desired display language
     * \return application HMI desired display language
     */
    const NsAppLinkRPC::Language &Application::getHMIDisplayLanguageDesired() const
    {
        return mHMIDisplayLanguageDesired;
    }

    /**
     * \brief retrieve application session ID
     * \return application session ID
     */
    unsigned char Application::getSessionID() const
    {
        return mSessionID;
    }

    /**
     * \brief retrieve application session ID
     * \return application connection ID
     */
    unsigned int Application::getConnectionID() const
    {
        return mConnectionID;
    }

    /**
     * \brief retrieve application name
     * \return application name
     */
    const std::string &Application::getName() const
    {
        return mName;
    }

    /**
     * \brief retrieve application system context
     * \return application system context
     */
    const NsAppLinkRPC::SystemContext &Application::getSystemContext() const
    {
        return mSystemContext;
    }

    /**
     * \brief add an interaction choice set item to the application
     * \param choiceSetId interaction choice set id
     * \param choiceSet interaction choice set
     */
    void Application::addChoiceSet(const unsigned int &choiceSetId, const ChoiceSet &choiceSet)
    {
        mChoiceSets.addItem(choiceSetId, choiceSet);
    }

    /**
     * \brief remove an interaction choice set from the application
     * \param choiceSetId interaction choice set id
     */
    void Application::removeChoiceSet(const unsigned int &choiceSetId)
    {
        mChoiceSets.removeItem(choiceSetId);
    }

    /**
     * \brief gets all interaction choice set items
     * \return interaction choice set items
     */
    ChoiceSetItems Application::getAllChoiceSets() const
    {
        return mChoiceSets.getAllChoiceSets();
    }

    /**
     * \brief get count of interaction choice sets
     * \return interaction choice sets count
     */
    size_t Application::getChoiceSetsCount() const
    {
        return mChoiceSets.size();
    }

    /**
     * \brief add a command to a menu
     * \param commandId command id
     * \param menuId menu id
     */
    void Application::addMenuCommand(const unsigned int &commandId, const unsigned int &menuId)
    {
        mMenuMapping.addCommand(commandId, menuId);
    }

    /**
     * \brief remove a command from a menu(s)
     * \param commandId command id
     */
    void Application::removeMenuCommand(const unsigned int &commandId)
    {
        mMenuMapping.removeCommand(commandId);
    }

    /**
     * \brief find commands within a menu
     * \param menuId menu id
     * \return commands residing within the given menu
     */
    MenuCommands Application::findMenuCommands(const unsigned int &menuId) const
    {
        return mMenuMapping.findCommandsAssignedToMenu(menuId);
    }

    /**
     * \brief add a menu item to the application
     * \param menuId menu id
     * \param menuName menu item name
     * \param position menu item position within the parent menu
     */
    void Application::addMenu(const unsigned int &menuId, const std::string &menuName, const unsigned int *position)
    {
        mMenus.addItem(menuId, menuName, position);
    }

    /**
     * \brief remove a menu item from the application
     * \param menuId menu id
     */
    void Application::removeMenu(const unsigned int &menuId)
    {
        mMenus.removeItem(menuId);
    }

    /**
     * \brief gets all application menus
     * \return application menus
     */
    MenuItems Application::getAllMenus() const
    {
        return mMenus.getAllMenuItems();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t Application::getMenusCount() const
    {
        return mMenus.size();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t Application::getMenuCommandsCount() const
    {
        return mMenuMapping.size();
    }

    /**
     * \brief add a command to an application
     * \param commandId command id
     * \param type command type
     * \param params VR or UI params supplied with the AddCommand request
     */
    void Application::addCommand(unsigned int commandId, CommandType type, CommandParams params)
    {
        mCommandMapping.addCommand(commandId, type, params);
    }

    /**
     * \brief remove a command from application
     * \param commandId command id
     * \param type a type of a command
     */
    void Application::removeCommand(unsigned int commandId, CommandType type)
    {
        mCommandMapping.removeCommand(commandId, type);
    }

    /**
     * \brief finds commands in application
     * \param commandId command id
     * \return command list
     */
    Commands Application::findCommands(unsigned int commandId) const
    {
        return mCommandMapping.findCommands(commandId);
    }

    /**
     * \brief gets all application commands
     * \return application commands
     */
    Commands Application::getAllCommands() const
    {
        return mCommandMapping.getAllCommands();
    }

    /**
     * \brief get count of items
     * \return items count
     */
    size_t Application::getCommandsCount() const
    {
        return mCommandMapping.size();
    }

    /**
     * \brief retrieve types associated with command id in current application
     * \param commandId command id to search for types
     * \param types input container of command types to be filled with result
     */
    void Application::getTypes(unsigned int commandId, CommandTypes &types) const
    {
        mCommandMapping.getTypes(commandId, types);
    }

    /**
     * \brief get count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to count unresponded requests for
     * \return unresponded requests count
     */
    unsigned int Application::getUnrespondedRequestCount(const unsigned int &cmdId) const
    {
        return mCommandMapping.getUnrespondedRequestCount(cmdId);
    }

    /**
     * \brief increment count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to increment unresponded request count for
     * \return unresponded requests count after the operation
     */
    unsigned int Application::incrementUnrespondedRequestCount(const unsigned int &cmdId)
    {
        return mCommandMapping.incrementUnrespondedRequestCount(cmdId);
    }

    /**
     * \brief decrement count of unresponsed requests associated with the given command id
     * \param cmdId id of command we need to decrement unresponded request count for
     * \return unresponded requests count after the operation
     */
    unsigned int Application::decrementUnrespondedRequestCount(const unsigned int &cmdId)
    {
        return mCommandMapping.decrementUnrespondedRequestCount(cmdId);
    }

}
