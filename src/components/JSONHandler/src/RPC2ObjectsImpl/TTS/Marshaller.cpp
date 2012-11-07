#include <cstring>
#include "../include/JSONHandler/RPC2Objects/TTS/Marshaller.h"

#include "../include/JSONHandler/RPC2Objects/TTS/GetCapabilities.h"
#include "../include/JSONHandler/RPC2Objects/TTS/GetCapabilitiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/TTS/Speak.h"
#include "../include/JSONHandler/RPC2Objects/TTS/SpeakResponse.h"

namespace RPC2Communication
{
  namespace TTS
  {
#include "Marshaller.inc"
  }
}

/*
  interface	RPC2Communication::TTS
  version	1.2
  generated at	Wed Nov  7 11:25:55 2012
  source stamp	Wed Nov  7 09:34:03 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::TTS;


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
      case METHOD_GETCAPABILITIES:
      {
        GetCapabilities *rv=new GetCapabilities;
        return GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_SPEAK:
      {
        Speak *rv=new Speak;
        return SpeakMarshaller::fromJSON(json,*rv) ? rv : 0;
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
  if(!json["result"].isMember("_Result") || !json["result"]["_Result"].isString())  return 0;
  if(!json["result"].isMember("_Method") || !json["result"]["_Method"].isString())  return 0;

  Methods m=getIndex(json["result"]["_Method"].asString().c_str());

  switch(m)
  {
    case METHOD_GETCAPABILITIESRESPONSE:
    {
      GetCapabilitiesResponse *rv=new GetCapabilitiesResponse;
      return GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_SPEAKRESPONSE:
    {
      SpeakResponse *rv=new SpeakResponse;
      return SpeakResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    default:
      return 0;
  }

  return 0;
}


Json::Value Marshaller::Notification2JSON(const RPC2Communication::RPC2Notification* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;

  switch(msg->getMethod())
  {
    default:
      return j;
  }
}


Json::Value Marshaller::Request2JSON(const RPC2Communication::RPC2Request* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_GETCAPABILITIES:
      return GetCapabilitiesMarshaller::toJSON(*static_cast<const GetCapabilities*>(msg));
    case METHOD_SPEAK:
      return SpeakMarshaller::toJSON(*static_cast<const Speak*>(msg));
    default:
      return j;
  }
}


Json::Value Marshaller::Response2JSON(const RPC2Communication::RPC2Response* msg)
{
  Json::Value j=Json::Value(Json::nullValue);
  if(!msg) return j;
  switch(msg->getMethod())
  {
    case METHOD_GETCAPABILITIESRESPONSE:
      return GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const GetCapabilitiesResponse*>(msg));
    case METHOD_SPEAKRESPONSE:
      return SpeakResponseMarshaller::toJSON(*static_cast<const SpeakResponse*>(msg));
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


const Marshaller::localHash Marshaller::mHashTable[4]=
{
  {"TTS.GetCapabilities",0,&Marshaller::mGetCapabilities},
  {"TTS.GetCapabilitiesResponse",1,&Marshaller::mGetCapabilitiesResponse},
  {"TTS.Speak",2,&Marshaller::mSpeak},
  {"TTS.SpeakResponse",3,&Marshaller::mSpeakResponse}

};

RPC2Communication::RPC2ErrorMarshaller Marshaller::mRPC2ErrorInternal;

GetCapabilitiesMarshaller Marshaller::mGetCapabilities;
GetCapabilitiesResponseMarshaller Marshaller::mGetCapabilitiesResponse;
SpeakMarshaller Marshaller::mSpeak;
SpeakResponseMarshaller Marshaller::mSpeakResponse;
