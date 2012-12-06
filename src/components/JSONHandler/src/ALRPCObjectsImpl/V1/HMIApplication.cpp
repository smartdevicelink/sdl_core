#include "../include/JSONHandler/ALRPCObjects/V1/HMIApplication.h"
#include "HMIApplicationMarshaller.h"

/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 16:02:39 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

HMIApplication& HMIApplication::operator =(const HMIApplication& c)
{
  appName=c.appName;
  icon=c.icon ? new std::string(c.icon[0]) : 0;
  ngnMediaScreenAppName=c.ngnMediaScreenAppName ? new std::string(c.ngnMediaScreenAppName[0]) : 0;

  return *this;
}


HMIApplication::~HMIApplication(void)
{
  if(icon)
    delete icon;
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
}


HMIApplication::HMIApplication(const HMIApplication& c)
{
  *this=c;
}


bool HMIApplication::checkIntegrity(void)
{
  return HMIApplicationMarshaller::checkIntegrity(*this);
}


HMIApplication::HMIApplication(void) :
    icon(0),
    ngnMediaScreenAppName(0)
{
}

void HMIApplication::set_appId(int id)
{
  appId = id;
}

bool HMIApplication::set_appName(const std::string& appName_)
{
  if(appName_.length()>100)  return false;
  appName=appName_;
  return true;
}

bool HMIApplication::set_icon(const std::string& icon_)
{
  delete icon;
  icon=0;

  icon=new std::string(icon_);
  return true;
}

void HMIApplication::reset_icon(void)
{
  if(icon)
    delete icon;
  icon=0;
}

bool HMIApplication::set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_)
{
  if(ngnMediaScreenAppName_.length()>100)  return false;
  delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;

  ngnMediaScreenAppName=new std::string(ngnMediaScreenAppName_);
  return true;
}

void HMIApplication::reset_ngnMediaScreenAppName(void)
{
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;
}


int  HMIApplication::get_appId() const
{
  return appId;
}

const std::string& HMIApplication::get_appName(void) const 
{
  return appName;
}


const std::string* HMIApplication::get_icon(void) const 
{
  return icon;
}


const std::string* HMIApplication::get_ngnMediaScreenAppName(void) const 
{
  return ngnMediaScreenAppName;
}


