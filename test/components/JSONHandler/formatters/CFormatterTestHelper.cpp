
#include "CFormatterTestHelper.hpp"

#include "JSONHandler/CSmartFactory.hpp"

using namespace test::components::JSONHandler::formatters;
using namespace NsAppLink::NsSmartObjects;
using namespace NsAppLink::NsJSONHandler::strings;

// ----------------------------------------------------------------------------

void CFormatterTestHelper::fillTestObject(CSmartObject& obj)
{
    obj[S_PARAMS][S_MESSAGE_TYPE] = "request";
    obj[S_PARAMS][S_FUNCTION_ID] = "some function";
    obj[S_PARAMS][S_CORRELATION_ID] = 12;
    obj[S_PARAMS][S_PROTOCOL_TYPE] = 0;
    obj[S_MSG_PARAMS]["appId"] = "APP ID";
    obj[S_MSG_PARAMS]["appName"] = "APP NAME";
    obj[S_MSG_PARAMS]["appType"][0] = "SYSTEM";
    obj[S_MSG_PARAMS]["appType"][1] = "COMMUNICATION";
    obj[S_MSG_PARAMS]["hmiDisplayLanguageDesired"] = "RU-RU";
    obj[S_MSG_PARAMS]["isMediaApplication"] = true;
    obj[S_MSG_PARAMS]["languageDesired"] = "EN-US";
    obj[S_MSG_PARAMS]["ngnMediaScreenAppName"] = "SCREEN NAME";
    obj[S_MSG_PARAMS]["syncMsgVersion"]["majorVersion"] = 2;
    obj[S_MSG_PARAMS]["syncMsgVersion"]["minorVersion"] = 10;
    obj[S_MSG_PARAMS]["ttsName"][0]["text"] = "ABC";
    obj[S_MSG_PARAMS]["ttsName"][0]["type"] = "TEXT";
    obj[S_MSG_PARAMS]["vrSynonyms"][0] = "Synonym1";
    obj[S_MSG_PARAMS]["vrSynonyms"][1] = "Synonym2";
    obj[S_MSG_PARAMS]["null"] = NsAppLink::NsSmartObjects::CSmartObject();
    obj[S_MSG_PARAMS]["double"] = -0.1234;
}

// ----------------------------------------------------------------------------

bool CFormatterTestHelper::checkTestObject(const CSmartObject& obj)
{

    return true;
}
