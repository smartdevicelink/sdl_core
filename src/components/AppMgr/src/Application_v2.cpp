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

#include "AppMgr/Application_v2.h"
#include "utils/logger.h"

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
        LOG4CXX_INFO_EXT(logger_, " Created an application " << name << " for the application id " << appId);
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
        LOG4CXX_INFO_EXT(logger_, " Deleted an application " << mName << " application id " << mAppID);
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
    const NsSmartDeviceLinkRPCV2::Language& Application_v2::getLanguageDesired( ) const
    {
        return mLanguageDesired;
    }

    /**
     * \brief Set application desired languuage
     * \param value application desired language
     */
    void Application_v2::setLanguageDesired(NsSmartDeviceLinkRPCV2::Language value)
    {
        mLanguageDesired = value;
    }

    /**
     * \brief retrieve application HMI desired display language
     * \return application HMI desired display language
     */
    const NsSmartDeviceLinkRPCV2::Language &Application_v2::getHMIDisplayLanguageDesired() const
    {
        return mHMIDisplayLanguageDesired;
    }

    /**
     * \brief Set application HMI desired display language
     * \param value application HMI desired display language
     */
    void Application_v2::setHMIDisplayLanguageDesired(NsSmartDeviceLinkRPCV2::Language value)
    {
        mHMIDisplayLanguageDesired = value;
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
     * \brief retrieve application sync message version
     * \return application sync msg version
     */
    const NsSmartDeviceLinkRPCV2::SyncMsgVersion& Application_v2::getSyncMsgVersion( ) const
    {
        return mSyncMsgVersion;
    }

    /**
     * \brief Set application sync message version
     * \param value application sync message version
     */
    void Application_v2::setSyncMsgVersion(NsSmartDeviceLinkRPCV2::SyncMsgVersion value)
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
    const ChoiceSetV2* Application_v2::findChoiceSet(const unsigned int &choiceSetId)
    {
        const ChoiceSetGeneric* choiceSetGeneric = mChoiceSets.findItem(choiceSetId);
        return choiceSetGeneric ? &choiceSetGeneric->choiceSetV2 : 0;
    }
}
