package com.ford.syncV4.proxy.rpc.enums;

public enum AmbientLightStatus {
	DARK,
	LIGHT,
	TWILIGHT,
	TUNNEL_ON,
	TUNNEL_OFF,
	NO_DATA_EXISTS;

    public static AmbientLightStatus valueForString(String value) {
        return valueOf(value);
    }
}
