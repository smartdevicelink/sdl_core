#include "JSONHandler/OnButtonEvent.h"
#include <json/reader.h>
#include <json/writer.h>
#include "JSONHandler/RPC2Marshaller.h"

using namespace RPC2Communication;

const RPC2Marshaller::Methods RPC2Marshaller::getIndex(const char* s)
{
  if(!s)
    return METHOD_INVALID;
  //const struct localHash* p=RPC2Marshaller_intHash::getPointer(s,strlen(s));
    const struct localHash* p = &mHashTable[0];
  return p ? static_cast<Methods>(p->idx) : METHOD_INVALID;
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

