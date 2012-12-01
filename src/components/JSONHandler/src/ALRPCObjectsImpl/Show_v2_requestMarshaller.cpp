#include "../include/JSONHandler/ALRPCObjects/Show_v2_request.h"
#include "ImageMarshaller.h"
#include "SoftButtonMarshaller.h"
#include "TextAlignmentMarshaller.h"

#include "Show_v2_requestMarshaller.h"


/*
  interface	Ford Sync RAPI
  version	1.2 / 2.0O
  date		2011-05-17 / 2012-11-02
  generated at	Thu Nov 29 14:49:08 2012
  source stamp	Thu Nov 29 06:50:10 2012
  author	robok0der
*/

using namespace NsAppLinkRPC;


bool Show_v2_requestMarshaller::checkIntegrity(Show_v2_request& s)
{
  return checkIntegrityConst(s);
}


bool Show_v2_requestMarshaller::fromString(const std::string& s,Show_v2_request& e)
{
  try
  {
    Json::Reader reader;
    Json::Value json;
    if(!reader.parse(s,json,false))  return false;
    if(!fromJSON(json,e))  return false;
  }
  catch(...)
  {
    return false;
  }
  return true;
}


const std::string Show_v2_requestMarshaller::toString(const Show_v2_request& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool Show_v2_requestMarshaller::checkIntegrityConst(const Show_v2_request& s)
{
  if(s.mainField1 && s.mainField1->length()>500)  return false;
  if(s.mainField2 && s.mainField2->length()>500)  return false;
  if(s.mainField3 && s.mainField3->length()>500)  return false;
  if(s.mainField4 && s.mainField4->length()>500)  return false;
  if(s.alignment && !TextAlignmentMarshaller::checkIntegrityConst(*s.alignment))  return false;
  if(s.statusBar && s.statusBar->length()>500)  return false;
  if(s.mediaClock && s.mediaClock->length()>500)  return false;
  if(s.mediaTrack && s.mediaTrack->length()>500)  return false;
  if(s.graphic && !ImageMarshaller::checkIntegrityConst(*s.graphic))  return false;
  if(s.softButtons)
  {
    unsigned int i=s.softButtons[0].size();
    if(i>8 || i<0)  return false;
    while(i--)
    {
    if(!SoftButtonMarshaller::checkIntegrityConst(s.softButtons[0][i]))   return false;
    }
  }
  if(s.customPresets)
  {
    unsigned int i=s.customPresets[0].size();
    if(i>6 || i<0)  return false;
    while(i--)
    {
      if(s.customPresets[0][i].length()>500)  return false;
    }
  }
  return true;
}

Json::Value Show_v2_requestMarshaller::toJSON(const Show_v2_request& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["request"]=Json::Value(Json::objectValue);
  json["request"]["name"]=Json::Value("Show_v2");
  json["request"]["correlationID"]=Json::Value(e.getCorrelationID());

  Json::Value j=Json::Value(Json::objectValue);

  if(e.mainField1)
    j["mainField1"]=Json::Value(*e.mainField1);

  if(e.mainField2)
    j["mainField2"]=Json::Value(*e.mainField2);

  if(e.mainField3)
    j["mainField3"]=Json::Value(*e.mainField3);

  if(e.mainField4)
    j["mainField4"]=Json::Value(*e.mainField4);

  if(e.alignment)
    j["alignment"]=TextAlignmentMarshaller::toJSON(*e.alignment);

  if(e.statusBar)
    j["statusBar"]=Json::Value(*e.statusBar);

  if(e.mediaClock)
    j["mediaClock"]=Json::Value(*e.mediaClock);

  if(e.mediaTrack)
    j["mediaTrack"]=Json::Value(*e.mediaTrack);

  if(e.graphic)
    j["graphic"]=ImageMarshaller::toJSON(*e.graphic);

  if(e.softButtons)
  {
    unsigned int sz=e.softButtons->size();
    j["softButtons"]=Json::Value(Json::arrayValue);
    j["softButtons"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["softButtons"][i]=SoftButtonMarshaller::toJSON(e.softButtons[0][i]);
  }

  if(e.customPresets)
  {
    unsigned int sz=e.customPresets->size();
    j["customPresets"]=Json::Value(Json::arrayValue);
    j["customPresets"].resize(sz);
    for(unsigned int i=0;i<sz;i++)
      j["customPresets"][i]=Json::Value(e.customPresets[0][i]);
  }

  json["request"]["parameters"]=j;
  return json;
}


bool Show_v2_requestMarshaller::fromJSON(const Json::Value& js,Show_v2_request& c)
{
  if(c.mainField1)  delete c.mainField1;
  c.mainField1=0;

  if(c.mainField2)  delete c.mainField2;
  c.mainField2=0;

  if(c.mainField3)  delete c.mainField3;
  c.mainField3=0;

  if(c.mainField4)  delete c.mainField4;
  c.mainField4=0;

  if(c.alignment)  delete c.alignment;
  c.alignment=0;

  if(c.statusBar)  delete c.statusBar;
  c.statusBar=0;

  if(c.mediaClock)  delete c.mediaClock;
  c.mediaClock=0;

  if(c.mediaTrack)  delete c.mediaTrack;
  c.mediaTrack=0;

  if(c.graphic)  delete c.graphic;
  c.graphic=0;

  if(c.softButtons)  delete c.softButtons;
  c.softButtons=0;

  if(c.customPresets)  delete c.customPresets;
  c.customPresets=0;

  try
  {
    if(!js.isObject())  return false;

    if(!js.isMember("request"))  return false;

    if(!js["request"].isObject())  return false;
    const Json::Value& j2=js["request"];

    if(!j2.isMember("name") || !j2["name"].isString() || j2["name"].asString().compare("Show_v2"))  return false;
    if(!j2.isMember("correlationID") || !j2["correlationID"].isInt())  return false;
    c.setCorrelationID(j2["correlationID"].asInt());

    if(!j2.isMember("parameters"))  return false;
    const Json::Value& json=j2["parameters"];
    if(!json.isObject())  return false;
    if(json.isMember("mainField1"))
    {
      const Json::Value& j=json["mainField1"];
      if(!j.isString())  return false;
      c.mainField1=new std::string(j.asString());
    }
    if(json.isMember("mainField2"))
    {
      const Json::Value& j=json["mainField2"];
      if(!j.isString())  return false;
      c.mainField2=new std::string(j.asString());
    }
    if(json.isMember("mainField3"))
    {
      const Json::Value& j=json["mainField3"];
      if(!j.isString())  return false;
      c.mainField3=new std::string(j.asString());
    }
    if(json.isMember("mainField4"))
    {
      const Json::Value& j=json["mainField4"];
      if(!j.isString())  return false;
      c.mainField4=new std::string(j.asString());
    }
    if(json.isMember("alignment"))
    {
      const Json::Value& j=json["alignment"];
      c.alignment=new TextAlignment();
      if(!TextAlignmentMarshaller::fromJSON(j,c.alignment[0]))
        return false;
    }
    if(json.isMember("statusBar"))
    {
      const Json::Value& j=json["statusBar"];
      if(!j.isString())  return false;
      c.statusBar=new std::string(j.asString());
    }
    if(json.isMember("mediaClock"))
    {
      const Json::Value& j=json["mediaClock"];
      if(!j.isString())  return false;
      c.mediaClock=new std::string(j.asString());
    }
    if(json.isMember("mediaTrack"))
    {
      const Json::Value& j=json["mediaTrack"];
      if(!j.isString())  return false;
      c.mediaTrack=new std::string(j.asString());
    }
    if(json.isMember("graphic"))
    {
      const Json::Value& j=json["graphic"];
      c.graphic=new Image();
      if(!ImageMarshaller::fromJSON(j,c.graphic[0]))
        return false;
    }
    if(json.isMember("softButtons"))
    {
      const Json::Value& j=json["softButtons"];
      if(!j.isArray())  return false;
      c.softButtons=new std::vector<SoftButton>();
      c.softButtons->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
      {
        SoftButton t;
        if(!SoftButtonMarshaller::fromJSON(j[i],t))
          return false;
        c.softButtons[0][i]=t;
      }

    }
    if(json.isMember("customPresets"))
    {
      const Json::Value& j=json["customPresets"];
      if(!j.isArray())  return false;
      c.customPresets=new std::vector<std::string>();
      c.customPresets->resize(j.size());
      for(unsigned int i=0;i<j.size();i++)
        if(!j[i].isString())
          return false;
        else
          c.customPresets[0][i]=j[i].asString();
    }

  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}

