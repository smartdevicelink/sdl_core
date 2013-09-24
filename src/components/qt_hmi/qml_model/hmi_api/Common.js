var ButtonName = {
    OK:              0,
    SEEKLEFT:        1,
    SEEKRIGHT:       2,
    TUNEUP:          3,
    TUNEDOWN:        4,
    PRESET_0:        5,
    PRESET_1:        6,
    PRESET_2:        7,
    PRESET_3:        8,
    PRESET_4:        9,
    PRESET_5:       10,
    PRESET_6:       11,
    PRESET_7:       12,
    PRESET_8:       13,
    PRESET_9:       14,
    CUSTOM_BUTTON:  15
}

var ButtonEventMode = {
    BUTTONUP:   0,
    BUTTONDOWN: 1
}

var ButtonPressMode = {
    LONG:  0,
    SHORT: 1
}

var SystemContext = {
    MAIN:         0,
    VRSESSION:    1,
    MENU:         2,
    HMI_OBSCURED: 3,
    ALERT:        4
}

var DisplayType = {
    CID:        0,
    TYPE2:      1,
    TYPE5:      2,
    NGN:        3,
    GEN2_8_DMA: 4,
    GEN2_6_DMA: 5,
    MFD3:       6,
    MFD4:       7,
    MFD5:       8
}

var TextFieldName = {
    mainField1:                 0,
    mainField2:                 1,
    mainField3:                 2,
    mainField4:                 3,
    statusBar:                  4,
    mediaClock:                 5,
    mediaTrack:                 6,
    alertText1:                 7,
    alertText2:                 8,
    alertText3:                 9,
    scrollableMessageBody:     10,
    initialInteractionText:    11,
    navigationText1:           12,
    navigationText2:           13,
    ETA:                       14,
    totalDistance:             15,
    navigationText:            16,
    audioPassThruDisplayText1: 17,
    audioPassThruDisplayText2: 18,
    sliderHeader:              19,
    sliderFooter:              20,
    notificationText:          21
}

var MediaClockFormat = {
    CLOCK1:     0,
    CLOCK2:     1,
    CLOCK3:     2,
    CLOCKTEXT1: 3,
    CLOCKTEXT2: 4,
    CLOCKTEXT3: 5,
    CLOCKTEXT4: 6
}

var HmiZoneCapabilities = {
    FRONT: 0,
    BACK: 1
}

var ImageType = {
    STATIC:   0,
    DYNAMIC : 1
}

var Language = {
    EN_US:  0,
    ES_MX:  1,
    FR_CA:  2,
    DE_DE:  3,
    ES_ES:  4,
    EN_GB:  5,
    RU_RU:  6,
    TR_TR:  7,
    PL_PL:  8,
    FR_FR:  9,
    IT_IT: 10,
    SV_SE: 11,
    PT_PT: 12,
    NL_NL: 13,
    EN_AU: 14,
    ZH_CN: 15,
    ZH_TW: 16,
    JA_JP: 17,
    AR_SA: 18,
    KO_KR: 19,
    PT_BR: 20,
    CS_CZ: 21,
    DA_DK: 22,
    NO_NO: 23
}

var DriverDistractionState = {
    DD_ON:  0,
    DD_OFF: 1
}
