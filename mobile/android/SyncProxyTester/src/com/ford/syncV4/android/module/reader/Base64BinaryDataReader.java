package com.ford.syncV4.android.module.reader;

import android.util.Log;

import com.ford.syncV4.util.Base64;

import java.io.IOException;

/**
 * Decodes the binary data from a Base64-encoded input string. The string must
 * start with the "base64:" prefix.
 * 
 * @author enikolsky
 * 
 */
public class Base64BinaryDataReader implements BinaryDataReader {
    private static final String TAG = Base64BinaryDataReader.class.getSimpleName();
	private static final String BASE64_PREFIX = "base64:";

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.ford.syncV4.android.module.BinaryDataReader#supportsReading(java.
	 * lang.String)
	 */
	@Override
	public boolean supportsReading(String input) {
		return input.startsWith(BASE64_PREFIX);
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.ford.syncV4.android.module.BinaryDataReader#read(java.lang.String)
	 */
	@Override
	public byte[] read(String input) {
		if (!supportsReading(input)) {
			return null;
		}
		
		String str = input.substring(BASE64_PREFIX.length());
        try {
            return Base64.decode(str);
        } catch (IOException e) {
            Log.e(TAG, "Can't decode base64 string", e);
            return null;
        }
    }

}
