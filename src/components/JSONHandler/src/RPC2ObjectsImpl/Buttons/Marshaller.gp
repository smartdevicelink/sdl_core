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
Buttons.GetCapabilities,0,&Marshaller::mGetCapabilities
Buttons.GetCapabilitiesResponse,1,&Marshaller::mGetCapabilitiesResponse
Buttons.OnButtonEvent,2,&Marshaller::mOnButtonEvent
Buttons.OnButtonPress,3,&Marshaller::mOnButtonPress
