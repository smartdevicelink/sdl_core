/**
 * \file AppMgrRegistry.cpp
 * \brief Application manager registry
 * \author vsalo
 */

#include "AppMgr/AppMgrRegistry.h"
#include "LoggerHelper.hpp"

namespace NsAppManager
{

    log4cplus::Logger AppMgrRegistry::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("AppMgrRegistry"));

    /**
     * \brief Returning class instance
     * \return class instance
     */
    AppMgrRegistry& AppMgrRegistry::getInstance( )
    {
        static AppMgrRegistry registry;
        return registry;
    }

    /**
     * \brief unregister an application
     * \param item a registry item associated with the aplication being unregistered
     */
    void AppMgrRegistry::unregisterApplication( RegistryItem* item )
    {
        if(!item)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to unregister null app!");
            return;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Unregistering an application " << item->getApplication()->getName() << " connection id " << item->getApplication()->getConnectionID() << " session id " << (uint)item->getApplication()->getSessionID());
        ItemsMap::const_iterator registryItemIterator = mRegistryItems.find(ApplicationUniqueID(item->getApplication()->getConnectionID(), item->getApplication()->getSessionID()));
        mRegistryItems.erase(registryItemIterator);
    }

    /**
     * \brief get registry item associated with the application
     * \param app application we need to retrieve a registry item for
     * \return RegistryItem instance
     */
    RegistryItem *AppMgrRegistry::getItem( const Application* app ) const
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Getting registry item for null application!");
            return 0;
        }
        return getItem( app->getConnectionID(), app->getSessionID() );
    }

    /**
     * \brief get registry item associated with the HMI_FULL application
     * \return Application instance
     */
    Application *AppMgrRegistry::getActiveItem() const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for the active registered application" );
        for(ItemsMap::const_iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
        {
            RegistryItem* item = it->second;
            if(!item)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Null-item found!");
                continue;
            }
            Application* app = item->getApplication();
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application for the item!");
                continue;
            }
            switch(app->getProtocolVersion())
            {
                case 1:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "An application " << app->getName() << " connection " << app->getConnectionID() << " session " << (uint)app->getSessionID() << " has HMI_LEVEL " << ((Application_v1*)app)->getApplicationHMIStatusLevel());
                    if(NsAppLinkRPC::HMILevel::HMI_FULL == ((Application_v1*)app)->getApplicationHMIStatusLevel())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "An application " << app->getName() << " connection " << app->getConnectionID() << " session " << (uint)app->getSessionID() << " is HMI_FULL");
                        return app;
                    }
                    break;
                }
                case 2:
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "An application " << app->getName() << " connection " << app->getConnectionID() << " session " << (uint)app->getSessionID() << " has HMI_LEVEL " << ((Application_v2*)app)->getApplicationHMIStatusLevel());
                    if(NsAppLinkRPCV2::HMILevel::HMI_FULL == ((Application_v2*)app)->getApplicationHMIStatusLevel())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, "An application " << app->getName() << " connection " << app->getConnectionID() << " session " << (uint)app->getSessionID() << " is HMI_FULL");
                        return app;
                    }
                    break;
                }
                default:
                {
                    LOG4CPLUS_ERROR_EXT(mLogger, " Unsupported version number " << app->getProtocolVersion() << " for application " << app->getName());
                    break;
                }
            }
        }
        LOG4CPLUS_ERROR_EXT(mLogger, " No active application found!");
        return 0;
    }

    /**
     * \brief get registry items associated with the application name
     * \param appName a name of the application we need to retrieve a registry items for
     * \return RegistryItem vector
     */
    AppMgrRegistry::Items AppMgrRegistry::getItems( const std::string& appName ) const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for registered applications by the name " << appName );
        Items items;
        for(ItemsMap::const_iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
        {
            RegistryItem* item = it->second;
            if(!item)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Null-item found!");
                break;
            }
            Application* app = item->getApplication();
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application for the item!");
                break;
            }
            if(appName == app->getName())
            {
                LOG4CPLUS_INFO_EXT(mLogger, "Adding an application from connection " << app->getConnectionID() << " session " << (uint)app->getSessionID());
                items.push_back(item);
            }
        }

        return items;
    }

    /**
     * \brief get registry item associated with the application
     * \param connectionId id of the connection associated with the application we need to retrieve a registry item for
     * \param sessionId id of the session associated with the application we need to retrieve a registry item for
     * \return RegistryItem instance
     */
    RegistryItem *AppMgrRegistry::getItem(unsigned int connectionId, unsigned char sessionId) const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for registered application with the connection id " << connectionId << " sesion id " << (uint)sessionId );
        ItemsMap::const_iterator it = mRegistryItems.find(ApplicationUniqueID(connectionId, sessionId));
        if(it != mRegistryItems.end())
        {
            RegistryItem* item = it->second;
            if(!item)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " NULL-application registered for the connection id " << connectionId << " session id " << (uint)sessionId);
                return 0;
            }
            return item;
        }
        LOG4CPLUS_ERROR_EXT(mLogger, " No application found with the connection id " << connectionId << " session id " << (uint)sessionId);
        return 0;
    }

    /**
     * \brief Returns registered applications list
     * \return registered applications list
     */
    const AppMgrRegistry::ItemsMap &AppMgrRegistry::getItems() const
    {
        return mRegistryItems;
    }

    /**
     * \brief Activates a registered app and deactivates currently active one
     * \param item registered application to activate
     * \return result success
     */
    bool AppMgrRegistry::activateApp(RegistryItem* item)
    {
        if(!item)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Cannot activate null application!");
            return false;
        }

        Application* app = item->getApplication();

        return activateApp(app);
    }

    /**
     * \brief Activates a registered app and deactivates currently active one
     * \param item registered application to activate
     * \return result success
     */
    bool AppMgrRegistry::activateApp(Application *app)
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Cannot activate null-application!");
            return false;
        }
        LOG4CPLUS_INFO_EXT(mLogger, "Activating an app " << app->getName() << " connection " << app->getConnectionID() << " session " << (uint)app->getSessionID());
        if(mRegistryItems.empty())
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Cannot activate an application: no applications registered!");
            return false;
        }
        for(ItemsMap::iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
        {
            RegistryItem* item_ = it->second;
            if(!item_)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Item is empty in registry!");
                return false;
            }
            Application* app_ = item_->getApplication();
            if(!app_)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, "No application for the item!");
                return false;
            }
            switch(app_->getProtocolVersion())
            {
                case 1:
                {
                    if(NsAppLinkRPC::HMILevel::HMI_FULL == ((Application_v1*)app_)->getApplicationHMIStatusLevel())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " Deactivating application " << app_->getName());
                        ((Application_v1*)app_)->setApplicationHMIStatusLevel(NsAppLinkRPC::HMILevel::HMI_BACKGROUND);
                    }
                    break;
                }
                case 2:
                {
                    if(NsAppLinkRPCV2::HMILevel::HMI_FULL == ((Application_v2*)app_)->getApplicationHMIStatusLevel())
                    {
                        LOG4CPLUS_INFO_EXT(mLogger, " Deactivating application " << app_->getName());
                        ((Application_v2*)app_)->setApplicationHMIStatusLevel(NsAppLinkRPCV2::HMILevel::HMI_BACKGROUND);
                    }
                    break;
                }
            }
        }

        switch(app->getProtocolVersion())
        {
            case 1:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " Activating application " << app->getName());
                ((Application_v1*)app)->setApplicationHMIStatusLevel(NsAppLinkRPC::HMILevel::HMI_FULL);
                ((Application_v1*)app)->setSystemContext(NsAppLinkRPC::SystemContext::SYSCTXT_MAIN);
                return true;
            }
            case 2:
            {
                LOG4CPLUS_INFO_EXT(mLogger, " Activating application " << app->getName());
                ((Application_v2*)app)->setApplicationHMIStatusLevel(NsAppLinkRPCV2::HMILevel::HMI_FULL);
                ((Application_v2*)app)->setSystemContext(NsAppLinkRPCV2::SystemContext::SYSCTXT_MAIN);
                return true;
            }
        }

        LOG4CPLUS_ERROR_EXT(mLogger, " Unsupported protocol version " << app->getProtocolVersion());
        return false;
    }

    /**
     * \brief Returns an application from the registry by application connection id and session id
     * \param connectionId id of the connection associated with the application we need to retrieve
     * \param sessionId id of the session associated with the application we need to retrieve
     * \return application, if the specified name found in a registry, NULL otherwise
     */
    Application *AppMgrRegistry::getApplication(unsigned int connectionId, unsigned char sessionId) const
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Searching for registered application with the connection id " << connectionId << " sesion id " << (uint)sessionId );

        RegistryItem* item = getItem(connectionId, sessionId);
        if(!item)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Item " << item << " is empty in registry!");
            return 0;
        }
        Application* app = item->getApplication();
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " No application for the item " << item);
            return 0;
        }
        if((app->getSessionID() != sessionId) || (app->getConnectionID() != connectionId))
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " An-application registered for the connection id " << connectionId << " session id " << (uint)sessionId << " has the connection id " << app->getConnectionID() << " and session id " << (uint)app->getSessionID() << " which is an error!");
            return 0;
        }
        return app;
    }

    /**
     * \brief Gets an application assigned to a command
     * \param cmdId a command
     * \return an application assigned to a command
     */
    Application *AppMgrRegistry::getApplicationByCommand(const unsigned int &cmdId) const
    {
        for(ItemsMap::const_iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
        {
            RegistryItem* item = it->second;
            if(!item)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " Item " << item << " is empty in registry!");
                continue;
            }
            Application* app = item->getApplication();
            if(!app)
            {
                LOG4CPLUS_ERROR_EXT(mLogger, " No application for the item " << item);
                continue;
            }
            if(app->getCommandsCount())
            {
                const Commands& cmds = app->findCommands(cmdId);
                if(!cmds.empty())
                {
                    return app;
                }
            }
        }
        LOG4CPLUS_ERROR_EXT(mLogger, " No applications found for the command " << cmdId);
        return 0;
    }

    /**
     * \brief cleans all the registry
     */
    void AppMgrRegistry::clear()
    {
        mRegistryItems.clear();
    }

    /**
     * \brief Default class constructor
     */
    AppMgrRegistry::AppMgrRegistry( )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Created a registry!");
    }

    /**
     * \brief Copy constructor
     */
    AppMgrRegistry::AppMgrRegistry(const AppMgrRegistry &)
    {
    }

    /**
     * \brief Default class destructor
     */
    AppMgrRegistry::~AppMgrRegistry( )
    {
        LOG4CPLUS_INFO_EXT(mLogger, " Destructing a registry!");
        for(ItemsMap::iterator it = mRegistryItems.begin(); it != mRegistryItems.end(); it++)
        {
            if( it->second )
            {
                delete it->second;
                it->second = 0;
            }
        }

        mRegistryItems.clear();
        LOG4CPLUS_INFO_EXT(mLogger, " Destructed a registry!");
    }

    /**
     * \brief register an application
     * \param app application we are registering
     * \return RegistryItem instance created for the application we've just registered
     */
    const RegistryItem* AppMgrRegistry::registerApplication( Application* app )
    {
        if(!app)
        {
            LOG4CPLUS_ERROR_EXT(mLogger, " Trying to register a null-application!");
            return 0;
        }
        LOG4CPLUS_INFO_EXT(mLogger, " Registering an application " << app->getName() << " connection id " << app->getConnectionID() << " session id " << (uint)app->getSessionID());
        mRegistryItems.insert(ItemsMapItem(ApplicationUniqueID(app->getConnectionID(), app->getSessionID()), new RegistryItem(app)));
        return mRegistryItems.find(ApplicationUniqueID(app->getConnectionID(), app->getSessionID()))->second;
    }

}
