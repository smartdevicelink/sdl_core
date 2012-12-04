#include "../include/JSONHandler/ALRPCObjects/V1/RegisterAppInterface_request.h"
#include "RegisterAppInterface_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "LanguageMarshaller.h"
#include "SyncMsgVersionMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Tue Dec  4 14:55:10 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV1;
RegisterAppInterface_request& RegisterAppInterface_request::operator =(const RegisterAppInterface_request& c)
{
  syncMsgVersion= c.syncMsgVersion;
  appName= c.appName;
  ngnMediaScreenAppName= c.ngnMediaScreenAppName ? new std::string(c.ngnMediaScreenAppName[0]) : 0;
  vrSynonyms= c.vrSynonyms ? new std::vector<std::string>(c.vrSynonyms[0]) : 0;
  usesVehicleData= c.usesVehicleData ? new bool(c.usesVehicleData[0]) : 0;
  isMediaApplication= c.isMediaApplication;
  languageDesired= c.languageDesired;
  autoActivateID= c.autoActivateID ? new std::string(c.autoActivateID[0]) : 0;

  return *this;}


RegisterAppInterface_request::~RegisterAppInterface_request(void)
{
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  if(vrSynonyms)
    delete vrSynonyms;
  if(usesVehicleData)
    delete usesVehicleData;
  if(autoActivateID)
    delete autoActivateID;
}


RegisterAppInterface_request::RegisterAppInterface_request(const RegisterAppInterface_request& c)
{
  *this=c;
}


bool RegisterAppInterface_request::checkIntegrity(void)
{
  return RegisterAppInterface_requestMarshaller::checkIntegrity(*this);
}


RegisterAppInterface_request::RegisterAppInterface_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_REGISTERAPPINTERFACE_REQUEST),
      ngnMediaScreenAppName(0),
    vrSynonyms(0),
    usesVehicleData(0),
    autoActivateID(0)
{
}



bool RegisterAppInterface_request::set_syncMsgVersion(const SyncMsgVersion& syncMsgVersion_)
{
  if(!SyncMsgVersionMarshaller::checkIntegrityConst(syncMsgVersion_))   return false;
  syncMsgVersion=syncMsgVersion_;
  return true;
}

bool RegisterAppInterface_request::set_appName(const std::string& appName_)
{
  if(appName_.length()>100)  return false;
  appName=appName_;
  return true;
}

bool RegisterAppInterface_request::set_ngnMediaScreenAppName(const std::string& ngnMediaScreenAppName_)
{
  if(ngnMediaScreenAppName_.length()>100)  return false;
  delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;

  ngnMediaScreenAppName=new std::string(ngnMediaScreenAppName_);
  return true;
}

void RegisterAppInterface_request::reset_ngnMediaScreenAppName(void)
{
  if(ngnMediaScreenAppName)
    delete ngnMediaScreenAppName;
  ngnMediaScreenAppName=0;
}

bool RegisterAppInterface_request::set_vrSynonyms(const std::vector<std::string>& vrSynonyms_)
{
  unsigned int i=vrSynonyms_.size();
  if(i>100 || i<1)  return false;
  while(i--)
  {
    if(vrSynonyms_[i].length()>40)  return false;
  }
  delete vrSynonyms;
  vrSynonyms=0;

  vrSynonyms=new std::vector<std::string>(vrSynonyms_);
  return true;
}

void RegisterAppInterface_request::reset_vrSynonyms(void)
{
  if(vrSynonyms)
    delete vrSynonyms;
  vrSynonyms=0;
}

bool RegisterAppInterface_request::set_usesVehicleData(bool usesVehicleData_)
{
  delete usesVehicleData;
  usesVehicleData=0;

  usesVehicleData=new bool(usesVehicleData_);
  return true;
}

void RegisterAppInterface_request::reset_usesVehicleData(void)
{
  if(usesVehicleData)
    delete usesVehicleData;
  usesVehicleData=0;
}

bool RegisterAppInterface_request::set_isMediaApplication(bool isMediaApplication_)
{
  isMediaApplication=isMediaApplication_;
  return true;
}

bool RegisterAppInterface_request::set_languageDesired(const Language& languageDesired_)
{
  if(!LanguageMarshaller::checkIntegrityConst(languageDesired_))   return false;
  languageDesired=languageDesired_;
  return true;
}

bool RegisterAppInterface_request::set_autoActivateID(const std::string& autoActivateID_)
{
  if(autoActivateID_.length()>16)  return false;
  delete autoActivateID;
  autoActivateID=0;

  autoActivateID=new std::string(autoActivateID_);
  return true;
}

void RegisterAppInterface_request::reset_autoActivateID(void)
{
  if(autoActivateID)
    delete autoActivateID;
  autoActivateID=0;
}




const SyncMsgVersion& RegisterAppInterface_request::get_syncMsgVersion(void) const 
{
  return syncMsgVersion;
}

const std::string& RegisterAppInterface_request::get_appName(void) const 
{
  return appName;
}

const std::string* RegisterAppInterface_request::get_ngnMediaScreenAppName(void) const 
{
  return ngnMediaScreenAppName;
}

const std::vector<std::string>* RegisterAppInterface_request::get_vrSynonyms(void) const 
{
  return vrSynonyms;
}

const bool* RegisterAppInterface_request::get_usesVehicleData(void) const 
{
  return usesVehicleData;
}

bool RegisterAppInterface_request::get_isMediaApplication(void) const
{
  return isMediaApplication;
}

const Language& RegisterAppInterface_request::get_languageDesired(void) const 
{
  return languageDesired;
}

const std::string* RegisterAppInterface_request::get_autoActivateID(void) const 
{
  return autoActivateID;
}

