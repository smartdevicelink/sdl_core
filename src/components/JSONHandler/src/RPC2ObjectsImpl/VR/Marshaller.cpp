#include <cstring>
#include "../include/JSONHandler/RPC2Objects/VR/Marshaller.h"

#include "../include/JSONHandler/RPC2Objects/VR/AddCommand.h"
#include "../include/JSONHandler/RPC2Objects/VR/AddCommandResponse.h"
#include "../include/JSONHandler/RPC2Objects/VR/DeleteCommand.h"
#include "../include/JSONHandler/RPC2Objects/VR/DeleteCommandResponse.h"
#include "../include/JSONHandler/RPC2Objects/VR/GetCapabilities.h"
#include "../include/JSONHandler/RPC2Objects/VR/GetCapabilitiesResponse.h"
#include "../include/JSONHandler/RPC2Objects/VR/OnCommand.h"

namespace RPC2Communication
{
  namespace VR
  {
#include "Marshaller.inc"
  }
}

/*
  interface	RPC2Communication::VR
  version	1.2
  generated at	Wed Nov  7 11:26:04 2012
  source stamp	Wed Nov  7 09:31:14 2012
  author	robok0der
*/

using namespace RPC2Communication;
using namespace AppLinkRPC;
using namespace RPC2Communication::VR;


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
      case METHOD_ONCOMMAND:
      {
        OnCommand *rv=new OnCommand;
        return OnCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
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
      case METHOD_ADDCOMMAND:
      {
        AddCommand *rv=new AddCommand;
        return AddCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_DELETECOMMAND:
      {
        DeleteCommand *rv=new DeleteCommand;
        return DeleteCommandMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      case METHOD_GETCAPABILITIES:
      {
        GetCapabilities *rv=new GetCapabilities;
        return GetCapabilitiesMarshaller::fromJSON(json,*rv) ? rv : 0;
      }
      default:
        return 0;
    }
    return 0;
  }
							// response
  if(!json.isMember("result"))  return 0;

  if(!json["id"].isInt()) return 0;

// here is extension of protocol, two fields added: resultCode and method
  if(!json["result"].isMember("resultCode") || !json["result"]["resultCode"].isString())  return 0;
  if(!json["result"].isMember("method") || !json["result"]["method"].isString())  return 0;

  Methods m=getIndex(json["result"]["method"].asString().c_str());

  switch(m)
  {
    case METHOD_ADDCOMMANDRESPONSE:
    {
      AddCommandResponse *rv=new AddCommandResponse;
      return AddCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_DELETECOMMANDRESPONSE:
    {
      DeleteCommandResponse *rv=new DeleteCommandResponse;
      return DeleteCommandResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
    }
    case METHOD_GETCAPABILITIESRESPONSE:
    {
      GetCapabilitiesResponse *rv=new GetCapabilitiesResponse;
      return GetCapabilitiesResponseMarshaller::fromJSON(json,*rv) ? rv : 0;
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
    case METHOD_ONCOMMAND:
      return OnCommandMarshaller::toJSON(*static_cast<const OnCommand*>(msg));
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
    case METHOD_ADDCOMMAND:
      return AddCommandMarshaller::toJSON(*static_cast<const AddCommand*>(msg));
    case METHOD_DELETECOMMAND:
      return DeleteCommandMarshaller::toJSON(*static_cast<const DeleteCommand*>(msg));
    case METHOD_GETCAPABILITIES:
      return GetCapabilitiesMarshaller::toJSON(*static_cast<const GetCapabilities*>(msg));
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
    case METHOD_ADDCOMMANDRESPONSE:
      return AddCommandResponseMarshaller::toJSON(*static_cast<const AddCommandResponse*>(msg));
    case METHOD_DELETECOMMANDRESPONSE:
      return DeleteCommandResponseMarshaller::toJSON(*static_cast<const DeleteCommandResponse*>(msg));
    case METHOD_GETCAPABILITIESRESPONSE:
      return GetCapabilitiesResponseMarshaller::toJSON(*static_cast<const GetCapabilitiesResponse*>(msg));
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


const Marshaller::localHash Marshaller::mHashTable[7]=
{
  {"VR.AddCommand",0,&Marshaller::mAddCommand},
  {"VR.AddCommandResponse",1,&Marshaller::mAddCommandResponse},
  {"VR.DeleteCommand",2,&Marshaller::mDeleteCommand},
  {"VR.DeleteCommandResponse",3,&Marshaller::mDeleteCommandResponse},
  {"VR.GetCapabilities",4,&Marshaller::mGetCapabilities},
  {"VR.GetCapabilitiesResponse",5,&Marshaller::mGetCapabilitiesResponse},
  {"VR.OnCommand",6,&Marshaller::mOnCommand}

};

RPC2Communication::RPC2ErrorMarshaller Marshaller::mRPC2ErrorInternal;

AddCommandMarshaller Marshaller::mAddCommand;
AddCommandResponseMarshaller Marshaller::mAddCommandResponse;
DeleteCommandMarshaller Marshaller::mDeleteCommand;
DeleteCommandResponseMarshaller Marshaller::mDeleteCommandResponse;
GetCapabilitiesMarshaller Marshaller::mGetCapabilities;
GetCapabilitiesResponseMarshaller Marshaller::mGetCapabilitiesResponse;
OnCommandMarshaller Marshaller::mOnCommand;
