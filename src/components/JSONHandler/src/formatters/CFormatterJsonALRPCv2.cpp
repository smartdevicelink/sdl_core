
#include "JSONHandler/formatters/CFormatterJsonALRPCv2.hpp"


bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv2::toString(
        NsAppLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    objToJsonValue(obj["msg_params"], root);

    outStr = root.toStyledString();

    return true;
}

