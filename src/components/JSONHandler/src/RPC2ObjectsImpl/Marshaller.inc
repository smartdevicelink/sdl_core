/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=../src/RPC2ObjectsImpl/Marshaller.inc ../src/RPC2ObjectsImpl/Marshaller.gp  */
/* Computed positions: -k'1,4,13,17,23' */

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


#define TOTAL_KEYWORDS 109
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 42
#define MIN_HASH_VALUE 8
#define MAX_HASH_VALUE 235
/* maximum key range = 228, duplicates = 0 */

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
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236,   0, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236,   0,   5,  35,   5,  10,
      236,   0, 236, 236, 236, 236,   0, 110,  20,  55,
       15, 236,  20,  70,   0,   0,  35, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236,   0, 120,  90,
       65,   0, 236, 120,  45,   5, 236, 236,  15,  80,
        0,   5,   0, 236,  65,   0,  15,  15,  95, 236,
       10,  50, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236, 236, 236, 236, 236,
      236, 236, 236, 236, 236, 236
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[22]];
      /*FALLTHROUGH*/
      case 22:
      case 21:
      case 20:
      case 19:
      case 18:
      case 17:
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
       0,  0,  0,  0,  0,  0,  0,  0,  8,  9,  0,  0,  0,  0,
       0, 15,  0, 17,  0, 19,  0, 16, 22, 23,  0,  0, 26, 27,
      13, 19, 25, 26,  0, 18, 34, 20, 21, 27, 33, 34,  0,  0,
       0, 28, 19, 25, 26,  0, 23, 24, 30,  0, 27, 33, 29, 25,
      31, 32,  0,  0,  0, 21, 37, 33, 19, 10, 21, 12, 18, 29,
       0, 21,  0,  0, 19,  0,  0,  7, 13,  9,  0, 26, 22, 13,
      29, 15, 21, 17, 18, 29, 20, 21, 37,  0,  0, 20, 21, 27,
      28,  0,  0, 16, 12, 28, 29,  0, 21, 22,  0, 19,  0, 16,
       0, 13, 29,  0, 26, 17, 23, 24,  0, 21,  0,  0, 34, 25,
      31, 32,  0, 24,  0, 16,  0,  0, 14, 30, 16,  0,  0, 24,
      20, 21, 22,  0, 19,  0,  0,  0,  0, 29,  0,  0, 27,  0,
      34,  0,  0, 27,  0, 24,  0,  0, 42,  0, 24,  0,  0,  0,
       0, 14,  0,  0,  0,  0, 24,  0,  0, 22,  0,  0,  0, 31,
      32,  0,  0,  0,  0,  0,  0,  0,  0,  0, 32,  0,  0,  0,
       0,  0,  0,  0, 40,  0,  0,  0,  0,  0,  0, 22,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 30
    };
  static const struct Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 62 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Alert",44,&Marshaller::mNsRPC2Communication_UI__Alert},
#line 56 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.Speak",38,&Marshaller::mNsRPC2Communication_TTS__Speak},
      {""}, {""}, {""}, {""}, {""},
#line 50 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetLanguage",32,&Marshaller::mNsRPC2Communication_TTS__GetLanguage},
      {""},
#line 57 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.SpeakResponse",39,&Marshaller::mNsRPC2Communication_TTS__SpeakResponse},
      {""},
#line 114 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetDTCs",96,&Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCs},
      {""},
#line 63 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AlertResponse",45,&Marshaller::mNsRPC2Communication_UI__AlertResponse},
#line 22 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetAppList",4,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppList},
#line 51 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetLanguageResponse",33,&Marshaller::mNsRPC2Communication_TTS__GetLanguageResponse},
      {""}, {""},
#line 118 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetVehicleType",100,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleType},
#line 115 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetDTCsResponse",97,&Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCsResponse},
#line 60 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddSubMenu",42,&Marshaller::mNsRPC2Communication_UI__AddSubMenu},
#line 48 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetCapabilities",30,&Marshaller::mNsRPC2Communication_TTS__GetCapabilities},
#line 24 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetDeviceList",6,&Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceList},
#line 116 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetVehicleData",98,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleData},
      {""},
#line 74 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetCapabilities",56,&Marshaller::mNsRPC2Communication_UI__GetCapabilities},
#line 119 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetVehicleTypeResponse",101,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse},
#line 46 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.EndAudioPassThru",28,&Marshaller::mNsRPC2Communication_TTS__EndAudioPassThru},
#line 61 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddSubMenuResponse",43,&Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse},
#line 49 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetCapabilitiesResponse",31,&Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse},
#line 25 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetDeviceListResponse",7,&Marshaller::mNsRPC2Communication_AppLinkCore__GetDeviceListResponse},
#line 117 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetVehicleDataResponse",99,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleDataResponse},
      {""}, {""}, {""},
#line 47 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.EndAudioPassThruResponse",29,&Marshaller::mNsRPC2Communication_TTS__EndAudioPassThruResponse},
#line 121 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.ReadDID",103,&Marshaller::mNsRPC2Communication_VehicleInfo__ReadDID},
#line 20 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.DeactivateApp",2,&Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateApp},
#line 75 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetCapabilitiesResponse",57,&Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse},
      {""},
#line 31 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.GetCapabilities",13,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilities},
#line 86 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ResetGlobalProperties",68,&Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties},
#line 23 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.GetAppListResponse",5,&Marshaller::mNsRPC2Communication_AppLinkCore__GetAppListResponse},
      {""},
#line 122 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.ReadDIDResponse",104,&Marshaller::mNsRPC2Communication_VehicleInfo__ReadDIDResponse},
#line 21 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.DeactivateAppResponse",3,&Marshaller::mNsRPC2Communication_AppLinkCore__DeactivateAppResponse},
#line 70 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSet",52,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet},
#line 40 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Navigation.UpdateTurnList",22,&Marshaller::mNsRPC2Communication_Navigation__UpdateTurnList},
#line 32 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.GetCapabilitiesResponse",14,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse},
#line 87 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ResetGlobalPropertiesResponse",69,&Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse},
      {""}, {""}, {""},
#line 64 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ChangeRegistration",46,&Marshaller::mNsRPC2Communication_UI__ChangeRegistration},
#line 71 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSetResponse",53,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse},
#line 41 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Navigation.UpdateTurnListResponse",23,&Marshaller::mNsRPC2Communication_Navigation__UpdateTurnListResponse},
#line 52 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.OnAudioPassThru",34,&Marshaller::mNsRPC2Communication_TTS__OnAudioPassThru},
#line 82 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnReady",64,&Marshaller::mNsRPC2Communication_UI__OnReady},
#line 33 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.OnButtonEvent",15,&Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent},
#line 78 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnCommand",60,&Marshaller::mNsRPC2Communication_UI__OnCommand},
#line 108 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetCapabilities",90,&Marshaller::mNsRPC2Communication_VR__GetCapabilities},
#line 65 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ChangeRegistrationResponse",47,&Marshaller::mNsRPC2Communication_UI__ChangeRegistrationResponse},
      {""},
#line 34 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.OnButtonPress",16,&Marshaller::mNsRPC2Communication_Buttons__OnButtonPress},
      {""}, {""},
#line 81 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnLanguageChange",63,&Marshaller::mNsRPC2Communication_UI__OnLanguageChange},
      {""}, {""},
#line 98 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Show",80,&Marshaller::mNsRPC2Communication_UI__Show},
#line 58 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddCommand",40,&Marshaller::mNsRPC2Communication_UI__AddCommand},
#line 100 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Slider",82,&Marshaller::mNsRPC2Communication_UI__Slider},
      {""},
#line 109 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetCapabilitiesResponse",91,&Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse},
#line 80 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnDriverDistraction",62,&Marshaller::mNsRPC2Communication_UI__OnDriverDistraction},
#line 90 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetAppIcon",72,&Marshaller::mNsRPC2Communication_UI__SetAppIcon},
#line 66 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.CreateInteractionChoiceSet",48,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet},
#line 99 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ShowResponse",81,&Marshaller::mNsRPC2Communication_UI__ShowResponse},
#line 59 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddCommandResponse",41,&Marshaller::mNsRPC2Communication_UI__AddCommandResponse},
#line 101 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SliderResponse",83,&Marshaller::mNsRPC2Communication_UI__SliderResponse},
#line 83 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnSystemContext",65,&Marshaller::mNsRPC2Communication_UI__OnSystemContext},
#line 27 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.OnAppUnregistered",9,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppUnregistered},
#line 88 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ScrollableMessage",70,&Marshaller::mNsRPC2Communication_UI__ScrollableMessage},
#line 91 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetAppIconResponse",73,&Marshaller::mNsRPC2Communication_UI__SetAppIconResponse},
#line 67 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.CreateInteractionChoiceSetResponse",49,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse},
      {""}, {""},
#line 29 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.SendData",11,&Marshaller::mNsRPC2Communication_AppLinkCore__SendData},
#line 104 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.ChangeRegistration",86,&Marshaller::mNsRPC2Communication_VR__ChangeRegistration},
#line 26 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.OnAppRegistered",8,&Marshaller::mNsRPC2Communication_AppLinkCore__OnAppRegistered},
#line 89 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ScrollableMessageResponse",71,&Marshaller::mNsRPC2Communication_UI__ScrollableMessageResponse},
      {""}, {""},
#line 68 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteCommand",50,&Marshaller::mNsRPC2Communication_UI__DeleteCommand},
#line 112 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.OnCommand",94,&Marshaller::mNsRPC2Communication_VR__OnCommand},
#line 30 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.SendDataResponse",12,&Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse},
#line 105 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.ChangeRegistrationResponse",87,&Marshaller::mNsRPC2Communication_VR__ChangeRegistrationResponse},
      {""},
#line 96 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetMediaClockTimer",78,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer},
#line 94 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetGlobalProperties",76,&Marshaller::mNsRPC2Communication_UI__SetGlobalProperties},
      {""},
#line 113 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.OnLanguageChange",95,&Marshaller::mNsRPC2Communication_VR__OnLanguageChange},
      {""},
#line 42 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Phone.DialNumber",24,&Marshaller::mNsRPC2Communication_Phone__DialNumber},
      {""},
#line 102 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.AddCommand",84,&Marshaller::mNsRPC2Communication_VR__AddCommand},
#line 97 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetMediaClockTimerResponse",79,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse},
      {""},
#line 38 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Navigation.ShowConstantTBT",20,&Marshaller::mNsRPC2Communication_Navigation__ShowConstantTBT},
#line 79 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnDeviceChosen",61,&Marshaller::mNsRPC2Communication_UI__OnDeviceChosen},
#line 18 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.ActivateApp",0,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateApp},
#line 54 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.PerformAudioPassThru",36,&Marshaller::mNsRPC2Communication_TTS__PerformAudioPassThru},
      {""},
#line 103 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.AddCommandResponse",85,&Marshaller::mNsRPC2Communication_VR__AddCommandResponse},
      {""}, {""},
#line 39 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Navigation.ShowConstantTBTResponse",21,&Marshaller::mNsRPC2Communication_Navigation__ShowConstantTBTResponse},
#line 120 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.OnVehicleData",102,&Marshaller::mNsRPC2Communication_VehicleInfo__OnVehicleData},
#line 19 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.ActivateAppResponse",1,&Marshaller::mNsRPC2Communication_AppLinkCore__ActivateAppResponse},
#line 55 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.PerformAudioPassThruResponse",37,&Marshaller::mNsRPC2Communication_TTS__PerformAudioPassThruResponse},
      {""},
#line 69 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteCommandResponse",51,&Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse},
      {""},
#line 72 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteSubMenu",54,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenu},
      {""}, {""},
#line 76 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetLanguage",58,&Marshaller::mNsRPC2Communication_UI__GetLanguage},
#line 95 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetGlobalPropertiesResponse",77,&Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse},
#line 106 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.DeleteCommand",88,&Marshaller::mNsRPC2Communication_VR__DeleteCommand},
      {""}, {""},
#line 43 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Phone.DialNumberResponse",25,&Marshaller::mNsRPC2Communication_Phone__DialNumberResponse},
#line 53 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.OnLanguageChange",35,&Marshaller::mNsRPC2Communication_TTS__OnLanguageChange},
#line 84 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformInteraction",66,&Marshaller::mNsRPC2Communication_UI__PerformInteraction},
#line 77 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetLanguageResponse",59,&Marshaller::mNsRPC2Communication_UI__GetLanguageResponse},
      {""},
#line 92 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetDisplayLayout",74,&Marshaller::mNsRPC2Communication_UI__SetDisplayLayout},
      {""}, {""}, {""}, {""},
#line 85 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformInteractionResponse",67,&Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse},
      {""}, {""},
#line 93 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetDisplayLayoutResponse",75,&Marshaller::mNsRPC2Communication_UI__SetDisplayLayoutResponse},
      {""},
#line 125 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.UnsubscribeVehicleData",107,&Marshaller::mNsRPC2Communication_VehicleInfo__UnsubscribeVehicleData},
      {""}, {""},
#line 37 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Navigation.OnTBTClientState",19,&Marshaller::mNsRPC2Communication_Navigation__OnTBTClientState},
      {""},
#line 73 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteSubMenuResponse",55,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse},
      {""}, {""},
#line 126 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.UnsubscribeVehicleDataResponse",108,&Marshaller::mNsRPC2Communication_VehicleInfo__UnsubscribeVehicleDataResponse},
      {""},
#line 107 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.DeleteCommandResponse",89,&Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse},
      {""}, {""}, {""}, {""},
#line 110 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetLanguage",92,&Marshaller::mNsRPC2Communication_VR__GetLanguage},
      {""}, {""}, {""}, {""},
#line 35 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Navigation.AlertManeuver",17,&Marshaller::mNsRPC2Communication_Navigation__AlertManeuver},
      {""}, {""},
#line 111 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetLanguageResponse",93,&Marshaller::mNsRPC2Communication_VR__GetLanguageResponse},
      {""}, {""}, {""},
#line 28 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.OnDeviceListUpdated",10,&Marshaller::mNsRPC2Communication_AppLinkCore__OnDeviceListUpdated},
#line 36 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"Navigation.AlertManeuverResponse",18,&Marshaller::mNsRPC2Communication_Navigation__AlertManeuverResponse},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 123 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.SubscribeVehicleData",105,&Marshaller::mNsRPC2Communication_VehicleInfo__SubscribeVehicleData},
      {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 124 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"AppLinkCore.SubscribeVehicleDataResponse",106,&Marshaller::mNsRPC2Communication_VehicleInfo__SubscribeVehicleDataResponse},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 44 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.ChangeRegistration",26,&Marshaller::mNsRPC2Communication_TTS__ChangeRegistration},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 45 "../src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.ChangeRegistrationResponse",27,&Marshaller::mNsRPC2Communication_TTS__ChangeRegistrationResponse}
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
