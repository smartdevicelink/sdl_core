#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetSupportedLanguages.h"
#include "JSONHandler/RPC2Objects/Marshaller.h"

using namespace NsRPC2Communication::UI;


GetSupportedLanguages& GetSupportedLanguages::operator =(const GetSupportedLanguages& c)
{
  return *this;
}


GetSupportedLanguages::~GetSupportedLanguages(void)
{
}


GetSupportedLanguages::GetSupportedLanguages(void) : 
  RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGES)
{
}


GetSupportedLanguages::GetSupportedLanguages(const GetSupportedLanguages& c) : RPC2Request(Marshaller::METHOD_NSRPC2COMMUNICATION_UI__GETSUPPORTEDLANGUAGES,c.getId())
{
  *this=c;
}

