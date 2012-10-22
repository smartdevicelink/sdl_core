#include "JSONHandler/ButtonEventModeMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

bool ButtonEventModeMarshaller::fromString(const std::string& s,ButtonEventMode& e)
{
    e.mInternal = ButtonEventMode::INVALID_ENUM;
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
    
std::string ButtonEventModeMarshaller::toString(const ButtonEventMode& e)
{
    Json::FastWriter writer;
    return e.mInternal==ButtonEventMode::INVALID_ENUM ? "" : writer.write(toJSON(e));
}

bool ButtonEventModeMarshaller::fromJSON(const Json::Value& s,ButtonEventMode& e)
{
    e.mInternal=ButtonEventMode::INVALID_ENUM;
    if(!s.isString())
        return false;

    //e.mInternal=getIndex(s.asString().c_str());
    if ( s.asString().compare( "BUTTONUP" ) ) {
        e.mInternal = ButtonEventMode::BUTTONUP;
    } else {
        e.mInternal = ButtonEventMode::BUTTONDOWN;
    }
    return (e.mInternal!=ButtonEventMode::INVALID_ENUM);
}

Json::Value ButtonEventModeMarshaller::toJSON(const ButtonEventMode& e)
{
    if(e.mInternal==ButtonEventMode::INVALID_ENUM) 
        return Json::Value(Json::nullValue);

    const char* s = 0;
    if (e.mInternal == 0 )
    {
        s = "BUTTONUP";
    } else 
    {
        s = "BUTTONDOWN";
    }
    
    return s ? Json::Value(s) : Json::Value(Json::nullValue);
}