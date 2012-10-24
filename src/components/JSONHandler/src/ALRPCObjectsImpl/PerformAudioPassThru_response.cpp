#include "../../include/JSONHandler/ALRPCObjects/PerformAudioPassThru_response.h"
#include "PerformAudioPassThru_responseMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "ResultMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/

PerformAudioPassThru_response& PerformAudioPassThru_response::operator =(const PerformAudioPassThru_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


PerformAudioPassThru_response::~PerformAudioPassThru_response(void)
{
  if(info)
    delete info;
}


PerformAudioPassThru_response::PerformAudioPassThru_response(const PerformAudioPassThru_response& c)
{
  *this=c;
}


bool PerformAudioPassThru_response::checkIntegrity(void)
{
  return PerformAudioPassThru_responseMarshaller::checkIntegrity(*this);
}


PerformAudioPassThru_response::PerformAudioPassThru_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_PERFORMAUDIOPASSTHRU_RESPONSE),
      info(0)
{
}



bool PerformAudioPassThru_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool PerformAudioPassThru_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool PerformAudioPassThru_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void PerformAudioPassThru_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool PerformAudioPassThru_response::get_success(void) const
{
  return success;
}

const Result& PerformAudioPassThru_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* PerformAudioPassThru_response::get_info(void) const 
{
  return info;
}

