package com.ford.sdlreverse.util;

import java.io.File;

import com.ford.sdlreverse.downloader.XAPKFile;
import com.ford.sdlreverse.exception.SDCardException;
import com.google.android.vending.expansion.downloader.Helpers;

import android.content.Context;
import android.os.Environment;

/**
 * Title: ExtStorageUtils.java<br>
 * Description: Utility class for regular operations with external storage (SC
 * card)<br>
 * 
 * @author Eugene Sagan
 */
public class ExtStorageUtils {

	public static boolean isExternalStorageAvailable() {
		if (Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED_READ_ONLY)) {
			return false;
		}
		return Environment.getExternalStorageState().equals(
				Environment.MEDIA_MOUNTED);
	}

	public static String getAppHomeFolderOnSDCard(Context ctx) {
		if (isExternalStorageAvailable()) {
			File externalFilesDir = ctx.getExternalFilesDir(null);
			if (externalFilesDir != null) {
				return externalFilesDir.getAbsolutePath();
			}
		}
		throw new SDCardException();
	}

	public static String getFilePathOnSDCard(Context ctx, String fileName) {
		return getAppHomeFolderOnSDCard(ctx) + File.separator
				+ fileName.toLowerCase();
	}

	public static File getFileOnSDCard(Context ctx, String fileName) {
		return new File(getFilePathOnSDCard(ctx, fileName));
	}

	/**
	 * Go through each of the Expansion APK files defined in the project and
	 * determine if the files are present and match the required size. Free
	 * applications should definitely consider doing this, as this allows the
	 * application to be launched for the first time without having a network
	 * connection present. Paid applications that use LVL should probably do at
	 * least one LVL check that requires the network to be present, so this is
	 * not as necessary.
	 * 
	 * @return true if they are present.
	 */
	public static boolean expansionFilesDelivered(Context ctx, XAPKFile xAPK) {
		String fileName = Helpers.getExpansionAPKFileName(ctx, xAPK.mIsMain,
				xAPK.mFileVersion);
		return Helpers.doesFileExist(ctx, fileName, xAPK.mFileSize, true);
	}

}