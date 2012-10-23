#include "JSONHandler/TextAlignmentMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

const TextAlignment::TextAlignmentInternal TextAlignmentMarshaller::getIndex(const std::string& s)
{
    if ( s.compare("LEFT_ALIGNED") == 0 )
    {
        return TextAlignment::LEFT_ALIGNED;
    }
    if ( s.compare("RIGHT_ALIGNED") == 0 )
    {
        return TextAlignment::RIGHT_ALIGNED;
    }
    if ( s.compare("CENTERED") == 0 )
    {
        return TextAlignment::CENTERED;
    }
    return TextAlignment::INVALID_ENUM;
}

const char* TextAlignmentMarshaller::getName(TextAlignment::TextAlignmentInternal e)
  {
     switch( e )
     {
        case TextAlignment::LEFT_ALIGNED:
            return "LEFT_ALIGNED";
        case TextAlignment::RIGHT_ALIGNED:
            return "RIGHT_ALIGNED";
        case TextAlignment::CENTERED:
            return "CENTERED";
        default:
            return "";
     }
  }

bool TextAlignmentMarshaller::fromString(const std::string& s,TextAlignment& e)
{
    e.mInternal = TextAlignment::INVALID_ENUM;
    try
    {
        Json::Reader reader;
        Json::Value json;
        if(!reader.parse(s,json,false))  return false;
        if(fromJSON(json,e))  return true;
    }
    catch(...)
    {
        return false;
    }
    return false;
}
    
std::string TextAlignmentMarshaller::toString(const TextAlignment& e)
{
    Json::FastWriter writer;
    return e.mInternal==TextAlignment::INVALID_ENUM ? "" : writer.write(toJSON(e));
}

bool TextAlignmentMarshaller::fromJSON(const Json::Value& s,TextAlignment& e)
{
    e.mInternal=TextAlignment::INVALID_ENUM;
    if(!s.isString())
        return false;

    e.mInternal=getIndex(s.asString());
    return (e.mInternal!=TextAlignment::INVALID_ENUM);
}

Json::Value TextAlignmentMarshaller::toJSON(const TextAlignment& e)
{
    if(e.mInternal==TextAlignment::INVALID_ENUM) 
        return Json::Value(Json::nullValue);

    const char* s = getName( e.mInternal );
    
    return s ? Json::Value(s) : Json::Value(Json::nullValue);
}