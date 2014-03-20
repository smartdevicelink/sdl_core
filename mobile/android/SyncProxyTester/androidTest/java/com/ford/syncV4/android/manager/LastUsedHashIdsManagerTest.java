package com.ford.syncV4.android.manager;

import android.test.InstrumentationTestCase;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/28/14
 * Time: 1:56 PM
 */
public class LastUsedHashIdsManagerTest extends InstrumentationTestCase {

    public void testInitManagerContainEmptyCollection() {
        LastUsedHashIdsManager lastUsedHashIdsManager = new LastUsedHashIdsManager();
        assertNotNull(lastUsedHashIdsManager.getDataForAdapter());
        assertEquals(0, lastUsedHashIdsManager.getDataForAdapter().length);
    }

    public void testAddIdToManager() {
        String id = "1234567890";
        LastUsedHashIdsManager lastUsedHashIdsManager = new LastUsedHashIdsManager();
        lastUsedHashIdsManager.addNewId(id);
        assertEquals(id, lastUsedHashIdsManager.getDataForAdapter()[0]);
    }

    public void testAddNullIdToManager() {
        String id = null;
        LastUsedHashIdsManager lastUsedHashIdsManager = new LastUsedHashIdsManager();
        lastUsedHashIdsManager.addNewId(id);
        assertEquals(id, lastUsedHashIdsManager.getDataForAdapter()[0]);
    }

    public void testFirstInLastOut() {
        String id_1 = "1234567890";
        String id_2 = "qwertyuiop";
        LastUsedHashIdsManager lastUsedHashIdsManager = new LastUsedHashIdsManager();
        lastUsedHashIdsManager.addNewId(id_1);
        lastUsedHashIdsManager.addNewId(id_2);

        assertEquals(id_2, lastUsedHashIdsManager.getDataForAdapter()[0]);
        assertEquals(id_1, lastUsedHashIdsManager.getDataForAdapter()[1]);
    }

    public void testSaveAndInitDataFILO() {
        String id_1 = "1234567890";
        String id_2 = "qwertyuiop";
        LastUsedHashIdsManager lastUsedHashIdsManager = new LastUsedHashIdsManager();
        lastUsedHashIdsManager.addNewId(id_1);
        lastUsedHashIdsManager.addNewId(id_2);
        lastUsedHashIdsManager.save();

        LastUsedHashIdsManager lastUsedHashIdsManagerNew = new LastUsedHashIdsManager();
        lastUsedHashIdsManagerNew.init();
        assertEquals(id_2, lastUsedHashIdsManagerNew.getDataForAdapter()[0]);
        assertEquals(id_1, lastUsedHashIdsManagerNew.getDataForAdapter()[1]);
    }

    public void testRemoveObsoleteIds() {
        LastUsedHashIdsManager lastUsedHashIdsManager = new LastUsedHashIdsManager();
        String id_base = "1234567890";
        int n = 10;
        for (int i = 0 ; i < n; i++) {
            lastUsedHashIdsManager.addNewId(id_base + "_" + i);
        }

        assertEquals(LastUsedHashIdsManager.LAST_USED_IDS_NUMBER,
                lastUsedHashIdsManager.getDataForAdapter().length);
        assertEquals(id_base + "_" + (n - 1),
                lastUsedHashIdsManager
                        .getDataForAdapter()[0]);
        assertEquals(id_base + "_" + (n - LastUsedHashIdsManager.LAST_USED_IDS_NUMBER),
                lastUsedHashIdsManager
                        .getDataForAdapter()[LastUsedHashIdsManager.LAST_USED_IDS_NUMBER - 1]);
    }
}