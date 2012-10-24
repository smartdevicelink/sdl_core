#include "../../include/JSONHandler/ALRPCObjects/EndAudioPassThru_request.h"
#include "EndAudioPassThru_requestMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


EndAudioPassThru_request::~EndAudioPassThru_request(void)
{
}


EndAudioPassThru_request::EndAudioPassThru_request(const EndAudioPassThru_request& c)
{
  *this=c;
}


bool EndAudioPassThru_request::checkIntegrity(void)
{
  return EndAudioPassThru_requestMarshaller::checkIntegrity(*this);
}


EndAudioPassThru_request::EndAudioPassThru_request(void) : ALRPCRequest(PROTOCOL_VERSION,Marshaller::METHOD_ENDAUDIOPASSTHRU_REQUEST)
{
}






