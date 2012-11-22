/**
 * \file Application.h
 * \brief Application metaphor
 * \author vsalo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "JSONHandler/ALRPCObjects/Language.h"
#include "JSONHandler/ALRPCObjects/SyncMsgVersion.h"
#include "JSONHandler/ALRPCObjects/HMILevel.h"
#include "JSONHandler/ALRPCObjects/AudioStreamingState.h"
#include <string>
#include <vector>
#include <tuple>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    /**
     * \brief An application unique id - combination of connection id and session id
     */
    typedef std::tuple<int, unsigned char> ApplicationUniqueID;

    /**
     * \brief class Application acts as a metaphor for every mobile application being registered on HMI
     */
    class Application
    {
    public:

        /**
         * \brief Class constructor
         * \param name application name
         * \param connectionId id of the connection associated with this application
         * \param sessionId id of the session associated with this application
         */
        Application(const std::string& name, unsigned int connectionId, unsigned char sessionId );

        /**
         * \brief Default class destructor
         */
        virtual ~Application( );

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
         * \brief Set application NGN media screen app name
         * \param value application NGN media screen app name
         */
        void setNgnMediaScreenAppName(const std::string& value);

        /**
         * \brief Set application voice recognition synonyms
         * \param value application voice recognition synonyms
         */
        void setVrSynonyms(const std::vector<std::string>& value);

        /**
         * \brief Set application usage of vehicle data
         * \param value does the application use vehicle data
         */
        void setUsesVehicleData(bool value);

        /**
         * \brief Set if the application is a media application
         * \param value is the application a media application
         */
        void setIsMediaApplication(bool value);

        /**
         * \brief Set application desired languuage
         * \param value application desired language
         */
        void setLanguageDesired(NsAppLinkRPC::Language value);

        /**
         * \brief Set application autoactivate ID
         * \param value application autoactivate ID
         */
        void setAutoActivateID(const std::string& value);

        /**
         * \brief Set application sync message version
         * \param value application sync message version
         */
        void setSyncMsgVersion(NsAppLinkRPC::SyncMsgVersion value);

        /**
         * \brief Set application ID
         * \param value application ID
         */
        void setAppID( const std::string& value );

        /**
         * \brief Set application HMI desired display language
         * \param value application HMI desired display language
         */
        void setHMIDisplayLanguageDesired( NsAppLinkRPC::Language value );

        /**
         * \brief retrieve application NGN media screen application name
         * \return application NGN media screen application name
         */
        const std::string& getNgnMediaScreenAppName( ) const;

        /**
         * \brief retrieve application voice-recognition synonyms
         * \return application voice-recognition synonyms
         */
        const std::vector<std::string>& getVrSynonyms( ) const;

        /**
         * \brief retrieve does the application use vehicle data
         * \return does the application use vehicle data
         */
        bool getUsesVehicleData( ) const;

        /**
         * \brief retrieve is the application a media application
         * \return is the application a media application
         */
        bool getIsMediaApplication( ) const;

        /**
         * \brief retrieve application desired language
         * \return application desired language
         */
        const NsAppLinkRPC::Language& getLanguageDesired( ) const;

        /**
         * \brief retrieve application auto-activate ID
         * \return application auto-activate ID
         */
        const std::string& getAutoActivateID( ) const;

        /**
         * \brief retrieve application sync message version
         * \return application sync msg version
         */
        const NsAppLinkRPC::SyncMsgVersion& getSyncMsgVersion( ) const;

        /**
         * \brief retrieve application ID
         * \return application ID
         */
        const std::string& getAppID( ) const;

        /**
         * \brief retrieve application HMI desired display language
         * \return application HMI desired display language
         */
        const NsAppLinkRPC::Language& getHMIDisplayLanguageDesired( ) const;

        /**
         * \brief retrieve application session ID
         * \return application session ID
         */
        unsigned char getSessionID() const;

        /**
         * \brief retrieve application session ID
         * \return application connection ID
         */
        unsigned char getConnectionID() const;

        /**
         * \brief retrieve application name
         * \return application name
         */
        const std::string& getName() const;

    private:

        /**
         * \brief Copy constructor
         */
        Application(const Application& );

        const std::string mName;
        const unsigned char mSessionID;
        const unsigned int mConnectionID;
        std::string mNgnMediaScreenAppName;
        std::vector<std::string> mVrSynonyms;
        bool m_bUsesVehicleData;
        bool m_bIsMediaApplication;
        NsAppLinkRPC::Language mLanguageDesired;
        std::string mAutoActivateID;
        NsAppLinkRPC::SyncMsgVersion mSyncMsgVersion;
        NsAppLinkRPC::HMILevel::HMILevelInternal mHMIStatusLevel;
        NsAppLinkRPC::AudioStreamingState mAudioStreamingState;
        std::string mAppID;
        NsAppLinkRPC::Language mHMIDisplayLanguageDesired;

        static log4cplus::Logger mLogger;
    };

} // namespace NsAppManager

#endif /* APPLICATION_H_ */
