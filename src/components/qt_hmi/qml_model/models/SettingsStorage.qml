import QtQuick 2.0
import "../hmi_api/Common.js" as Common

Item
{
    property var capabilities: []
    property bool vrAvailable: false
    property bool ttsAvailable: false
    property bool navigationAvailable: false
    property bool vehileInfoAvailable: false
    property bool uiAvailable: false

    property var sdlLanguagesList: [
        Common.Language.EN_US,
        Common.Language.ES_MX,
        Common.Language.FR_CA,
        Common.Language.DE_DE,
        Common.Language.ES_ES,
        Common.Language.EN_GB,
        Common.Language.RU_RU,
        Common.Language.TR_TR,
        Common.Language.PL_PL,
        Common.Language.FR_FR,
        Common.Language.IT_IT,
        Common.Language.SV_SE,
        Common.Language.PT_PT,
        Common.Language.NL_NL,
        Common.Language.ZH_TW,
        Common.Language.JA_JP,
        Common.Language.AR_SA,
        Common.Language.KO_KR,
        Common.Language.PT_BR,
        Common.Language.CS_CZ,
        Common.Language.DA_DK,
        Common.Language.NO_NO
    ]


    property var buttonCapabilities: []
}
