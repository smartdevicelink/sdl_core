package com.ford.syncV4.util.logger;

import android.content.Context;
import android.os.Build;
import android.os.Debug;
import android.util.Log;

import com.ford.syncV4.util.CommonUtils;
import com.ford.syncV4.util.MemoryUtils;
import com.ford.syncV4.util.StringUtils;

import org.apache.log4j.Layout;
import org.apache.log4j.Level;
import org.apache.log4j.PatternLayout;
import org.apache.log4j.RollingFileAppender;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.zip.GZIPOutputStream;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 03.04.14
 * Time: 14:59
 */
public class Logger {

    public static final String LOG_TAG = "SYNC_V4";

    private static final String lineEnd = "\r\n";
    private static final String ERROR_LOG_PREFIX = "LOG_ERR: ";
    private static final String LOG_FILENAME = "SyncV4.log";
    private static final int MAX_BACKUP_INDEX = 3;
    private static final String MAX_FILE_SIZE = "750KB";

    private static String sInitLogsDirectory;
    private static final org.apache.log4j.Logger sLogger =
            org.apache.log4j.Logger.getLogger(Logger.class);

    /**
     * Initialize Logger, create directory and file to collect logs
     *
     * @param context applications context
     */
    public static void initLogger(Context context) {
        initLogsDirectories(context);
        String fileName = getCurrentLogsDirectory(context) + "/" + LOG_FILENAME;
        sLogger.setLevel(Level.DEBUG);
        Layout layout = new PatternLayout("%d [%t] %-5p %m%n");
        try {
            sLogger.removeAllAppenders();
        } catch (Exception e) {
            Logger.e("Unable to remove sLogger uppenders.");
        }
        try {
            RollingFileAppender rollingFileAppender = new RollingFileAppender(layout, fileName);
            rollingFileAppender.setMaxFileSize(MAX_FILE_SIZE);
            rollingFileAppender.setMaxBackupIndex(MAX_BACKUP_INDEX);
            sLogger.addAppender(rollingFileAppender);
        } catch (IOException ioe) {
            Log.e(LOG_TAG, "unable to create log file: " + fileName);
        }
        Logger.d("Current log stored to " + fileName);
    }

    public static String getCurrentLogsDirectory(Context context) {
        if (CommonUtils.externalStorageAvailable()) {
            String extLogsDirectory = CommonUtils.getExternalStorageDir(context);
            if (StringUtils.isNotEmpty(extLogsDirectory)) {
                return extLogsDirectory + "/logs";
            }
        }
        return sInitLogsDirectory;
    }

    public static File[] getLogsDirectories(Context context) {
        if (CommonUtils.externalStorageAvailable()) {
            String extLogsDirectory = CommonUtils.getExternalStorageDir(context);
            if (StringUtils.isNotEmpty(extLogsDirectory)) {
                return new File[] {
                        new File(sInitLogsDirectory), new File(extLogsDirectory + "/logs")
                };
            }
        }
        return new File[] {
                new File(sInitLogsDirectory)
        };
    }

    public static File[] getAllLogs(Context context) {
        ArrayList<File> logs = new ArrayList<File>();
        File[] logDirs = getLogsDirectories(context);
        for (File dir : logDirs) {
            if (dir.exists()) {
                logs.addAll(Arrays.asList(getLogs(dir)));
            }
        }
        return logs.toArray(new File[logs.size()]);
    }

    /**
     * ZIP all application setting in single file
     *
     * @param context                       context of the application
     * @param applicationAdditionalSettings {@link com.ford.syncV4.util.logger.ApplicationAdditionalSettings} object
     * @return a URL to the ZIPed file
     * @throws IOException
     */
    public static String zipLogFiles(Context context,
                                     ApplicationAdditionalSettings applicationAdditionalSettings)
            throws IOException {
        String zipFileName = getCurrentLogsDirectory(context) + "/SyncV4Logs-" + getFileNameAsDate() + ".gz";
        GZIPOutputStream outputStream = new GZIPOutputStream(new FileOutputStream(new File(zipFileName)));

        outputStream.write(lineEnd.getBytes("UTF-8"));

        String additionalInfo = getAdditionInfo(applicationAdditionalSettings);

        outputStream.write(additionalInfo.getBytes("UTF-8"));

        File[] logs = getAllLogs(context);
        PrintWriter printWriter = new PrintWriter(outputStream);

        for (File log : logs) {
            BufferedReader bufferedReader = new BufferedReader(new FileReader(log.getPath()));
            String line = bufferedReader.readLine();
            while (line != null) {
                printWriter.println(line);
                line = bufferedReader.readLine();
            }
            bufferedReader.close();
        }

        outputStream.write(lineEnd.getBytes("UTF-8"));

        printWriter.flush();
        outputStream.flush();

        printWriter.close();
        outputStream.close();
        return zipFileName;
    }

    public static File[] getInternalLogs() {
        return getLogs(new File(sInitLogsDirectory));
    }

    public static void removeAllLogs(Context context) {
        removeFiles(Logger.getAllLogs(context));
        Logger.initLogger(context);
    }

    public static void e(String logMsg) {
        e(logMsg, (Throwable) null);
    }

    public static void w(String logMsg) {
        w(logMsg, (Throwable) null);
    }

    public static void i(String logMsg) {
        i(logMsg, (Throwable) null);
    }

    public static void d(String logMsg) {
        d(logMsg, (Throwable) null);
    }

    public static void e(String logPrefix, String logMsg) {
        e(logPrefix + logMsg);
    }

    public static void w(String logPrefix, String logMsg) {
        w(logPrefix + logMsg);
    }

    public static void i(String logPrefix, String logMsg) {
        i(logPrefix + logMsg);
    }

    public static void d(String logPrefix, String logMsg) {
        d(logPrefix + logMsg);
    }

    public static void e(String logMsg, Throwable t) {
        logMsg = ERROR_LOG_PREFIX + logMsg;
        if (t != null) {
            sLogger.error(logMsg, t);
            Log.e(LOG_TAG, logMsg, t);
        } else {
            sLogger.error(logMsg);
            Log.e(LOG_TAG, logMsg);
        }
    }

    public static void w(String logMsg, Throwable t) {
        if (t != null) {
            sLogger.warn(logMsg, t);
            Log.w(LOG_TAG, logMsg, t);
        } else {
            sLogger.warn(logMsg);
            Log.w(LOG_TAG, logMsg);
        }
    }

    public static void i(String logMsg, Throwable t) {
        if (t != null) {
            sLogger.info(logMsg, t);
            Log.i(LOG_TAG, logMsg, t);
        } else {
            sLogger.info(logMsg);
            Log.i(LOG_TAG, logMsg);
        }
    }

    public static void d(String logMsg, Throwable t) {
        if (t != null) {
            sLogger.debug(logMsg, t);
            Log.d(LOG_TAG, logMsg, t);
        } else {
            sLogger.debug(logMsg);
            Log.d(LOG_TAG, logMsg);
        }
    }

    private static void removeFiles(File[] files) {
        if (files == null) {
            return; //may be mounted device
        }
        for (File file : files) {
            if (!file.delete()) {
                Logger.w("Unable to remove log file: " + file.getName());
            }
        }

        Logger.i("All files where removed");
    }

    private static void initLogsDirectories(Context context) {
        sInitLogsDirectory = context.getFilesDir() + "/logs";
    }

    private static File[] getLogs(File directory) {
        if (directory.isFile()) {
            throw new IllegalArgumentException("directory is not folder " + directory.getAbsolutePath());
        }
        return directory.listFiles(new FilenameFilter() {
            public boolean accept(File dir, String name) {
                if (name != null && name.toLowerCase().endsWith(".log")) {
                    return true;
                }
                for (int i = 1; i <= MAX_BACKUP_INDEX; i++) {
                    if (name != null && name.toLowerCase().endsWith(".log." + i)) {
                        return true;
                    }
                }
                return false;
            }
        });
    }

    /**
     * Return string with addition info about device, application name and other.
     *
     * @return String with addition info (App name, version ...)
     */
    private static String getAdditionInfo(ApplicationAdditionalSettings settings) {
        StringBuilder addInfo = new StringBuilder(1000);
        addInfo.append("App name: ").append(settings.getAppName()).append("\n");
        addInfo.append("App version: ").append(settings.getAppVersion()).append(";  ").append(settings.getBuildDate()).append("\n");
        if (settings.getApplicationInfo() != null) {
            addInfo.append("App dir: ").append(settings.getApplicationInfo().dataDir).append("\n");
            addInfo.append("App source dir: ").append(settings.getApplicationInfo().sourceDir).append("\n");
            addInfo.append("App public source dir: ").append(settings.getApplicationInfo().publicSourceDir).append("\n");
        }
        addInfo.append("\n------- Device -----------\n");
        addInfo.append("Brand: ").append(Build.BRAND).append("\n");
        addInfo.append("Board: ").append(Build.BOARD).append("\n");
        addInfo.append("Device: ").append(Build.DEVICE).append("\n");
        addInfo.append("Model: ").append(Build.MODEL).append("\n");
        addInfo.append("Id: ").append(Build.ID).append("\n");
        addInfo.append("Product: ").append(Build.PRODUCT).append("\n");
        addInfo.append("Display: ").append(Build.DISPLAY).append("\n");
        addInfo.append("--------- Firmware ------------\n");
        addInfo.append("SDK: ").append(Build.VERSION.SDK_INT).append("\n");
        addInfo.append("Release: ").append(Build.VERSION.RELEASE).append("\n");
        addInfo.append("Tags: ").append(Build.TAGS).append("\n");
        addInfo.append("Incremental: ").append(Build.VERSION.INCREMENTAL).append("\n");
        addInfo.append("--------- Misc -------------\n");
        addInfo.append("Total space: ").append(MemoryUtils.roundMemorySize(MemoryUtils.getTotalInternalMemorySize())).append("\n");
        addInfo.append("Avail space: ").append(MemoryUtils.roundMemorySize(MemoryUtils.getAvailableInternalMemorySize())).append("\n");
        addInfo.append("Total memory: ").append(MemoryUtils.roundMemorySize(Runtime.getRuntime().totalMemory())).append("\n");
        addInfo.append("Avail memory: ").append(MemoryUtils.roundMemorySize(Runtime.getRuntime().freeMemory())).append("\n");
        addInfo.append("Native memory: ").append(MemoryUtils.roundMemorySize(Debug.getNativeHeapAllocatedSize())).append("\n");
        addInfo.append("-------------------------------\n\n");
        return addInfo.toString();
    }

    private static String getFileNameAsDate() {
        Date date = new Date(System.currentTimeMillis());
        return new SimpleDateFormat("dd-MM-yyyy-H:m").format(date);
    }
}