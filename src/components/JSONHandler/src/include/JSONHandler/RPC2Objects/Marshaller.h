#ifndef MARSHALLER_INCLUDE
#define MARSHALLER_INCLUDE

#include <string>
#include <json/json.h>

#include "JSONHandler/RPC2Error.h"
#include "JSONHandler/RPC2Notification.h"
#include "JSONHandler/RPC2Request.h"
#include "JSONHandler/RPC2Response.h"

#include "JSONHandler/RPC2ErrorMarshaller.h"

#include "../src/src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/SendDataMarshaller.h"
#include "../src/src/RPC2ObjectsImpl//NsRPC2Communication/AppLinkCore/SendDataResponseMarshaller.h"

/*
  generated at	Fri Dec 14 12:58:14 2012
  source stamp	Fri Dec 14 12:58:07 2012
  author	robok0der
*/


class Marshaller
{
public:

  enum Methods
  {
    METHOD_INVALID=-1,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA,
    METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE
  };


  Marshaller()			{}
  ~Marshaller()			{}
  
  static ::NsRPC2Communication::RPC2Command* fromString(const std::string&);
  static std::string toString(const ::NsRPC2Communication::RPC2Command* msg);
  
  static ::NsRPC2Communication::RPC2Command* fromJSON(const Json::Value&);
  static Json::Value toJSON(const ::NsRPC2Communication::RPC2Command* msg);
  
protected:

// NsRPC2Communication::AppLinkCore::SendData
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::SendData& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendData.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::SendData& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendData.toString(res);
  }

// NsRPC2Communication::AppLinkCore::SendDataResponse
  static bool fromString(const std::string& str, NsRPC2Communication::AppLinkCore::SendDataResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendDataResponse.fromString(str, res);
  }
  
  static std::string toString(const NsRPC2Communication::AppLinkCore::SendDataResponse& res)
  {
    return mNsRPC2Communication_AppLinkCore__SendDataResponse.toString(res);
  }


  struct localHash
  {
    const char *name;
    unsigned int idx;
    void* marshaller;					// Bingo!!!  old good plain C with direct casting!!!
  };
  
private:
  
  static Json::Value Request2JSON(const ::NsRPC2Communication::RPC2Request* msg);
  static Json::Value Response2JSON(const ::NsRPC2Communication::RPC2Response* msg);
  static Json::Value Notification2JSON(const ::NsRPC2Communication::RPC2Notification* msg);
  
  static const char* getName(Methods e)
  {
    return (e>=0 && e<2) ? mHashTable[e].name : NULL;
  }
  
  static const Methods getIndex(const char* s);
  
  static const localHash mHashTable[2];
  friend class Marshaller_intHash;
  
  static ::NsRPC2Communication::RPC2ErrorMarshaller mRPC2ErrorInternal;

  static NsRPC2Communication::AppLinkCore::SendDataMarshaller mNsRPC2Communication_AppLinkCore__SendData;
  static NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller mNsRPC2Communication_AppLinkCore__SendDataResponse;

};

#endif
