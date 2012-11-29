#include "../include/JSONHandler/ALRPCObjects/DeleteFile_response.h"
#include "DeleteFile_responseMarshaller.h"
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
DeleteFile_response& DeleteFile_response::operator =(const DeleteFile_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  spaceAvailable= c.spaceAvailable;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


DeleteFile_response::~DeleteFile_response(void)
{
  if(info)
    delete info;
}


DeleteFile_response::DeleteFile_response(const DeleteFile_response& c)
{
  *this=c;
}


bool DeleteFile_response::checkIntegrity(void)
{
  return DeleteFile_responseMarshaller::checkIntegrity(*this);
}


DeleteFile_response::DeleteFile_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_DELETEFILE_RESPONSE),
      info(0)
{
}



bool DeleteFile_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool DeleteFile_response::set_resultCode(const Result_v2& resultCode_)
{
  if(!Result_v2Marshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool DeleteFile_response::set_spaceAvailable(unsigned int spaceAvailable_)
{
  if(spaceAvailable_>2000000000)  return false;
  spaceAvailable=spaceAvailable_;
  return true;
}

bool DeleteFile_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void DeleteFile_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool DeleteFile_response::get_success(void) const
{
  return success;
}

const Result_v2& DeleteFile_response::get_resultCode(void) const 
{
  return resultCode;
}

unsigned int DeleteFile_response::get_spaceAvailable(void) const
{
  return spaceAvailable;
}

const std::string* DeleteFile_response::get_info(void) const 
{
  return info;
}

