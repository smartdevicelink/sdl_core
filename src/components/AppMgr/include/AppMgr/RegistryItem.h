/*
 * RegistryItem.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef REGISTRYITEM_H_
#define REGISTRYITEM_H_

#include "IApplication.h"
#include "AppPolicy.h"
#include <set>
#include <string>

class RegistryItem
{
public:
	RegistryItem( const IApplication& app );
	~RegistryItem( );

	const AppPolicy& registerPolicy( const std::string& hash );
	void unregisterPolicy( AppPolicy& policy );
	const IApplication& getApplication() const;
	bool operator<(const RegistryItem& item2) const;

	std::set<AppPolicy> getApplicationPolicies(const IApplication& app) const;
	std::set<AppPolicy> getApplicationPolicies(const std::string& app) const;

private:
	std::set<AppPolicy> mAppPolicies;
	const IApplication& mApplication;
};

#endif /* REGISTRYITEM_H_ */
