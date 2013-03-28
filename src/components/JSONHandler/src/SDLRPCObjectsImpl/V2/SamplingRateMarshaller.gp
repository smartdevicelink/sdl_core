%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name SamplingRate_intHash
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
8KHZ,0
16KHZ,1
22KHZ,2
44KHZ,3
