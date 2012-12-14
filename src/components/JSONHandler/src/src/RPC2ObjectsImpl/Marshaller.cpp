#include <cstring>
#include "../src/include/JSONHandler/RPC2Objects/Marshaller.h"

#include "../src/include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/SendData.h"
#include "../src/include/JSONHandler/RPC2Objects//NsRPC2Communication/AppLinkCore/SendDataResponse.h"

#include "../src/src/RPC2ObjectsImpl/Marshaller.inc"

/*
  generated at	Fri Dec 14 12:58:14 2012
  source stamp	Fri Dec 14 12:58:07 2012
  author	robok0der
*/

using namespace NsRPC2Communication;


const Marshaller::Methods Marshaller::getIndex(const char* s)
{
  if(!s)
    return METHOD_INVALID;  const struct localHash* p=Marshaller_intHash::getPointer(s,strlen(s));
  return p ? static_cast<Methods>(p->idx) : METHOD_INVALID;
}

RPC2Command* Marshaller::fromString(const std::string& s)
{
  RPC2Command* rv=0;
  try
  {
    Json::Reader reader;
    Json::Value json;

    if(!reader.parse(s,json,false))  return 0;    if(!(rv=fromJSON(json)))  return 0;
  }
  catch(...)
  {
    return 0;
  }
  return rv;
}

std::string Marshaller::toString(const RPC2Command* msg)
{
  if(!msg)  return "";

  Json::Value json=toJSON(msg);

  if(json.isNull()) return "";

  Json::FastWriter writer;
  std::string rv;
  return writer.write(json);
}

RPC2Command* Marshaller::fromJSON(const Json::Value& json)
{
  if(!json.isObject())  return 0;
  if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return 0;

  if(json.isMember("error"))				// error
  {
    RPC2Error rv;
    if(!RPC2ErrorMarshaller::fromJSON(json,rv))  return 0;

    return new RPC2Error(rv);
  }

  if(!json.isMember("id"))				// notification
  {
    if(!json.isMember("method") || !json["method"].isString())  return 0;

    Methods m=getIndex(json["method"].asString().c_str());

    switch(m)
    {
      default:
        return 0;
    }
    return 0;
  }

  if(json.isMember("method"))				// request
  {
    if(!json["id"].isInt())  return 0;

    Methods m=getIndex(json["method"].asString().c_str());
    switch(m)
    {
      case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA:
      {
        NsRPC2Communication::AppLinkCore::SendData *rv=new NsRPC2Communication::AppLinkCore::SendData;
        return NsRPC2Communication::AppLinkCore::SendDataMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      default:
        return 0;
    }
    return 0;
  }
							// response
  if(!json.isMember("result"))  return 0;

  if(!json["id"].isInt()) return 0;

// here is extension of protocol, two fields added: _Result and _Method
  if(!json["result"].isMember("resultCode") || !json["result"]["resultCode"].isString())  return 0;
  if(!json["result"].isMember("method") || !json["result"]["method"].isString())  return 0;

  Methods m=getIndex(json["result"]["method"].asString().c_str());

  switch(m)
  {
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE:
    {
      NsRPC2Communication::AppLinkCore::SendDataResponse *rv=new NsRPC2Communication::AppLinkCore::SendDataResponse;
      return NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    default:
      return 0;
  }

  return 0;
}


Json::Value Marshaller::Notification2JSON(const NsRPC2Communication::RPC2Notification* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;

  switch(msg->getMethod())
  {
    default:
      return j;
  }
}


Json::Value Marshaller::Request2JSON(const NsRPC2Communication::RPC2Request* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATA:
      return NsRPC2Communication::AppLinkCore::SendDataMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::SendData*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::Response2JSON(const NsRPC2Communication::RPC2Response* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_NSRPC2COMMUNICATION_APPLINKCORE__SENDDATARESPONSE:
      return NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller::toJSON(*static_cast<const NsRPC2Communication::AppLinkCore::SendDataResponse*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::toJSON(const RPC2Command* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;

  switch(msg->getCommandType())
  {
    case RPC2Command::REQUEST:
      return Request2JSON(static_cast<const RPC2Request*>(msg));
    case RPC2Command::RESPONSE:
      return Response2JSON(static_cast<const RPC2Response*>(msg));
    case RPC2Command::NOTIFICATION:
      return Notification2JSON(static_cast<const RPC2Notification*>(msg));

    case RPC2Command::ERROR:
      return RPC2ErrorMarshaller::toJSON(*static_cast<const RPC2Error*>(msg));

    case RPC2Command::UNDEFINED:
    default:
      return j;
  }
}


const Marshaller::localHash Marshaller::mHashTable[2]=
{
  {"AppLinkCore.SendData",0,&Marshaller::mNsRPC2Communication_AppLinkCore__SendData},
  {"AppLinkCore.SendDataResponse",1,&Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse}

};

NsRPC2Communication::RPC2ErrorMarshaller Marshaller::mRPC2ErrorInternal;

NsRPC2Communication::AppLinkCore::SendDataMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__SendData;
NsRPC2Communication::AppLinkCore::SendDataResponseMarshaller Marshaller::mNsRPC2Communication_AppLinkCore__SendDataResponse;
