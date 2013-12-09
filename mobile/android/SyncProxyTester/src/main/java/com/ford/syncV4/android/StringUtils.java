package com.ford.syncV4.android;

import java.util.Vector;

/**
 * Created by enikolsky on 2013-11-25.
 */
public class StringUtils {
    public static final String DEFAULT_JOIN_STRING = ",";

    public static String joinStrings(Iterable<String> strings,
                                     String joinString) {
        StringBuilder sb = new StringBuilder();
        int i = 0;
        for (String string : strings) {
            if (i != 0) {
                sb.append(joinString);
            }
            sb.append(string);
            ++i;
        }
        return sb.toString();
    }

    public static String joinStrings(Iterable<String> strings) {
        return joinStrings(strings, DEFAULT_JOIN_STRING);
    }
}
