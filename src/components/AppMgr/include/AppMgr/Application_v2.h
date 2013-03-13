//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef APPLICATION_V2_H
#define APPLICATION_V2_H

#include "AppMgr/Application.h"
#include "AppMgr/AppChoiceSets.h"

#include "JSONHandler/ALRPCObjects/V2/Language.h"
#include "JSONHandler/ALRPCObjects/V2/AppType.h"
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
         * \param appId application id
         */
        Application_v2(const std::string& name, int appid );

        /**
         * \brief Default class destructor
         */
        virtual ~Application_v2( );

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

        /**
         * \brief add an interaction choice set item to the application
         * \param choiceSetId interaction choice set id
         * \param choiceSet interaction choice set
         */
        void addChoiceSet(const unsigned int& choiceSetId, const ChoiceSetV2& choiceSet);

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

        /**
         * \brief find a mapped choice set item
         * \param choiceSetId interaction choice set id
         * \return a mapped choice set item
         */
        const ChoiceSetV2 *findChoiceSet(const unsigned int& choiceSetId);

    private:

        /**
         * \brief Copy constructor
         */
        Application_v2(const Application_v2& );

        NsAppLinkRPCV2::Language mLanguageDesired;
        
        NsAppLinkRPCV2::Language mHMIDisplayLanguageDesired;

        NsAppLinkRPCV2::SyncMsgVersion mSyncMsgVersion;

   //     std::string mAppID;
        AppTypes mAppType;
        AppChoiceSets mChoiceSets;
        TTSName mTtsName;
    };
}

#endif // APPLICATION_V2_H
