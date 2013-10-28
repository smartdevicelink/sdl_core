/**
 * 
 */
package com.ford.avatar.util;

import android.util.Log;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * @author KBotnar
 * 
 */
public class LogUtils {
    private final static boolean LOG_TO_FILE = false;
    /** enabled debug messages, for release should be false */
    public final static boolean DEBUG = false;

    /**
     * log information
     * @param msg
     * 		message to log
     * @param TAG
     * 		TAG
     * @param DEBUG
     * 		true or false
     */
    public static void printLog(String msg, String TAG, boolean DEBUG) {
        if (DEBUG && LogUtils.DEBUG) {
            Log.i(TAG, msg);
            if (LOG_TO_FILE) {
                Calendar c = Calendar.getInstance();
                SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss,");
                String formattedDate = df.format(c.getTime());
                String str = formattedDate + " : " + TAG + " ---> " + msg
                        + "\n";
                writeToFile(str);
            }
        }
    }

    public static void printException(Exception e, String func, String TAG,
            boolean DEBUG) {
        if (DEBUG && LogUtils.DEBUG) {
            Log.i(TAG, "Exception in " + func);
            e.printStackTrace();
            if (LOG_TO_FILE) {
                Calendar c = Calendar.getInstance();
                SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm,");
                String formattedDate = df.format(c.getTime());
                String str = formattedDate + " : " + TAG + " ---> "
                        + "Exception in " + func + " || " + e.toString() + "\n";
                writeToFile(str);
            }
        }
    }

    private static void writeToFile(String text) {
        File logFile = new File("sdcard/MFTGLog.txt");
        if (!logFile.exists()) {
            try {
                logFile.createNewFile();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        try {
            // BufferedWriter for performance, true to set append to file flag
            BufferedWriter buf = new BufferedWriter(new FileWriter(logFile,
                    true));
            buf.append(text);
            buf.newLine();
            buf.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }

}
