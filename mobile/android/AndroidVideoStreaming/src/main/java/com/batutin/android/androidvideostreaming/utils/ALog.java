package com.batutin.android.androidvideostreaming.utils;


import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.io.Writer;
import java.net.UnknownHostException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class ALog {
    private static final DateFormat FLOG_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
    private static final File LOG_DIR = new File(
            Environment.getExternalStorageDirectory() + File.separator + "alog");
    ;
    private static final BlockingQueue<String> logQueue = new LinkedBlockingQueue<String>();
    private static boolean fileLogging = false;
    private static String tag = "<tag unset>";
    private static Level level = Level.V;
    private static Runnable queueRunner = new Runnable() {
        @Override
        public void run() {
            String line;
            try {
                while ((line = logQueue.take()) != null) {

                    if (!Environment.getExternalStorageState().equals(
                            Environment.MEDIA_MOUNTED)) {
                        continue;
                    }
                    if (!LOG_DIR.exists() && !LOG_DIR.mkdirs()) {
                        continue;
                    }

                    File logFile = new File(LOG_DIR, tag + ".log");
                    Writer w = null;
                    try {
                        w = new FileWriter(logFile, true);
                        w.write(line);
                        w.close();
                    } catch (IOException e) {
                    } finally {
                        if (w != null) {
                            try {
                                w.close();
                            } catch (IOException e1) {
                            }
                        }
                    }
                }
            } catch (InterruptedException e) {
            }
        }
    };

    static {
        new Thread(queueRunner).start();
    }

    private static LogContext getContext() {
        StackTraceElement[] trace = Thread.currentThread().getStackTrace();
        StackTraceElement element = trace[5]; // frame below us; the caller
        LogContext context = new LogContext(element);
        return context;
    }

    private static final String getMessage(String s, Object... args) {
        s = String.format(s, args);
        LogContext c = getContext();
        String msg = c.simpleClassName + "." + c.methodName + "@"
                + c.lineNumber + ": " + s;
        return msg;
    }

    private static String getSimpleClassName(String className) {
        int i = className.lastIndexOf(".");
        if (i == -1) {
            return className;
        }
        return className.substring(i + 1);
    }

    public static void setLevel(Level l) {
        level = l;
    }

    public static void setTag(String t) {
        tag = t;
    }

    public static void setFileLogging(boolean enable) {
        fileLogging = enable;
    }

    public static void v(String format, Object... args) {
        if (level.getValue() > Level.V.getValue()) {
            return;
        }
        String msg = getMessage(format, args);
        Log.v(tag, msg);
        if (fileLogging) {
            flog(Level.V, msg);
        }
    }

    public static void d(String format, Object... args) {
        if (level.getValue() > Level.D.getValue()) {
            return;
        }
        String msg = getMessage(format, args);
        Log.d(tag, msg);
        if (fileLogging) {
            flog(Level.D, msg);
        }
    }

    public static void i(String format, Object... args) {
        if (level.getValue() > Level.I.getValue()) {
            return;
        }
        String msg = getMessage(format, args);
        Log.i(tag, msg);
        if (fileLogging) {
            flog(Level.I, msg);
        }
    }

    public static void w(String format, Object... args) {
        if (level.getValue() > Level.W.getValue()) {
            return;
        }
        String msg = getMessage(format, args);
        Log.w(tag, msg);
        if (fileLogging) {
            flog(Level.W, msg);
        }
    }

    public static void w(String format, Throwable t, Object... args) {
        if (level.getValue() > Level.W.getValue()) {
            return;
        }
        String msg = getMessage(format, args);
        Log.w(tag, msg, t);
        if (fileLogging) {
            flog(Level.W, msg, t);
        }
    }

    public static void e(String format, Object... args) {
        if (level.getValue() > Level.E.getValue()) {
            return;
        }
        String msg = getMessage(format, args);
        Log.e(tag, msg);
        if (fileLogging) {
            flog(Level.E, msg);
        }
    }

    public static void e(String format, Throwable t, Object... args) {
        if (level.getValue() > Level.E.getValue()) {
            return;
        }
        String msg = getMessage(format, args);
        Log.e(tag, msg, t);
        if (fileLogging) {
            flog(Level.E, msg, t);
        }
    }

    public static void trace() {
        try {
            throw new Throwable("dumping stack trace ...");
        } catch (Throwable t) {
            ALog.e("trace:", t);
        }
    }

    public static String getStackTraceString(Throwable tr) {
        if (tr == null) {
            return "";
        }

        Throwable t = tr;
        while (t != null) {
            if (t instanceof UnknownHostException) {
                return "";
            }
            t = t.getCause();
        }

        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);
        tr.printStackTrace(pw);
        return sw.toString();
    }

    private static void flog(Level l, String msg) {
        flog(l, msg, null);
    }

    private static void flog(Level l, String msg, Throwable t) {
        String timeString = FLOG_FORMAT.format(new Date());
        String line = timeString + " " + l.toString() + "/" + tag + ": " + msg
                + "\n";
        if (t != null) {
            line += getStackTraceString(t) + "\n";
        }
        logQueue.offer(line);
    }

    public enum Level {
        V(1), D(2), I(3), W(4), E(5);
        private int value;

        private Level(int value) {
            this.value = value;
        }

        int getValue() {
            return value;
        }
    }

    private static class LogContext {
        // String className;
        String simpleClassName;
        String methodName;
        int lineNumber;

        LogContext(StackTraceElement element) {
            // this.className = element.getClassName();
            this.simpleClassName = getSimpleClassName(element.getClassName());
            this.methodName = element.getMethodName();
            this.lineNumber = element.getLineNumber();
        }
    }
}
