#include "AppMgr/Application_v2.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{
    /**
     * \brief Class constructor
     * \param name application name
     * \param connectionId id of the connection associated with this application
     * \param sessionId id of the session associated with this application
     * \param protocolVersion protocol version
     * \param appId application id
     */
    Application_v2::Application_v2(const std::string& name , unsigned int connectionId, unsigned char sessionId, int appId)
        :Application(name, connectionId, sessionId, appId, 2)
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Created an application " << name << " for the connection id " << connectionId << " session id " << (uint)sessionId);
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
        LOG4CPLUS_INFO_EXT(mLogger, " Deleted an application " << mName << " connection id " << mConnectionID << " session id " << mSessionID);
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
}
