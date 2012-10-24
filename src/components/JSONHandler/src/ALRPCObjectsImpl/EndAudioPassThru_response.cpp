#include "../../include/JSONHandler/ALRPCObjects/EndAudioPassThru_response.h"
#include "EndAudioPassThru_responseMarshaller.h"
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

EndAudioPassThru_response& EndAudioPassThru_response::operator =(const EndAudioPassThru_response& c)
{
  success= c.success;
  resultCode= c.resultCode;
  info= c.info ? new std::string(c.info[0]) : 0;

  return *this;}


EndAudioPassThru_response::~EndAudioPassThru_response(void)
{
  if(info)
    delete info;
}


EndAudioPassThru_response::EndAudioPassThru_response(const EndAudioPassThru_response& c)
{
  *this=c;
}


bool EndAudioPassThru_response::checkIntegrity(void)
{
  return EndAudioPassThru_responseMarshaller::checkIntegrity(*this);
}


EndAudioPassThru_response::EndAudioPassThru_response(void) : ALRPCResponse(PROTOCOL_VERSION,Marshaller::METHOD_ENDAUDIOPASSTHRU_RESPONSE),
      info(0)
{
}



bool EndAudioPassThru_response::set_success(bool success_)
{
  success=success_;
  return true;
}

bool EndAudioPassThru_response::set_resultCode(const Result& resultCode_)
{
  if(!ResultMarshaller::checkIntegrityConst(resultCode_))   return false;
  resultCode=resultCode_;
  return true;
}

bool EndAudioPassThru_response::set_info(const std::string& info_)
{
  if(info_.length()>1000)  return false;
  delete info;
  info=0;

  info=new std::string(info_);
  return true;
}

void EndAudioPassThru_response::reset_info(void)
{
  if(info)
    delete info;
  info=0;
}




bool EndAudioPassThru_response::get_success(void) const
{
  return success;
}

const Result& EndAudioPassThru_response::get_resultCode(void) const 
{
  return resultCode;
}

const std::string* EndAudioPassThru_response::get_info(void) const 
{
  return info;
}

