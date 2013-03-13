//
// Copyright (c) 2013 Ford Motor Company
//

#include "AppMgr/Application_v1.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    /**
     * \brief Class constructor
     * \param name application name
     * \param appId application id
     */
    Application_v1::Application_v1(const std::string& name, int appId)
        :Application(name, appId, 1)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Created an application " << name
            << " for the application id " << appId);
    }

    /**
     * \brief Copy constructor
     */
    Application_v1::Application_v1( const Application_v1& app )
        :Application(app)
    {
    }

    /**
     * \brief Default class destructor
     */
    Application_v1::~Application_v1( )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Deleted an application " << mName
            << " appId " << mAppID);
    }

    /**
     * \brief retrieve does the application use vehicle data
     * \return does the application use vehicle data
     */
    bool Application_v1::getUsesVehicleData( ) const
    {
        return m_bUsesVehicleData;
    }

    /**
     * \brief Set application usage of vehicle data
     * \param value does the application use vehicle data
     */
    void Application_v1::setUsesVehicleData(bool value)
    {
        m_bUsesVehicleData = value;
    }

    /**
     * \brief retrieve application desired language
     * \return application desired language
     */
    const NsAppLinkRPC::Language& Application_v1::getLanguageDesired( ) const
    {
        return mLanguageDesired;
    }

    /**
     * \brief Set application desired languuage
     * \param value application desired language
     */
    void Application_v1::setLanguageDesired(NsAppLinkRPC::Language value)
    {
        mLanguageDesired = value;
    }

    /**
     * \brief retrieve application HMI desired display language
     * \return application HMI desired display language
     */
    const NsAppLinkRPC::Language &Application_v1::getHMIDisplayLanguageDesired() const
    {
        return mHMIDisplayLanguageDesired;
    }

    /**
     * \brief Set application HMI desired display language
     * \param value application HMI desired display language
     */
    void Application_v1::setHMIDisplayLanguageDesired(NsAppLinkRPC::Language value)
    {
        mHMIDisplayLanguageDesired = value;
    }

    
    /**
     * \brief retrieve application sync message version
     * \return application sync msg version
     */
    const NsAppLinkRPC::SyncMsgVersion& Application_v1::getSyncMsgVersion( ) const
    {
        return mSyncMsgVersion;
    }

    /**
     * \brief Set application sync message version
     * \param value application sync message version
     */
    void Application_v1::setSyncMsgVersion(NsAppLinkRPC::SyncMsgVersion value)
    {
        mSyncMsgVersion = value;
    }

    /**
     * \brief add an interaction choice set item to the application
     * \param choiceSetId interaction choice set id
     * \param choiceSet interaction choice set
     */
    void Application_v1::addChoiceSet(const unsigned int &choiceSetId, const ChoiceSetV1 &choiceSet)
    {
        mChoiceSets.addItem(choiceSetId, choiceSet);
    }

    /**
     * \brief remove an interaction choice set from the application
     * \param choiceSetId interaction choice set id
     */
    void Application_v1::removeChoiceSet(const unsigned int &choiceSetId)
    {
        mChoiceSets.removeItem(choiceSetId);
    }

    /**
     * \brief gets all interaction choice set items
     * \return interaction choice set items
     */
    ChoiceSetItems Application_v1::getAllChoiceSets() const
    {
        return mChoiceSets.getAllChoiceSets();
    }

    /**
     * \brief get count of interaction choice sets
     * \return interaction choice sets count
     */
    size_t Application_v1::getChoiceSetsCount() const
    {
        return mChoiceSets.size();
    }

    /**
     * \brief find a mapped choice set item
     * \param choiceSetId interaction choice set id
     * \return a mapped choice set item
     */
    const ChoiceSetV1* Application_v1::findChoiceSet(const unsigned int &choiceSetId)
    {
        const ChoiceSetGeneric* choiceSetGeneric = mChoiceSets.findItem(choiceSetId);
        return choiceSetGeneric ? &choiceSetGeneric->choiceSetV1 : 0;
    }
}
