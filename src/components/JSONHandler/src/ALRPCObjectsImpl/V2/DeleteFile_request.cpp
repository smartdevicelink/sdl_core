#include "../include/JSONHandler/ALRPCObjects/V2/DeleteFile_request.h"
#include "DeleteFile_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 17:03:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

DeleteFile_request::~DeleteFile_request(void)
{
}


DeleteFile_request::DeleteFile_request(const DeleteFile_request& c) : NsAppLinkRPC::ALRPCMessage(c)
{
  *this=c;
}


bool DeleteFile_request::checkIntegrity(void)
{
  return DeleteFile_requestMarshaller::checkIntegrity(*this);
}


DeleteFile_request::DeleteFile_request(void) : NsAppLinkRPC::ALRPCMessage(PROTOCOL_VERSION)
{
}



bool DeleteFile_request::set_syncFileName(const std::string& syncFileName_)
{
  if(syncFileName_.length()>500)  return false;
  syncFileName=syncFileName_;
  return true;
}




const std::string& DeleteFile_request::get_syncFileName(void) const 
{
  return syncFileName;
}

