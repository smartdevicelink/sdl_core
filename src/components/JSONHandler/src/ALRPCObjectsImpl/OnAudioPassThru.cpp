#include "../../include/JSONHandler/ALRPCObjects/OnAudioPassThru.h"
#include "OnAudioPassThruMarshaller.h"
#include "../../include/JSONHandler/ALRPCObjects/Marshaller.h"
#include "DataTypeMarshaller.h"

#define PROTOCOL_VERSION	1


/*
  interface	Ford Sync RAPI
  version	2.0L
  date		2012-09-13
  generated at	Wed Oct 24 13:40:36 2012
  source stamp	Wed Oct 24 13:40:27 2012
  author	robok0der
*/


OnAudioPassThru::~OnAudioPassThru(void)
{
}


OnAudioPassThru::OnAudioPassThru(const OnAudioPassThru& c)
{
  *this=c;
}


bool OnAudioPassThru::checkIntegrity(void)
{
  return OnAudioPassThruMarshaller::checkIntegrity(*this);
}


OnAudioPassThru::OnAudioPassThru(void) : ALRPCNotification(PROTOCOL_VERSION,Marshaller::METHOD_ONAUDIOPASSTHRU)
{
}



bool OnAudioPassThru::set_audioPacket(const DataType& audioPacket_)
{
  if(!DataTypeMarshaller::checkIntegrityConst(audioPacket_))   return false;
  audioPacket=audioPacket_;
  return true;
}




const DataType& OnAudioPassThru::get_audioPacket(void) const 
{
  return audioPacket;
}

