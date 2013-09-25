import QtQuick 2.0
import "../hmi_api/Common.js" as Common

Item {
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

    property var displayCapabilities: {
        "displayType": Common.DisplayType.GEN2_8_DMA,
        "textFields": [
            Common.TextFieldName.mainField1,
            Common.TextFieldName.mainField2,
            Common.TextFieldName.mainField1,
            Common.TextFieldName.mainField2,
            Common.TextFieldName.statusBar,
            Common.TextFieldName.mediaClock,
            Common.TextFieldName.mediaTrack,
            Common.TextFieldName.alertText1,
            Common.TextFieldName.alertText2,
            Common.TextFieldName.alertText3,
            Common.TextFieldName.scrollableMessageBody,
            Common.TextFieldName.initialInteractionText,
            Common.TextFieldName.navigationText1,
            Common.TextFieldName.navigationText2,
            Common.TextFieldName.ETA,
            Common.TextFieldName.totalDistance,
            Common.TextFieldName.navigationText,
            Common.TextFieldName.audioPassThruDisplayText1,
            Common.TextFieldName.audioPassThruDisplayText2,
            Common.TextFieldName.sliderHeader,
            Common.TextFieldName.sliderFooter,
            Common.TextFieldName.notificationText
        ],
        "mediaClockFormats": [
            Common.MediaClockFormat.CLOCK1,
            Common.MediaClockFormat.CLOCK2,
            Common.MediaClockFormat.CLOCK3,
            Common.MediaClockFormat.CLOCKTEXT1,
            Common.MediaClockFormat.CLOCKTEXT2,
            Common.MediaClockFormat.CLOCKTEXT3,
            Common.MediaClockFormat.CLOCKTEXT4
        ],
        "imageCapabilities": [
            Common.ImageType.DYNAMIC
        ]
    }

    property var softButtonCapabilities: {
        "shortPressAvailable": true,
        "longPressAvailable": true,
        "upDownAvailable": true,
        "imageSupported": true
    }
}
