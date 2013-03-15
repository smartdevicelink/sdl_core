/* Copyright (c) 2013 Ford Motor Company */
/* C++ code produced by gperf version 3.0.3 */
/* Command-line: gperf --output-file=./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.inc ./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp  */
/* Computed positions: -k'1,7,13,23,$' */

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


#define TOTAL_KEYWORDS 110
#define MIN_WORD_LENGTH 7
#define MAX_WORD_LENGTH 43
#define MIN_HASH_VALUE 17
#define MAX_HASH_VALUE 208
/* maximum key range = 192, duplicates = 0 */

namespace NsRPC2Communication {

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
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209,  80,   0,  30,  40, 209,
      209,  10, 209, 209, 209, 209,  90, 122, 209,   5,
        0, 209,  70,  25,  65,   0,  40, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209,   5, 209,  15,
        0,   0,  85,   0,  55,  20, 209,   0,  25, 115,
        0,   5,   5, 209,  25,   0,   5,  50, 209,  70,
      209,  95, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209, 209, 209, 209, 209,
      209, 209, 209, 209, 209, 209
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
  return hval + asso_values[(unsigned char)str[len - 1]];
}

const struct Marshaller::localHash *
Marshaller_intHash::getPointer (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
       0,  0,  0, 12,  0, 19,  0,  0, 17,  0, 19, 20, 21, 17,
      23, 24, 25, 21, 22, 28,  9,  0, 31, 32,  8, 29,  0, 21,
      37, 13, 29,  0, 16, 37,  0, 29,  0, 21, 27,  0, 24, 35,
       0, 12, 38, 34, 35, 36, 32,  0, 19,  0,  0, 22, 43, 19,
       0, 21, 17, 18,  9, 30,  0,  0, 38,  0, 25, 21, 27, 13,
       0, 15, 26, 17, 13, 29, 15, 21, 22, 13, 24,  0, 21, 32,
      23, 24, 40, 21, 32,  0, 14, 10, 26,  0,  0, 19, 30, 21,
      22, 18,  0,  0, 16, 27,  0, 29, 25, 26, 27, 18, 34,  0,
      26,  0, 33,  0,  0, 16,  0,  0, 29,  0,  0, 37, 13, 24,
       0, 26, 27,  0, 14,  0, 24,  7,  0, 34,  0,  0, 22,  0,
      19,  0,  0,  0, 18, 19,  0,  0,  0,  0,  0,  0, 31, 27,
       0, 19, 30, 16,  0,  0,  0, 20, 29,  0,  0, 24,  0,  0,
       0,  0,  0,  0,  0,  0, 16,  0,  0,  0,  0, 21,  0,  0,
       0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 23
    };
  static const struct Marshaller::localHash wordlist[] =
    {
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 74 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnCommand",56,&Marshaller::mNsRPC2Communication_UI__OnCommand},
      {""},
#line 80 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnTBTClientState",62,&Marshaller::mNsRPC2Communication_UI__OnTBTClientState},
      {""}, {""},
#line 102 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SliderResponse",84,&Marshaller::mNsRPC2Communication_UI__SliderResponse},
      {""},
#line 77 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnLanguageChange",59,&Marshaller::mNsRPC2Communication_UI__OnLanguageChange},
#line 87 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ScrollableMessage",69,&Marshaller::mNsRPC2Communication_UI__ScrollableMessage},
#line 34 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.OnButtonPress",16,&Marshaller::mNsRPC2Communication_Buttons__OnButtonPress},
#line 103 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.UpdateTurnList",85,&Marshaller::mNsRPC2Communication_UI__UpdateTurnList},
#line 31 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.GetCapabilities",13,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilities},
#line 85 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ResetGlobalProperties",67,&Marshaller::mNsRPC2Communication_UI__ResetGlobalProperties},
#line 104 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.UpdateTurnListResponse",86,&Marshaller::mNsRPC2Communication_UI__UpdateTurnListResponse},
#line 33 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.OnButtonEvent",15,&Marshaller::mNsRPC2Communication_Buttons__OnButtonEvent},
#line 93 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetGlobalProperties",75,&Marshaller::mNsRPC2Communication_UI__SetGlobalProperties},
#line 88 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ScrollableMessageResponse",70,&Marshaller::mNsRPC2Communication_UI__ScrollableMessageResponse},
#line 101 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Slider",83,&Marshaller::mNsRPC2Communication_UI__Slider},
      {""},
#line 32 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"Buttons.GetCapabilitiesResponse",14,&Marshaller::mNsRPC2Communication_Buttons__GetCapabilitiesResponse},
#line 86 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ResetGlobalPropertiesResponse",68,&Marshaller::mNsRPC2Communication_UI__ResetGlobalPropertiesResponse},
#line 50 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Alert",32,&Marshaller::mNsRPC2Communication_UI__Alert},
#line 60 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSet",42,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSet},
      {""},
#line 54 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ChangeRegistration",36,&Marshaller::mNsRPC2Communication_UI__ChangeRegistration},
#line 61 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteInteractionChoiceSetResponse",43,&Marshaller::mNsRPC2Communication_UI__DeleteInteractionChoiceSetResponse},
#line 46 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddCommand",28,&Marshaller::mNsRPC2Communication_UI__AddCommand},
#line 56 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.CreateInteractionChoiceSet",38,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSet},
      {""},
#line 51 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AlertResponse",33,&Marshaller::mNsRPC2Communication_UI__AlertResponse},
#line 57 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.CreateInteractionChoiceSetResponse",39,&Marshaller::mNsRPC2Communication_UI__CreateInteractionChoiceSetResponse},
      {""},
#line 55 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ChangeRegistrationResponse",37,&Marshaller::mNsRPC2Communication_UI__ChangeRegistrationResponse},
      {""},
#line 47 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddCommandResponse",29,&Marshaller::mNsRPC2Communication_UI__AddCommandResponse},
#line 29 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.SendData",11,&Marshaller::mNsRPC2Communication_BasicCommunication__SendData},
      {""},
#line 72 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetSupportedLanguages",54,&Marshaller::mNsRPC2Communication_UI__GetSupportedLanguages},
#line 30 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.SendDataResponse",12,&Marshaller::mNsRPC2Communication_BasicCommunication__SendDataResponse},
      {""},
#line 117 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.OnCommand",99,&Marshaller::mNsRPC2Communication_VR__OnCommand},
#line 28 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.OnDeviceListUpdated",10,&Marshaller::mNsRPC2Communication_BasicCommunication__OnDeviceListUpdated},
#line 26 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.OnAppRegistered",8,&Marshaller::mNsRPC2Communication_BasicCommunication__OnAppRegistered},
#line 24 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.OnAppDeactivated",6,&Marshaller::mNsRPC2Communication_BasicCommunication__OnAppDeactivated},
#line 27 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.OnAppUnregistered",9,&Marshaller::mNsRPC2Communication_BasicCommunication__OnAppUnregistered},
#line 73 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetSupportedLanguagesResponse",55,&Marshaller::mNsRPC2Communication_UI__GetSupportedLanguagesResponse},
      {""},
#line 118 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.OnLanguageChange",100,&Marshaller::mNsRPC2Communication_VR__OnLanguageChange},
      {""}, {""},
#line 76 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnDriverDistraction",58,&Marshaller::mNsRPC2Communication_UI__OnDriverDistraction},
#line 25 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.OnAppDeactivatedResponse",7,&Marshaller::mNsRPC2Communication_UI__OnDriverDistraction},  // TODO(AK): remove in new code generation
#line 119 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.GetDTCs",101,&Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCs},
      {""},
#line 65 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DialNumberResponse",47,&Marshaller::mNsRPC2Communication_UI__DialNumberResponse},
#line 75 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnDeviceChosen",57,&Marshaller::mNsRPC2Communication_UI__OnDeviceChosen},
#line 68 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetCapabilities",50,&Marshaller::mNsRPC2Communication_UI__GetCapabilities},
#line 44 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.Speak",26,&Marshaller::mNsRPC2Communication_TTS__Speak},
#line 18 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.ActivateApp",0,&Marshaller::mNsRPC2Communication_BasicCommunication__ActivateApp},
      {""}, {""},
#line 19 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.ActivateAppResponse",1,&Marshaller::mNsRPC2Communication_BasicCommunication__ActivateAppResponse},
      {""},
#line 125 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.OnVehicleData",107,&Marshaller::mNsRPC2Communication_VehicleInfo__OnVehicleData},
#line 107 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.ChangeRegistration",89,&Marshaller::mNsRPC2Communication_VR__ChangeRegistration},
#line 120 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.GetDTCsResponse",102,&Marshaller::mNsRPC2Communication_VehicleInfo__GetDTCsResponse},
#line 105 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.AddCommand",87,&Marshaller::mNsRPC2Communication_VR__AddCommand},
      {""},
#line 98 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ShowResponse",80,&Marshaller::mNsRPC2Communication_UI__ShowResponse},
#line 69 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetCapabilitiesResponse",51,&Marshaller::mNsRPC2Communication_UI__GetCapabilitiesResponse},
#line 45 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.SpeakResponse",27,&Marshaller::mNsRPC2Communication_TTS__SpeakResponse},
#line 64 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DialNumber",46,&Marshaller::mNsRPC2Communication_UI__DialNumber},
#line 108 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.ChangeRegistrationResponse",90,&Marshaller::mNsRPC2Communication_VR__ChangeRegistrationResponse},
#line 39 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetLanguage",21,&Marshaller::mNsRPC2Communication_TTS__GetLanguage},
#line 106 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.AddCommandResponse",88,&Marshaller::mNsRPC2Communication_VR__AddCommandResponse},
#line 35 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.ChangeRegistration",17,&Marshaller::mNsRPC2Communication_TTS__ChangeRegistration},
#line 89 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetAppIcon",71,&Marshaller::mNsRPC2Communication_UI__SetAppIcon},
#line 115 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetSupportedLanguages",97,&Marshaller::mNsRPC2Communication_VR__GetSupportedLanguages},
      {""},
#line 49 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddSubMenuResponse",31,&Marshaller::mNsRPC2Communication_UI__AddSubMenuResponse},
#line 22 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.GetDeviceList",4,&Marshaller::mNsRPC2Communication_BasicCommunication__GetDeviceList},
#line 40 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetLanguageResponse",22,&Marshaller::mNsRPC2Communication_TTS__GetLanguageResponse},
#line 53 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AlertManeuverResponse",35,&Marshaller::mNsRPC2Communication_UI__AlertManeuverResponse},
#line 23 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.GetDeviceListResponse",5,&Marshaller::mNsRPC2Communication_BasicCommunication__GetDeviceListResponse},
#line 90 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetAppIconResponse",72,&Marshaller::mNsRPC2Communication_UI__SetAppIconResponse},
#line 116 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetSupportedLanguagesResponse",98,&Marshaller::mNsRPC2Communication_VR__GetSupportedLanguagesResponse},
      {""},
#line 70 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetLanguage",52,&Marshaller::mNsRPC2Communication_UI__GetLanguage},
#line 78 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnReady",60,&Marshaller::mNsRPC2Communication_UI__OnReady},
#line 100 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ShowConstantTBTResponse",82,&Marshaller::mNsRPC2Communication_UI__ShowConstantTBTResponse},
      {""}, {""},
#line 37 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetCapabilities",19,&Marshaller::mNsRPC2Communication_TTS__GetCapabilities},
#line 94 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetGlobalPropertiesResponse",76,&Marshaller::mNsRPC2Communication_UI__SetGlobalPropertiesResponse},
#line 83 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformInteraction",65,&Marshaller::mNsRPC2Communication_UI__PerformInteraction},
#line 71 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.GetLanguageResponse",53,&Marshaller::mNsRPC2Communication_UI__GetLanguageResponse},
#line 111 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetCapabilities",93,&Marshaller::mNsRPC2Communication_VR__GetCapabilities},
      {""}, {""},
#line 52 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AlertManeuver",34,&Marshaller::mNsRPC2Communication_UI__AlertManeuver},
#line 67 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.EndAudioPassThruResponse",49,&Marshaller::mNsRPC2Communication_UI__EndAudioPassThruResponse},
      {""},
#line 84 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformInteractionResponse",66,&Marshaller::mNsRPC2Communication_UI__PerformInteractionResponse},
#line 41 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetSupportedLanguages",23,&Marshaller::mNsRPC2Communication_TTS__GetSupportedLanguages},
#line 121 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.GetVehicleData",103,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleData},
#line 38 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetCapabilitiesResponse",20,&Marshaller::mNsRPC2Communication_TTS__GetCapabilitiesResponse},
#line 79 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.OnSystemContext",61,&Marshaller::mNsRPC2Communication_UI__OnSystemContext},
#line 122 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.GetVehicleDataResponse",104,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleDataResponse},
      {""},
#line 112 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetCapabilitiesResponse",94,&Marshaller::mNsRPC2Communication_VR__GetCapabilitiesResponse},
      {""},
#line 42 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.GetSupportedLanguagesResponse",24,&Marshaller::mNsRPC2Communication_TTS__GetSupportedLanguagesResponse},
      {""}, {""},
#line 58 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteCommand",40,&Marshaller::mNsRPC2Communication_UI__DeleteCommand},
      {""}, {""},
#line 20 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.GetAppList",2,&Marshaller::mNsRPC2Communication_BasicCommunication__GetAppList},
      {""}, {""},
#line 21 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"BasicCommunication.GetAppListResponse",3,&Marshaller::mNsRPC2Communication_BasicCommunication__GetAppListResponse},
#line 48 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.AddSubMenu",30,&Marshaller::mNsRPC2Communication_UI__AddSubMenu},
#line 59 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteCommandResponse",41,&Marshaller::mNsRPC2Communication_UI__DeleteCommandResponse},
      {""},
#line 123 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.GetVehicleType",105,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleType},
#line 127 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.ReadDIDResponse",109,&Marshaller::mNsRPC2Communication_VehicleInfo__ReadDIDResponse},
      {""},
#line 113 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetLanguage",95,&Marshaller::mNsRPC2Communication_VR__GetLanguage},
      {""},
#line 63 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteSubMenuResponse",45,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenuResponse},
#line 97 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.Show",79,&Marshaller::mNsRPC2Communication_UI__Show},
      {""},
#line 124 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.GetVehicleTypeResponse",106,&Marshaller::mNsRPC2Communication_VehicleInfo__GetVehicleTypeResponse},
      {""}, {""},
#line 114 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.GetLanguageResponse",96,&Marshaller::mNsRPC2Communication_VR__GetLanguageResponse},
      {""},
#line 66 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.EndAudioPassThru",48,&Marshaller::mNsRPC2Communication_UI__EndAudioPassThru},
      {""}, {""}, {""},
#line 99 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.ShowConstantTBT",81,&Marshaller::mNsRPC2Communication_UI__ShowConstantTBT},
#line 91 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetDisplayLayout",73,&Marshaller::mNsRPC2Communication_UI__SetDisplayLayout},
      {""}, {""}, {""}, {""}, {""}, {""},
#line 82 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformAudioPassThruResponse",64,&Marshaller::mNsRPC2Communication_UI__PerformAudioPassThruResponse},
#line 92 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetDisplayLayoutResponse",74,&Marshaller::mNsRPC2Communication_UI__SetDisplayLayoutResponse},
      {""},
#line 126 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VehicleInfo.ReadDID",108,&Marshaller::mNsRPC2Communication_VehicleInfo__ReadDID},
#line 36 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.ChangeRegistrationResponse",18,&Marshaller::mNsRPC2Communication_TTS__ChangeRegistrationResponse},
#line 109 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.DeleteCommand",91,&Marshaller::mNsRPC2Communication_VR__DeleteCommand},
      {""}, {""}, {""},
#line 43 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"TTS.OnLanguageChange",25,&Marshaller::mNsRPC2Communication_TTS__OnLanguageChange},
#line 96 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetMediaClockTimerResponse",78,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimerResponse},
      {""}, {""},
#line 110 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"VR.DeleteCommandResponse",92,&Marshaller::mNsRPC2Communication_VR__DeleteCommandResponse},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
#line 62 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.DeleteSubMenu",44,&Marshaller::mNsRPC2Communication_UI__DeleteSubMenu},
      {""}, {""}, {""}, {""},
#line 95 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.SetMediaClockTimer",77,&Marshaller::mNsRPC2Communication_UI__SetMediaClockTimer},
      {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""}, {""},
      {""}, {""}, {""}, {""}, {""},
#line 81 "./src/components/JSONHandler/src/RPC2ObjectsImpl/Marshaller.gp"
      {"UI.PerformAudioPassThru",63,&Marshaller::mNsRPC2Communication_UI__PerformAudioPassThru}
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
