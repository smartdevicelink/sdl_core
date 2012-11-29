#include "../include/JSONHandler/ALRPCObjects/TTSChunk_v2.h"
#include "TTSChunk_v2Marshaller.h"
#include "SpeechCapabilities_v2Marshaller.h"
/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/



using namespace NsAppLinkRPC;

TTSChunk_v2::TTSChunk_v2(const TTSChunk_v2& c)
{
  *this=c;
}


bool TTSChunk_v2::checkIntegrity(void)
{
  return TTSChunk_v2Marshaller::checkIntegrity(*this);
}


TTSChunk_v2::TTSChunk_v2(void)
{
}



bool TTSChunk_v2::set_text(const std::string& text_)
{
  if(text_.length()>500)  return false;
  text=text_;
  return true;
}

bool TTSChunk_v2::set_type(const SpeechCapabilities_v2& type_)
{
  if(!SpeechCapabilities_v2Marshaller::checkIntegrityConst(type_))   return false;
  type=type_;
  return true;
}




const std::string& TTSChunk_v2::get_text(void) const 
{
  return text;
}


const SpeechCapabilities_v2& TTSChunk_v2::get_type(void) const 
{
  return type;
}


