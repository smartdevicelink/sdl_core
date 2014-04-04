package com.ford.syncV4.util;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 04.04.14
 * Time: 12:15
 */

import android.os.Environment;
import android.os.StatFs;

import java.io.File;

/**
 * This class provide methods to monitor memory parameters
 */
public class MemoryUtils {

    /**
     * @return total internal memory size
     */
    public static long getTotalInternalMemorySize() {
        File path = Environment.getDataDirectory();
        StatFs stat = new StatFs(path.getPath());
        long blockSize = stat.getBlockSize();
        long totalBlocks = stat.getBlockCount();
        return totalBlocks * blockSize;
    }

    /**
     * @return available internal memory size
     */
    public static long getAvailableInternalMemorySize() {
        File path = Environment.getDataDirectory();
        StatFs stat = new StatFs(path.getPath());
        long blockSize = stat.getBlockSize();
        long availableBlocks = stat.getAvailableBlocks();
        return availableBlocks * blockSize;
    }

    /**
     * Round memory size to human readable value
     * @param memSize memory size
     * @return string value
     */
    public static String roundMemorySize(long memSize) {
        if (memSize < 1024) {
            return memSize + "b";
        }
        if (memSize < 1024 * 1024) {
            int kbSize = (int) (memSize / 1024);
            return kbSize + "Kb";
        }
        if (memSize < 1024 * 1024 * 1024) {
            double mbSize = 100 * memSize / (1024 * 1024);
            mbSize = Math.round(mbSize) / 100.0;
            return mbSize + "Mb";
        }
        double mbSize = 100 * memSize / (1024 * 1024 * 1024);
        mbSize = Math.round(mbSize) / 100.0;
        return mbSize + "Gb";
    }
}