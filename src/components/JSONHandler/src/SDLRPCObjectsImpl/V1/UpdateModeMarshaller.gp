%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name UpdateMode_intHash
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
COUNTUP,0
COUNTDOWN,1
PAUSE,2
RESUME,3
