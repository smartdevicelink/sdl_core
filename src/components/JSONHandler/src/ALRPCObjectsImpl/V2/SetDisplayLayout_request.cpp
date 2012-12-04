#include "../include/JSONHandler/ALRPCObjects/V2/SetDisplayLayout_request.h"
#include "SetDisplayLayout_requestMarshaller.h"
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

SetDisplayLayout_request::~SetDisplayLayout_request(void)
{
}


SetDisplayLayout_request::SetDisplayLayout_request(const SetDisplayLayout_request& c) : ALRPC2Message(c)
{
  *this=c;
}


bool SetDisplayLayout_request::checkIntegrity(void)
{
  return SetDisplayLayout_requestMarshaller::checkIntegrity(*this);
}


SetDisplayLayout_request::SetDisplayLayout_request(void) : ALRPC2Message(PROTOCOL_VERSION)
{
}



bool SetDisplayLayout_request::set_displayLayout(const std::string& displayLayout_)
{
  displayLayout=displayLayout_;
  return true;
}




const std::string& SetDisplayLayout_request::get_displayLayout(void) const 
{
  return displayLayout;
}

