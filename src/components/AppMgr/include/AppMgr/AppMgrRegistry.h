/**
 * \file AppMgrRegistry.h
 * \brief Application manager registry
 * \author vsalo
 */

#ifndef APPMGRREGISTRY_H_
#define APPMGRREGISTRY_H_

#include "Application_v1.h"
#include "Application_v2.h"
#include "RegistryItem.h"

#include <map>
#include <tuple>
#include <string>
#include <vector>

namespace log4cplus
{
    class Logger;
}

namespace NsAppManager
{

    /**
     * \brief AppMgrRegistry acts as a registry for applications registered on HMI
     */
    class AppMgrRegistry
    {
    public:

        /**
         * \brief An application_id-registry_item map
         */
        typedef std::map<ApplicationUniqueID, RegistryItem*> ItemsMap;

        /**
         * \brief An application_id-registry_item map item
         */
        typedef std::pair<ApplicationUniqueID, RegistryItem*> ItemsMapItem;

        /**
         * \brief RegistryItem vector
         */
        typedef std::vector<RegistryItem*> Items;

        /**
         * \brief Default class destructor
         */
        ~AppMgrRegistry( );

        /**
         * \brief Returning class instance
         * \return class instance
         */
        static AppMgrRegistry& getInstance( );

        /**
         * \brief register an application
         * \param app application we are registering
         * \return RegistryItem instance created for the application we've just registered
         */
        //const RegistryItem* registerApplication(Application *app );

        /**
         * \brief unregister an application
         * \param item a registry item associated with the aplication being unregistered
         */
        //void unregisterApplication( RegistryItem* item  );

        /**
         * \brief get registry item associated with the application
         * \param app application we need to retrieve a registry item for
         * \return RegistryItem instance
         */
        //RegistryItem *getItem( const Application* app ) const;

        /**
         * \brief get registry item associated with the HMI_FULL application
         * \return Application instance
         */
        //Application *getActiveItem() const;

        /**
         * \brief get registry items associated with the application name
         * \param appName a name of the application we need to retrieve a registry items for
         * \return RegistryItem vector
         */
        //Items getItems(const std::string& appName ) const;

        /**
         * \brief get registry item associated with the application
         * \param connectionId id of the connection associated with the application we need to retrieve a registry item for
         * \param sessionId id of the session associated with the application we need to retrieve a registry item for
         * \return RegistryItem instance
         */
        //RegistryItem *getItem( int appId ) const;

        /**
         * \brief Returns registered applications list
         * \return registered applications list
         */
        //const ItemsMap &getItems() const;

        /**
         * \brief Activates a registered app and deactivates currently active one
         * \param item registered application to activate
         * \return result success
         */
        //bool activateApp(RegistryItem* item);

        /**
         * \brief Activates a registered app and deactivates currently active one
         * \param item registered application to activate
         * \return result success
         */
        //bool activateApp(Application* app);

        /**
         * \brief Returns an application from the registry by application connection id and session id
         * \param connectionId id of the connection associated with the application we need to retrieve
         * \param sessionId id of the session associated with the application we need to retrieve
         * \return application, if the specified name found in a registry, NULL otherwise
         */
        //Application* getApplication( int appId ) const;

        /**
         * \brief Gets an application assigned to a command
         * \param cmdId a command
         * \return an application assigned to a command
         */
        //Application* getApplicationByCommand( const unsigned int& cmdId ) const;

        /**
         * \brief cleans all the registry
         */
        //void clear( );

    private:

        /**
         * \brief Default class constructor
         */
        AppMgrRegistry( );

        /**
         * \brief Copy constructor
         */
        AppMgrRegistry( const AppMgrRegistry& );

        //ItemsMap mRegistryItems;
        static log4cplus::Logger mLogger;
    };

} // namespace NsAppManager

#endif /* APPMGRREGISTRY_H_ */
