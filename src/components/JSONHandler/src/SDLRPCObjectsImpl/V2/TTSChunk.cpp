#include "../include/JSONHandler/SDLRPCObjects/V2/TTSChunk.h"
#include "TTSChunkMarshaller.h"
#include "SpeechCapabilitiesMarshaller.h"
/*
  interface	Ford Sync RAPI
  version	2.0O
  date		2012-11-02
  generated at	Thu Jan 24 06:36:23 2013
  source stamp	Thu Jan 24 06:35:41 2013
  author	robok0der
*/



using namespace NsSmartDeviceLinkRPCV2;

TTSChunk::TTSChunk(const TTSChunk& c)
{
  *this=c;
}


bool TTSChunk::checkIntegrity(void)
{
  return TTSChunkMarshaller::checkIntegrity(*this);
}


TTSChunk::TTSChunk(void)
{
}



bool TTSChunk::set_text(const std::string& text_)
{
  if(text_.length()>500)  return false;
  text=text_;
  return true;
}

bool TTSChunk::set_type(const SpeechCapabilities& type_)
{
  if(!SpeechCapabilitiesMarshaller::checkIntegrityConst(type_))   return false;
  type=type_;
  return true;
}




const std::string& TTSChunk::get_text(void) const 
{
  return text;
}


const SpeechCapabilities& TTSChunk::get_type(void) const 
{
  return type;
}


