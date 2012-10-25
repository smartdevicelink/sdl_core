/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=gen/deep/pimpl/next/AudioCaptureQualityMarshaller.inc gen/deep/pimpl/next/AudioCaptureQualityMarshaller.gp  */
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
  static const struct PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
AudioCaptureQuality_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct PerfectHashTable *
AudioCaptureQuality_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4,  5
    };
  static const struct PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 17 "gen/deep/pimpl/next/AudioCaptureQualityMarshaller.gp"
      {"8KHZ",0},
#line 18 "gen/deep/pimpl/next/AudioCaptureQualityMarshaller.gp"
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
