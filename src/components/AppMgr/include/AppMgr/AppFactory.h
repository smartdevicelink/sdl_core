/*
 * AppFactory.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPFACTORY_H_
#define APPFACTORY_H_

namespace NsAppManager
{

class AppFactory
{
public:
	AppFactory( );
	static AppFactory& getInstance();
private:
	~AppFactory( );
};

}; // namespace NsAppManager

#endif /* APPFACTORY_H_ */
