package com.ford.syncV4.trace;

import com.ford.syncV4.trace.enums.DetailLevel;
import com.ford.syncV4.trace.enums.Mod;


public class DiagLevel {

	static private DetailLevel[] levels;
	
	static {   // this is a static c-tor!!
		levels = new DetailLevel[Mod.values().length];
		setAllLevels(DetailLevel.OFF);
	}
	
	public static void setAllLevels(DetailLevel thisDetail) {
		for (int i=0; i < levels.length; i++) {
			levels[i] = thisDetail; //  
		}
	}
	
    public static void setLevel(Mod thisMod, DetailLevel thisDetail) {
    	levels[thisMod.ordinal()] = thisDetail;
    }
	
	public static DetailLevel getLevel(Mod thisMod) {
		return levels[thisMod.ordinal()];
	}
	
	public static boolean isValidDetailLevel(String dtString) {
		// Assume false
		Boolean isValid = false;
		
		if (dtString.equalsIgnoreCase("verbose"))
			isValid = true;
		else if (dtString.equalsIgnoreCase("terse"))
			isValid = true;
		else if (dtString.equalsIgnoreCase("off"))
			isValid = true;
		
		return isValid;
	}
	
	public static DetailLevel toDetailLevel(String dtString) {
		DetailLevel dt = DetailLevel.OFF;
		if (dtString.equalsIgnoreCase("verbose"))
			dt = DetailLevel.VERBOSE;
		else if (dtString.equalsIgnoreCase("terse"))
			dt = DetailLevel.TERSE;
		return dt;
	}
}
