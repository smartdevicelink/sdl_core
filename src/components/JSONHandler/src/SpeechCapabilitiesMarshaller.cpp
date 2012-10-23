#include "JSONHandler/SpeechCapabilitiesMarshaller.h"
#include <json/reader.h>
#include <json/writer.h>

const SpeechCapabilities::SpeechCapabilitiesInternal SpeechCapabilitiesMarshaller::getIndex(const std::string& s)
{
    if ( s.compare("TEXT") == 0 )
    {
        return SpeechCapabilities::TEXT;
    }
    if ( s.compare("SAPI_PHONEMES") == 0 )
    {
        return SpeechCapabilities::SAPI_PHONEMES;
    }
    if ( s.compare("PRE_RECORDED") == 0 )
    {
        return SpeechCapabilities::PRE_RECORDED;
    }
    if ( s.compare("SILENCE") == 0 )
    {
        return SpeechCapabilities::SILENCE;
    }
    return SpeechCapabilities::INVALID_ENUM;
}

const char* SpeechCapabilitiesMarshaller::getName(SpeechCapabilities::SpeechCapabilitiesInternal e)
  {
     switch( e )
     {
        case SpeechCapabilities::TEXT:
            return "TEXT";
        case SpeechCapabilities::SAPI_PHONEMES:
            return "SAPI_PHONEMES";
        case SpeechCapabilities::PRE_RECORDED:
            return "PRE_RECORDED";
        case SpeechCapabilities::SILENCE:
            return "SILENCE";
        default:
            return "";
     }
  }

bool SpeechCapabilitiesMarshaller::fromString(const std::string& s,SpeechCapabilities& e)
{
    e.mInternal = SpeechCapabilities::INVALID_ENUM;
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
    
std::string SpeechCapabilitiesMarshaller::toString(const SpeechCapabilities& e)
{
    Json::FastWriter writer;
    return e.mInternal==SpeechCapabilities::INVALID_ENUM ? "" : writer.write(toJSON(e));
}

bool SpeechCapabilitiesMarshaller::fromJSON(const Json::Value& s,SpeechCapabilities& e)
{
    e.mInternal=SpeechCapabilities::INVALID_ENUM;
    if(!s.isString())
        return false;

    e.mInternal=getIndex(s.asString());
    return (e.mInternal!=SpeechCapabilities::INVALID_ENUM);
}

Json::Value SpeechCapabilitiesMarshaller::toJSON(const SpeechCapabilities& e)
{
    if(e.mInternal==SpeechCapabilities::INVALID_ENUM) 
        return Json::Value(Json::nullValue);

    const char* s = getName( e.mInternal );
    
    return s ? Json::Value(s) : Json::Value(Json::nullValue);
}