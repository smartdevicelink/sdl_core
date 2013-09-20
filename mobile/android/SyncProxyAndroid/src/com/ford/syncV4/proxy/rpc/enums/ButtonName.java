package com.ford.syncV4.proxy.rpc.enums;

public enum ButtonName{
    OK,
    SEEKLEFT,
    SEEKRIGHT,
    TUNEUP,
    TUNEDOWN,
    PRESET_0,
    PRESET_1,
    PRESET_2,
    PRESET_3,
    PRESET_4,
    PRESET_5,
    PRESET_6,
    PRESET_7,
    PRESET_8,
    PRESET_9,
    CUSTOM_BUTTON;

    public static ButtonName valueForString(String value) {
        return valueOf(value);
    }
    
    /**
     * indexForPresetButton returns the integer index for preset buttons
     * which match the preset order. E.G.: indexForPresetButton(PRESET_1)
     * returns the value 1. If the buttonName given is not a preset button,
     * the method will return null.
     *  
     * @param buttonName
     * @return Integer
     */
    public static Integer indexForPresetButton(ButtonName buttonName) {
    	
    	Integer returnIndex = null;
    	
    	switch(buttonName) {        	
        	case PRESET_0:
        		returnIndex = 0;
        		break;
        	case PRESET_1:
        		returnIndex = 1;
        		break;
        	case PRESET_2:
        		returnIndex = 2;
        		break;
        	case PRESET_3:
        		returnIndex = 3;
        		break;
        	case PRESET_4:
        		returnIndex = 4;
        		break;
        	case PRESET_5:
        		returnIndex = 5;
        		break;
        	case PRESET_6:
        		returnIndex = 6;
        		break;
        	case PRESET_7:
        		returnIndex = 7;
        		break;
        	case PRESET_8:
        		returnIndex = 8;
        		break;
        	case PRESET_9:
        		returnIndex = 9;
        		break;
    	}
    	
    	return returnIndex;
    }
}
