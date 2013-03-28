%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name DisplayType_intHash
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
CID,0
TYPE2,1
TYPE5,2
NGN,3
GEN2_8_DMA,4
GEN2_6_DMA,5
MFD3,6
MFD4,7
MFD5,8
