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
AppLinkCore.SendData,0,&Marshaller::mNsRPC2Communication_AppLinkCore__SendData
AppLinkCore.SendDataResponse,1,&Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse
