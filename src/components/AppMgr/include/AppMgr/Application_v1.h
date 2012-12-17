#ifndef APPLICATION_V1_H
#define APPLICATION_V1_H

#include "AppMgr/Application.h"
#include "AppMgr/AppChoiceSets.h"
#include "JSONHandler/ALRPCObjects/V1/AudioStreamingState.h"
#include "JSONHandler/ALRPCObjects/V1/SystemContext.h"
#include "JSONHandler/ALRPCObjects/V1/Language.h"
#include "JSONHandler/ALRPCObjects/V1/SyncMsgVersion.h"
#include "JSONHandler/ALRPCObjects/V1/HMILevel.h"

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
         * \param appId application id
         */
        Application_v1(const std::string& name, int appId);

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

        /**
         * \brief Set application HMI status level
         * \param hmiLevel HMI status level of application
         */
        void setApplicationHMIStatusLevel( const NsAppLinkRPC::HMILevel::HMILevelInternal& hmiLevel );

        /**
         * \brief retrieve aplication HMI status level
         * \return HMI status level of application
         */
        const NsAppLinkRPC::HMILevel::HMILevelInternal& getApplicationHMIStatusLevel( ) const;

        /**
         * \brief Set application sync message version
         * \param value application sync message version
         */
        void setSyncMsgVersion(NsAppLinkRPC::SyncMsgVersion value);

        /**
         * \brief retrieve application sync message version
         * \return application sync msg version
         */
        const NsAppLinkRPC::SyncMsgVersion& getSyncMsgVersion( ) const;

        /**
         * \brief add an interaction choice set item to the application
         * \param choiceSetId interaction choice set id
         * \param choiceSet interaction choice set
         */
        void addChoiceSet(const unsigned int& choiceSetId, const ChoiceSetV1& choiceSet);

        /**
         * \brief remove an interaction choice set from the application
         * \param choiceSetId interaction choice set id
         */
        void removeChoiceSet(const unsigned int& choiceSetId);

        /**
         * \brief gets all interaction choice set items
         * \return interaction choice set items
         */
        ChoiceSetItems getAllChoiceSets() const;

        /**
         * \brief get count of interaction choice sets
         * \return interaction choice sets count
         */
        size_t getChoiceSetsCount() const;

    private:

        /**
         * \brief Copy constructor
         */
        Application_v1(const Application_v1& );

        NsAppLinkRPC::Language mLanguageDesired;
        NsAppLinkRPC::AudioStreamingState mAudioStreamingState;
        NsAppLinkRPC::Language mHMIDisplayLanguageDesired;
        NsAppLinkRPC::SystemContext mSystemContext;
        NsAppLinkRPC::HMILevel::HMILevelInternal mHMIStatusLevel;
        NsAppLinkRPC::SyncMsgVersion mSyncMsgVersion;
        AppChoiceSets mChoiceSets;
        bool m_bUsesVehicleData;
    };
}

#endif // APPLICATION_V1_H
