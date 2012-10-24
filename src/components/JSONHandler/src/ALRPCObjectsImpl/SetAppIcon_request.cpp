#include "../../include/JSONHandler/ALRPCObjects/SetAppIcon_request.h"
#include "SetAppIcon_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

SetAppIcon_request& SetAppIcon_request::operator =(const SetAppIcon_request& c)
{
  syncFileName= c.syncFileName ? new std::string(c.syncFileName[0]) : 0;

  return *this;}


SetAppIcon_request::~SetAppIcon_request(void)
{
  if(syncFileName)
    delete syncFileName;
}


SetAppIcon_request::SetAppIcon_request(const SetAppIcon_request& c)
{
  *this=c;
}


bool SetAppIcon_request::checkIntegrity(void)
{
  return SetAppIcon_requestMarshaller::checkIntegrity(*this);
}


SetAppIcon_request::SetAppIcon_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SETAPPICON_REQUEST),
      syncFileName(0)
{
}



bool SetAppIcon_request::set_syncFileName(const std::string& syncFileName_)
{
  if(syncFileName_.length()>500)  return false;
  delete syncFileName;
  syncFileName=0;

  syncFileName=new std::string(syncFileName_);
  return true;
}

void SetAppIcon_request::reset_syncFileName(void)
{
  if(syncFileName)
    delete syncFileName;
  syncFileName=0;
}




const std::string* SetAppIcon_request::get_syncFileName(void) const 
{
  return syncFileName;
}

