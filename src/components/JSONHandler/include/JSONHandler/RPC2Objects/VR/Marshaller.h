#ifndef RPC2COMMUNICATION_VR_MARSHALLER_INCLUDE
#define RPC2COMMUNICATION_VR_MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Error.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/RPC2ErrorMarshaller.h"

#include "../src/RPC2ObjectsImpl/VR/AddCommandMarshaller.h"
#include "../src/RPC2ObjectsImpl/VR/AddCommandResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/VR/DeleteCommandMarshaller.h"
#include "../src/RPC2ObjectsImpl/VR/DeleteCommandResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/VR/GetCapabilitiesMarshaller.h"
#include "../src/RPC2ObjectsImpl/VR/GetCapabilitiesResponseMarshaller.h"
#include "../src/RPC2ObjectsImpl/VR/OnCommandMarshaller.h"

/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

namespace RPC2Communication
{
  namespace VR
  {

    class Marshaller
    {
    public:
    
      enum Methods
      {
        METHOD_INVALID=-1,
        METHOD_ADDCOMMAND,
        METHOD_ADDCOMMANDRESPONSE,
        METHOD_DELETECOMMAND,
        METHOD_DELETECOMMANDRESPONSE,
        METHOD_GETCAPABILITIES,
        METHOD_GETCAPABILITIESRESPONSE,
        METHOD_ONCOMMAND
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

// OnCommand
      static bool fromString(const std::string& str, OnCommand& res)
      {
        return mOnCommand.fromString(str, res);
      }
      
      static std::string toString(const OnCommand& res)
      {
        return mOnCommand.toString(res);
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
        return (e>=0 && e<7) ? mHashTable[e].name : NULL;
      }
      
      static const Methods getIndex(const char* s);
      
      static const localHash mHashTable[7];
      friend class Marshaller_intHash;
      
      static ::RPC2Communication::RPC2ErrorMarshaller mRPC2ErrorInternal;

      static AddCommandMarshaller mAddCommand;
      static AddCommandResponseMarshaller mAddCommandResponse;
      static DeleteCommandMarshaller mDeleteCommand;
      static DeleteCommandResponseMarshaller mDeleteCommandResponse;
      static GetCapabilitiesMarshaller mGetCapabilities;
      static GetCapabilitiesResponseMarshaller mGetCapabilitiesResponse;
      static OnCommandMarshaller mOnCommand;

    };
  }
}

#endif
