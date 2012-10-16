/*
 * IApplication.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef IAPPLICATION_H_
#define IAPPLICATION_H_

#include <string>

namespace NsAppManager
{
	
typedef enum Priority
{
	ACTIVE,
	BACKDROUND,
	NONE
} AppPriority;

class IApplication
{
public:
	IApplication( const std::string& name );
	virtual ~IApplication( )=0;

	virtual void setApplicationPriority( const AppPriority& priority )=0;
	virtual const std::string& getName() const;

protected:
	IApplication(const IApplication&);

	const std::string mName;
	AppPriority mPriority;
};

}; // namespace NsAppManager

#endif /* IAPPLICATION_H_ */
