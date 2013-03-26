
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"


bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::toString(
        const NsAppLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    NsAppLink::NsSmartObjects::CSmartObject formattedObj(obj);
    formattedObj.getSchema().unapplySchema(formattedObj);       // converts enums(as int) to strings

    objToJsonValue(formattedObj.getElement(NsAppLink::NsJSONHandler::strings::S_MSG_PARAMS), root);

    outStr = root.toStyledString();

    return true;
}

