%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name ImageType_intHash
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
STATIC,0
DYNAMIC,1
