package com.ford.sdlreverse.util;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import android.util.Log;

/**
 * Title: MD5Utils.java<br>
 * Description: Utility class for MD5 hash calculation for downloaded content<br>
 * 
 * @author vsaenko
 */
public class MD5Utils {

	private static final String TAG = MD5Utils.class.getSimpleName();

	private static String calculateMD5(InputStream inStream) {

		if (Log.isLoggable(TAG, Log.DEBUG)) {
			Log.d(TAG, "calculateMD5");
		}

		MessageDigest digest;
		try {

			digest = MessageDigest.getInstance(Const.MD5);
			final byte[] buffer = new byte[8192];
			int read;

			while ((read = inStream.read(buffer)) > 0) {
				digest.update(buffer, 0, read);
			}
			final byte[] md5sum = digest.digest();
			final BigInteger bigInt = new BigInteger(1, md5sum);
			String output = bigInt.toString(16);
			// Fill to 32 chars
			output = String.format("%32s", output).replace(' ', '0');
			return output;

		} catch (final NoSuchAlgorithmException e) {
			Log.e(Const.APP_TAG, e.getMessage(), e);
		} catch (FileNotFoundException e) {
			Log.e(Const.APP_TAG, e.getMessage(), e);
		} catch (final IOException e) {
			Log.e(Const.APP_TAG, e.getMessage(), e);
		} finally {
			try {
				if (inStream != null) {
					inStream.close();
				}
			} catch (final IOException e) {
				Log.e(Const.APP_TAG, e.getMessage(), e);
			}
		}

		return null;
	}

	public static boolean isFileMD5Valid(String filePath, final String md5) {
		FileInputStream fis;
		try {
			fis = new FileInputStream(filePath);
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return false;
		}
		return md5.equalsIgnoreCase(calculateMD5(fis));
	}

}
