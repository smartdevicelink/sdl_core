#include "../src/../include/JSONHandler/RPC2Objects/NsRPC2Communication/UI/Show.h"
#include "../src/ALRPCObjectsImpl/TextAlignmentMarshaller.h"
#include "../src/ALRPCObjectsImpl/ImageMarshaller.h"
#include "../src/ALRPCObjectsImpl/SoftButtonMarshaller.h"
#include "../src/ALRPCObjectsImpl/ResultMarshaller.h"
#include "../src/../src/RPC2ObjectsImpl//NsRPC2Communication/UI/ShowMarshaller.h"

/*
  interface	NsRPC2Communication::UI
  version	1.2
  generated at	Thu Nov 29 14:32:09 2012
  source stamp	Thu Nov 29 14:32:05 2012
  author	robok0der
*/

using namespace NsRPC2Communication::UI;

bool ShowMarshaller::checkIntegrity(Show& s)
{
  return checkIntegrityConst(s);
}


bool ShowMarshaller::fromString(const std::string& s,Show& e)
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


const std::string ShowMarshaller::toString(const Show& e)
{
  Json::FastWriter writer;
  return checkIntegrityConst(e) ? writer.write(toJSON(e)) : "";
}


bool ShowMarshaller::checkIntegrityConst(const Show& s)
{
  if(s.mainField1 && (s.mainField1[0].length()>500))  return false;

  if(s.mainField2 && (s.mainField2[0].length()>500))  return false;

  if(s.mainField3 && (s.mainField3[0].length()>500))  return false;

  if(s.mainField4 && (s.mainField4[0].length()>500))  return false;

  if(s.alignment && (!NsAppLinkRPC::TextAlignmentMarshaller::checkIntegrityConst(s.alignment[0])))  return false;

  if(s.statusBar && (s.statusBar[0].length()>500))  return false;

  if(s.mediaClock && (s.mediaClock[0].length()>500))  return false;

  if(s.mediaTrack && (s.mediaTrack[0].length()>500))  return false;

  if(s.graphic && (!NsAppLinkRPC::ImageMarshaller::checkIntegrityConst(s.graphic[0])))  return false;

  if(s.softButtons)
  {
    unsigned int i=s.softButtons[0].size();
    if(i<0)  return false;
    if(i>8)  return false;
  }

  if(s.customPresets)
  {
    unsigned int i=s.customPresets[0].size();
    if(i<0)  return false;
    if(i>6)  return false;
    while(i--)
    {
      if(s.customPresets[0][i].length()>500)  return false;
    }
  }

  return true;
}


Json::Value ShowMarshaller::toJSON(const Show& e)
{
  Json::Value json(Json::objectValue);
  if(!checkIntegrityConst(e))
    return Json::Value(Json::nullValue);

  json["jsonrpc"]=Json::Value("2.0");
  json["method"]=Json::Value("UI.Show");

  json["id"]=Json::Value(e.getId());
  json["params"]=Json::Value(Json::objectValue);
  if(e.mainField1)
    json["params"]["mainField1"]=Json::Value(e.mainField1[0]);;
  if(e.mainField2)
    json["params"]["mainField2"]=Json::Value(e.mainField2[0]);;
  if(e.mainField3)
    json["params"]["mainField3"]=Json::Value(e.mainField3[0]);;
  if(e.mainField4)
    json["params"]["mainField4"]=Json::Value(e.mainField4[0]);;
  if(e.alignment)
    json["params"]["alignment"]=NsAppLinkRPC::TextAlignmentMarshaller::toJSON(e.alignment[0]);;
  if(e.statusBar)
    json["params"]["statusBar"]=Json::Value(e.statusBar[0]);;
  if(e.mediaClock)
    json["params"]["mediaClock"]=Json::Value(e.mediaClock[0]);;
  if(e.mediaTrack)
    json["params"]["mediaTrack"]=Json::Value(e.mediaTrack[0]);;
  if(e.graphic)
    json["params"]["graphic"]=NsAppLinkRPC::ImageMarshaller::toJSON(e.graphic[0]);;
  if(e.softButtons)
  {
    unsigned int i=e.softButtons[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=NsAppLinkRPC::SoftButtonMarshaller::toJSON(e.softButtons[0][i]);

    json["params"]["softButtons"]=j;
  }
  if(e.customPresets)
  {
    unsigned int i=e.customPresets[0].size();
    Json::Value j=Json::Value(Json::arrayValue);
    j.resize(i);
    while(i--)
      j[i]=Json::Value(e.customPresets[0][i]);

    json["params"]["customPresets"]=j;
  }
  json["params"]["appId"]=Json::Value(e.appId);;
  return json;
}


bool ShowMarshaller::fromJSON(const Json::Value& json,Show& c)
{
  try
  {
    if(!json.isObject())  return false;
    if(!json.isMember("jsonrpc") || !json["jsonrpc"].isString() || json["jsonrpc"].asString().compare("2.0"))  return false;
    if(!json.isMember("method") || !json["method"].isString() || json["method"].asString().compare("UI.Show"))  return false;
    if(!json.isMember("id") || !json["id"].isInt()) return false;
    c.setId(json["id"].asInt());

    if(!json.isMember("params")) return false;

    Json::Value js=json["params"];
    if(!js.isObject())  return false;
    if(c.mainField1)  delete c.mainField1;
    c.mainField1=0;
    if(js.isMember("mainField1"))
    {
      if(!js["mainField1"].isString())  return false;
      c.mainField1=new std::string();
      c.mainField1[0]=js["mainField1"].asString();
      if(c.mainField1[0].length()>500)  return false;

    }

    if(c.mainField2)  delete c.mainField2;
    c.mainField2=0;
    if(js.isMember("mainField2"))
    {
      if(!js["mainField2"].isString())  return false;
      c.mainField2=new std::string();
      c.mainField2[0]=js["mainField2"].asString();
      if(c.mainField2[0].length()>500)  return false;

    }

    if(c.mainField3)  delete c.mainField3;
    c.mainField3=0;
    if(js.isMember("mainField3"))
    {
      if(!js["mainField3"].isString())  return false;
      c.mainField3=new std::string();
      c.mainField3[0]=js["mainField3"].asString();
      if(c.mainField3[0].length()>500)  return false;

    }

    if(c.mainField4)  delete c.mainField4;
    c.mainField4=0;
    if(js.isMember("mainField4"))
    {
      if(!js["mainField4"].isString())  return false;
      c.mainField4=new std::string();
      c.mainField4[0]=js["mainField4"].asString();
      if(c.mainField4[0].length()>500)  return false;

    }

    if(c.alignment)  delete c.alignment;
    c.alignment=0;
    if(js.isMember("alignment"))
    {
      c.alignment=new NsAppLinkRPC::TextAlignment();
      if(!NsAppLinkRPC::TextAlignmentMarshaller::fromJSON(js["alignment"],c.alignment[0]))  return false;
    }

    if(c.statusBar)  delete c.statusBar;
    c.statusBar=0;
    if(js.isMember("statusBar"))
    {
      if(!js["statusBar"].isString())  return false;
      c.statusBar=new std::string();
      c.statusBar[0]=js["statusBar"].asString();
      if(c.statusBar[0].length()>500)  return false;

    }

    if(c.mediaClock)  delete c.mediaClock;
    c.mediaClock=0;
    if(js.isMember("mediaClock"))
    {
      if(!js["mediaClock"].isString())  return false;
      c.mediaClock=new std::string();
      c.mediaClock[0]=js["mediaClock"].asString();
      if(c.mediaClock[0].length()>500)  return false;

    }

    if(c.mediaTrack)  delete c.mediaTrack;
    c.mediaTrack=0;
    if(js.isMember("mediaTrack"))
    {
      if(!js["mediaTrack"].isString())  return false;
      c.mediaTrack=new std::string();
      c.mediaTrack[0]=js["mediaTrack"].asString();
      if(c.mediaTrack[0].length()>500)  return false;

    }

    if(c.graphic)  delete c.graphic;
    c.graphic=0;
    if(js.isMember("graphic"))
    {
      c.graphic=new NsAppLinkRPC::Image();
      if(!NsAppLinkRPC::ImageMarshaller::fromJSON(js["graphic"],c.graphic[0]))  return false;
    }

    if(c.softButtons)  delete c.softButtons;
    c.softButtons=0;
    if(js.isMember("softButtons"))
    {
      if(!js["softButtons"].isArray()) return false;
      unsigned int i=js["softButtons"].size();
      if(i<0)  return false;
      if(i>8)  return false;

      c.softButtons=new std::vector<NsAppLinkRPC::SoftButton>();
      c.softButtons->resize(js["softButtons"].size());

      while(i--)
        if(!NsAppLinkRPC::SoftButtonMarshaller::fromJSON(js["softButtons"][i],c.softButtons[0][i]))  return false;
    }


    if(c.customPresets)  delete c.customPresets;
    c.customPresets=0;
    if(js.isMember("customPresets"))
    {
      if(!js["customPresets"].isArray()) return false;
      unsigned int i=js["customPresets"].size();
      if(i<0)  return false;
      if(i>6)  return false;

      c.customPresets=new std::vector<std::string>();
      c.customPresets->resize(js["customPresets"].size());

      while(i--)
      {
        if(!js["customPresets"][i].isString())
          return false;

        c.customPresets[0][i]=js["customPresets"][i].asString();
        if(c.customPresets[0][i].length()>500)  return false;
      }
    }


    if(!js.isMember("appId") || !js["appId"].isInt())  return false;
    c.appId=js["appId"].asInt();
    
  }
  catch(...)
  {
    return false;
  }
  return checkIntegrity(c);
}
