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
VR.AddCommand,0,&Marshaller::mAddCommand
VR.AddCommandResponse,1,&Marshaller::mAddCommandResponse
VR.DeleteCommand,2,&Marshaller::mDeleteCommand
VR.DeleteCommandResponse,3,&Marshaller::mDeleteCommandResponse
VR.GetCapabilities,4,&Marshaller::mGetCapabilities
VR.GetCapabilitiesResponse,5,&Marshaller::mGetCapabilitiesResponse
VR.OnCommand,6,&Marshaller::mOnCommand
