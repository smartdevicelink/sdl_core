/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/RPC2ObjectsImpl/Marshaller.inc ../src/RPC2ObjectsImpl/Marshaller.gp  */
/* Computed positions: -k'1,4,13,17' */

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


#define TOTAL_KEYWORDS 74
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 38
#define MIN_HASH_VALUE 7
#define MAX_HASH_VALUE 159
/* maximum key range = 153, duplicates = 0 */
namespace NsRPC2Communication
{
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
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160,  40, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160,   5,  40,  10,   0,  15,
      160,   0, 160, 160, 160, 160,   0,  80, 160,   0,
       20, 160,  55,   0,  15,   0,  25, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160,   0, 160,  45,
       30,   0, 160,  20,  25,   0, 160, 160,  25,   0,
        5,  15,   0, 160, 160, 160,  15,  65,  30, 160,
       25, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160, 160, 160, 160, 160,
      160, 160, 160, 160, 160, 160
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
      /*FALLTHROUGH*/
      case 3:
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

const struct Marshaller::localHash *
Marshaller_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  7,  0,  9, 10,  0, 12,  8,
       0,  0, 16, 17, 13,  0, 15,  0, 22,  0, 19, 20, 21, 22,
       0,  0, 25, 26, 27, 28, 29, 30, 16, 12, 33, 34,  0, 16,
      22, 18, 29, 25,  0, 17, 13, 19, 30, 26, 37, 33, 29,  0,
      21,  0, 18, 29,  0, 21, 37, 23,  9,  0, 31, 37, 18, 29,
       0, 31, 17, 13, 19,  0, 26,  0, 23, 24,  0, 21, 27, 13,
      29,  0, 31,  0,  0,  0,  0, 21, 37,  0, 24, 20, 16,  0,
       0,  0,  0, 21, 32,  0, 24,  0, 21,  0,  0, 29, 30, 21,
       0,  0,  0,  0,  0,  0, 38,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0, 24
    };
  static const struct Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 76 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Show",58,&Marshaller::mNsRPC2Communication_UI__Show},
      {""},
#line 78 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Slider",60,&Marshaller::mNsRPC2Communication_UI__Slider},
#line 64 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnReady",46,&Marshaller::mNsRPC2Communication_UI__OnReady},
      {""},
#line 60 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnCommand",42,&Marshaller::mNsRPC2Communication_UI__OnCommand},
#line 46 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Alert",28,&Marshaller::mNsRPC2Communication_UI__Alert},
      {""}, {""},
#line 52 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteCommand",34,&Marshaller::mNsRPC2Communication_UI__DeleteCommand},
#line 79 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SliderResponse",61,&Marshaller::mNsRPC2Communication_UI__SliderResponse},
#line 70 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetAppIcon",52,&Marshaller::mNsRPC2Communication_UI__SetAppIcon},
      {""},
#line 77 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ShowResponse",59,&Marshaller::mNsRPC2Communication_UI__ShowResponse},
      {""},
#line 62 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnDriverDistraction",44,&Marshaller::mNsRPC2Communication_UI__OnDriverDistraction},
      {""},
#line 63 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnLanguageChange",45,&Marshaller::mNsRPC2Communication_UI__OnLanguageChange},
#line 29 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.SendData",11,&Marshaller::mNsRPC2Communication_AppLinkCore__SendData},
#line 71 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetAppIconResponse",53,&Marshaller::mNsRPC2Communication_UI__SetAppIconResponse},
#line 22 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetAppList",4,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppList},
      {""}, {""},
#line 24 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetDeviceList",6,&Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceList},
#line 90 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetVehicleType",72,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleType},
#line 26 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.OnAppRegistered",8,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppRegistered},
#line 30 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.SendDataResponse",12,&Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse},
#line 27 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.OnAppUnregistered",9,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppUnregistered},
#line 23 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetAppListResponse",5,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppListResponse},
#line 47 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AlertResponse",29,&Marshaller::mNsRPC2Communication_UI__AlertResponse},
#line 88 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.OnCommand",70,&Marshaller::mNsRPC2Communication_VR__OnCommand},
#line 25 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetDeviceListResponse",7,&Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceListResponse},
#line 91 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetVehicleTypeResponse",73,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse},
      {""},
#line 84 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.DeleteCommand",66,&Marshaller::mNsRPC2Communication_VR__DeleteCommand},
#line 72 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetGlobalProperties",54,&Marshaller::mNsRPC2Communication_UI__SetGlobalProperties},
#line 58 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetCapabilities",40,&Marshaller::mNsRPC2Communication_UI__GetCapabilities},
#line 54 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSet",36,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet},
#line 20 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.DeactivateApp",2,&Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateApp},
      {""},
#line 61 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnDeviceChosen",43,&Marshaller::mNsRPC2Communication_UI__OnDeviceChosen},
#line 42 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddCommand",24,&Marshaller::mNsRPC2Communication_UI__AddCommand},
#line 89 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.OnLanguageChange",71,&Marshaller::mNsRPC2Communication_VR__OnLanguageChange},
#line 73 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetGlobalPropertiesResponse",55,&Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse},
#line 59 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetCapabilitiesResponse",41,&Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse},
#line 55 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSetResponse",37,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse},
#line 21 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.DeactivateAppResponse",3,&Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateAppResponse},
#line 50 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.CreateInteractionChoiceSet",32,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet},
      {""},
#line 43 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddCommandResponse",25,&Marshaller::mNsRPC2Communication_UI__AddCommandResponse},
      {""},
#line 65 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnSystemContext",47,&Marshaller::mNsRPC2Communication_UI__OnSystemContext},
#line 48 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ChangeLanguageRegistration",30,&Marshaller::mNsRPC2Communication_UI__ChangeLanguageRegistration},
      {""},
#line 74 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetMediaClockTimer",56,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer},
#line 51 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.CreateInteractionChoiceSetResponse",33,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse},
#line 18 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.ActivateApp",0,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateApp},
#line 40 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.Speak",22,&Marshaller::mNsRPC2Communication_TTS__Speak},
      {""},
#line 28 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.OnDeviceListUpdated",10,&Marshaller::mNsRPC2Communication_AppLinkCore__OnDeviceListUpdated},
#line 49 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ChangeLanguageRegistrationResponse",31,&Marshaller::mNsRPC2Communication_UI__ChangeLanguageRegistrationResponse},
#line 86 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetCapabilities",68,&Marshaller::mNsRPC2Communication_VR__GetCapabilities},
#line 75 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetMediaClockTimerResponse",57,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse},
      {""},
#line 19 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.ActivateAppResponse",1,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateAppResponse},
#line 41 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.SpeakResponse",23,&Marshaller::mNsRPC2Communication_TTS__SpeakResponse},
#line 80 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.AddCommand",62,&Marshaller::mNsRPC2Communication_VR__AddCommand},
#line 37 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetCapabilities",19,&Marshaller::mNsRPC2Communication_TTS__GetCapabilities},
      {""},
#line 87 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetCapabilitiesResponse",69,&Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse},
      {""},
#line 31 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.GetCapabilities",13,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilities},
#line 53 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteCommandResponse",35,&Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse},
      {""},
#line 81 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.AddCommandResponse",63,&Marshaller::mNsRPC2Communication_VR__AddCommandResponse},
#line 38 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetCapabilitiesResponse",20,&Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse},
#line 44 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddSubMenu",26,&Marshaller::mNsRPC2Communication_UI__AddSubMenu},
#line 82 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.ChangeLanguageRegistration",64,&Marshaller::mNsRPC2Communication_VR__ChangeLanguageRegistration},
      {""},
#line 32 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.GetCapabilitiesResponse",14,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse},
      {""}, {""}, {""}, {""},
#line 45 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddSubMenuResponse",27,&Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse},
#line 83 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.ChangeLanguageRegistrationResponse",65,&Marshaller::mNsRPC2Communication_VR__ChangeLanguageRegistrationResponse},
      {""},
#line 68 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ResetGlobalProperties",50,&Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties},
#line 39 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.OnLanguageChange",21,&Marshaller::mNsRPC2Communication_TTS__OnLanguageChange},
#line 56 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteSubMenu",38,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenu},
      {""}, {""}, {""}, {""},
#line 66 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformInteraction",48,&Marshaller::mNsRPC2Communication_UI__PerformInteraction},
#line 69 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ResetGlobalPropertiesResponse",51,&Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse},
      {""},
#line 85 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.DeleteCommandResponse",67,&Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse},
      {""},
#line 33 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.OnButtonEvent",15,&Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent},
      {""}, {""},
#line 67 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformInteractionResponse",49,&Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse},
#line 35 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.ChangeLanguageRegistration",17,&Marshaller::mNsRPC2Communication_TTS__ChangeLanguageRegistration},
#line 34 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.OnButtonPress",16,&Marshaller::mNsRPC2Communication_Buttons__OnButtonPress},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 36 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.ChangeLanguageRegistrationResponse",18,&Marshaller::mNsRPC2Communication_TTS__ChangeLanguageRegistrationResponse},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""},
#line 57 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteSubMenuResponse",39,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse}
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