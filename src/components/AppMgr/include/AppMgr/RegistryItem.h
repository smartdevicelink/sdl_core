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

namespace NsAppManager
{
	
class RegistryItem
{
public:
	RegistryItem( const Application* app );
	~RegistryItem( );

	const AppPolicy& registerPolicy( const std::string& hash );
	void unregisterPolicy( AppPolicy& policy );
	const Application* getApplication() const;
	bool operator<(const RegistryItem& item2) const;

	std::set<AppPolicy> getApplicationPolicies(const Application* app) const;
	std::set<AppPolicy> getApplicationPolicies(const std::string& app) const;

private:
	std::set<AppPolicy> mAppPolicies;
	const Application* mApplication;
};

}; // namespace NsAppManager

#endif /* REGISTRYITEM_H_ */
