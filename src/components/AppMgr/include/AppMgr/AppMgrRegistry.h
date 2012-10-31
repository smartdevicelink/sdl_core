/*
 * AppMgrRegistry.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
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
};

namespace NsAppManager
{
	
class AppMgrRegistry
{
public:
	typedef std::map<std::string, RegistryItem*> Items;
	typedef std::pair<std::string, RegistryItem*> Item;
	
	~AppMgrRegistry( );

	static AppMgrRegistry& getInstance( );

	const RegistryItem* registerApplication( const Application* app );
	void unregisterApplication( RegistryItem* item  );

    RegistryItem *getItem( const Application* app ) const;
    RegistryItem* getItem( const std::string& app ) const;
    RegistryItem *getItem( unsigned char sessionID ) const;

private:
	AppMgrRegistry( );
    AppMgrRegistry( const AppMgrRegistry& );
	Items mRegistryItems;
	static log4cplus::Logger mLogger;
};

}; // namespace NsAppManager

#endif /* APPMGRREGISTRY_H_ */
