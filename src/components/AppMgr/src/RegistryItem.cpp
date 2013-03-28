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

#include "AppMgr/RegistryItem.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger RegistryItem::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("RegistryItem"));

    /**
     * \brief Class constructor
     * \param app application to register
     */
    RegistryItem::RegistryItem( Application* app )
        : mApplication(app)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "No application to register! Please specify a valid application instance!");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " RegistryItem constructed for the application " << app->getName());
    }

    /**
     * \brief Copy constructor
     */
    RegistryItem::RegistryItem( const RegistryItem& item )
        : mApplication(item.getApplication())
    {
    }

    /**
     * \brief Default class destructor
     */
    RegistryItem::~RegistryItem( )
    {
        if(mApplication)
        {
            LOG4CPLUS_INFO_EXT(mLogger, " Destroying RegistryItem for the application " << mApplication->getName()
                << " application id " << mApplication->getAppID() << " ...");
            delete mApplication;
            mApplication = 0;
        }

        for(Policies::iterator it = mAppPolicies.begin(); it != mAppPolicies.end(); it++)
        {
            if( *it )
            {
                delete *it;
            }
        }

        mAppPolicies.clear();
        LOG4CPLUS_INFO_EXT(mLogger, " Registry item was destroyed!");
    }

    /**
     * \brief operator ==
     * \param item the item to compare with
     * \return comparison result
     */
    bool RegistryItem::operator ==(const RegistryItem &item) const
    {
        return this->getApplication() == item.getApplication();
    }

    /**
     * \brief register application policy
     * \param hash policy text representation
     * \return app policy instance
     */
    const AppPolicy* RegistryItem::registerPolicy( const std::string& hash )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Registering a policy " << hash);
        AppPolicy* policy = new AppPolicy(hash);
        mAppPolicies.insert(policy);
        return *mAppPolicies.find(policy);
    }

    /**
     * \brief unregister application policy
     * \param policy policy to be unregistered
     */
    void RegistryItem::unregisterPolicy( AppPolicy* policy )
    {
        if(!policy)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to unregister null policy!");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Unregistering a policy " << policy->getPolicyHash());
        Policies::iterator policyIterator = mAppPolicies.find(policy);
        mAppPolicies.erase(policyIterator);
    }

    /**
     * \brief get associated application
     * \return asssociated application
     */
    Application* RegistryItem::getApplication( ) const
    {
        if(!mApplication)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "About to return a null application: a null ptr exception may occur right after this line!");
        }
        return mApplication;
    }

    /**
     * \brief comparison operator
     * \param item2 item to compare with
     * \return comparison result
     */
    bool RegistryItem::operator <(const RegistryItem& item2 ) const
    {
        if(!this->getApplication() || !item2.getApplication())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, "Cannot perform registry items comparison, due to null application(s) assigned to (some of) them");
            return false;
        }
        return this->getApplication()->getName() < item2.getApplication()->getName();
    }

    /**
     * \brief get application policies
     * \param app application to get policies for
     * \return app policy set
     */
    RegistryItem::Policies RegistryItem::getApplicationPolicies( const Application* app ) const
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to get policies of null application!");
         //   return;
        }
        Policies policySet;
        return policySet;
    }

    /**
     * \brief get application policies
     * \param app name of an application to get policies for
     * \return app policy set
     */
    RegistryItem::Policies RegistryItem::getApplicationPolicies( const std::string& app ) const
    {
        Policies policySet;
        return policySet;
    }

}
