#ifndef RPC2COMMUNICATION_APPLINKCORE_MARSHALLER_INCLUDE
#define RPC2COMMUNICATION_APPLINKCORE_MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Error.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/RPC2ErrorMarshaller.h"

#include "../src/RPC2ObjectsImpl/AppLinkCore/ActivateAppMarshaller.h"
#include "../src/RPC2ObjectsImpl/AppLinkCore/ActivateAppResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/AppLinkCore/OnAppRegisteredMarshaller.h"
#include "../src/RPC2ObjectsImpl/AppLinkCore/OnAppUnregisteredMarshaller.h"

/*
  interface	RPC2Communication::AppLinkCore
  version	1.2
  generated at	Wed Nov  7 11:25:43 2012
  source stamp	Wed Nov  7 09:35:35 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace AppLinkCore
  {

    class Marshaller
    {
    public:
    
      enum Methods
      {
        METHOD_INVALID=-1,
        METHOD_ACTIVATEAPP=0,
        METHOD_ACTIVATEAPPRESPONSE=1,
        METHOD_ONAPPREGISTERED=2,
        METHOD_ONAPPUNREGISTERED=3
      };


      Marshaller()			{}
      ~Marshaller()			{}
      
      static ::RPC2Communication::RPC2Command* fromString(const std::string&);
      static std::string toString(const ::RPC2Communication::RPC2Command* msg);
      
      static ::RPC2Communication::RPC2Command* fromJSON(const Json::Value&);
      static Json::Value toJSON(const ::RPC2Communication::RPC2Command* msg);
      
    protected:

// ActivateApp
      static bool fromString(const std::string& str, ActivateApp& res)
      {
        return mActivateApp.fromString(str, res);
      }
      
      static std::string toString(const ActivateApp& res)
      {
        return mActivateApp.toString(res);
      }

// ActivateAppResponse
      static bool fromString(const std::string& str, ActivateAppResponse& res)
      {
        return mActivateAppResponse.fromString(str, res);
      }
      
      static std::string toString(const ActivateAppResponse& res)
      {
        return mActivateAppResponse.toString(res);
      }

// OnAppRegistered
      static bool fromString(const std::string& str, OnAppRegistered& res)
      {
        return mOnAppRegistered.fromString(str, res);
      }
      
      static std::string toString(const OnAppRegistered& res)
      {
        return mOnAppRegistered.toString(res);
      }

// OnAppUnregistered
      static bool fromString(const std::string& str, OnAppUnregistered& res)
      {
        return mOnAppUnregistered.fromString(str, res);
      }
      
      static std::string toString(const OnAppUnregistered& res)
      {
        return mOnAppUnregistered.toString(res);
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

      static ActivateAppMarshaller mActivateApp;
      static ActivateAppResponseMarshaller mActivateAppResponse;
      static OnAppRegisteredMarshaller mOnAppRegistered;
      static OnAppUnregisteredMarshaller mOnAppUnregistered;

    };
  }
}

#endif
