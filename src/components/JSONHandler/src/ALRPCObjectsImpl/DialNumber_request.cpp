#include "../../include/JSONHandler/ALRPCObjects/DialNumber_request.h"
#include "DialNumber_requestMarshaller.h"
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

