#ifndef RPC2COMMUNICATION_UI_MARSHALLER_INCLUDE
#define RPC2COMMUNICATION_UI_MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Error.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/RPC2ErrorMarshaller.h"

#include "../src/RPC2ObjectsImpl/UI/AddCommandMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/AddCommandResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/AddSubMenuMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/AddSubMenuResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/AlertMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/AlertResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/CreateInteractionChoiceSetMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/CreateInteractionChoiceSetResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/DeleteCommandMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/DeleteCommandResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/DeleteInteractionChoiceSetMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/DeleteInteractionChoiceSetResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/DeleteSubMenuMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/DeleteSubMenuResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/GetCapabilitiesMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/GetCapabilitiesResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/OnAppActivatedMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/OnAppDeactivatedMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/OnCommandMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/OnSystemContextMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/PerformInteractionMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/PerformInteractionResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/ResetGlobalPropertiesMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/ResetGlobalPropertiesResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/SetGlobalPropertiesMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/SetGlobalPropertiesResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/SetMediaClockTimerMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/SetMediaClockTimerResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/ShowMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/ShowResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/OnReadyMarshaller.h"
#include "../src/RPC2ObjectsImpl/UI/OnDriverDistractionMarshaller.h"

/*
  interface	RPC2Communication::UI
  version	1.2
  generated at	Wed Nov  7 11:26:14 2012
  source stamp	Wed Nov  7 09:29:07 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace UI
  {

    class Marshaller
    {
    public:
    
      enum Methods
      {
        METHOD_INVALID=-1,
        METHOD_ADDCOMMAND=12,
        METHOD_ADDCOMMANDRESPONSE=13,
        METHOD_ADDSUBMENU=14,
        METHOD_ADDSUBMENURESPONSE=15,
        METHOD_ALERT=16,
        METHOD_ALERTRESPONSE=17,
        METHOD_CREATEINTERACTIONCHOICESET=18,
        METHOD_CREATEINTERACTIONCHOICESETRESPONSE=19,
        METHOD_DELETECOMMAND=20,
        METHOD_DELETECOMMANDRESPONSE=21,
        METHOD_DELETEINTERACTIONCHOICESET=22,
        METHOD_DELETEINTERACTIONCHOICESETRESPONSE=23,
        METHOD_DELETESUBMENU=24,
        METHOD_DELETESUBMENURESPONSE=25,
        METHOD_GETCAPABILITIES=26,
        METHOD_GETCAPABILITIESRESPONSE=27,
        METHOD_ONAPPACTIVATED=28,
        METHOD_ONAPPDEACTIVATED=29,
        METHOD_ONCOMMAND=30,
        METHOD_ONSYSTEMCONTEXT=31,
        METHOD_PERFORMINTERACTION=32,
        METHOD_PERFORMINTERACTIONRESPONSE=33,
        METHOD_RESETGLOBALPROPERTIES=34,
        METHOD_RESETGLOBALPROPERTIESRESPONSE=35,
        METHOD_SETGLOBALPROPERTIES=36,
        METHOD_SETGLOBALPROPERTIESRESPONSE=37,
        METHOD_SETMEDIACLOCKTIMER=38,
        METHOD_SETMEDIACLOCKTIMERRESPONSE=39,
        METHOD_SHOW=40,
        METHOD_SHOWRESPONSE=41,
        METHOD_ONREADY=50,
        METHOD_ONDRIVERDISTRACTION=51
      };


      Marshaller()			{}
      ~Marshaller()			{}
      
      static ::RPC2Communication::RPC2Command* fromString(const std::string&);
      static std::string toString(const ::RPC2Communication::RPC2Command* msg);
      
      static ::RPC2Communication::RPC2Command* fromJSON(const Json::Value&);
      static Json::Value toJSON(const ::RPC2Communication::RPC2Command* msg);
      
    protected:

// AddCommand
      static bool fromString(const std::string& str, AddCommand& res)
      {
        return mAddCommand.fromString(str, res);
      }
      
      static std::string toString(const AddCommand& res)
      {
        return mAddCommand.toString(res);
      }

// AddCommandResponse
      static bool fromString(const std::string& str, AddCommandResponse& res)
      {
        return mAddCommandResponse.fromString(str, res);
      }
      
      static std::string toString(const AddCommandResponse& res)
      {
        return mAddCommandResponse.toString(res);
      }

// AddSubMenu
      static bool fromString(const std::string& str, AddSubMenu& res)
      {
        return mAddSubMenu.fromString(str, res);
      }
      
      static std::string toString(const AddSubMenu& res)
      {
        return mAddSubMenu.toString(res);
      }

// AddSubMenuResponse
      static bool fromString(const std::string& str, AddSubMenuResponse& res)
      {
        return mAddSubMenuResponse.fromString(str, res);
      }
      
      static std::string toString(const AddSubMenuResponse& res)
      {
        return mAddSubMenuResponse.toString(res);
      }

// Alert
      static bool fromString(const std::string& str, Alert& res)
      {
        return mAlert.fromString(str, res);
      }
      
      static std::string toString(const Alert& res)
      {
        return mAlert.toString(res);
      }

// AlertResponse
      static bool fromString(const std::string& str, AlertResponse& res)
      {
        return mAlertResponse.fromString(str, res);
      }
      
      static std::string toString(const AlertResponse& res)
      {
        return mAlertResponse.toString(res);
      }

// CreateInteractionChoiceSet
      static bool fromString(const std::string& str, CreateInteractionChoiceSet& res)
      {
        return mCreateInteractionChoiceSet.fromString(str, res);
      }
      
      static std::string toString(const CreateInteractionChoiceSet& res)
      {
        return mCreateInteractionChoiceSet.toString(res);
      }

// CreateInteractionChoiceSetResponse
      static bool fromString(const std::string& str, CreateInteractionChoiceSetResponse& res)
      {
        return mCreateInteractionChoiceSetResponse.fromString(str, res);
      }
      
      static std::string toString(const CreateInteractionChoiceSetResponse& res)
      {
        return mCreateInteractionChoiceSetResponse.toString(res);
      }

// DeleteCommand
      static bool fromString(const std::string& str, DeleteCommand& res)
      {
        return mDeleteCommand.fromString(str, res);
      }
      
      static std::string toString(const DeleteCommand& res)
      {
        return mDeleteCommand.toString(res);
      }

// DeleteCommandResponse
      static bool fromString(const std::string& str, DeleteCommandResponse& res)
      {
        return mDeleteCommandResponse.fromString(str, res);
      }
      
      static std::string toString(const DeleteCommandResponse& res)
      {
        return mDeleteCommandResponse.toString(res);
      }

// DeleteInteractionChoiceSet
      static bool fromString(const std::string& str, DeleteInteractionChoiceSet& res)
      {
        return mDeleteInteractionChoiceSet.fromString(str, res);
      }
      
      static std::string toString(const DeleteInteractionChoiceSet& res)
      {
        return mDeleteInteractionChoiceSet.toString(res);
      }

// DeleteInteractionChoiceSetResponse
      static bool fromString(const std::string& str, DeleteInteractionChoiceSetResponse& res)
      {
        return mDeleteInteractionChoiceSetResponse.fromString(str, res);
      }
      
      static std::string toString(const DeleteInteractionChoiceSetResponse& res)
      {
        return mDeleteInteractionChoiceSetResponse.toString(res);
      }

// DeleteSubMenu
      static bool fromString(const std::string& str, DeleteSubMenu& res)
      {
        return mDeleteSubMenu.fromString(str, res);
      }
      
      static std::string toString(const DeleteSubMenu& res)
      {
        return mDeleteSubMenu.toString(res);
      }

// DeleteSubMenuResponse
      static bool fromString(const std::string& str, DeleteSubMenuResponse& res)
      {
        return mDeleteSubMenuResponse.fromString(str, res);
      }
      
      static std::string toString(const DeleteSubMenuResponse& res)
      {
        return mDeleteSubMenuResponse.toString(res);
      }

// GetCapabilities
      static bool fromString(const std::string& str, GetCapabilities& res)
      {
        return mGetCapabilities.fromString(str, res);
      }
      
      static std::string toString(const GetCapabilities& res)
      {
        return mGetCapabilities.toString(res);
      }

// GetCapabilitiesResponse
      static bool fromString(const std::string& str, GetCapabilitiesResponse& res)
      {
        return mGetCapabilitiesResponse.fromString(str, res);
      }
      
      static std::string toString(const GetCapabilitiesResponse& res)
      {
        return mGetCapabilitiesResponse.toString(res);
      }

// OnAppActivated
      static bool fromString(const std::string& str, OnAppActivated& res)
      {
        return mOnAppActivated.fromString(str, res);
      }
      
      static std::string toString(const OnAppActivated& res)
      {
        return mOnAppActivated.toString(res);
      }

// OnAppDeactivated
      static bool fromString(const std::string& str, OnAppDeactivated& res)
      {
        return mOnAppDeactivated.fromString(str, res);
      }
      
      static std::string toString(const OnAppDeactivated& res)
      {
        return mOnAppDeactivated.toString(res);
      }

// OnCommand
      static bool fromString(const std::string& str, OnCommand& res)
      {
        return mOnCommand.fromString(str, res);
      }
      
      static std::string toString(const OnCommand& res)
      {
        return mOnCommand.toString(res);
      }

// OnSystemContext
      static bool fromString(const std::string& str, OnSystemContext& res)
      {
        return mOnSystemContext.fromString(str, res);
      }
      
      static std::string toString(const OnSystemContext& res)
      {
        return mOnSystemContext.toString(res);
      }

// PerformInteraction
      static bool fromString(const std::string& str, PerformInteraction& res)
      {
        return mPerformInteraction.fromString(str, res);
      }
      
      static std::string toString(const PerformInteraction& res)
      {
        return mPerformInteraction.toString(res);
      }

// PerformInteractionResponse
      static bool fromString(const std::string& str, PerformInteractionResponse& res)
      {
        return mPerformInteractionResponse.fromString(str, res);
      }
      
      static std::string toString(const PerformInteractionResponse& res)
      {
        return mPerformInteractionResponse.toString(res);
      }

// ResetGlobalProperties
      static bool fromString(const std::string& str, ResetGlobalProperties& res)
      {
        return mResetGlobalProperties.fromString(str, res);
      }
      
      static std::string toString(const ResetGlobalProperties& res)
      {
        return mResetGlobalProperties.toString(res);
      }

// ResetGlobalPropertiesResponse
      static bool fromString(const std::string& str, ResetGlobalPropertiesResponse& res)
      {
        return mResetGlobalPropertiesResponse.fromString(str, res);
      }
      
      static std::string toString(const ResetGlobalPropertiesResponse& res)
      {
        return mResetGlobalPropertiesResponse.toString(res);
      }

// SetGlobalProperties
      static bool fromString(const std::string& str, SetGlobalProperties& res)
      {
        return mSetGlobalProperties.fromString(str, res);
      }
      
      static std::string toString(const SetGlobalProperties& res)
      {
        return mSetGlobalProperties.toString(res);
      }

// SetGlobalPropertiesResponse
      static bool fromString(const std::string& str, SetGlobalPropertiesResponse& res)
      {
        return mSetGlobalPropertiesResponse.fromString(str, res);
      }
      
      static std::string toString(const SetGlobalPropertiesResponse& res)
      {
        return mSetGlobalPropertiesResponse.toString(res);
      }

// SetMediaClockTimer
      static bool fromString(const std::string& str, SetMediaClockTimer& res)
      {
        return mSetMediaClockTimer.fromString(str, res);
      }
      
      static std::string toString(const SetMediaClockTimer& res)
      {
        return mSetMediaClockTimer.toString(res);
      }

// SetMediaClockTimerResponse
      static bool fromString(const std::string& str, SetMediaClockTimerResponse& res)
      {
        return mSetMediaClockTimerResponse.fromString(str, res);
      }
      
      static std::string toString(const SetMediaClockTimerResponse& res)
      {
        return mSetMediaClockTimerResponse.toString(res);
      }

// Show
      static bool fromString(const std::string& str, Show& res)
      {
        return mShow.fromString(str, res);
      }
      
      static std::string toString(const Show& res)
      {
        return mShow.toString(res);
      }

// ShowResponse
      static bool fromString(const std::string& str, ShowResponse& res)
      {
        return mShowResponse.fromString(str, res);
      }
      
      static std::string toString(const ShowResponse& res)
      {
        return mShowResponse.toString(res);
      }

// OnReady
      static bool fromString(const std::string& str, OnReady&res)
      {
        return mOnReady.fromString(str, res);
      }

      static std::string toString(const OnReady& res)
      {
        return mOnReady.toString(res);
      }

// OnDriverDistraction
      static bool fromString(const std::string& str, OnDriverDistraction&res)
      {
        return mOnDriverDistraction.fromString(str, res);
      }

      static std::string toString(const OnDriverDistraction& res)
      {
        return mOnDriverDistraction.toString(res);
      }


      struct localHash
      {
        const char *name;
        unsigned int idx;
        void* marshaller;					// Bingo!!!  old good plain C with direct casting!!!
      };
      
    private:
      
      static Json::Value Request2JSON(const ::RPC2Communication::RPC2Request* msg);
      static Json::Value Response2JSON(const ::RPC2Communication::RPC2Response* msg);
      static Json::Value Notification2JSON(const ::RPC2Communication::RPC2Notification* msg);
      
      static const char* getName(Methods e)
      {
        return (e>=0 && e<31) ? mHashTable[e].name : NULL;
      }
      
      static const Methods getIndex(const char* s);
      
      static const localHash mHashTable[31];
      friend class Marshaller_intHash;
      
      static ::RPC2Communication::RPC2ErrorMarshaller mRPC2ErrorInternal;

      static AddCommandMarshaller mAddCommand;
      static AddCommandResponseMarshaller mAddCommandResponse;
      static AddSubMenuMarshaller mAddSubMenu;
      static AddSubMenuResponseMarshaller mAddSubMenuResponse;
      static AlertMarshaller mAlert;
      static AlertResponseMarshaller mAlertResponse;
      static CreateInteractionChoiceSetMarshaller mCreateInteractionChoiceSet;
      static CreateInteractionChoiceSetResponseMarshaller mCreateInteractionChoiceSetResponse;
      static DeleteCommandMarshaller mDeleteCommand;
      static DeleteCommandResponseMarshaller mDeleteCommandResponse;
      static DeleteInteractionChoiceSetMarshaller mDeleteInteractionChoiceSet;
      static DeleteInteractionChoiceSetResponseMarshaller mDeleteInteractionChoiceSetResponse;
      static DeleteSubMenuMarshaller mDeleteSubMenu;
      static DeleteSubMenuResponseMarshaller mDeleteSubMenuResponse;
      static GetCapabilitiesMarshaller mGetCapabilities;
      static GetCapabilitiesResponseMarshaller mGetCapabilitiesResponse;
      static OnAppActivatedMarshaller mOnAppActivated;
      static OnAppDeactivatedMarshaller mOnAppDeactivated;
      static OnCommandMarshaller mOnCommand;
      static OnSystemContextMarshaller mOnSystemContext;
      static PerformInteractionMarshaller mPerformInteraction;
      static PerformInteractionResponseMarshaller mPerformInteractionResponse;
      static ResetGlobalPropertiesMarshaller mResetGlobalProperties;
      static ResetGlobalPropertiesResponseMarshaller mResetGlobalPropertiesResponse;
      static SetGlobalPropertiesMarshaller mSetGlobalProperties;
      static SetGlobalPropertiesResponseMarshaller mSetGlobalPropertiesResponse;
      static SetMediaClockTimerMarshaller mSetMediaClockTimer;
      static SetMediaClockTimerResponseMarshaller mSetMediaClockTimerResponse;
      static ShowMarshaller mShow;
      static ShowResponseMarshaller mShowResponse;
      static OnReadyMarshaller mOnReady;
      static OnDriverDistractionMarshaller mOnDriverDistraction;

    };
  }
}

#endif
