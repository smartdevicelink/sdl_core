%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name SpeechCapabilities_intHash
%omit-struct-type
%readonly-tables
%compare-strncmp
%compare-lengths

struct NsAppLinkRPCV2::PerfectHashTable
{
  const char *name;
  unsigned int idx;
};

%%
TEXT,0
SAPI_PHONEMES,1
LHPLUS_PHONEMES,2
PRE_RECORDED,3
SILENCE,4
