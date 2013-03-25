
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

#include "json/json.h"


void NsAppLink::NsJSONHandler::Formatters::CFormatterJsonBase::jsonValueToObj(
        const Json::Value& value, NsAppLink::NsSmartObjects::CSmartObject& obj)
{
    if (value.type() == Json::objectValue)
    {
        Json::Value::Members members = value.getMemberNames();

        for (int i = 0; i < members.size(); i++)
        {
            jsonValueToObj(value[members[i]], obj[members[i]]);
        }
    }
    else if (value.type() == Json::arrayValue)
    {
        for (int i = 0; i < value.size(); i++)
        {
            jsonValueToObj(value[i], obj[i]);
        }
    }
    else if (value.type() == Json::intValue || value.type() == Json::uintValue)
    {
        obj = value.asInt();
    }
    else if (value.type() == Json::realValue)
    {
        obj = value.asDouble();
    }
    else if (value.type() == Json::booleanValue)
    {
        obj = value.asBool();
    }
    else if (value.type() == Json::stringValue)
    {
        obj = value.asString();
    }

    //TODO: Passed object can be not null
    // There's nothing for nullValue because Uninitialized object is already a null object
}

// ----------------------------------------------------------------------------

void NsAppLink::NsJSONHandler::Formatters::CFormatterJsonBase::objToJsonValue(
        NsAppLink::NsSmartObjects::CSmartObject &obj,
        Json::Value &item)
{
    if (NsAppLink::NsSmartObjects::SmartType_Array == obj.getType())
    {
        for (int i = 0; i < obj.length(); i++)
        {
            Json::Value value(Json::nullValue);

            objToJsonValue(obj[i], value);

            item.append(value);
        }
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Map == obj.getType())
    {
        std::set<std::string> keys = obj.enumerate();

        for (std::set<std::string>::const_iterator key = keys.begin(); key != keys.end(); key++)
        {
            Json::Value value(Json::nullValue);

            objToJsonValue(obj[*key], value);

            item[*key] = value;
        }
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Boolean == obj.getType())
    {
        item = static_cast<bool>(obj);
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Integer == obj.getType())
    {
        item = static_cast<int>(obj);
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Double == obj.getType())
    {
        item = static_cast<double>(obj);
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Null == obj.getType())
    {
        item = Json::nullValue;
    }
    else
    {
        item = static_cast<std::string>(obj);
    }
}
