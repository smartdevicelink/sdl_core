package com.ford.syncV4.proxy.rpc.enums;

import java.util.EnumSet;

public enum Language {
    EN_US("EN-US"),
    ES_MX("ES-MX"),
    FR_CA("FR-CA"),
    DE_EU("DE-EU"),
    ES_EU("ES-EU"),
    EN_EU("EN-EU"),
    RU_RU("RU-RU"),
    TR_TR("TR-TR"),
    PL_EU("PL-EU"),
    FR_EU("FR-EU"),
    IT_EU("IT-EU"),
    SV_EU("SV-EU"),
    PT_EU("PT-EU"),
    NL_EU("NL-EU"),
    EN_AU("EN-AU"),
    ZH_CN("ZH-CN"),
    ZH_TW("ZH-TW"),
    JA_JP("JA-JP"),
    AR("AR"),
    KO_KR("KO-KR");

    String internalName;
    
    private Language(String internalName) {
        this.internalName = internalName;
    }
    
    public String toString() {
        return this.internalName;
    }
    
    public static Language valueForString(String value) {       	
    	for (Language anEnum : EnumSet.allOf(Language.class)) {
            if (anEnum.toString().equals(value)) {
                return anEnum;
            }
        }
        return null;
    }
}
