#include "AppMgr/Application_v2.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    /**
     * \brief Class constructor
     * \param name application name
     * \param protocolVersion protocol version
     * \param appId application id
     */
    Application_v2::Application_v2(const std::string& name , int appId)
        :Application(name, appId, 2)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Created an application " << name << " for the application id " << appId);
    }

    /**
     * \brief Copy constructor
     */
    Application_v2::Application_v2( const Application_v2& app )
        :Application(app)
    {
    }

    /**
     * \brief Default class destructor
     */
    Application_v2::~Application_v2( )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Deleted an application " << mName << " application id " << mAppID);
    }

    /**
     * \brief retrieve application ID
     * \return application ID
     */
 /*   const std::string &Application_v2::getAppID() const
    {
        return mAppID;
    }
*/
    /**
     * \brief Set application ID
     * \param value application ID
     */
 /*   void Application_v2::setAppID(const std::string &value)
    {
        mAppID = value;
    }
*/
    /**
     * \brief retrieve application desired language
     * \return application desired language
     */
    const NsAppLinkRPCV2::Language& Application_v2::getLanguageDesired( ) const
    {
        return mLanguageDesired;
    }

    /**
     * \brief Set application desired languuage
     * \param value application desired language
     */
    void Application_v2::setLanguageDesired(NsAppLinkRPCV2::Language value)
    {
        mLanguageDesired = value;
    }

    /**
     * \brief retrieve application HMI desired display language
     * \return application HMI desired display language
     */
    const NsAppLinkRPCV2::Language &Application_v2::getHMIDisplayLanguageDesired() const
    {
        return mHMIDisplayLanguageDesired;
    }

    /**
     * \brief Set application HMI desired display language
     * \param value application HMI desired display language
     */
    void Application_v2::setHMIDisplayLanguageDesired(NsAppLinkRPCV2::Language value)
    {
        mHMIDisplayLanguageDesired = value;
    }

    /**
     * \brief retrieve application audio streaming state
     * \return application audio streaming state
     */
    const NsAppLinkRPCV2::AudioStreamingState& Application_v2::getApplicationAudioStreamingState( ) const
    {
        return mAudioStreamingState;
    }

    /**
     * \brief Set application audio streaming state
     * \param streamingState audio streaming state of application
     */
    void Application_v2::setApplicationAudioStreamingState( const NsAppLinkRPCV2::AudioStreamingState& streamingState )
    {
        mAudioStreamingState = streamingState;
    }

    /**
     * \brief retrieve application system context
     * \return application system context
     */
    const NsAppLinkRPCV2::SystemContext &Application_v2::getSystemContext() const
    {
        return mSystemContext;
    }

    /**
     * \brief set application system context
     * \param application system context
     */
    void Application_v2::setSystemContext(NsAppLinkRPCV2::SystemContext value)
    {
        mSystemContext = value;
    }

    /**
     * \brief Set application type
     * \param appType application type
     */
    void Application_v2::setAppType(const AppTypes &appType)
    {
        mAppType = appType;
    }

    /**
     * \brief retreive application type
     * \param appId application type
     */
    const AppTypes &Application_v2::getAppType() const
    {
        return mAppType;
    }

    /**
     * \brief retrieve aplication HMI status level
     * \return HMI status level of application
     */
    const NsAppLinkRPCV2::HMILevel::HMILevelInternal& Application_v2::getApplicationHMIStatusLevel( ) const
    {
        return mHMIStatusLevel;
    }

    /**
     * \brief Set application HMI status level
     * \param hmiLevel HMI status level of application
     */
    void Application_v2::setApplicationHMIStatusLevel( const NsAppLinkRPCV2::HMILevel::HMILevelInternal& hmiLevel )
    {
        mHMIStatusLevel = hmiLevel;
    }

    /**
     * \brief retrieve application sync message version
     * \return application sync msg version
     */
    const NsAppLinkRPCV2::SyncMsgVersion& Application_v2::getSyncMsgVersion( ) const
    {
        return mSyncMsgVersion;
    }

    /**
     * \brief Set application sync message version
     * \param value application sync message version
     */
    void Application_v2::setSyncMsgVersion(NsAppLinkRPCV2::SyncMsgVersion value)
    {
        mSyncMsgVersion = value;
    }

    /**
     * \brief retrieve application text-to-speech name
     * \return application text-to-speech name
     */
    const TTSName &Application_v2::getTtsName() const
    {
        return mTtsName;
    }

    /**
     * \brief Set application text-to-speech name
     * \param value application text-to-speech name
     */
    void Application_v2::setTtsName(const TTSName &value)
    {
        mTtsName = value;
    }

    /**
     * \brief add an interaction choice set item to the application
     * \param choiceSetId interaction choice set id
     * \param choiceSet interaction choice set
     */
    void Application_v2::addChoiceSet(const unsigned int &choiceSetId, const ChoiceSetV2 &choiceSet)
    {
        mChoiceSets.addItem(choiceSetId, choiceSet);
    }

    /**
     * \brief remove an interaction choice set from the application
     * \param choiceSetId interaction choice set id
     */
    void Application_v2::removeChoiceSet(const unsigned int &choiceSetId)
    {
        mChoiceSets.removeItem(choiceSetId);
    }

    /**
     * \brief gets all interaction choice set items
     * \return interaction choice set items
     */
    ChoiceSetItems Application_v2::getAllChoiceSets() const
    {
        return mChoiceSets.getAllChoiceSets();
    }

    /**
     * \brief get count of interaction choice sets
     * \return interaction choice sets count
     */
    size_t Application_v2::getChoiceSetsCount() const
    {
        return mChoiceSets.size();
    }

    /**
     * \brief find a mapped choice set item
     * \param choiceSetId interaction choice set id
     * \return a mapped choice set item
     */
    ChoiceSetV2 Application_v2::findChoiceSet(const unsigned int &choiceSetId)
    {
        const ChoiceSetGeneric* choiceSetGeneric = mChoiceSets.findItem(choiceSetId);
        return choiceSetGeneric ? choiceSetGeneric->choiceSetV2 : ChoiceSetV2();
    }
}
