#include "../include/JSONHandler/ALRPCObjects/GenericResponse_response.h"
#include "GenericResponse_responseMarshaller.h"
#include "../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Mon Nov 19 06:40:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;
GenericResponse_response& GenericResponse_response::operator =(const GenericResponse_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


GenericResponse_response::~GenericResponse_response(void)
{
  if(info)
    delete info;
}


GenericResponse_response::GenericResponse_response(const GenericResponse_response& c)
{
  *this=c;
}


bool GenericResponse_response::checkIntegrity(void)
{
  return GenericResponse_responseMarshaller::checkIntegrity(*this);
}


GenericResponse_response::GenericResponse_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_GENERICRESPONSE_RESPONSE),
      info(0)
{
}



bool GenericResponse_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool GenericResponse_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool GenericResponse_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void GenericResponse_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool GenericResponse_response::get_success(void) const
{
  return success;
}

const Result& GenericResponse_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* GenericResponse_response::get_info(void) const 
{
  return info;
}

