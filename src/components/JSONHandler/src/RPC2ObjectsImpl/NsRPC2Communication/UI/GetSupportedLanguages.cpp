//
// Copyright (c) 2013 Ford Motor Company
//

#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/GetSupportedLanguages.h"
#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/

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


bool GetSupportedLanguages::checkIntegrity(void)
{
  return GetSupportedLanguagesMarshaller::checkIntegrity(*this);
}
