#ifndef RPC2MARSHALLER_CLASS
#define RPC2MARSHALLER_CLASS

#include <json/value.h>
#include "JSONHandler/RPC2Command.h"
#include "JSONHandler/OnButtonEventMarshaller.h"
#include "JSONHandler/SpeakMarshaller.h"
#include "JSONHandler/SpeakResponseMarshaller.h"
#include "JSONHandler/AlertMarshaller.h"
#include "JSONHandler/AlertResponseMarshaller.h"
#include "JSONHandler/ShowMarshaller.h"
#include "JSONHandler/ShowResponseMarshaller.h"
#include "JSONHandler/GetCapabilitiesMarshaller.h"
#include "JSONHandler/GetCapabilitiesResponseMarshaller.h"
#include "JSONHandler/OnButtonPressMarshaller.h"
#include "JSONHandler/SetGlobalPropertiesMarshaller.h"
#include "JSONHandler/SetGlobalPropertiesResponseMarshaller.h"
#include "JSONHandler/ResetGlobalPropertiesMarshaller.h"
#include "JSONHandler/ResetGlobalPropertiesResponseMarshaller.h"
#include "JSONHandler/ResetGlobalPropertiesResponse.h"
#include "JSONHandler/OnAppRegisteredMarshaller.h"
#include "JSONHandler/OnAppUnregisteredMarshaller.h"
#include "JSONHandler/ActivateAppMarshaller.h"
#include "JSONHandler/ActivateAppResponseMarshaller.h"
#include "JSONHandler/AddCommandMarshaller.h"
#include "JSONHandler/AddCommandResponseMarshaller.h"
#include "JSONHandler/DeleteCommandMarshaller.h"
#include "JSONHandler/DeleteCommandResponseMarshaller.h"
#include "JSONHandler/OnCommandMarshaller.h"
#include "JSONHandler/AddSubMenuMarshaller.h"
#include "JSONHandler/AddSubMenuResponseMarshaller.h"
#include "JSONHandler/DeleteSubMenuMarshaller.h"
#include "JSONHandler/DeleteSubMenuResponseMarshaller.h"
#include "JSONHandler/CreateInteractionChoiceSetMarshaller.h"
#include "JSONHandler/CreateInteractionChoiceSetResponseMarshaller.h"
#include "JSONHandler/DeleteInteractionChoiceSetMarshaller.h"
#include "JSONHandler/DeleteInteractionChoiceSetResponseMarshaller.h"
#include "JSONHandler/PerformInteractionMarshaller.h"

namespace RPC2Communication
{
    class RPC2Marshaller
    {
    public:
        enum Methods
        {
        METHOD_INVALID=-1,
        METHOD_ONBUTTONEVENT=0,
        METHOD_SPEAK_REQUEST=1,
        METHOD_SPEAK_RESPONSE=2,
        METHOD_ALERT_REQUEST=3,
        METHOD_ALERT_RESPONSE=4,
        METHOD_SHOW_REQUEST=5,
        METHOD_SHOW_RESPONSE=6,
        METHOD_GET_CAPABILITIES_REQUEST=7,
        METHOD_GET_CAPABILITIES_RESPONSE=8,
        METHOD_ONBUTTONPRESS=9,
        METHOD_SET_GLOBAL_PROPERTIES_REQUEST=10,
        METHOD_SET_GLOBAL_PROPERTIES_RESPONSE=11,
        METHOD_RESET_GLOBAL_PROPERTIES_REQUEST=12,
        METHOD_RESET_GLOBAL_PROPERTIES_RESPONSE=13,
        METHOD_ONAPPREGISTERED=14,
        METHOD_ONAPPUNREDISTERED=15,
        METHOD_ACTIVATEAPP_REQUEST=16,
        METHOD_ACTIVATEAPP_RESPONSE=17,
        METHOD_ADDCOMMAND_REQUEST=18,
        METHOD_ADDCOMMAND_RESPONSE=19,
        METHOD_DELETECOMMAND_REQUEST=20,
        METHOD_DELETECOMMAND_RESPONSE=21,
        METHOD_UIONCOMMAND_NOTIFICATION=22,
        METHOD_ADDSUBMENU_REQUEST=23,
        METHOD_ADDSUBMENU_RESPONSE=24,
        METHOD_DELETESUBMENU_REQUEST=25,
        METHOD_DELETESUBMENU_RESPONSE=26,
        METHOD_CREATEINTERACTIONCHOICESET_REQUEST=27,
        METHOD_CREATEINTERACTIONCHOICESET_RESPONSE=28,
        METHOD_DELETEINTERACTIONCHOICESET_REQUEST=29,
        METHOD_DELETEINTERACTIONCHOICESET_RESPONSE=30,
        METHOD_PERFORMINTERACTION_REQUEST=31,
        METHOD_PERFORMINTERACTION_RESPONSE=32
        };

        RPC2Marshaller();
        ~RPC2Marshaller();

        static RPC2Command* fromJSON(const Json::Value&, const std::string & methodName = "");
        static Json::Value toJSON(const RPC2Command* msg);

        static RPC2Command* fromString(const std::string&);
        static std::string toString(const RPC2Command* msg);

        static std::string toString(const OnButtonEvent& res)  
        {
            return mOnButtonEventMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, OnButtonEvent& res) 
        {
            return mOnButtonEventMarshaller.fromString(str, res);
        }

        static std::string toString(const Speak& res) 
        {
            return mSpeakMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, Speak& res) 
        {
            return mSpeakMarshaller.fromString(str, res);
        }

        static std::string toString(const SpeakResponse & res)
        {
            return mSpeakResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string&str, SpeakResponse& res)
        {
            return mSpeakResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const Alert & res)
        {
            return mAlertMarshaller.toString(res);
        }

        static bool fromString(const std::string&str, Alert& res)
        {
            return mAlertMarshaller.fromString(str, res);
        }

        static bool fromString(const std::string&str, AlertResponse& res)
        {
            return mAlertResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const AlertResponse& res)
        {
            return mAlertResponseMarshaller.toString(res);
        }

        static std::string toString(const Show & res)
        {
            return mShowMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, Show &res)
        {
            return mShowMarshaller.fromString(str, res);
        }

        static bool fromString(const std::string& str, ShowResponse& res)
        {
            return mShowResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const ShowResponse& res)
        {
            return mShowResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, GetCapabilities&res)
        {
            return mGetCapabilitiesMarshaller.fromString(str, res);
        }

        static std::string toString(const GetCapabilities& res)
        {
            return mGetCapabilitiesMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, GetCapabilitiesResponse& res)
        {
            return mGetCapabilitiesResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const GetCapabilitiesResponse& res)
        {
            return mGetCapabilitiesResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string &str, OnButtonPress&res)
        {
            return mOnButtonPressMarshaller.fromString(str, res);
        }

        static std::string toString(const OnButtonPress& res)
        {
            return mOnButtonPressMarshaller.toString(res);
        }

        static bool fromString(const std::string &str, SetGlobalProperties&res)
        {
            return mSetGlobalPropertiesMarshaller.fromString(str, res);
        }

        static std::string toString(const SetGlobalProperties& res)
        {
            return mSetGlobalPropertiesMarshaller.toString(res);
        }

        static bool fromString(const std::string &str, SetGlobalPropertiesResponse& res)
        {
            return mSetGlobalPropertiesResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const SetGlobalPropertiesResponse& res)
        {
            return mSetGlobalPropertiesResponseMarshaller.toString(res);
        }

        static bool fromString( const std::string& str, ResetGlobalProperties& res )
        {
            return mResetGlobalPropertiesMarshaller.fromString(str, res);
        }

        static std::string toString(const ResetGlobalProperties& res)
        {
            return mResetGlobalPropertiesMarshaller.toString(res);
        }

        static bool fromString( const std::string & str, ResetGlobalPropertiesResponse& res )
        {
            return mResetGlobalPropertiesResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const ResetGlobalPropertiesResponse& res)
        {
            return mResetGlobalPropertiesResponseMarshaller.toString(res);
        }

        static bool fromString( const std::string & str, OnAppRegistered& res )
        {
            return mOnAppRegisteredMarshaller.fromString(str, res);
        }

        static std::string toString(const OnAppRegistered& res)
        {
            return mOnAppRegisteredMarshaller.toString(res);
        }

        static bool fromString( const std::string& str, OnAppUnregistered& res )
        {
            return mOnAppUnregisteredMarshaller.fromString(str, res);
        }

        static std::string toString(const OnAppUnregistered& res)
        {
            return mOnAppUnregisteredMarshaller.toString(res);
        }

        static bool fromString( const std::string & str, ActivateApp& res )
        {
            return mActivateAppMarshaller.fromString(str, res);
        }

        static std::string toString(const ActivateApp& res)
        {
            return mActivateAppMarshaller.toString(res);
        } 

        static bool fromString(const std::string& str, ActivateAppResponse& res)
        {
            return mActivateAppResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const ActivateAppResponse& res)
        {
            return mActivateAppResponseMarshaller.toString(res);
        }
        static bool fromString(const std::string& str, AddCommand&res)
        {
            return mAddCommandMarshaller.fromString(str, res);
        }
        static std::string toString(const AddCommand& res)
        {
            return mAddCommandMarshaller.toString(res);
        }
        static bool fromString(const std::string& str, AddCommandResponse&res)
        {
            return mAddCommandResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const AddCommandResponse& res)
        {
            return mAddCommandResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string &str, DeleteCommand& res)
        {
            return mDeleteCommandMarshaller.fromString(str, res);
        }

        static std::string toString(const DeleteCommand& res)
        {
            return mDeleteCommandMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, DeleteCommandResponse& res)
        {
            return mDeleteCommandResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const DeleteCommandResponse& res)
        {
            return mDeleteCommandResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, OnCommand& res)
        {
            return mOnCommandMarshaller.fromString(str, res);
        }

        static std::string toString(const OnCommand& res)
        {
            return mOnCommandMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, AddSubMenu& res)
        {
            return mAddSubMenuMarshaller.fromString(str, res);
        }

        static std::string toString(const AddSubMenu&res)
        {
            return mAddSubMenuMarshaller.toString(res);
        }

        static bool fromString(const std::string &str, AddSubMenuResponse& res)
        {
            return mAddSubMenuResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const AddSubMenuResponse& res)
        {
            return mAddSubMenuResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string &str, DeleteSubMenu& res)
        {
            return mDeleteSubMenuMarshaller.fromString(str, res);
        }

        static std::string toString(const DeleteSubMenu& res)
        {
            return mDeleteSubMenuMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, DeleteSubMenuResponse& res)
        {
            return mDeleteSubMenuResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const DeleteSubMenuResponse& res)
        {
            return mDeleteSubMenuResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, CreateInteractionChoiceSet& res)
        {
            return mCreateInteractionChoiceSetMarshaller.fromString(str, res);
        }

        static std::string toString(const CreateInteractionChoiceSet& res)
        {
            return mCreateInteractionChoiceSetMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, CreateInteractionChoiceSetResponse& res)
        {
            return mCreateInteractionChoiceSetResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const CreateInteractionChoiceSetResponse& res)
        {
            return mCreateInteractionChoiceSetResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string &str, DeleteInteractionChoiceSet&res)
        {
            return mDeleteInteractionChoiceSetMarshaller.fromString(str, res);
        }

        static std::string toString(const DeleteInteractionChoiceSet& res)
        {
            return mDeleteInteractionChoiceSetMarshaller.toString(res);
        }

        static bool fromString(const std::string & str, DeleteInteractionChoiceSetResponse& res)
        {
            return mDeleteInteractionChoiceSetResponseMarshaller.fromString(str, res);
        }

        static std::string toString(const DeleteInteractionChoiceSetResponse& res)
        {
            return mDeleteInteractionChoiceSetResponseMarshaller.toString(res);
        }

        static bool fromString(const std::string& str, PerformInteraction& res)
        {
            return mPerformInteractionMarshaller.fromString(str, res);
        }

        static std::string toString(const PerformInteraction&res)
        {
            return mPerformInteractionMarshaller.toString(res);
        }

    private:
        struct localHash
        {
            const char *name;
            unsigned int idx;
            void* marshaller;
        };

        static const char* getName(Methods e)
        {
            return (e>=0 && e<4) ? mHashTable[e].name : NULL;
        }

        static const Methods getIndex(const std::string & s);
        static const Methods getResponseIndex(const std::string & s);

        static const localHash mHashTable[3];

        static OnButtonEventMarshaller mOnButtonEventMarshaller;

        static SpeakMarshaller mSpeakMarshaller;

        static SpeakResponseMarshaller mSpeakResponseMarshaller;

        static AlertMarshaller mAlertMarshaller;
        static AlertResponseMarshaller mAlertResponseMarshaller;
        static ShowMarshaller mShowMarshaller;
        static ShowResponseMarshaller mShowResponseMarshaller;
        static GetCapabilitiesMarshaller mGetCapabilitiesMarshaller;
        static GetCapabilitiesResponseMarshaller mGetCapabilitiesResponseMarshaller;
        static OnButtonPressMarshaller mOnButtonPressMarshaller;
        static SetGlobalPropertiesMarshaller mSetGlobalPropertiesMarshaller;
        static SetGlobalPropertiesResponseMarshaller mSetGlobalPropertiesResponseMarshaller;
        static ResetGlobalPropertiesMarshaller mResetGlobalPropertiesMarshaller;
        static ResetGlobalPropertiesResponseMarshaller mResetGlobalPropertiesResponseMarshaller;
        static OnAppRegisteredMarshaller mOnAppRegisteredMarshaller;
        static OnAppUnregisteredMarshaller mOnAppUnregisteredMarshaller;
        static ActivateAppMarshaller mActivateAppMarshaller;
        static ActivateAppResponseMarshaller mActivateAppResponseMarshaller;
        static AddCommandMarshaller mAddCommandMarshaller;
        static AddCommandResponseMarshaller mAddCommandResponseMarshaller;
        static DeleteCommandMarshaller mDeleteCommandMarshaller;
        static DeleteCommandResponseMarshaller mDeleteCommandResponseMarshaller;
        static OnCommandMarshaller mOnCommandMarshaller;
        static AddSubMenuMarshaller mAddSubMenuMarshaller;
        static AddSubMenuResponseMarshaller mAddSubMenuResponseMarshaller;
        static DeleteSubMenuMarshaller mDeleteSubMenuMarshaller;
        static DeleteSubMenuResponseMarshaller mDeleteSubMenuResponseMarshaller;
        static CreateInteractionChoiceSetMarshaller mCreateInteractionChoiceSetMarshaller;
        static CreateInteractionChoiceSetResponseMarshaller mCreateInteractionChoiceSetResponseMarshaller;
        static DeleteInteractionChoiceSetMarshaller mDeleteInteractionChoiceSetMarshaller;
        static DeleteInteractionChoiceSetResponseMarshaller mDeleteInteractionChoiceSetResponseMarshaller;
        static PerformInteractionMarshaller mPerformInteractionMarshaller;
    };

}

#endif