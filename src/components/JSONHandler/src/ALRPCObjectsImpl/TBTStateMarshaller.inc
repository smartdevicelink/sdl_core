/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=gen/deep/pimpl/next/TBTStateMarshaller.inc gen/deep/pimpl/next/TBTStateMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 13
#define MAX_WORD_LENGTH 20
#define MIN_HASH_VALUE 13
#define MAX_HASH_VALUE 20
/* maximum key range = 8, duplicates = 0 */

class TBTState_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct AppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
TBTState_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct AppLinkRPC::PerfectHashTable *
TBTState_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 13,
      14, 15,  0,  0,  0,  0, 20
    };
  static const struct AppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 19 "gen/deep/pimpl/next/TBTStateMarshaller.gp"
      {"ROUTE_REFUSED",2},
#line 18 "gen/deep/pimpl/next/TBTStateMarshaller.gp"
      {"ROUTE_ACCEPTED",1},
#line 20 "gen/deep/pimpl/next/TBTStateMarshaller.gp"
      {"ROUTE_CANCELLED",3},
      {""}, {""}, {""}, {""},
#line 17 "gen/deep/pimpl/next/TBTStateMarshaller.gp"
      {"ROUTE_UPDATE_REQUEST",0}
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
