package com.ford.syncV4.android.utils;

import android.content.pm.ApplicationInfo;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.os.Looper;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.util.logger.ApplicationAdditionalSettings;
import com.ford.syncV4.util.logger.Logger;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 1/30/14
 * Time: 2:15 PM
 */
public class AppUtils {

    @SuppressWarnings("unused")
    private static final String TAG = AppUtils.class.getSimpleName();

    public static boolean isRunningUIThread() {
        return Looper.myLooper() == Looper.getMainLooper();
    }

    /**
     * Returns the file contents from the specified resource.
     *
     * @param resource Resource id (in res/ directory)
     * @return         The resource file's contents
     */
    public static byte[] contentsOfResource(int resource) {
        return contentsOfResource(MainApp.getInstance().getResources().openRawResource(resource));
    }

    /**
     * Read a File and return bytes array
     * @param file
     * @return
     */
    public static byte[] contentsOfResource(File file) {
        try {
            return contentsOfResource(new FileInputStream(file));
        } catch (FileNotFoundException e) {
            Logger.e(TAG + " Contents Of Resource exception", e.getMessage());
        }
        return new byte[0];
    }

    /**
     *
     * @param fileName
     * @return
     */
    public static byte[] contentsOfResource(String fileName) {
        return contentsOfResource(new File(fileName));
    }

    /**
     * Save bytes to a file
     * @param data
     * @param filePath
     * @return
     */
    public static boolean saveDataToFile(byte[] data, String filePath) {
        File file = new File(filePath);
        Logger.d(TAG + " Saving data to file '" + filePath + "', exists:" + file.exists());
        if (file.exists()) {
            file.delete();
        }
        FileOutputStream mFileOutputStream = null;
        boolean result = false;
        try {
            mFileOutputStream = new FileOutputStream(file.getPath());
            mFileOutputStream.write(data);

            result = true;
        } catch (IOException e) {
            Logger.e(TAG + " Save Data To File IOException", e);
        } finally {
            if (mFileOutputStream != null) {
                try {
                    mFileOutputStream.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }

    /**
     *
     * @param data
     * @param dirName
     * @param fileName
     * @return
     */
    public static boolean saveDataToFile(byte[] data, String dirName, String fileName) {
        createDirIfNeeded(dirName);
        return saveDataToFile(data, dirName + "/" + fileName);
    }

    /**
     * Build and return an object which describes Application additional settings needed for the
     * Logger
     * @return {@link com.ford.syncV4.util.logger.ApplicationAdditionalSettings} object
     */
    public static ApplicationAdditionalSettings getAppAdditionalSettings() {
        ApplicationAdditionalSettings settings = new ApplicationAdditionalSettings();
        settings.setApplicationInfo(getApplicationInfo());
        settings.setAppName("Sync Proxy Tester");
        settings.setAppVersion(getApplicationVersion());
        settings.setBuildDate(getAssetsContents("build.info", "Build info not available"));
        return settings;
    }

    /**
     * Read a content of the Input Stream into bytes array
     *
     * @param inputStream a stream of incoming bytes
     * @return            bytes array
     */
    private static byte[] contentsOfResource(InputStream inputStream) {
        try {
            ByteArrayOutputStream byteArrayOutputStream =
                    new ByteArrayOutputStream(inputStream.available());
            final int bufferSize = 4096;
            final byte[] buffer = new byte[bufferSize];
            int available;
            while ((available = inputStream.read(buffer)) >= 0) {
                byteArrayOutputStream.write(buffer, 0, available);
            }
            return byteArrayOutputStream.toByteArray();
        } catch (IOException e) {
            Logger.w(TAG + " Can't read file", e);
            return null;
        } finally {
            if (inputStream != null) {
                try {
                    inputStream.close();
                } catch (IOException e) {
                    Logger.e(TAG, e.toString());
                }
            }
        }
    }

    /**
     * @return an instance of the {@link android.content.pm.ApplicationInfo} class
     */
    public static ApplicationInfo getApplicationInfo() {
        PackageInfo packageInfo = getPackageInfo();
        if (packageInfo != null) {
            return packageInfo.applicationInfo;
        } else {
            Logger.w("Can't get application info");
            return null;
        }
    }

    /**
     * @return application version as string
     */
    public static String getApplicationVersion() {
        PackageInfo packageInfo = getPackageInfo();
        if (packageInfo != null) {
            return packageInfo.versionName;
        } else {
            Logger.w("Can't get application version");
            return "?";
        }
    }

    /**
     * @return application version as int
     */
    public static int getApplicationVersionNumber() {
        String[] versions = getApplicationVersion().split("\\.");
        if(versions.length == 2) {
            String versionMajor = versions[0];
            String versionMinor = versions[1];
            return (Integer.valueOf(versionMajor) * 100) + Integer.valueOf(versionMinor);
        }
        return 0;
    }

    /**
     * @return application code version
     */
    public static int getCodeVersionNumber() {
        PackageInfo packageInfo = getPackageInfo();
        if (packageInfo != null) {
            return packageInfo.versionCode;
        } else {
            Logger.w("Can't get code version");
            return 0;
        }
    }

    /**
     * @return information about application build
     */
    public static String getBuildInfo() {
        return getAssetsContents("build.info", "Build info not available");
    }

    /**
     * @return text of the CHANGELOG file
     */
    public static String getChangeLog() {
        return getAssetsContents("CHANGELOG.txt", "Changelog not available");
    }

    /**
     * @return application package information
     */
    private static PackageInfo getPackageInfo() {
        final PackageManager pm = MainApp.getInstance().getPackageManager();
        if (pm == null) {
            Logger.w("Package manager is NULL");
            return null;
        }
        String packageName = "";
        try {
            packageName = MainApp.getInstance().getPackageName();
            return pm.getPackageInfo(packageName, 0);
        } catch (PackageManager.NameNotFoundException e) {
            Logger.e("Failed to find PackageInfo : " + packageName);
            return null;
        } catch (RuntimeException e) {
            // To catch RuntimeException("Package manager has died") that can occur on some version of Android,
            // when the remote PackageManager is unavailable. I suspect this sometimes occurs when the App is being reinstalled.
            Logger.e("Package manager has died : " + packageName);
            return null;
        } catch (Throwable e) {
            Logger.e("Package manager has Throwable : " + e);
            return null;
        }
    }

    private static String getAssetsContents(String filename, String defaultString) {
        StringBuilder builder = new StringBuilder();
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new InputStreamReader(
                    MainApp.getInstance().getAssets().open(filename)));
            String line;
            while ((line = reader.readLine()) != null) {
                builder.append(line + "\n");
            }
        } catch (IOException e) {
            Logger.d("Can't open file with build info", e);
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    Logger.e(e.toString());
                }
            }
        }
        return builder.length() > 0 ? builder.toString().trim() : defaultString;
    }

    /**
     * This method creates a directory with given name is such does not exists
     *
     * @param path a path to the directory
     */
    public static void createDirIfNeeded(String path) {
        File file = new File(path);
        if (file.exists() && !file.isDirectory()) {
            file.delete();
        }
        if (!file.exists()) {
            file.mkdirs();
        }
    }
}