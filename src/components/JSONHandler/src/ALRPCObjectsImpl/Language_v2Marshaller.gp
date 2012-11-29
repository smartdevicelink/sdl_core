%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name Language_v2_intHash
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
EN-US,0
ES-MX,1
FR-CA,2
DE-EU,3
ES-EU,4
EN-EU,5
RU-RU,6
TR-TR,7
PL-EU,8
FR-EU,9
IT-EU,10
SV-EU,11
PT-EU,12
NL-EU,13
EN-AU,14
ZH-CN,15
ZH-TW,16
JA-JP,17
AR,18
KO-KR,19
