#ifndef APPLICATION_V1_H
#define APPLICATION_V1_H

#include "AppMgr/Application.h"
#include "JSONHandler/ALRPCObjects/V1/AudioStreamingState.h"
#include "JSONHandler/ALRPCObjects/V1/SystemContext.h"
#include "JSONHandler/ALRPCObjects/V1/Language.h"

namespace NsAppManager
{
    /**
     * \brief class Application acts as a metaphor for every mobile application of protocol v1 being registered on HMI
     */
    class Application_v1 : public Application
    {
    public:

        /**
         * \brief Class constructor
         * \param name application name
         * \param connectionId id of the connection associated with this application
         * \param sessionId id of the session associated with this application
         * \param appId application id
         */
        Application_v1(const std::string& name, unsigned int connectionId, unsigned char sessionId , int appId);

        /**
         * \brief Default class destructor
         */
        virtual ~Application_v1( );

        /**
         * \brief Set application usage of vehicle data
         * \param value does the application use vehicle data
         */
        void setUsesVehicleData(bool value);

        /**
         * \brief retrieve does the application use vehicle data
         * \return does the application use vehicle data
         */
        bool getUsesVehicleData( ) const;

        /**
         * \brief Set application desired languuage
         * \param value application desired language
         */
        void setLanguageDesired(NsAppLinkRPC::Language value);

        /**
         * \brief retrieve application desired language
         * \return application desired language
         */
        const NsAppLinkRPC::Language& getLanguageDesired( ) const;

        /**
         * \brief Set application HMI desired display language
         * \param value application HMI desired display language
         */
        void setHMIDisplayLanguageDesired( NsAppLinkRPC::Language value );

        /**
         * \brief retrieve application HMI desired display language
         * \return application HMI desired display language
         */
        const NsAppLinkRPC::Language& getHMIDisplayLanguageDesired( ) const;

        /**
         * \brief set application system context
         * \param application system context
         */
        void setSystemContext( NsAppLinkRPC::SystemContext value );

        /**
         * \brief retrieve application system context
         * \return application system context
         */
        const NsAppLinkRPC::SystemContext& getSystemContext() const;

        /**
         * \brief Set application audio streaming state
         * \param streamingState audio streaming state of application
         */
        void setApplicationAudioStreamingState( const NsAppLinkRPC::AudioStreamingState& hmiLevel );

        /**
         * \brief retrieve application audio streaming state
         * \return application audio streaming state
         */
        const NsAppLinkRPC::AudioStreamingState& getApplicationAudioStreamingState( ) const;

    private:

        /**
         * \brief Copy constructor
         */
        Application_v1(const Application_v1& );

        NsAppLinkRPC::Language mLanguageDesired;
        NsAppLinkRPC::AudioStreamingState mAudioStreamingState;
        NsAppLinkRPC::Language mHMIDisplayLanguageDesired;
        NsAppLinkRPC::SystemContext mSystemContext;
        bool m_bUsesVehicleData;
    };
}

#endif // APPLICATION_V1_H
