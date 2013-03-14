/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.inc ./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp  */
/* Computed positions: -k'1-2,4' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


#define TOTAL_KEYWORDS 20
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 5
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 50
/* maximum key range = 48, duplicates = 0 */

namespace NsSmartDeviceLinkRPCV2 {

class Language_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
Language_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51,  1, 51,  3, 28,  5,
      30, 51,  5, 13,  0, 15, 25, 15,  0, 15,
      10, 51,  0,  3, 10,  0, 10, 51, 51, 51,
       0, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct NsSmartDeviceLinkRPCV2::PerfectHashTable *
Language_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  2,  0,  5,  5,  0,  0,  0,  5,  5,  0,  5,
       0,  5,  0,  0,  5,  0,  5,  0,  0,  5,  0,  5,  0,  0,
       5,  0,  5,  0,  0,  5,  0,  5,  0,  0,  5,  0,  5,  0,
       0,  5,  0,  5,  0,  0,  0,  0,  5
    };
  static const struct NsSmartDeviceLinkRPCV2::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""},
#line 35 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"AR",18},
      {""},
#line 23 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"RU-RU",6},
#line 34 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"JA-JP",17},
      {""}, {""}, {""},
#line 17 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"EN-US",0},
#line 31 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"EN-AU",14},
      {""},
#line 32 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"ZH-CN",15},
      {""},
#line 22 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"EN-EU",5},
      {""}, {""},
#line 21 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"ES-EU",4},
      {""},
#line 33 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"ZH-TW",16},
      {""}, {""},
#line 28 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"SV-EU",11},
      {""},
#line 24 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"TR-TR",7},
      {""}, {""},
#line 18 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"ES-MX",1},
      {""},
#line 29 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"PT-EU",12},
      {""}, {""},
#line 27 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"IT-EU",10},
      {""},
#line 30 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"NL-EU",13},
      {""}, {""},
#line 19 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"FR-CA",2},
      {""},
#line 26 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"FR-EU",9},
      {""}, {""},
#line 20 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"DE-EU",3},
      {""},
#line 25 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"PL-EU",8},
      {""}, {""}, {""}, {""},
#line 36 "./src/components/JSONHandler/src/SDLRPCObjectsImpl/V2/LanguageMarshaller.gp"
      {"KO-KR",19}
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