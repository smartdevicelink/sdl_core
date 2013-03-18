/* Copyright (c) 2013 Ford Motor Company */

/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/SpeechCapabilitiesMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/SpeechCapabilitiesMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 5
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 15
/* maximum key range = 12, duplicates = 0 */

class SpeechCapabilities_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
SpeechCapabilities_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsSmartDeviceLinkRPC::PerfectHashTable *
SpeechCapabilities_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4,  0,  0,  7,  0,  0,  0,  0, 12, 13,
       0, 15
    };
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/SpeechCapabilitiesMarshaller.gp"
      {"TEXT",0},
      {""}, {""},
#line 21 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/SpeechCapabilitiesMarshaller.gp"
      {"SILENCE",4},
      {""}, {""}, {""}, {""},
#line 20 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/SpeechCapabilitiesMarshaller.gp"
      {"PRE_RECORDED",3},
#line 18 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/SpeechCapabilitiesMarshaller.gp"
      {"SAPI_PHONEMES",1},
      {""},
#line 19 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/SpeechCapabilitiesMarshaller.gp"
      {"LHPLUS_PHONEMES",2}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}
