#include "../include/JSONHandler/ALRPCObjects/V2/SetAppIcon_request.h"
#include "SetAppIcon_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

SetAppIcon_request::~SetAppIcon_request(void)
{
}


SetAppIcon_request::SetAppIcon_request(const SetAppIcon_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool SetAppIcon_request::checkIntegrity(void)
{
  return SetAppIcon_requestMarshaller::checkIntegrity(*this);
}


SetAppIcon_request::SetAppIcon_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool SetAppIcon_request::set_syncFileName(const std::string& syncFileName_)
{
  if(syncFileName_.length()>500)  return false;
  syncFileName=syncFileName_;
  return true;
}




const std::string& SetAppIcon_request::get_syncFileName(void) const 
{
  return syncFileName;
}

