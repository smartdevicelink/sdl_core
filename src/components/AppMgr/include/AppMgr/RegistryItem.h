/*
 * RegistryItem.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef REGISTRYITEM_H_
#define REGISTRYITEM_H_

#include "Application.h"
#include "AppPolicy.h"
#include <set>
#include <string>

namespace log4cplus
{
	class Logger;
};

namespace NsAppManager
{
	
class RegistryItem
{
public:
	typedef std::set<AppPolicy*> Policies;
	
    RegistryItem(Application *app );
	virtual ~RegistryItem( );

	void setApplication( const Application* app );
	const AppPolicy* registerPolicy( const std::string& hash );
	void unregisterPolicy( AppPolicy* policy );
    Application *getApplication() const;
	bool operator<(const RegistryItem& item2) const;

	Policies getApplicationPolicies(const Application* app) const;
	Policies getApplicationPolicies(const std::string& app) const;

private:
	RegistryItem( const RegistryItem& );
	Policies mAppPolicies;
    Application* mApplication;
	static log4cplus::Logger mLogger;
};

}; // namespace NsAppManager

#endif /* REGISTRYITEM_H_ */
