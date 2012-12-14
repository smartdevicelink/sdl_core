#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/VR/GetLanguage.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::VR
  version	1.2
  generated at	Fri Dec 14 06:14:25 2012
  source stamp	Fri Dec 14 06:14:23 2012
  author	robok0der
*/

using namespace NsRPC2Communication::VR;


GetLanguage& GetLanguage::operator =(const GetLanguage& c)
{
  return *this;
}


GetLanguage::~GetLanguage(void)
{
}


GetLanguage::GetLanguage(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGE)
{
}


GetLanguage::GetLanguage(const GetLanguage& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_VR__GETLANGUAGE,c.getId())
{
  *this=c;
}


bool GetLanguage::checkIntegrity(void)
{
  return GetLanguageMarshaller::checkIntegrity(*this);
}
