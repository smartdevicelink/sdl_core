package com.ford.syncV4.android.module.reader;

import android.util.Log;

import com.ford.syncV4.util.Base64;

import java.io.IOException;

/**
 * Converts a plain input string to binary data.
 *
 * @author enikolsky
 */
public class PlainStringBinaryDataReader implements BinaryDataReader {
    private static final String TAG =
            PlainStringBinaryDataReader.class.getSimpleName();

    /*
     * (non-Javadoc)
     *
     * @see
     * com.ford.syncV4.android.module.BinaryDataReader#supportsReading(java.
     * lang.String)
     */
    @Override
    public boolean supportsReading(String input) {
        return true;
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

        return input.getBytes();
    }

}
