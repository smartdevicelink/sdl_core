
#include "JSONHandler/formatters/CFormatterJsonALRPCv1.hpp"

#include "JSONHandler/CSmartFactory.hpp"

namespace Formatters = NsAppLink::NsJSONHandler::Formatters;
namespace SmartObjects = NsAppLink::NsSmartObjects;
namespace Strings = NsAppLink::NsJSONHandler::strings;

// ----------------------------------------------------------------------------

const std::string Formatters::CFormatterJsonALRPCv1::S_REQUEST("request");
const std::string Formatters::CFormatterJsonALRPCv1::S_RESPONSE("response");
const std::string Formatters::CFormatterJsonALRPCv1::S_PARAMETERS("parameters");
const std::string Formatters::CFormatterJsonALRPCv1::S_NAME("name");
const std::string Formatters::CFormatterJsonALRPCv1::S_CORRELATION_ID("correlationID");

// ----------------------------------------------------------------------------

const std::string Formatters::CFormatterJsonALRPCv1::getMessageType(
        SmartObjects::CSmartObject& obj)
{
    return obj[Strings::S_PARAMS][Strings::S_MESSAGE_TYPE];
}

// ----------------------------------------------------------------------------

const std::string Formatters::CFormatterJsonALRPCv1::getMessageType(
        const Json::Value& root)
{
    std::string type;

    if (true == root.isMember(S_REQUEST))
    {
        type = S_REQUEST;
    }
    else if (true == root.isMember(S_RESPONSE))
    {
        type = S_RESPONSE;
    }
    else
    {}

    return type;
}

// ----------------------------------------------------------------------------

bool Formatters::CFormatterJsonALRPCv1::toString(
        SmartObjects::CSmartObject& obj,
        std::string& outStr)
{
    Json::Value root(Json::objectValue);
    Json::Value params(Json::objectValue);

    objToJsonValue(obj[Strings::S_MSG_PARAMS], params);

    std::string type = getMessageType(obj);
    root[type] = Json::Value(Json::objectValue);
    root[type][S_PARAMETERS] = params;

    root[type][S_CORRELATION_ID] = static_cast<int>(obj[Strings::S_PARAMS][Strings::S_CORRELATION_ID]);
    root[type][S_NAME] = static_cast<std::string>(obj[Strings::S_PARAMS][Strings::S_FUNCTION_ID]);

    outStr = root.toStyledString();

    return true;
}

// ----------------------------------------------------------------------------

bool Formatters::CFormatterJsonALRPCv1::fromString(
        const std::string& str,
        SmartObjects::CSmartObject& out)
{
    Json::Value root;
    Json::Reader reader;
    std::string type;

    bool parsingSuccessful = reader.parse(str, root);

    if (true == parsingSuccessful)
    {
        type = getMessageType(root);
    }

    parsingSuccessful = parsingSuccessful && (!type.empty());

    if (true == parsingSuccessful)
    {
        jsonValueToObj(root[type][S_PARAMETERS], out[Strings::S_MSG_PARAMS]);

        out[Strings::S_PARAMS][Strings::S_MESSAGE_TYPE] = type;
        out[Strings::S_PARAMS][Strings::S_FUNCTION_ID] = root[type][S_NAME].asString();
        out[Strings::S_PARAMS][Strings::S_CORRELATION_ID] = root[type][S_CORRELATION_ID].asInt();
        out[Strings::S_PARAMS][Strings::S_PROTOCOL_VERSION] = 1;
    }

    return parsingSuccessful;
}

