#ifndef APPLICATION_V2_H
#define APPLICATION_V2_H

#include "AppMgr/Application.h"
#include "JSONHandler/ALRPCObjects/V2/AudioStreamingState.h"
#include "JSONHandler/ALRPCObjects/V2/SystemContext.h"
#include "JSONHandler/ALRPCObjects/V2/Language.h"
#include "JSONHandler/ALRPCObjects/V2/AppType.h"
#include "JSONHandler/ALRPCObjects/V2/HMILevel.h"
#include "JSONHandler/ALRPCObjects/V2/SyncMsgVersion.h"
#include "JSONHandler/ALRPCObjects/V2/TTSChunk.h"

namespace NsAppManager
{
    /**
     * \brief Application types
     */
    typedef std::vector<NsAppLinkRPCV2::AppType> AppTypes;

    /**
     * \brief TTS name
     */
    typedef std::vector<NsAppLinkRPCV2::TTSChunk> TTSName;

    /**
     * \brief class Application acts as a metaphor for every mobile application of protocol v2 being registered on HMI
     */
    class Application_v2 : public Application
    {
    public:

        /**
         * \brief Class constructor
         * \param name application name
         * \param connectionId id of the connection associated with this application
         * \param sessionId id of the session associated with this application
         * \param appId application id
         */
        Application_v2(const std::string& name, unsigned int connectionId, unsigned char sessionId, int appId );

        /**
         * \brief Default class destructor
         */
        virtual ~Application_v2( );

        /**
         * \brief Set application ID
         * \param value application ID
         */
   //     void setAppID( const std::string& value );

        /**
         * \brief retrieve application ID
         * \return application ID
         */
    //    const std::string& getAppID( ) const;

        /**
         * \brief Set application desired languuage
         * \param value application desired language
         */
        void setLanguageDesired(NsAppLinkRPCV2::Language value);

        /**
         * \brief retrieve application desired language
         * \return application desired language
         */
        const NsAppLinkRPCV2::Language& getLanguageDesired( ) const;

        /**
         * \brief Set application HMI desired display language
         * \param value application HMI desired display language
         */
        void setHMIDisplayLanguageDesired( NsAppLinkRPCV2::Language value );

        /**
         * \brief retrieve application HMI desired display language
         * \return application HMI desired display language
         */
        const NsAppLinkRPCV2::Language& getHMIDisplayLanguageDesired( ) const;

        /**
         * \brief set application system context
         * \param application system context
         */
        void setSystemContext( NsAppLinkRPCV2::SystemContext value );

        /**
         * \brief retrieve application system context
         * \return application system context
         */
        const NsAppLinkRPCV2::SystemContext& getSystemContext() const;

        /**
         * \brief Set application audio streaming state
         * \param streamingState audio streaming state of application
         */
        void setApplicationAudioStreamingState( const NsAppLinkRPCV2::AudioStreamingState& hmiLevel );

        /**
         * \brief retreive application audio streaming state
         * \return application audio streaming state
         */
        const NsAppLinkRPCV2::AudioStreamingState& getApplicationAudioStreamingState( ) const;

        /**
         * \brief Set application type
         * \param appType application type
         */
        void setAppType(const AppTypes& appType);

        /**
         * \brief retreive application type
         * \param appId application type
         */
        const AppTypes& getAppType() const;

        /**
         * \brief Set application HMI status level
         * \param hmiLevel HMI status level of application
         */
        void setApplicationHMIStatusLevel( const NsAppLinkRPCV2::HMILevel::HMILevelInternal& hmiLevel );

        /**
         * \brief retrieve aplication HMI status level
         * \return HMI status level of application
         */
        const NsAppLinkRPCV2::HMILevel::HMILevelInternal& getApplicationHMIStatusLevel( ) const;

        /**
         * \brief Set application sync message version
         * \param value application sync message version
         */
        void setSyncMsgVersion(NsAppLinkRPCV2::SyncMsgVersion value);

        /**
         * \brief retrieve application sync message version
         * \return application sync msg version
         */
        const NsAppLinkRPCV2::SyncMsgVersion& getSyncMsgVersion( ) const;

        /**
         * \brief Set application text-to-speech name
         * \param value application text-to-speech name
         */
        void setTtsName(const TTSName& value);

        /**
         * \brief retrieve application text-to-speech name
         * \return application text-to-speech name
         */
        const TTSName& getTtsName( ) const;

    private:

        /**
         * \brief Copy constructor
         */
        Application_v2(const Application_v2& );

        NsAppLinkRPCV2::Language mLanguageDesired;
        NsAppLinkRPCV2::AudioStreamingState mAudioStreamingState;
        NsAppLinkRPCV2::Language mHMIDisplayLanguageDesired;
        NsAppLinkRPCV2::SystemContext mSystemContext;
        NsAppLinkRPCV2::HMILevel::HMILevelInternal mHMIStatusLevel;
        NsAppLinkRPCV2::SyncMsgVersion mSyncMsgVersion;

   //     std::string mAppID;
        AppTypes mAppType;
        TTSName mTtsName;
    };
}

#endif // APPLICATION_V2_H
