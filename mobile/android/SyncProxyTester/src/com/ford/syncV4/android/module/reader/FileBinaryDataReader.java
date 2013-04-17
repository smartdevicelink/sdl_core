package com.ford.syncV4.android.module.reader;

import java.io.BufferedInputStream;
import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStream;

import android.annotation.SuppressLint;
import android.util.Log;

/**
 * Reads the binary data from the file which name is specified by the input
 * string. The path to the file must start with a reference to the external
 * storage (e.g. "/sdcard/").
 * 
 * @author enikolsky
 * 
 */
public class FileBinaryDataReader implements BinaryDataReader {
	private static final String TAG = FileBinaryDataReader.class
			.getSimpleName();

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.ford.syncV4.android.module.BinaryDataReader#supportsReading(java.
	 * lang.String)
	 */
	@SuppressLint("SdCardPath")
	@Override
	public boolean supportsReading(String input) {
		return input.startsWith("/sdcard/");
	}

	/*
	 * (non-Javadoc)
	 * 
	 * @see
	 * com.ford.syncV4.android.module.BinaryDataReader#read(java.lang.String)
	 */
	@Override
	public byte[] read(String input) {
		// fail fast if input is not supported
		if (!supportsReading(input)) {
			return null;
		}

		InputStream is = null;
		try {
			is = new BufferedInputStream(new FileInputStream(input));
			ByteArrayOutputStream os = new ByteArrayOutputStream(is.available());
			final int buffersize = 4096;
			final byte[] buffer = new byte[buffersize];
			int available = 0;
			while ((available = is.read(buffer)) >= 0) {
				os.write(buffer, 0, available);
			}
			return os.toByteArray();
		} catch (IOException e) {
			Log.w(TAG, "Can't read file " + input, e);
			return null;
		} catch (OutOfMemoryError e) {
			Log.e(TAG, "File " + input + " is too big", e);
			return null;
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

}
