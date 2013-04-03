
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

#include "json/json.h"


void NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonBase::jsonValueToObj(
        const Json::Value& value, NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj)
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

void NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonBase::objToJsonValue(
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject &obj,
        Json::Value &item)
{
    if (NsSmartDeviceLink::NsSmartObjects::SmartType_Array == obj.getType())
    {
        for (int i = 0; i < obj.length(); i++)
        {
            Json::Value value(Json::nullValue);

            objToJsonValue(obj.getElement(i), value);

            item.append(value);
        }
    }
    else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Map == obj.getType())
    {
        std::set<std::string> keys = obj.enumerate();

        for (std::set<std::string>::const_iterator key = keys.begin(); key != keys.end(); key++)
        {
            Json::Value value(Json::nullValue);

            objToJsonValue(obj.getElement(*key), value);

            item[*key] = value;
        }
    }
    else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Boolean == obj.getType())
    {
        item = static_cast<bool>(obj);
    }
    else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Integer == obj.getType())
    {
        item = static_cast<int>(obj);
    }
    else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Double == obj.getType())
    {
        item = static_cast<double>(obj);
    }
    else if (NsSmartDeviceLink::NsSmartObjects::SmartType_Null == obj.getType())
    {
        item = Json::nullValue;
    }
    else
    {
        item = static_cast<std::string>(obj);
    }
}
