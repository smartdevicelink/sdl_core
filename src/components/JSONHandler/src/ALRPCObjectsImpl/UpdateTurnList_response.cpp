#include "../include/JSONHandler/ALRPCObjects/UpdateTurnList_response.h"
#include "UpdateTurnList_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "Result_v2Marshaller.h"

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
UpdateTurnList_response& UpdateTurnList_response::operator =(const UpdateTurnList_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


UpdateTurnList_response::~UpdateTurnList_response(void)
{
  if(info)
    delete info;
}


UpdateTurnList_response::UpdateTurnList_response(const UpdateTurnList_response& c)
{
  *this=c;
}


bool UpdateTurnList_response::checkIntegrity(void)
{
  return UpdateTurnList_responseMarshaller::checkIntegrity(*this);
}


UpdateTurnList_response::UpdateTurnList_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_UPDATETURNLIST_RESPONSE),
      info(0)
{
}



bool UpdateTurnList_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool UpdateTurnList_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool UpdateTurnList_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void UpdateTurnList_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool UpdateTurnList_response::get_success(void) const
{
  return success;
}

const Result_v2& UpdateTurnList_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* UpdateTurnList_response::get_info(void) const 
{
  return info;
}

