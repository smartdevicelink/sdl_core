/*
 * Application.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_

#include "IApplication.h"
#include <string>

namespace NsAppManager
{

class Application : public IApplication
{
public:
	Application( const std::string& name );
	virtual ~Application( );
	virtual void setApplicationPriority( const AppPriority& priority );
};

}; // namespace NsAppManager

#endif /* APPLICATION_H_ */
