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
AppLinkCore.ActivateApp,0,&Marshaller::mActivateApp
AppLinkCore.ActivateAppResponse,1,&Marshaller::mActivateAppResponse
AppLinkCore.OnAppRegistered,2,&Marshaller::mOnAppRegistered
AppLinkCore.OnAppUnregistered,3,&Marshaller::mOnAppUnregistered
