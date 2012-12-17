/**
 * \file RegistryItem.cpp
 * \brief Registered application in a registry
 * \author vsalo
 */

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
