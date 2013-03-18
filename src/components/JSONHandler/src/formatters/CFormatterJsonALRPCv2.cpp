
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"

#include "json/json.h"


bool NsAppLink::NsJSONHandler::CFormatterJsonALRPCv2::toString(
        NsAppLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    objToJsonValue(obj, root);

    outStr = root.toStyledString();

    return true;
}

// ----------------------------------------------------------------------------

void NsAppLink::NsJSONHandler::CFormatterJsonALRPCv2::objToJsonValue(
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
    else
    {
        item = static_cast<std::string>(obj);
    }
}
