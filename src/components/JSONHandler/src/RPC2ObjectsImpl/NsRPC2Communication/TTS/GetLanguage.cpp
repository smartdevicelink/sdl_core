#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/TTS/GetLanguage.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::TTS
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	robok0der
*/

using namespace NsRPC2Communication::TTS;


GetLanguage& GetLanguage::operator =(const GetLanguage& c)
{
  return *this;
}


GetLanguage::~GetLanguage(void)
{
}


GetLanguage::GetLanguage(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGE)
{
}


GetLanguage::GetLanguage(const GetLanguage& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_TTS__GETLANGUAGE,c.getId())
{
  *this=c;
}


bool GetLanguage::checkIntegrity(void)
{
  return GetLanguageMarshaller::checkIntegrity(*this);
}
