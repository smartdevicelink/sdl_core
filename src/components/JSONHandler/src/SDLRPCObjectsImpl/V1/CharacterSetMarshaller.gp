%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name CharacterSet_intHash
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
TYPE2SET,0
TYPE5SET,1
CID1SET,2
CID2SET,3
