/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=gen/deep/pimpl/next/SingleTirePressureStatusMarshaller.inc gen/deep/pimpl/next/SingleTirePressureStatusMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 5
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 13
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 13
/* maximum key range = 11, duplicates = 0 */

class SingleTirePressureStatus_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
SingleTirePressureStatus_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct PerfectHashTable *
SingleTirePressureStatus_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  3,  0,  5,  6,  7,  0,  0,  0,  0,  0, 13
    };
  static const struct PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""},
#line 19 "gen/deep/pimpl/next/SingleTirePressureStatusMarshaller.gp"
      {"LOW",2},
      {""},
#line 20 "gen/deep/pimpl/next/SingleTirePressureStatusMarshaller.gp"
      {"FAULT",3},
#line 18 "gen/deep/pimpl/next/SingleTirePressureStatusMarshaller.gp"
      {"NORMAL",1},
#line 17 "gen/deep/pimpl/next/SingleTirePressureStatusMarshaller.gp"
      {"UNKNOWN",0},
      {""}, {""}, {""}, {""}, {""},
#line 21 "gen/deep/pimpl/next/SingleTirePressureStatusMarshaller.gp"
      {"NOT_SUPPORTED",4}
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
