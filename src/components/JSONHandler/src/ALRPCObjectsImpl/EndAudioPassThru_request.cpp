#include "../include/JSONHandler/ALRPCObjects/EndAudioPassThru_request.h"
#include "EndAudioPassThru_requestMarshaller.h"
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






