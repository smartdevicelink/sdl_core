#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/DialNumber.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::UI;


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
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DIALNUMBER)
{
}


DialNumber::DialNumber(const DialNumber& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__DIALNUMBER,c.getId())
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
