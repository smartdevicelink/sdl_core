/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/AudioCaptureQuality_v2Marshaller.inc ../src/ALRPCObjectsImpl/AudioCaptureQuality_v2Marshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 2
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 6
#define MIN_HASH_VALUE 5
#define MAX_HASH_VALUE 6
/* maximum key range = 2, duplicates = 0 */

class AudioCaptureQuality_v2_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
AudioCaptureQuality_v2_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsAppLinkRPC::PerfectHashTable *
AudioCaptureQuality_v2_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  5,  6
    };
  static const struct NsAppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""},
#line 17 "../src/ALRPCObjectsImpl/AudioCaptureQuality_v2Marshaller.gp"
      {"8_BIT",0},
#line 18 "../src/ALRPCObjectsImpl/AudioCaptureQuality_v2Marshaller.gp"
      {"16_BIT",1}
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
