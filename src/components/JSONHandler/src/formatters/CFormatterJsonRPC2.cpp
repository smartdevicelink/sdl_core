
#include "JSONHandler/formatters/CFormatterJsonRPC2.hpp"


bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonRPC2::toString(
        NsAppLink::NsSmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);

    // TODO: Implement RPC2 serialization

    // objToJsonValue(obj, root);

    outStr = root.toStyledString();

    return true;
}

// ----------------------------------------------------------------------------

bool NsAppLink::NsJSONHandler::Formatters::CFormatterJsonRPC2::fromString(
        const std::string& str,
        NsAppLink::NsSmartObjects::CSmartObject& out)
{
    Json::Value root;
    Json::Reader reader;

    // TODO: Implement RPC2 deserialisation

    bool parsingSuccessful = reader.parse(str, root);

    if (true == parsingSuccessful)
    {
    //    jsonValueToObj(root, out);
    }

    return parsingSuccessful;
}

