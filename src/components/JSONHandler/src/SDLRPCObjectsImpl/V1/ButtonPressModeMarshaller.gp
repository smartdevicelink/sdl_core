%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name ButtonPressMode_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPC::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
LONG,0
SHORT,1
