
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

#include "json/json.h"

void NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::jsonValueToObj(
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
    // There's nothing for nullValue because Uninitialized object is already a null object
}

// ----------------------------------------------------------------------------

bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::toString(
        NsAppLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    objToJsonValue(obj["msg_params"], root);

    outStr = root.toStyledString();

    return true;
}

// ----------------------------------------------------------------------------

void NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::objToJsonValue(
        NsAppLink::NsSmartObjects::CSmartObject &obj,
        Json::Value &item)
{
    if (NsAppLink::NsSmartObjects::SmartType_Array == obj.get_type())
    {
        for (int i = 0; i < obj.length(); i++)
        {
            Json::Value value(Json::nullValue);

            objToJsonValue(obj[i], value);

            item.append(value);
        }
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Map == obj.get_type())
    {
        std::vector<std::string> keys = obj.enumerate();

        for (int i = 0; i < keys.size(); i++)
        {
            Json::Value value(Json::nullValue);

            objToJsonValue(obj[keys[i]], value);

            item[keys[i]] = value;
        }
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Boolean == obj.get_type())
    {
        item = static_cast<bool>(obj);
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Integer == obj.get_type())
    {
        item = static_cast<int>(obj);
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Double == obj.get_type())
    {
        item = static_cast<double>(obj);
    }
    else if (NsAppLink::NsSmartObjects::SmartType_Null == obj.get_type())
    {
        item = Json::nullValue;
    }
    else
    {
        item = static_cast<std::string>(obj);
    }
}
