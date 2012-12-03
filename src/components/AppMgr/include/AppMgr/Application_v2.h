#ifndef APPLICATION_V2_H
#define APPLICATION_V2_H

#include "AppMgr/Application.h"
#include "JSONHandler/ALRPCObjects/AudioStreamingState_v2.h"
#include "JSONHandler/ALRPCObjects/SystemContext_v2.h"
#include "JSONHandler/ALRPCObjects/Language_v2.h"
#include "JSONHandler/ALRPCObjects/AppType.h"

namespace NsAppManager
{
    /**
     * \brief Application types
     */
    typedef std::vector<NsAppLinkRPC::AppType> AppTypes;

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
         */
        Application_v2(const std::string& name, unsigned int connectionId, unsigned char sessionId );

        /**
         * \brief Default class destructor
         */
        virtual ~Application_v2( );

        /**
         * \brief Set application ID
         * \param value application ID
         */
        void setAppID( const std::string& value );

        /**
         * \brief retrieve application ID
         * \return application ID
         */
        const std::string& getAppID( ) const;

        /**
         * \brief Set application desired languuage
         * \param value application desired language
         */
        void setLanguageDesired(NsAppLinkRPC::Language_v2 value);

        /**
         * \brief retrieve application desired language
         * \return application desired language
         */
        const NsAppLinkRPC::Language_v2& getLanguageDesired( ) const;

        /**
         * \brief Set application HMI desired display language
         * \param value application HMI desired display language
         */
        void setHMIDisplayLanguageDesired( NsAppLinkRPC::Language_v2 value );

        /**
         * \brief retrieve application HMI desired display language
         * \return application HMI desired display language
         */
        const NsAppLinkRPC::Language_v2& getHMIDisplayLanguageDesired( ) const;

        /**
         * \brief set application system context
         * \param application system context
         */
        void setSystemContext( NsAppLinkRPC::SystemContext_v2 value );

        /**
         * \brief retrieve application system context
         * \return application system context
         */
        const NsAppLinkRPC::SystemContext_v2& getSystemContext() const;

        /**
         * \brief Set application audio streaming state
         * \param streamingState audio streaming state of application
         */
        void setApplicationAudioStreamingState( const NsAppLinkRPC::AudioStreamingState_v2& hmiLevel );

        /**
         * \brief retreive application audio streaming state
         * \return application audio streaming state
         */
        const NsAppLinkRPC::AudioStreamingState_v2& getApplicationAudioStreamingState( ) const;

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

    private:

        /**
         * \brief Copy constructor
         */
        Application_v2(const Application_v2& );

        NsAppLinkRPC::Language_v2 mLanguageDesired;
        NsAppLinkRPC::AudioStreamingState_v2 mAudioStreamingState;
        NsAppLinkRPC::Language_v2 mHMIDisplayLanguageDesired;
        NsAppLinkRPC::SystemContext_v2 mSystemContext;

        std::string mAppID;
        AppTypes mAppType;
    };
}

#endif // APPLICATION_V2_H
