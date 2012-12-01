%language=C++
%struct-type
%define lookup-function-name getPointer
%define class-name FileType_intHash
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
GRAPHIC_BMP,0
GRAPHIC_JPEG,1
GRAPHIC_PNG,2
AUDIO_WAVE,3
AUDIO_MP3,4
