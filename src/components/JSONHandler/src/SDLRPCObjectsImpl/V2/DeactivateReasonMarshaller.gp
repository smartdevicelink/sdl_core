%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name DeactivateReason_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsSmartDeviceLinkRPCV2::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
AUDIO,0
PHONECALL,1
NAVIGATIONMAP,2
PHONEMENU,3
SYNCSETTINGS,4
GENERAL,5
