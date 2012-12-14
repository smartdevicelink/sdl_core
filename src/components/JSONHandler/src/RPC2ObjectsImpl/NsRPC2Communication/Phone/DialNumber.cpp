#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/Phone/DialNumber.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::Phone
  version	2.0
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::Phone;


DialNumber& DialNumber::operator =(const DialNumber& c)
{
  number=c.number;
  appId=c.appId;
  return *this;
}


DialNumber::~DialNumber(void)
{
}


DialNumber::DialNumber(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBER)
{
}


DialNumber::DialNumber(const DialNumber& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_PHONE__DIALNUMBER,c.getId())
{
  *this=c;
}


const std::string& DialNumber::get_number(void)
{
  return number;
}

bool DialNumber::set_number(const std::string& number_)
{
  number=number_;
  return true;
}

int DialNumber::get_appId(void)
{
  return appId;
}

bool DialNumber::set_appId(int appId_)
{
  appId=appId_;
  return true;
}

bool DialNumber::checkIntegrity(void)
{
  return DialNumberMarshaller::checkIntegrity(*this);
}
