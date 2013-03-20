
#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"


bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::toString(
        NsAppLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    objToJsonValue(obj, root);

    outStr = root.toStyledString();

    return true;
}

// ----------------------------------------------------------------------------

bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonALRPCv1::fromString(
        const std::string& str,
        NsAppLink::NsSmartObjects::CSmartObject& out)
{
    Json::Value root;
    Json::Reader reader;

    bool parsingSuccessful = reader.parse(str, root);

    if (true == parsingSuccessful)
    {
        jsonValueToObj(root, out);
    }

    return parsingSuccessful;
}

