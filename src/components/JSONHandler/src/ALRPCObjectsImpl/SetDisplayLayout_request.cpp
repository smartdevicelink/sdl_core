#include "../include/JSONHandler/ALRPCObjects/SetDisplayLayout_request.h"
#include "SetDisplayLayout_requestMarshaller.h"
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
SetDisplayLayout_request& SetDisplayLayout_request::operator =(const SetDisplayLayout_request& c)
{
  displayLayout= c.displayLayout ? new std::string(c.displayLayout[0]) : 0;

  return *this;}


SetDisplayLayout_request::~SetDisplayLayout_request(void)
{
  if(displayLayout)
    delete displayLayout;
}


SetDisplayLayout_request::SetDisplayLayout_request(const SetDisplayLayout_request& c)
{
  *this=c;
}


bool SetDisplayLayout_request::checkIntegrity(void)
{
  return SetDisplayLayout_requestMarshaller::checkIntegrity(*this);
}


SetDisplayLayout_request::SetDisplayLayout_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_SETDISPLAYLAYOUT_REQUEST),
      displayLayout(0)
{
}



bool SetDisplayLayout_request::set_displayLayout(const std::string& displayLayout_)
{
  delete displayLayout;
  displayLayout=0;

  displayLayout=new std::string(displayLayout_);
  return true;
}

void SetDisplayLayout_request::reset_displayLayout(void)
{
  if(displayLayout)
    delete displayLayout;
  displayLayout=0;
}




const std::string* SetDisplayLayout_request::get_displayLayout(void) const 
{
  return displayLayout;
}

