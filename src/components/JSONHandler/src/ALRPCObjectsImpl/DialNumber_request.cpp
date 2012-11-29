#include "../include/JSONHandler/ALRPCObjects/DialNumber_request.h"
#include "DialNumber_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	42


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;

DialNumber_request::~DialNumber_request(void)
{
}


DialNumber_request::DialNumber_request(const DialNumber_request& c)
{
  *this=c;
}


bool DialNumber_request::checkIntegrity(void)
{
  return DialNumber_requestMarshaller::checkIntegrity(*this);
}


DialNumber_request::DialNumber_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_DIALNUMBER_REQUEST)
{
}



bool DialNumber_request::set_number(const std::string& number_)
{
  if(number_.length()>500)  return false;
  number=number_;
  return true;
}




const std::string& DialNumber_request::get_number(void) const 
{
  return number;
}

