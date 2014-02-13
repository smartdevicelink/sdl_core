package com.ford.syncV4.android.utils;

import android.os.Environment;

import junit.framework.TestCase;

import java.io.File;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/13/14
 * Time: 11:37 AM
 */
public class AppUtilsTest extends TestCase {

    public void testReadFromFile() {

        // In order to perform this test a file "policyTableUpdate.json" must be located at SD Card

        String mPolicyTableUpdatePath = Environment.getExternalStorageDirectory() +
                "/policyTableUpdate.json";
        File mPolicyUpdateFile = new File(mPolicyTableUpdatePath);
        assertTrue(mPolicyUpdateFile.exists());
        byte[] data = AppUtils.readDataFromFile(mPolicyUpdateFile);
        assertNotNull(data);
    }
}