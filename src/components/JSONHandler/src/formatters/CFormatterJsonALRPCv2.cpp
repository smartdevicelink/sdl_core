
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"


bool NsSmartDeviceLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::toString(
        const NsSmartDeviceLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    NsSmartDeviceLink::NsSmartObjects::CSmartObject formattedObj(obj);
    formattedObj.getSchema().unapplySchema(formattedObj);       // converts enums(as int) to strings

    objToJsonValue(formattedObj.getElement(NsSmartDeviceLink::NsJSONHandler::strings::S_MSG_PARAMS), root);

    outStr = root.toStyledString();

    return true;
}

