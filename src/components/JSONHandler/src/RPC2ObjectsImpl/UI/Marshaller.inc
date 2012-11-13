/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/RPC2ObjectsImpl/UI/Marshaller.inc ../src/RPC2ObjectsImpl/UI/Marshaller.gp  */
/* Computed positions: -k'4,13' */

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


#define TOTAL_KEYWORDS 30
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 37
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 62
/* maximum key range = 56, duplicates = 0 */

class Marshaller_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct Marshaller::localHash *getPointer (const char *str, unsigned int len);
};

inline unsigned int
Marshaller_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63,  0, 63, 20,  0, 63,
      63, 10, 63, 63, 63, 63, 63, 25, 63,  0,
       0, 63, 10,  0, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63,  5, 63, 63,
      25,  5, 63, 63, 63, 63, 63, 63, 15,  0,
       0, 15, 63, 63, 63, 63,  0,  5,  0, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63, 63, 63, 63, 63,
      63, 63, 63, 63, 63, 63
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[12]];
      /*FALLTHROUGH*/
      case 12:
      case 11:
      case 10:
      case 9:
      case 8:
      case 7:
      case 6:
      case 5:
      case 4:
        hval += asso_values[(unsigned char)str[3]];
        break;
    }
  return hval;
}

const struct Marshaller::localHash *
Marshaller_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  7,  8,  0,  0,  0, 12,  0,
       0, 15, 16, 17, 13, 19,  0, 21, 22,  0, 24,  0, 21,  0,
       0, 29, 30, 16,  0, 18, 29,  0, 21,  0, 13, 24,  0, 16,
      37, 18, 29,  0, 21, 32,  0, 24,  0, 26,  0,  0, 29,  0,
       0,  0,  0,  0,  0,  0, 37
    };
  static const struct Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 46 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.Show",40,&Marshaller::mShow},
#line 22 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.Alert",16,&Marshaller::mAlert},
      {""}, {""}, {""},
#line 36 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.OnCommand",30,&Marshaller::mOnCommand},
      {""}, {""},
#line 47 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.ShowResponse",41,&Marshaller::mShowResponse},
#line 26 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.DeleteCommand",20,&Marshaller::mDeleteCommand},
#line 34 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.OnAppActivated",28,&Marshaller::mOnAppActivated},
#line 20 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.AddSubMenu",14,&Marshaller::mAddSubMenu},
#line 35 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.OnAppDeactivated",29,&Marshaller::mOnAppDeactivated},
      {""},
#line 38 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.PerformInteraction",32,&Marshaller::mPerformInteraction},
#line 42 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.SetGlobalProperties",36,&Marshaller::mSetGlobalProperties},
      {""},
#line 27 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.DeleteCommandResponse",21,&Marshaller::mDeleteCommandResponse},
      {""},
#line 21 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.AddSubMenuResponse",15,&Marshaller::mAddSubMenuResponse},
      {""}, {""},
#line 39 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.PerformInteractionResponse",33,&Marshaller::mPerformInteractionResponse},
#line 43 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.SetGlobalPropertiesResponse",37,&Marshaller::mSetGlobalPropertiesResponse},
#line 23 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.AlertResponse",17,&Marshaller::mAlertResponse},
      {""},
#line 37 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.OnSystemContext",31,&Marshaller::mOnSystemContext},
#line 28 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSet",22,&Marshaller::mDeleteInteractionChoiceSet},
      {""},
#line 44 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.SetMediaClockTimer",38,&Marshaller::mSetMediaClockTimer},
      {""},
#line 18 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.AddCommand",12,&Marshaller::mAddCommand},
#line 40 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.ResetGlobalProperties",34,&Marshaller::mResetGlobalProperties},
      {""},
#line 30 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.DeleteSubMenu",24,&Marshaller::mDeleteSubMenu},
#line 29 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSetResponse",23,&Marshaller::mDeleteInteractionChoiceSetResponse},
#line 32 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.GetCapabilities",26,&Marshaller::mGetCapabilities},
#line 45 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.SetMediaClockTimerResponse",39,&Marshaller::mSetMediaClockTimerResponse},
      {""},
#line 19 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.AddCommandResponse",13,&Marshaller::mAddCommandResponse},
#line 41 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.ResetGlobalPropertiesResponse",35,&Marshaller::mResetGlobalPropertiesResponse},
      {""},
#line 31 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.DeleteSubMenuResponse",25,&Marshaller::mDeleteSubMenuResponse},
      {""},
#line 33 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.GetCapabilitiesResponse",27,&Marshaller::mGetCapabilitiesResponse},
      {""}, {""},
#line 24 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.CreateInteractionChoiceSet",18,&Marshaller::mCreateInteractionChoiceSet},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 25 "../src/RPC2ObjectsImpl/UI/Marshaller.gp"
      {"UI.CreateInteractionChoiceSetResponse",19,&Marshaller::mCreateInteractionChoiceSetResponse},
      {"UI.OnReady", 50,&Marshaller::mOnReady}
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
