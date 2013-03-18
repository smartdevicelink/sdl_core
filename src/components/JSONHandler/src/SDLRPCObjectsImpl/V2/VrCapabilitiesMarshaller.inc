/* Copyright (c) 2013 Ford Motor Company */

/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/VrCapabilitiesMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/VrCapabilitiesMarshaller.gp  */
/* Computed positions: -k'' */


#define TOTAL_KEYWORDS 1
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 4
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 4
/* maximum key range = 1, duplicates = 0 */

namespace NsSmartDeviceLinkRPCV2 {

class VrCapabilities_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline /*ARGSUSED*/
unsigned int
VrCapabilities_intHash::hash (register const char *str, register unsigned int len)
{
  return len;
}

const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *
VrCapabilities_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4
    };
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/VrCapabilitiesMarshaller.gp"
      {"TEXT",0}
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
}