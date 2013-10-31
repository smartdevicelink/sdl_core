package com.ford.avarsdl.downloader;

/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import java.io.File;
import java.io.IOException;

import android.app.PendingIntent;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.graphics.Color;
import android.os.AsyncTask;
import android.os.Messenger;
import android.util.Log;
import android.view.Gravity;
import android.widget.TextView;
import android.widget.Toast;

import com.android.vending.expansion.zipfile.APKExpansionSupport;
import com.android.vending.expansion.zipfile.ZipResourceFile;
import com.ford.avarsdl.R;
import com.ford.avarsdl.activity.AvatarActivity;
import com.ford.avarsdl.util.ActivityUtils;
import com.ford.avarsdl.util.Const;
import com.ford.avarsdl.util.LogUtils;
import com.ford.avarsdl.util.MD5Utils;
import com.google.android.vending.expansion.downloader.DownloadProgressInfo;
import com.google.android.vending.expansion.downloader.DownloaderClientMarshaller;
import com.google.android.vending.expansion.downloader.DownloaderServiceMarshaller;
import com.google.android.vending.expansion.downloader.Helpers;
import com.google.android.vending.expansion.downloader.IDownloaderClient;
import com.google.android.vending.expansion.downloader.IDownloaderService;
import com.google.android.vending.expansion.downloader.IStub;
import com.google.android.vending.expansion.downloader.impl.DownloadInfo;
import com.google.android.vending.expansion.downloader.impl.DownloadsDB;

/**
 * This is sample code for a project built against the downloader library. It
 * implements the IDownloaderClient that the client marshaler will talk to as
 * messages are delivered from the DownloaderService.
 */
public class DownloaderClient implements IDownloaderClient {
	private final static String LOG_TAG = DownloaderClient.class
			.getSimpleName();
	private final static boolean DEBUG = true;

	private AvatarActivity mContext;
	private boolean mStatePaused;

	private IDownloaderService mRemoteService;

	private IStub mDownloaderClientStub;

	private XAPKFile mMainXAPK;
	private XAPKFile mPatchXAPK;
	
	private boolean mFileIsValidated = false;

	private boolean MAINFILE_IS_PROCESSING = false;
	private boolean PATCHFILE_IS_PROCESSING = false;
	public static boolean IS_PROCESSING_VIDEO = false;
	public static boolean FINISH_NOTIFY = false;
	public static boolean SDCARD_IS_FULL = false;
	public static int DOWNLOADED_PERSENS = 0;

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

	/**
	 * Called when the activity is first create; we wouldn't create a layout in
	 * the case where we have the file and are moving to another activity
	 * without downloading.
	 */
	public DownloaderClient(AvatarActivity context, int mainVersionCode,
			boolean mainMD5IsCalculated, int patchVersionCode,
			boolean patchMD5IsCalculated) {
		LogUtils.printLog("Create download client", LOG_TAG, DEBUG);
		mContext = context;
		mMainXAPK = new XAPKFile(true, mainVersionCode,
				Const.MAIN_EXP_FILE_SIZE);
		mPatchXAPK = new XAPKFile(false, patchVersionCode,
				Const.PATCH_EXP_FILE_SIZE);

		/**
		 * Before we do anything, are the files we expect already here and
		 * delivered (presumably by Market) For free titles, this is probably
		 * worth doing. (so no Market request is necessary)
		 */
		if (mainVersionCode == 0
				|| (patchVersionCode == 0 && Const.PATCH_EXP_FILE_SIZE != 0)) {

			try {
				cleanOldExpansionFilesIfExists(mMainXAPK);
				cleanOldExpansionFilesIfExists(mPatchXAPK);

				Intent launchIntent = mContext.getIntent();
				Intent intentToLaunchThisActivityFromNotification = new Intent(
						mContext, DownloaderClient.this.getClass());
				intentToLaunchThisActivityFromNotification
						.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK
								| Intent.FLAG_ACTIVITY_CLEAR_TOP);
				intentToLaunchThisActivityFromNotification
						.setAction(launchIntent.getAction());

				if (launchIntent.getCategories() != null) {
					for (String category : launchIntent.getCategories()) {
						intentToLaunchThisActivityFromNotification
								.addCategory(category);
					}
				}
				// start service to download expansion file
				// Build PendingIntent used to open this activity from
				// Notification
				PendingIntent pendingIntent = PendingIntent.getActivity(
						mContext, 0,
						intentToLaunchThisActivityFromNotification,
						PendingIntent.FLAG_UPDATE_CURRENT);
				// Request to start the download
				int startResult = DownloaderClientMarshaller
						.startDownloadServiceIfRequired(mContext,
								pendingIntent, AvatarDownloaderService.class);

				if (startResult != DownloaderClientMarshaller.NO_DOWNLOAD_REQUIRED) {
					// The DownloaderService has started downloading the files,
					// Instantiate a member instance of IStub
					mDownloaderClientStub = DownloaderClientMarshaller
							.CreateStub(this, AvatarDownloaderService.class);
					if (null != mDownloaderClientStub) {
						mDownloaderClientStub.connect(mContext);
					}
					return;
				} // otherwise, download not needed so we fall through to
					// starting the movie
			} catch (NameNotFoundException e) {
				Log.e(LOG_TAG, "Cannot find own package! MAYDAY!");
				e.printStackTrace();
			} catch (Exception e) {
				Log.e(LOG_TAG, "An internal application error: ", e);
			}

		} else if (!mainMD5IsCalculated || !patchMD5IsCalculated) {
			validateXAPKZipFiles();
		}
	}

	/**
	 * Connect the stub to our service on resume.
	 */
	public void resumeDownload() {
		LogUtils.printLog("resume download", LOG_TAG, DEBUG);
		if (null != mDownloaderClientStub) {
			mDownloaderClientStub.connect(mContext);
		}
		if (null != mRemoteService && mStatePaused) {
			mRemoteService.requestContinueDownload();
		}
	}

	/**
	 * Disconnect the stub from our service on stop
	 */
	public void stopDownload() {
		LogUtils.printLog("stop download", LOG_TAG, DEBUG);
		if (null != mDownloaderClientStub) {
			mDownloaderClientStub.disconnect(mContext);
		}
	}

	/**
	 * Critical implementation detail. In onServiceConnected we create the
	 * remote service and marshaler. This is how we pass the client information
	 * back to the service so the client can be properly notified of changes. We
	 * must do this every time we reconnect to the service.
	 */
	@Override
	public void onServiceConnected(Messenger m) {
		LogUtils.printLog("on service connected", LOG_TAG, DEBUG);
		mRemoteService = DownloaderServiceMarshaller.CreateProxy(m);
		mRemoteService.onClientUpdated(mDownloaderClientStub.getMessenger());
	}

	/**
	 * The download state should trigger changes in the UI --- it may be useful
	 * to show the state as being indeterminate at times. This sample can be
	 * considered a guideline.
	 */
	@Override
	public void onDownloadStateChanged(int newState) {
		LogUtils.printLog("Download state = " + String.valueOf(newState),
				LOG_TAG, DEBUG);
		switch (newState) {
		case IDownloaderClient.STATE_DOWNLOADING:
			IS_PROCESSING_VIDEO = true;
			MAINFILE_IS_PROCESSING = true;
			PATCHFILE_IS_PROCESSING = true;
			break;
		case IDownloaderClient.STATE_FAILED_CANCELED:
			DownloaderClient.IS_PROCESSING_VIDEO = false;
			DownloadsDB db = DownloadsDB.getDB(mContext);
			for (DownloadInfo downloadInfo : db.getDownloads()) {
				downloadInfo.mCurrentBytes = 0;
				downloadInfo.mTotalBytes = 0;
				db.updateDownloadCurrentBytes(downloadInfo);
			}
			closeServer();
			return;
		case IDownloaderClient.STATE_FAILED_FETCHING_URL:
			// TODO : no file on server
			break;
		case IDownloaderClient.STATE_FAILED_SDCARD_FULL:
			SDCARD_IS_FULL = true;
			break;
		case IDownloaderClient.STATE_COMPLETED:
			if (!mFileIsValidated) { // to prevent double validation at once
				// increment downloader counter
				SharedPreferences prefs = mContext.getSharedPreferences(
						Const.SHPREF_REDOWNLOAD_COUNTER, 0);
				int counter = prefs.getInt(Const.SHPREF_REDOWNLOAD_COUNTER, 1);
				SharedPreferences.Editor editor = prefs.edit();
				if (editor != null) {
					editor.putInt(Const.SHPREF_REDOWNLOAD_COUNTER, ++counter);
				}
				editor.commit();
				setDownloadedFiles();
				validateXAPKZipFiles();
				mFileIsValidated = true;
			}
			return;
		default:

		}

	}

	private void setDownloadedFiles() {
		String name = Helpers.getSaveFilePath(mContext);
		File dir = new File(name);
		String mainVersion = "0";
		String patchVersion = "0";
		String fileEnding = "." + mContext.getPackageName() + ".obb";
		for (String fileName : dir.list()) {
			if (fileName.contains("main.") && fileName.contains(fileEnding)) {
				int start = fileName.indexOf(".");
				int end = fileName.indexOf(".", ++start);
				LogUtils.printLog(
						"filename - " + fileName + "; start = "
								+ String.valueOf(start) + "; end = "
								+ String.valueOf(end), LOG_TAG, DEBUG);
				mainVersion = fileName.substring(start, end);
			} else if (fileName.contains("patch.")
					&& fileName.contains(fileEnding)) {
				int start = fileName.indexOf(".");
				int end = fileName.indexOf('.', ++start);
				patchVersion = fileName.substring(start, end);
			}
		}

		int mainVersionCode = Integer.valueOf(mainVersion);
		mMainXAPK = new XAPKFile(true, mainVersionCode,
				Const.MAIN_EXP_FILE_SIZE);

		int patchVersionCode = Integer.valueOf(patchVersion);
		mPatchXAPK = new XAPKFile(false, patchVersionCode,
				Const.PATCH_EXP_FILE_SIZE);

		LogUtils.printLog("Downloaded versions are - " + mainVersionCode
				+ " and " + patchVersion, LOG_TAG, DEBUG);
	}

	@Override
	public void onDownloadProgress(DownloadProgressInfo progress) {
		DownloaderClient.DOWNLOADED_PERSENS = (int) ((double)progress.mOverallProgress/(double)progress.mOverallTotal*100);
	}

	private void closeServer() {
		LogUtils.printLog("close server", LOG_TAG, DEBUG);
	}

	/**
	 * Go through each of the Expansion APK files and open each as a zip file.
	 * Calculate the CRC for each file and return false if any fail to match.
	 * 
	 * @return true if XAPKZipFile is successful
	 */
	private void validateXAPKZipFiles() {
		LogUtils.printLog("validate XAPKZip Files with async task", LOG_TAG,
				DEBUG);
		SharedPreferences prefs = mContext.getSharedPreferences(
				Const.SHPREF_DOWNLOADER_PREFS, 0);
		SharedPreferences.Editor editor = prefs.edit();
		if (editor != null) {
			editor.putBoolean(Const.SHPREF_MAIN_EXPFILE_VALID, false);
			editor.putBoolean(Const.SHPREF_PATCH_EXPFILE_VALID, false);
		}
		editor.commit();

		validateExpansionFile(mMainXAPK, Const.MAIN_EXPANSION_FILE_MD5,
				Const.SHPREF_MAIN_EXPFILE_VALID);
		if (Const.PATCH_EXP_FILE_SIZE > 0) {
			validateExpansionFile(mPatchXAPK, Const.PATCH_EXPANSION_FILE_MD5,
					Const.SHPREF_PATCH_EXPFILE_VALID);
		} else { // no patch file is needed
			if (editor != null) {
				editor.putBoolean(Const.SHPREF_PATCH_EXPFILE_VALID, true);
				PATCHFILE_IS_PROCESSING = false;
				if (!MAINFILE_IS_PROCESSING) {
					setFinishVideoProcessing();
				}
			}
			editor.commit();
		}
	}

	private void setFinishVideoProcessing() {
		LogUtils.printLog("setFinishVideoProcessing", LOG_TAG, DEBUG);
		// set expansion file
		try {
			ZipResourceFile file = APKExpansionSupport.getAPKExpansionZipFile(
					mContext, mMainXAPK.mFileVersion, mPatchXAPK.mFileVersion);
			mContext.setExpantionFile(file);
		} catch (IOException e) {
			e.printStackTrace();
		}
		IS_PROCESSING_VIDEO = false;
		// notify user with toast about video availability
		if (FINISH_NOTIFY && ActivityUtils.getAppIsForeground()) {
			TextView view = new TextView(mContext);
			view.setBackgroundColor(Color.BLACK);
			view.setTextColor(Color.WHITE);
			view.setText(R.string.toast_download_complete);
			view.setGravity(Gravity.CENTER);
			Toast toastMsg = new Toast(mContext);
			toastMsg.setView(view);
			toastMsg.setDuration(Toast.LENGTH_LONG);
			toastMsg.show();
			
		}
	}

	private void validateExpansionFile(final XAPKFile xFile,
			final String md5Hash, final String prefName) {

		AsyncTask<Object, DownloadProgressInfo, Boolean> validationTask = new AsyncTask<Object, DownloadProgressInfo, Boolean>() {

			@Override
			protected void onPreExecute() {
				super.onPreExecute();
			}

			@Override
			protected Boolean doInBackground(Object... params) {
				LogUtils.printLog("Do in background of validation", LOG_TAG,
						DEBUG);
				String fileName = Helpers.getExpansionAPKFileName(mContext,
						xFile.mIsMain, xFile.mFileVersion);
				if (!Helpers.doesFileExist(mContext, fileName, xFile.mFileSize,
						false)) {
					LogUtils.printLog("Validation is failed: no expansion file"
							+ fileName, LOG_TAG, DEBUG);
					return false;
				}

				String filePath = Helpers.generateSaveFileName(mContext,
						fileName);
				if (MD5Utils.isFileMD5Valid(filePath, md5Hash)) {
					LogUtils.printLog("Validation succeed", LOG_TAG, DEBUG);
					return true;
				}
				LogUtils.printLog("Validation error: md5 is not valid",
						LOG_TAG, DEBUG);
				return false;
			}

			@Override
			protected void onPostExecute(Boolean result) {
				LogUtils.printLog("Do on post execute of validation", LOG_TAG,
						DEBUG);
				if (result) {
					SharedPreferences prefs = mContext.getSharedPreferences(
							Const.SHPREF_DOWNLOADER_PREFS, 0);
					SharedPreferences.Editor editor = prefs.edit();
					if (editor != null) {
						editor.putBoolean(prefName, true);
					}
					editor.commit();
				} else {
					LogUtils.printLog("Cleanup expansion file with wrong MD5",
							LOG_TAG, DEBUG);
					try {
						String fileName = Helpers.getExpansionAPKFileName(
								mContext, xFile.mIsMain, xFile.mFileVersion);
						File file = new File(Helpers.generateSaveFileName(
								mContext, fileName));
						if (file.exists()) {
							file.delete();
						}
					} catch (Exception e) {
						Log.e(LOG_TAG, "Invalid expansion file wasn't deleted",
								e);
					}
				}
				// finish processing only if main and patch processing has been
				// finished
				if (xFile.mIsMain) {
					MAINFILE_IS_PROCESSING = false;
					if (!PATCHFILE_IS_PROCESSING) {
						setFinishVideoProcessing();
					}
				} else {
					PATCHFILE_IS_PROCESSING = false;
					if (!MAINFILE_IS_PROCESSING) {
						setFinishVideoProcessing();
					}
				}
				super.onPostExecute(result);
			}

		};
		validationTask.execute(new Object());
	}

	/**
	 * delete old not used by current vesion expansion files
	 */
	private void cleanOldExpansionFilesIfExists(XAPKFile xAPKFile) {
		LogUtils.printLog("cleanOldExpansionFilesIfExists", LOG_TAG, DEBUG);
		String saveFilePath = Helpers.getSaveFilePath(mContext);
		String expAPKFileName = Helpers.getExpansionAPKFileName(mContext,
				xAPKFile.mIsMain, xAPKFile.mFileVersion);
		DownloadsDB db = DownloadsDB.getDB(mContext);
		if (db != null && db.getDownloads() != null) {
			for (DownloadInfo di : db.getDownloads()) {
				if (isOldExpAPKFile(di, expAPKFileName)) {
					deleteOldExpAPKFile(saveFilePath + File.separator
							+ di.mFileName);
				}
			}
		}

	}

	/**
	 * Check if populated in database expansion file is for previous version of
	 * app
	 */
	private boolean isOldExpAPKFile(DownloadInfo di, String newExpAPKFile) {
		LogUtils.printLog("isOldExpAPKFile", LOG_TAG, DEBUG);
		return di.mFileName.contains(Const.EXP_APK_MAIN_PREFIX)
				&& !newExpAPKFile.equalsIgnoreCase(di.mFileName);
	}

	/**
	 * Delete old expansion apk file used by previous version of app
	 */
	private boolean deleteOldExpAPKFile(String pathToFile) {
		LogUtils.printLog("deleteOldExpAPKFile", LOG_TAG, DEBUG);
		File oldExpAPKFile = new File(pathToFile);
		if (oldExpAPKFile.exists()) {
			return oldExpAPKFile.delete();
		} else {
			return false;
		}
	}

}
