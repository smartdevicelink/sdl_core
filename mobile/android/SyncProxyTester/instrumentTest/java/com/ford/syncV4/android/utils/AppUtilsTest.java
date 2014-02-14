package com.ford.syncV4.android.utils;

import android.os.Environment;

import junit.framework.TestCase;

import java.io.File;
import java.nio.charset.Charset;
import java.util.Random;

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
        byte[] data = AppUtils.contentsOfResource(mPolicyUpdateFile);
        assertNotNull(data);
    }

    public void testSaveDataToFile() {
        String mPath = Environment.getExternalStorageDirectory() +
                "/dataToBeSaved.json";
        byte[] mData = getRandomBytes(256);

        assertTrue(AppUtils.saveDataToFile(mData, mPath));
    }

    public void testSaveAndReadDataFromFile() {
        String mPath = Environment.getExternalStorageDirectory() +
                "/dataToBeSavedAndRead.json";
        String mString = "{\"glossary\": {\"title\": \"example glossary\", \"GlossDiv\": {\"title\": " +
                "\"S\", \"GlossList\": {\"GlossEntry\": {\"ID\": \"SGML\", \"SortAs\": \"SGML\", " +
                "\"GlossTerm\": \"Standard Generalized Markup Language\", \"Acronym\": \"SGML\", " +
                "\"Abbrev\": \"ISO 8879:1986\", \"GlossDef\": {\"para\": \"A meta-markup language, " +
                "used to create markup languages such as DocBook.\", \"GlossSeeAlso\": [\"GML\", " +
                "\"XML\"]}, \"GlossSee\": \"markup\" }}}}}";
        byte[] mData = mString.getBytes(Charset.forName("UTF-8"));
        AppUtils.saveDataToFile(mData, mPath);
        byte[] mReadData = AppUtils.contentsOfResource(new File(mPath));
        String mReadString = new String(mReadData);

        assertEquals(mData.length, mReadData.length);
        assertEquals(mString, mReadString);
    }

    private static byte[] getRandomBytes(int dataSize) {
        // to simplify matching if bytes mess up
        final byte[] data = new byte[dataSize];
        new Random().nextBytes(data);
        return data;
    }
}