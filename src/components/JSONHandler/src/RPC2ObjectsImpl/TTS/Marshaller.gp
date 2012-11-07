%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Marshaller_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct Marshaller::localHash
{
  const char *name;
  unsigned int idx;
  void* method;
};

%%
TTS.GetCapabilities,0,&Marshaller::mGetCapabilities
TTS.GetCapabilitiesResponse,1,&Marshaller::mGetCapabilitiesResponse
TTS.Speak,2,&Marshaller::mSpeak
TTS.SpeakResponse,3,&Marshaller::mSpeakResponse
