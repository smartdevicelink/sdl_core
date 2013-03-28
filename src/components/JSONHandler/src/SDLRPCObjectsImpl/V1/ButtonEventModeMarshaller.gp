%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name ButtonEventMode_intHash
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
BUTTONUP,0
BUTTONDOWN,1
