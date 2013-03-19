//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef APPLICATION_V1_H
#define APPLICATION_V1_H

#include "AppMgr/Application.h"
#include "AppMgr/AppChoiceSets.h"
#include "JSONHandler/SDLRPCObjects/V1/Language.h"
#include "JSONHandler/SDLRPCObjects/V1/SyncMsgVersion.h"

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
        void setLanguageDesired(NsSmartDeviceLinkRPC::Language value);

        /**
         * \brief retrieve application desired language
         * \return application desired language
         */
        const NsSmartDeviceLinkRPC::Language& getLanguageDesired( ) const;

        /**
         * \brief Set application HMI desired display language
         * \param value application HMI desired display language
         */
        void setHMIDisplayLanguageDesired( NsSmartDeviceLinkRPC::Language value );

        /**
         * \brief retrieve application HMI desired display language
         * \return application HMI desired display language
         */
        const NsSmartDeviceLinkRPC::Language& getHMIDisplayLanguageDesired( ) const;

        

        /**
         * \brief Set application sync message version
         * \param value application sync message version
         */
        void setSyncMsgVersion(NsSmartDeviceLinkRPC::SyncMsgVersion value);

        /**
         * \brief retrieve application sync message version
         * \return application sync msg version
         */
        const NsSmartDeviceLinkRPC::SyncMsgVersion& getSyncMsgVersion( ) const;

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

        /**
         * \brief find a mapped choice set item
         * \param choiceSetId interaction choice set id
         * \return a mapped choice set item
         */
        const ChoiceSetV1 *findChoiceSet(const unsigned int& choiceSetId);

    private:

        /**
         * \brief Copy constructor
         */
        Application_v1(const Application_v1& );

        NsSmartDeviceLinkRPC::Language mLanguageDesired;
        
        NsSmartDeviceLinkRPC::Language mHMIDisplayLanguageDesired;
        
        NsSmartDeviceLinkRPC::SyncMsgVersion mSyncMsgVersion;
        AppChoiceSets mChoiceSets;
        bool m_bUsesVehicleData;
    };
}

#endif // APPLICATION_V1_H
