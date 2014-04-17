package com.ford.syncV4.android.policies;

import android.os.Environment;

import com.ford.syncV4.android.constants.Const;

import junit.framework.TestCase;

import java.io.File;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 15.04.14
 * Time: 16:52
 */
public class PolicyFilesManagerTest extends TestCase {

    /**
     * Run this test only on real android device
     */
    public void _testSavePolicyTableSnapshotCreatesDirAndFile() {
        PolicyFilesManager.savePolicyTableSnapshot(new byte[1024]);
        File file = new File(Environment.getExternalStorageDirectory() + "/" +
                Const.TEST_DATA_DIR_NAME, "policyTableSnapshot.json");
        assertTrue(file.exists());
        assertFalse(file.isDirectory());
    }
}