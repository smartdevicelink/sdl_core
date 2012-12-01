%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name messageType_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPC::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
request,0
response,1
notification,2
