#include "../../include/JSONHandler/ALRPCObjects/Speak_response.h"
#include "Speak_responseMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	1.2
  date		2011-05-17
  generated at	Fri Oct 26 06:31:48 2012
  source stamp	Thu Oct 25 06:49:27 2012
  author	robok0der
*/

Speak_response& Speak_response::operator =(const Speak_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


Speak_response::~Speak_response(void)
{
  if(info)
    delete info;
}


Speak_response::Speak_response(const Speak_response& c)
{
  *this=c;
}


bool Speak_response::checkIntegrity(void)
{
  return Speak_responseMarshaller::checkIntegrity(*this);
}


Speak_response::Speak_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_SPEAK_RESPONSE),
      info(0)
{
}



bool Speak_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool Speak_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool Speak_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void Speak_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool Speak_response::get_success(void) const
{
  return success;
}

const Result& Speak_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* Speak_response::get_info(void) const 
{
  return info;
}

