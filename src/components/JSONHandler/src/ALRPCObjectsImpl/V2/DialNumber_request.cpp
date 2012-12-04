#include "../include/JSONHandler/ALRPCObjects/V2/DialNumber_request.h"
#include "DialNumber_requestMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/V2/Marshaller.h"

#define PROTOCOL_VERSION	2


/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Tue Dec  4 14:30:13 2012
  source stamp	Tue Dec  4 14:21:32 2012
  author	robok0der
*/

using namespace NsAppLinkRPCV2;

DialNumber_request::~DialNumber_request(void)
{
}


DialNumber_request::DialNumber_request(const DialNumber_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool DialNumber_request::checkIntegrity(void)
{
  return DialNumber_requestMarshaller::checkIntegrity(*this);
}


DialNumber_request::DialNumber_request(void) : ALRPC2Message(PROTOCOL_VERSION)
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

