/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=src/RPC2ObjectsImpl/Marshaller.inc src/RPC2ObjectsImpl/Marshaller.gp  */
/* Computed positions: -k'1,7,10,17' */

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


#define TOTAL_KEYWORDS 55
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 37
#define MIN_HASH_VALUE 12
#define MAX_HASH_VALUE 91
/* maximum key range = 80, duplicates = 0 */
namespace NsRPC2Communication{
class Marshaller_intHash
{
private:
  static inline unsigned int hash (const char *str, unsigned int len);
public:
  static const struct NsRPC2Communication::Marshaller::localHash *getPointer (const char *str, unsigned int len);
};
}
inline unsigned int
NsRPC2Communication::Marshaller_intHash::hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92,  5, 20,  0,  5, 30,
      92,  0, 92,  5, 92, 92, 92,  0, 92, 92,
      20, 92,  0, 25,  0,  5,  0, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92,  0,  0, 20,
      10,  5, 20, 92, 92, 30, 92,  0, 20,  0,
      20,  0, 20, 92, 35,  0,  0, 92,  5, 10,
       5,  0, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92, 92, 92, 92, 92,
      92, 92, 92, 92, 92, 92
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[16]];
      /*FALLTHROUGH*/
      case 16:
      case 15:
      case 14:
      case 13:
      case 12:
      case 11:
      case 10:
        hval += asso_values[(unsigned char)str[9]];
      /*FALLTHROUGH*/
      case 9:
      case 8:
      case 7:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
      case 5:
      case 4:
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct NsRPC2Communication::Marshaller::localHash *
NsRPC2Communication::Marshaller_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 12, 13,
       9,  0,  0, 12, 13,  0, 10, 16,  7, 18,  0,  0, 16, 17,
      18, 24, 15, 26, 22, 18, 24,  0, 26,  0,  0, 29, 30, 21,
       0, 13, 29,  0, 21, 37,  8, 19,  0, 16, 37,  0, 24,  0,
      21,  0,  0, 24,  0, 16, 32,  0, 29, 20, 21, 22, 23, 19,
       0, 21, 17, 28, 29,  0, 31, 27, 23,  0,  0, 21, 22,  0,
       0,  0, 31, 27,  0, 29, 30, 21
    };
  static const struct NsRPC2Communication::Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""},
#line 72 "src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.OnCommand",54,&NsRPC2Communication::Marshaller::mNsRPC2Communication_VR__OnCommand},
#line 66 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"VR.AddCommand",48,&NsRPC2Communication::Marshaller::mNsRPC2Communication_VR__AddCommand},
#line 32 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"TTS.Speak",14,&NsRPC2Communication::Marshaller::mNsRPC2Communication_TTS__Speak},
      {""}, {""},
#line 52 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.OnCommand",34,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__OnCommand},
#line 34 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.AddCommand",16,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__AddCommand},
      {""},
#line 54 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.OnReady",36,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__OnReady},
#line 68 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"VR.DeleteCommand",50,&NsRPC2Communication::Marshaller::mNsRPC2Communication_VR__DeleteCommand},
#line 64 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.Show",46,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__Show},
#line 70 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"VR.GetCapabilities",52,&NsRPC2Communication::Marshaller::mNsRPC2Communication_VR__GetCapabilities},
      {""}, {""},
#line 42 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.DeleteCommand",24,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__DeleteCommand},
#line 33 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"TTS.SpeakResponse",15,&NsRPC2Communication::Marshaller::mNsRPC2Communication_TTS__SpeakResponse},
#line 48 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.GetCapabilities",30,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__GetCapabilities},
#line 69 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"VR.DeleteCommandResponse",51,&NsRPC2Communication::Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse},
#line 65 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.ShowResponse",47,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__ShowResponse},
#line 71 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"VR.GetCapabilitiesResponse",53,&NsRPC2Communication::Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse},
#line 60 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.SetGlobalProperties",42,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__SetGlobalProperties},
#line 55 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.OnSystemContext",37,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__OnSystemContext},
#line 43 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.DeleteCommandResponse",25,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse},
      {""},
#line 49 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.GetCapabilitiesResponse",31,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse},
      {""}, {""},
#line 40 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.CreateInteractionChoiceSet",22,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet},
#line 61 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.SetGlobalPropertiesResponse",43,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse},
#line 67 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"VR.AddCommandResponse",49,&NsRPC2Communication::Marshaller::mNsRPC2Communication_VR__AddCommandResponse},
      {""},
#line 36 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.AddSubMenu",18,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__AddSubMenu},
#line 44 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.DeleteInteractionChoiceSet",26,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet},
      {""},
#line 35 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.AddCommandResponse",17,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__AddCommandResponse},
#line 41 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.CreateInteractionChoiceSetResponse",23,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse},
#line 38 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.Alert",20,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__Alert},
#line 51 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.OnAppDeactivated",33,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__OnAppDeactivated},
      {""},
#line 46 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.DeleteSubMenu",28,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__DeleteSubMenu},
#line 45 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.DeleteInteractionChoiceSetResponse",27,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse},
      {""},
#line 58 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.ResetGlobalProperties",40,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties},
      {""},
#line 62 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.SetMediaClockTimer",44,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer},
      {""}, {""},
#line 47 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.DeleteSubMenuResponse",29,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse},
      {""},
#line 39 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.AlertResponse",21,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__AlertResponse},
#line 59 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.ResetGlobalPropertiesResponse",41,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse},
      {""},
#line 63 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.SetMediaClockTimerResponse",45,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse},
#line 24 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.SendData",6,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__SendData},
#line 56 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.PerformInteraction",38,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__PerformInteraction},
#line 53 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.OnDriverDistraction",35,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__OnDriverDistraction},
#line 18 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.ActivateApp",0,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__ActivateApp},
#line 30 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"TTS.GetCapabilities",12,&NsRPC2Communication::Marshaller::mNsRPC2Communication_TTS__GetCapabilities},
      {""},
#line 37 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.AddSubMenuResponse",19,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse},
#line 50 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.OnAppActivated",32,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__OnAppActivated},
#line 25 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.SendDataResponse",7,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse},
#line 57 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"UI.PerformInteractionResponse",39,&NsRPC2Communication::Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse},
      {""},
#line 19 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.ActivateAppResponse",1,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__ActivateAppResponse},
#line 31 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"TTS.GetCapabilitiesResponse",13,&NsRPC2Communication::Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse},
#line 26 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"Buttons.GetCapabilities",8,&NsRPC2Communication::Marshaller::mNsRPC2Communication_Buttons__GetCapabilities},
      {""}, {""},
#line 29 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"Buttons.OnButtonPress",11,&NsRPC2Communication::Marshaller::mNsRPC2Communication_Buttons__OnButtonPress},
#line 20 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.GetAppList",2,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__GetAppList},
      {""}, {""}, {""},
#line 27 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"Buttons.GetCapabilitiesResponse",9,&NsRPC2Communication::Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse},
#line 22 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.OnAppRegistered",4,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__OnAppRegistered},
      {""},
#line 23 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.OnAppUnregistered",5,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__OnAppUnregistered},
#line 21 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"AppLinkCore.GetAppListResponse",3,&NsRPC2Communication::Marshaller::mNsRPC2Communication_AppLinkCore__GetAppListResponse},
#line 28 "src/RPC2ObjectsImpl/NsRPC2Communication::Marshaller.gp"
      {"Buttons.OnButtonEvent",10,&NsRPC2Communication::Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent}
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
