/* Copyright (c) 2013 Ford Motor Company */

/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/UpdateModeMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/UpdateModeMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 4
#define MIN_WORD_LENGTH 5
#define MAX_WORD_LENGTH 9
#define MIN_HASH_VALUE 5
#define MAX_HASH_VALUE 9
/* maximum key range = 5, duplicates = 0 */

class UpdateMode_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
UpdateMode_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsSmartDeviceLinkRPC::PerfectHashTable *
UpdateMode_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  5,  6,  7,  0,  9
    };
  static const struct NsSmartDeviceLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""},
#line 19 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/UpdateModeMarshaller.gp"
      {"PAUSE",2},
#line 20 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/UpdateModeMarshaller.gp"
      {"RESUME",3},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/UpdateModeMarshaller.gp"
      {"COUNTUP",0},
      {""},
#line 18 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V1/UpdateModeMarshaller.gp"
      {"COUNTDOWN",1}
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
