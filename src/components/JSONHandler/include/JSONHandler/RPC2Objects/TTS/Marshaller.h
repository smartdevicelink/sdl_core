#ifndef RPC2COMMUNICATION_TTS_MARSHALLER_INCLUDE
#define RPC2COMMUNICATION_TTS_MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Error.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/RPC2ErrorMarshaller.h"

#include "../src/RPC2ObjectsImpl/TTS/GetCapabilitiesMarshaller.h"
#include "../src/RPC2ObjectsImpl/TTS/GetCapabilitiesResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/TTS/SpeakMarshaller.h"
#include "../src/RPC2ObjectsImpl/TTS/SpeakResponseMarshaller.h"

/*
  interface	RPC2Communication::TTS
  version	1.2
  generated at	Wed Nov  7 11:25:55 2012
  source stamp	Wed Nov  7 09:34:03 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace TTS
  {

    class Marshaller
    {
    public:
    
      enum Methods
      {
        METHOD_INVALID=-1,
        METHOD_GETCAPABILITIES=8,
        METHOD_GETCAPABILITIESRESPONSE=9,
        METHOD_SPEAK=10,
        METHOD_SPEAKRESPONSE=11
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

// Speak
      static bool fromString(const std::string& str, Speak& res)
      {
        return mSpeak.fromString(str, res);
      }
      
      static std::string toString(const Speak& res)
      {
        return mSpeak.toString(res);
      }

// SpeakResponse
      static bool fromString(const std::string& str, SpeakResponse& res)
      {
        return mSpeakResponse.fromString(str, res);
      }
      
      static std::string toString(const SpeakResponse& res)
      {
        return mSpeakResponse.toString(res);
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
      static SpeakMarshaller mSpeak;
      static SpeakResponseMarshaller mSpeakResponse;

    };
  }
}

#endif
