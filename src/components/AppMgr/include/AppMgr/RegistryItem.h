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
}

namespace NsAppManager
{
	
/**
 * \brief RegistryItem acts as a wrapper for registered application in AppMgrRegistry
 */
class RegistryItem
{
public:
	typedef std::set<AppPolicy*> Policies;
	
    /**
     * \brief Class constructor
     * \param app application to register
     */
    RegistryItem(Application *app );

    /**
     * \brief Default class destructor
     */
	virtual ~RegistryItem( );

    /**
     * \brief register application policy
     * \param hash policy text representation
     * \return app policy instance
     */
	const AppPolicy* registerPolicy( const std::string& hash );

    /**
     * \brief unregister application policy
     * \param policy policy to be unregistered
     */
	void unregisterPolicy( AppPolicy* policy );

    /**
     * \brief get associated application
     * \return asssociated application
     */
    Application *getApplication() const;

    /**
     * \brief comparison operator
     * \param item2 item to compare with
     * \return comparison result
     */
	bool operator<(const RegistryItem& item2) const;

    /**
     * \brief get application policies
     * \param app application to get policies for
     * \return app policy set
     */
	Policies getApplicationPolicies(const Application* app) const;

    /**
     * \brief get application policies
     * \param app name of an application to get policies for
     * \return app policy set
     */
	Policies getApplicationPolicies(const std::string& app) const;

private:

    /**
     * \brief Copy constructor
     */
	RegistryItem( const RegistryItem& );

	Policies mAppPolicies;
    Application* mApplication;
	static log4cplus::Logger mLogger;
};

} // namespace NsAppManager

#endif /* REGISTRYITEM_H_ */
