#ifndef RPC2COMMUNICATION_BUTTONS_MARSHALLER_INCLUDE
#define RPC2COMMUNICATION_BUTTONS_MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Error.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/RPC2ErrorMarshaller.h"

#include "../src/RPC2ObjectsImpl/Buttons/GetCapabilitiesMarshaller.h"
#include "../src/RPC2ObjectsImpl/Buttons/GetCapabilitiesResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/Buttons/OnButtonEventMarshaller.h"
#include "../src/RPC2ObjectsImpl/Buttons/OnButtonPressMarshaller.h"

/*
  interface	RPC2Communication::Buttons
  version	1.2
  generated at	Wed Nov  7 11:25:50 2012
  source stamp	Wed Nov  7 09:31:20 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace Buttons
  {

    class Marshaller
    {
    public:
    
      enum Methods
      {
        METHOD_INVALID=-1,
        METHOD_GETCAPABILITIES,
        METHOD_GETCAPABILITIESRESPONSE,
        METHOD_ONBUTTONEVENT,
        METHOD_ONBUTTONPRESS
      };


      Marshaller()			{}
      ~Marshaller()			{}
      
      static ::RPC2Communication::RPC2Command* fromString(const std::string&);
      static std::string toString(const ::RPC2Communication::RPC2Command* msg);
      
      static ::RPC2Communication::RPC2Command* fromJSON(const Json::Value&);
      static Json::Value toJSON(const ::RPC2Communication::RPC2Command* msg);
      
    protected:

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

// OnButtonEvent
      static bool fromString(const std::string& str, OnButtonEvent& res)
      {
        return mOnButtonEvent.fromString(str, res);
      }
      
      static std::string toString(const OnButtonEvent& res)
      {
        return mOnButtonEvent.toString(res);
      }

// OnButtonPress
      static bool fromString(const std::string& str, OnButtonPress& res)
      {
        return mOnButtonPress.fromString(str, res);
      }
      
      static std::string toString(const OnButtonPress& res)
      {
        return mOnButtonPress.toString(res);
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
        return (e>=0 && e<4) ? mHashTable[e].name : NULL;
      }
      
      static const Methods getIndex(const char* s);
      
      static const localHash mHashTable[4];
      friend class Marshaller_intHash;
      
      static ::RPC2Communication::RPC2ErrorMarshaller mRPC2ErrorInternal;

      static GetCapabilitiesMarshaller mGetCapabilities;
      static GetCapabilitiesResponseMarshaller mGetCapabilitiesResponse;
      static OnButtonEventMarshaller mOnButtonEvent;
      static OnButtonPressMarshaller mOnButtonPress;

    };
  }
}

#endif
