#include "JSONHandler/OnButtonEvent.h"
#include <json/reader.h>
#include <json/writer.h>
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

const RPC2Marshaller::Methods RPC2Marshaller::getIndex(const std::string & s)
{
  if ( s.compare("Buttons.OnButtonEvent") == 0 )
    {
        return METHOD_ONBUTTONEVENT;
    }
    if ( s.compare("TTS.Speak") == 0 )
    {
        return METHOD_SPEAK_REQUEST;
    }
    if ( s.compare("UI.Alert") == 0 )
    {
        return METHOD_ALERT_REQUEST;
    }
    if ( s.compare("UI.Show") == 0 )
    {
        return METHOD_SHOW_REQUEST;
    }
    if ( s.compare("Buttons.GetCapabilities") == 0 )
    {
        return METHOD_GET_CAPABILITIES_REQUEST;
    }
    if ( s.compare("Buttons.OnButtonPress") == 0 )
    {
        return METHOD_ONBUTTONPRESS;
    }
    return METHOD_INVALID;
}


const RPC2Marshaller::localHash RPC2Marshaller::mHashTable[3]=
{
  {"OnButtonEvent",METHOD_ONBUTTONEVENT,&RPC2Marshaller::mOnButtonEventMarshaller}
};


RPC2Command* RPC2Marshaller::fromJSON(const Json::Value& json)
{
  if(!json.isObject())  return NULL;
  
  if(!json.isMember("method") || !json["method"].isString()) return NULL;
  Methods m=getIndex(json["method"].asString().c_str());
  
  switch(m)                         
  {
    case METHOD_INVALID:
      return NULL;

    case METHOD_ONBUTTONEVENT:
      {
         OnButtonEvent* rv=new OnButtonEvent;
         if(OnButtonEventMarshaller::fromJSON(json,*rv))
           return rv;
         delete rv;
         return NULL;
      }
    case METHOD_SPEAK_REQUEST:
    {
        Speak * rv = new Speak;
        if ( SpeakMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ALERT_REQUEST:
    {
        Alert * rv = new Alert;
        if ( AlertMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_SHOW_REQUEST:
    {
        Show * rv = new Show; 
        if ( ShowMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_GET_CAPABILITIES_REQUEST:
    {
        GetCapabilities * rv = new GetCapabilities;
        if ( GetCapabilitiesMarshaller::fromJSON(json, *rv) )
          return rv;
        delete rv;
        return NULL;
    }
    case METHOD_ONBUTTONPRESS:
    {
        OnButtonPress * rv = new OnButtonPress;
        if (OnButtonPressMarshaller::fromJSON(json, *rv))
          return rv;
        delete rv;
        return NULL;
    }
  }

  return NULL;
}

Json::Value RPC2Marshaller::toJSON(const RPC2Command* msg)
{
  Json::Value j=Json::Value(Json::nullValue);

  if(!msg) return j;
  Methods m=static_cast<Methods>(msg->getMethod());

  switch(m)
  {
    case METHOD_INVALID:
      return j;

    case METHOD_ONBUTTONEVENT:
      return OnButtonEventMarshaller::toJSON(* static_cast<const OnButtonEvent*>(msg));
    case METHOD_SPEAK_REQUEST:
      return SpeakMarshaller::toJSON(* static_cast<const Speak*>(msg));
    case METHOD_SPEAK_RESPONSE:
      return SpeakResponseMarshaller::toJSON(* static_cast<const SpeakResponse*>(msg));
    case METHOD_ALERT_REQUEST:
      return AlertMarshaller::toJSON(* static_cast<const Alert*>(msg)) ;
    case METHOD_ALERT_RESPONSE:
      return AlertResponseMarshaller::toJSON(* static_cast<const AlertResponse*>(msg));
    case METHOD_SHOW_REQUEST:
      return ShowMarshaller::toJSON(* static_cast<const Show*> (msg));
    case METHOD_SHOW_RESPONSE:
      return ShowResponseMarshaller::toJSON(* static_cast<const ShowResponse*>(msg));
    case METHOD_GET_CAPABILITIES_REQUEST:
      return GetCapabilitiesMarshaller::toJSON(* static_cast<const GetCapabilities*>(msg));
    case METHOD_GET_CAPABILITIES_RESPONSE:
      return GetCapabilitiesResponseMarshaller::toJSON(* static_cast<const GetCapabilitiesResponse*>(msg));
    case METHOD_ONBUTTONPRESS:
      return OnButtonPressMarshaller::toJSON(* static_cast<const OnButtonPress*>(msg));
  }

  return j;
}

RPC2Command* RPC2Marshaller::fromString(const std::string& s)
{
  RPC2Command* rv=0;
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return 0;
    if(!(rv=fromJSON(json)))  return 0;
  }
  catch(...)
  {
    return 0;
  }
  return rv;
}

std::string RPC2Marshaller::toString(const RPC2Command* msg)
{
  if(!msg)  return "";
  Json::Value json=toJSON(msg);
  if(json.isNull()) return "";

  Json::FastWriter writer;
  std::string rv;
  return writer.write(json);
}

OnButtonEventMarshaller RPC2Marshaller::mOnButtonEventMarshaller;
SpeakMarshaller RPC2Marshaller::mSpeakMarshaller;
SpeakResponseMarshaller RPC2Marshaller::mSpeakResponseMarshaller;
AlertMarshaller RPC2Marshaller::mAlertMarshaller;
AlertResponseMarshaller RPC2Marshaller::mAlertResponseMarshaller;
ShowMarshaller RPC2Marshaller::mShowMarshaller;
ShowResponseMarshaller RPC2Marshaller::mShowResponseMarshaller;
GetCapabilitiesMarshaller RPC2Marshaller::mGetCapabilitiesMarshaller;
GetCapabilitiesResponseMarshaller RPC2Marshaller::mGetCapabilitiesResponseMarshaller;
OnButtonPressMarshaller RPC2Marshaller::mOnButtonPressMarshaller;
