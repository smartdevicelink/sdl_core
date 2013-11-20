package com.ford.avarsdl.util;

import android.content.Context;
import android.util.Log;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Level;
import org.apache.log4j.PatternLayout;
import org.apache.log4j.RollingFileAppender;

import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

public class Logger {

    public static final String LOG_TAG = "OPEN_XC";
	public static final String ERROR_LOG_PREFIX = "LOG_ERR: ";
	private static final String LOG_FILENAME = "OpenXC.log";
	private static final int MAX_BACKUP_INDEX = 3;
	private static final String MAX_FILE_SIZE = "750KB";

    private static String intlogsDirectory;
	private static final org.apache.log4j.Logger logger = org.apache.log4j.Logger.getLogger(com.openxc.util.Logger.class);
    private static boolean isEnableVerboseLog;
    
	//TODO
	//Implement LogCat appender
	//Need to be called from main activity onCreate;
	//Implement Properties from File
	public static void initLogger(Context context, boolean verboseEnabled) {
        initLogsDirectories(context);
        enableVerboseLog(verboseEnabled);
		String fileName = getCurrentLogsDirectory() + "/" + LOG_FILENAME;
		logger.setLevel(Level.DEBUG);
		org.apache.log4j.Layout layout = new PatternLayout("%d [%t] %-5p %m%n");
        try {
            logger.removeAllAppenders();
        } catch (Exception e){
            com.openxc.util.Logger.e("Unable to remove logger uppenders.");
        }
		try {
			RollingFileAppender rollingFileAppender = new RollingFileAppender(layout, fileName);
			rollingFileAppender.setMaxFileSize(MAX_FILE_SIZE);
			rollingFileAppender.setMaxBackupIndex(MAX_BACKUP_INDEX);
			logger.addAppender(rollingFileAppender);
		} catch(IOException ioe) {
			Log.e("OPEN_XC", "unable to create log file: " + fileName);
		}
        Logger.d("Current log stored to " + fileName);
	}
    
    public static void enableVerboseLog(boolean enable) {
//#if isdebug
//@        isEnableVerboseLog = true;
//#else
        isEnableVerboseLog = enable;
//#endif
    }

    public static boolean isVerboseModeEnabled() {
        return isEnableVerboseLog;
    }

    private static void initLogsDirectories(Context context) {
        intlogsDirectory = context.getFilesDir() + "/logs";
    }
        
    public static String getCurrentLogsDirectory() {
        if (AppUtils.externalStorageAvailable()) {
            String extLogsDirectory = AppUtils.getExternalStorageDir();
            if (StringUtils.isNotEmpty(extLogsDirectory)) {
                return extLogsDirectory + "/logs";
            }
        }
        return intlogsDirectory;
    }

    public static boolean isUsingExternalStorage() {
        return AppUtils.externalStorageAvailable();
    }
	
	public static File[] getLogsDirectories() {
        if (AppUtils.externalStorageAvailable()) {
            String extLogsDirectory = AppUtils.getExternalStorageDir();
            if (StringUtils.isNotEmpty(extLogsDirectory)) {
                return new File[] {new File(intlogsDirectory), new File(extLogsDirectory + "/logs")};
            }
        }
		return new File[] {new File(intlogsDirectory)} ;
	}

    public static File[] getAllLogs() {
        ArrayList<File> logs = new ArrayList<File>();
        File[] logDirs = getLogsDirectories();
        for (File dir: logDirs) {
            if (dir.exists()) {
                logs.addAll(Arrays.asList(getLogs(dir)));
            }
        }
        return logs.toArray(new File[logs.size()]);
    }

    public static File[] getInternalLogs() {
        return getLogs(new File(intlogsDirectory));
    }
    
    private static File[] getLogs(File directory) {
        if (directory.isFile()) {
            throw new IllegalArgumentException("directory is not folder " + directory.getAbsolutePath());
        }
        return directory.listFiles(new FilenameFilter(){
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
	public static void v(String logMsg) {
        v(logMsg, (Throwable) null);
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
            logger.error(logMsg, t);
            Log.e(LOG_TAG, logMsg, t);
        } else {
            logger.error(logMsg);
            Log.e(LOG_TAG, logMsg);
        }
    }
    public static void w(String logMsg, Throwable t) {
        if (t != null) {
            logger.warn(logMsg, t);
            Log.w(LOG_TAG, logMsg, t);
        } else {
            logger.warn(logMsg);
            Log.w(LOG_TAG, logMsg);
        }
    }
    public static void i(String logMsg, Throwable t) {
        if (t != null) {
            logger.info(logMsg, t);
            Log.i(LOG_TAG, logMsg, t);
        } else {
            logger.info(logMsg);
            Log.i(LOG_TAG, logMsg);
        }
    }
    public static void d(String logMsg, Throwable t) {
        if (t != null) {
            logger.debug(logMsg, t);
            Log.d(LOG_TAG, logMsg, t);
        } else {
            logger.debug(logMsg);
            Log.d(LOG_TAG, logMsg);
        }
    }
    public static void v(String logMsg, Throwable t) {
        if (isEnableVerboseLog) {
            if (t != null) {
                logger.debug(logMsg, t);
                Log.d(LOG_TAG, logMsg, t);
            } else {
                logger.debug(logMsg);
                Log.d(LOG_TAG, logMsg);
            }
        }
    }
}