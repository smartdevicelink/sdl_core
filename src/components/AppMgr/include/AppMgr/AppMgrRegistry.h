/**
 * \file AppMgrRegistry.h
 * \brief Application manager registry
 * \author vsalo
 */

#ifndef APPMGRREGISTRY_H_
#define APPMGRREGISTRY_H_

#include "Application.h"
#include "RegistryItem.h"

#include <map>
#include <string>

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
     * \brief An application_name-registry_item map
     */
	typedef std::map<std::string, RegistryItem*> Items;

    /**
     * \brief An application_name-registry_item map item
     */
	typedef std::pair<std::string, RegistryItem*> Item;
	
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
    const RegistryItem* registerApplication(Application *app );

    /**
     * \brief unregister an application
     * \param item a registry item associated with the aplication being unregistered
     */
	void unregisterApplication( RegistryItem* item  );

    /**
     * \brief get registry item associated with the application
     * \param app application we need to retrieve a registry tem for
     * \return RegistryItem instance
     */
    RegistryItem *getItem( const Application* app ) const;

    /**
     * \brief get registry item associated with the application
     * \param app a name of the application we need to retrieve a registry tem for
     * \return RegistryItem instance
     */
    RegistryItem* getItem( const std::string& app ) const;

    /**
     * \brief get registry item associated with the application
     * \param sessionID id of a session associated with the application we need to retrieve a registry item for
     * \return RegistryItem instance
     */
    RegistryItem *getItem( unsigned char sessionID ) const;

    /**
     * \brief Returns registered applications list
     * \return registered applications list
     */
    const Items &getItems() const;

private:

    /**
     * \brief Default class constructor
     */
	AppMgrRegistry( );

    /**
     * \brief Copy constructor
     */
    AppMgrRegistry( const AppMgrRegistry& );

	Items mRegistryItems;
	static log4cplus::Logger mLogger;
};

} // namespace NsAppManager

#endif /* APPMGRREGISTRY_H_ */
