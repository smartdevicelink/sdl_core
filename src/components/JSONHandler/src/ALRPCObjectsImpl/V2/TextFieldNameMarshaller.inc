/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.inc ../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp  */
/* Computed positions: -k'8,$' */

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
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 25
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 50
/* maximum key range = 48, duplicates = 0 */

class TextFieldName_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPCV2::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
TextFieldName_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51,  5,
       0, 23, 30, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51,  0, 51, 51, 51, 51,
      51, 51, 51,  0, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 51, 51, 51,
      51, 51, 51, 51, 51, 51, 51, 10,  0, 51,
      51,  0, 51, 51, 51, 25, 51,  8, 10, 51,
      51,  5, 51, 51,  0,  5,  0, 51, 51, 51,
       0,  0, 51, 51, 51, 51, 51, 51, 51, 51,
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
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
      case 4:
      case 3:
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

const struct NsAppLinkRPCV2::PerfectHashTable *
TextFieldName_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  3,  0,  0,  0,  0,  0,  0, 10,  0, 12,  0,
       0, 10,  0, 12, 13,  9, 10, 21, 22, 10,  0, 10,  0,  0,
      10,  0, 25,  0,  0, 10,  0, 25,  0,  0,  0,  0, 15,  0,
       0, 10,  0, 15,  0,  0,  0,  0, 10
    };
  static const struct NsAppLinkRPCV2::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""},
#line 31 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"ETA",14},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 25 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"alertText2",8},
      {""},
#line 35 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"sliderHeader",18},
      {""}, {""},
#line 24 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"alertText1",7},
      {""},
#line 36 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"sliderFooter",19},
#line 32 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"totalDistance",15},
#line 21 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"statusBar",4},
#line 18 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"mainField2",1},
#line 27 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"scrollableMessageBody",10},
#line 28 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"initialInteractionText",11},
#line 22 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"mediaClock",5},
      {""},
#line 17 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"mainField1",0},
      {""}, {""},
#line 23 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"mediaTrack",6},
      {""},
#line 34 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"audioPassThruDisplayText2",17},
      {""}, {""},
#line 26 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"alertText3",9},
      {""},
#line 33 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"audioPassThruDisplayText1",16},
      {""}, {""}, {""}, {""},
#line 30 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"navigationText2",13},
      {""}, {""},
#line 19 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"mainField3",2},
      {""},
#line 29 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"navigationText1",12},
      {""}, {""}, {""}, {""},
#line 20 "../src/ALRPCObjectsImpl/V2/TextFieldNameMarshaller.gp"
      {"mainField4",3}
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
