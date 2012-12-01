/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/ALRPCObjectsImpl/FunctionIDMarshaller.inc ../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp  */
/* Computed positions: -k'4,9' */

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


#define TOTAL_KEYWORDS 51
#define MIN_WORD_LENGTH 6
#define MAX_WORD_LENGTH 28
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 87
/* maximum key range = 81, duplicates = 0 */

class FunctionID_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsAppLinkRPC::PerfectHashTable *getPointer (const char *str, unsigned int len);
};

inline unsigned int
FunctionID_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 45,  5, 55,  0,  0,
      40,  0, 88, 88, 88, 88, 88, 30, 88, 88,
      88,  0, 88, 50, 88, 88,  0, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88,  0, 30, 20,
       5, 10,  0, 88, 50, 30,  0, 88,  0, 20,
       5,  0, 30, 88, 25,  0,  0,  0, 30, 20,
      88, 88,  0, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88, 88, 88, 88,
      88, 88, 88, 88, 88, 88, 88
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[8]+1];
      /*FALLTHROUGH*/
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

const struct NsAppLinkRPC::PerfectHashTable *
FunctionID_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  7,  8,  9,  0, 11, 12,  8,
       9, 15, 16, 17, 18, 19, 20, 11, 22, 18, 24, 15,  6, 17,
      28,  0, 15, 21,  7, 28, 24, 20, 16, 17, 28, 19, 15, 21,
      12, 13,  0, 15, 21, 17, 18,  9, 20,  0, 22, 23,  0, 15,
       0, 22, 28,  0,  0,  0, 12, 18,  0,  0,  0, 12, 18,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0, 12
    };
  static const struct NsAppLinkRPC::PerfectHashTable wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 31 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SpeakID",14},
#line 17 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"RESERVED",0},
#line 43 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"GetDTCsID",26},
      {""},
#line 53 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"ListFilesID",36},
#line 34 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"DialNumberID",17},
#line 45 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SliderID",28},
#line 42 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"ReadDIDID",25},
#line 59 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnButtonPressID",42},
#line 48 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"UpdateTurnListID",31},
#line 37 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SubscribeButtonID",20},
#line 33 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"EncodedSyncPDataID",16},
#line 38 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"UnsubscribeButtonID",21},
#line 27 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"PerformInteractionID",10},
#line 61 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnCommandID",44},
#line 39 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SubscribeVehicleDataID",22},
#line 63 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnTBTClientStateID",46},
#line 40 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"UnsubscribeVehicleDataID",23},
#line 60 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnVehicleDataID",43},
#line 30 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"ShowID",13},
#line 50 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"GenericResponseID",33},
#line 26 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"CreateInteractionChoiceSetID",9},
      {""},
#line 23 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"DeleteCommandID",6},
#line 65 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnPermissionsChangeID",48},
#line 29 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"AlertID",12},
#line 49 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"ChangeLanguageRegistrationID",32},
#line 19 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"UnregisterAppInterfaceID",2},
#line 62 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnEncodedSyncPDataID",45},
#line 41 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"GetVehicleDataID",24},
#line 46 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"ShowConstantTBTID",29},
#line 28 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"DeleteInteractionChoiceSetID",11},
#line 44 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"ScrollableMessageID",27},
#line 47 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"AlertManeuverID",30},
#line 20 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SetGlobalPropertiesID",3},
#line 52 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"DeleteFileID",35},
#line 56 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnHMIStatusID",39},
      {""},
#line 25 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"DeleteSubMenuID",8},
#line 64 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnDriverDistractionID",47},
#line 66 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnAudioPassThruID",49},
#line 55 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SetDisplayLayoutID",38},
#line 51 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"PutFileID",34},
#line 32 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SetMediaClockTimerID",15},
      {""},
#line 35 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"PerformAudioPassThruID",18},
#line 21 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"ResetGlobalPropertiesID",4},
      {""},
#line 58 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnButtonEventID",41},
      {""},
#line 18 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"RegisterAppInterfaceID",1},
#line 57 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnAppInterfaceUnregisteredID",40},
      {""}, {""}, {""},
#line 24 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"AddSubMenuID",7},
#line 36 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"EndAudioPassThruID",19},
      {""}, {""}, {""},
#line 22 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"AddCommandID",5},
#line 67 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"OnLanguageChangeID",50},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 54 "../src/ALRPCObjectsImpl/FunctionIDMarshaller.gp"
      {"SetAppIconID",37}
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
