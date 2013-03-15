//
// Copyright (c) 2013 Ford Motor Company
//

#ifndef NSRPC2COMMUNICATION_UI_RPC2_INCLUDE
#define NSRPC2COMMUNICATION_UI_RPC2_INCLUDE

#include <string>
#include <json/json.h>

#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddSubMenu.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AddSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Alert.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AlertResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AlertManeuver.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/AlertManeuverResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ChangeRegistration.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ChangeRegistrationResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/CreateInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/CreateInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteCommandResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteInteractionChoiceSet.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteInteractionChoiceSetResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteSubMenu.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DeleteSubMenuResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DialNumber.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/DialNumberResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/EndAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/EndAudioPassThruResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetCapabilities.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetCapabilitiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetLanguage.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetLanguageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetSupportedLanguages.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/GetSupportedLanguagesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnCommand.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnDeviceChosen.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnDriverDistraction.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnLanguageChange.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnReady.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnSystemContext.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/OnTBTClientState.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformAudioPassThru.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformAudioPassThruResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformInteraction.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/PerformInteractionResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ResetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ResetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ScrollableMessage.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ScrollableMessageResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetAppIcon.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetAppIconResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetDisplayLayout.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetDisplayLayoutResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetGlobalProperties.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetGlobalPropertiesResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetMediaClockTimer.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SetMediaClockTimerResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Show.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ShowResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ShowConstantTBT.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/ShowConstantTBTResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/Slider.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/SliderResponse.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/UpdateTurnList.h"
#include "../src/../include/JSONHandler/RPC2Objects//NsRPC2Communication/UI/UpdateTurnListResponse.h"

#include "../src/../include/JSONHandler/RPC2Objects/Marshaller.h"


/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Jan 24 06:41:15 2013
  source stamp	Wed Jan 23 13:56:28 2013
  author	RC
*/


#endif
