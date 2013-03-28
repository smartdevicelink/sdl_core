%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name AudioCaptureQuality_intHash
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
8KHZ,0
16KHZ,1
