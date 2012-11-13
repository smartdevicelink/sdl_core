/*
 * AppPolicy.h
 *
 *  Created on: Oct 5, 2012
 *      Author: vsalo
 */

#ifndef APPPOLICY_H_
#define APPPOLICY_H_

#include <string>

namespace log4cplus
{
	class Logger;
}

namespace NsAppManager
{
	
/**
 * \brief AppPolicy acts as application behaviour modifier according to some rule
 */
class AppPolicy
{
public:

    /**
     * \brief Class constructor
     * \param policy text representation
     */
	AppPolicy( const std::string& policy );

    /**
     * \brief Default class destructor
     */
	~AppPolicy( );

    /**
     * \brief comparison operator
     * \return comparison result
     */
	bool operator<(const AppPolicy& item2) const;

    /**
     * \brief gets application policy text representation
     * \return application policy test representation
     */
	const std::string& getPolicyHash() const;

private:

    /**
     * \brief Default class copy constructor
     */
    AppPolicy( const AppPolicy& );

	const std::string mPolicy;
	static log4cplus::Logger mLogger;
};

} // namespace NsAppManager

#endif /* APPPOLICY_H_ */
