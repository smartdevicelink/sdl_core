/*
 * AppFactory.h
 *
 *  Created on: Oct 4, 2012
 *      Author: vsalo
 */

#ifndef APPFACTORY_H_
#define APPFACTORY_H_

class AppFactory
{
public:
	AppFactory( );
	static AppFactory& getInstance();
private:
	~AppFactory( );
};

#endif /* APPFACTORY_H_ */
