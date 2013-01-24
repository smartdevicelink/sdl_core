/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.inc ../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp  */
/* Computed positions: -k'8' */

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


#define TOTAL_KEYWORDS 15
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 9
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 23
/* maximum key range = 22, duplicates = 0 */
namespace NsAppLinkRPC
{
class ButtonName_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
ButtonName_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24,  7,  2,
      13,  8,  3, 14,  9,  4, 15, 10, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24,  0, 24, 24, 24, 24, 24,  5, 24,
      24, 24, 24, 24,  0, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24, 24, 24, 24, 24,
      24, 24, 24, 24, 24, 24
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
        break;
    }
  return hval;
}

const struct NsAppLinkRPC::PerfectHashTable *
ButtonName_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  2,  0,  0,  0,  6,  0,  8,  9,  8,  8,  8,  8,
       0,  8,  8,  8,  8,  0,  0,  8,  8,  8
    };
  static const struct NsAppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""},
#line 17 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"OK",0},
      {""}, {""}, {""},
#line 20 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"TUNEUP",3},
      {""},
#line 18 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"SEEKLEFT",1},
#line 19 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"SEEKRIGHT",2},
#line 23 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_1",6},
#line 26 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_4",9},
#line 29 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_7",12},
#line 21 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"TUNEDOWN",4},
      {""},
#line 22 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_0",5},
#line 25 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_3",8},
#line 28 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_6",11},
#line 31 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_9",14},
      {""}, {""},
#line 24 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_2",7},
#line 27 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_5",10},
#line 30 "../src/ALRPCObjectsImpl/V1/ButtonNameMarshaller.gp"
      {"PRESET_8",13}
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
