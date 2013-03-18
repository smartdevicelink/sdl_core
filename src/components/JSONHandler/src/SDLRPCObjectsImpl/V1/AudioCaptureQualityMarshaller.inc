/* Copyright (c) 2013 Ford Motor Company */

/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/AudioCaptureQualityMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/AudioCaptureQualityMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 2
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 5
/* maximum key range = 2, duplicates = 0 */

class AudioCaptureQuality_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
AudioCaptureQuality_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsSmartDeviceLinkRPC::PerfectHashTable *
AudioCaptureQuality_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4,  5
    };
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/AudioCaptureQualityMarshaller.gp"
      {"8KHZ",0},
#line 18 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/AudioCaptureQualityMarshaller.gp"
      {"16KHZ",1}
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
