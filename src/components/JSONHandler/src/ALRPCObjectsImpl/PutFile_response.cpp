#include "../include/JSONHandler/ALRPCObjects/PutFile_response.h"
#include "PutFile_responseMarshaller.h"
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
PutFile_response& PutFile_response::operator =(const PutFile_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  spaceAvailable= c.spaceAvailable;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


PutFile_response::~PutFile_response(void)
{
  if(info)
    delete info;
}


PutFile_response::PutFile_response(const PutFile_response& c)
{
  *this=c;
}


bool PutFile_response::checkIntegrity(void)
{
  return PutFile_responseMarshaller::checkIntegrity(*this);
}


PutFile_response::PutFile_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_PUTFILE_RESPONSE),
      info(0)
{
}



bool PutFile_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool PutFile_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool PutFile_response::set_spaceAvailable(unsigned int spaceAvailable_)
{
  if(spaceAvailable_>2000000000)  return false;
  spaceAvailable=spaceAvailable_;
  return true;
}

bool PutFile_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void PutFile_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool PutFile_response::get_success(void) const
{
  return success;
}

const Result_v2& PutFile_response::get_resultCode(void) const 
{
  return resultCode;
}

unsigned int PutFile_response::get_spaceAvailable(void) const
{
  return spaceAvailable;
}

const std::string* PutFile_response::get_info(void) const 
{
  return info;
}

