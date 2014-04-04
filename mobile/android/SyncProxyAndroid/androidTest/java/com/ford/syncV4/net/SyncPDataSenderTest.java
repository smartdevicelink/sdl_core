package com.ford.syncV4.net;

import android.test.InstrumentationTestCase;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCRequest;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 20.03.14
 * Time: 17:59
 */
public class SyncPDataSenderTest extends InstrumentationTestCase {

    private static final String LOG_TAG = "SyncPDataSenderTest";
    private static final String URL = "http://policies.telematicsqa.ford.com/api/policies";
    private static final String SYNC_P_DATA_BASE64_PAYLOAD_FILE_NAME = "syncPDataBase64EncodedPayload";
    private static final int TIMEOUT = 1000;
    /**
     * In milliseconds
     */
    private static final int AWAIT_TIMEOUT = 1000;

    private static String sSyncPDataBase64Payload = "";

    public void testSendSyncPData() throws SyncException, UnsupportedEncodingException,
            InterruptedException {

        SyncPDataSender syncPDataSender = new SyncPDataSender();

        final CountDownLatch countDownLatch = new CountDownLatch(1);
        final boolean[] result = {false};

        syncPDataSender.sendPData(URL, getSyncPDataBase64Payload(),
                TIMEOUT, new SyncPDataSender.SyncPDataSenderCallback() {
                    @Override
                    public void onComplete(RPCRequest rpcRequest) {
                        result[0] = true;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onError(String message) {
                        countDownLatch.countDown();
                    }
                });

        countDownLatch.await(AWAIT_TIMEOUT, TimeUnit.MILLISECONDS);

        assertTrue(result[0]);
    }

    /*public void testSendEncodedSyncPData() throws SyncException, UnsupportedEncodingException,
            InterruptedException {
        Vector<String> encodedData = new Vector<String>();
        encodedData.add(SyncPDataSender.TEST_BASE_64_ENCODED_PAYLOAD);
        encodedData.add(SyncPDataSender.TEST_BASE_64_ENCODED_PAYLOAD);
        encodedData.add(SyncPDataSender.TEST_BASE_64_ENCODED_PAYLOAD);

        String url = "http://policies.telematicsqa.ford.com/api/policies";
        int timeOut = 1000;

        SyncProxyBase syncProxyBase = getSyncProxyBase();
        syncProxyBase.sendEncodedSyncPDataToUrl(url, encodedData, timeOut);

        Thread.sleep(10000000);
    }*/

    private String getSyncPDataBase64Payload() {
        if (sSyncPDataBase64Payload.isEmpty()) {
            sSyncPDataBase64Payload = getAssetsFileContent(SYNC_P_DATA_BASE64_PAYLOAD_FILE_NAME);
        }
        return sSyncPDataBase64Payload;
    }

    private String getAssetsFileContent(String fileName) {
        StringBuilder builder = new StringBuilder();
        BufferedReader reader = null;
        try {
            reader = new BufferedReader(new InputStreamReader(
                    getInstrumentation().getTargetContext().getAssets().open(fileName)));
            String line;
            while ((line = reader.readLine()) != null) {
                builder.append(line);
            }
        } catch (IOException e) {
            Log.d(LOG_TAG, "Can't open file", e);
        } finally {
            if (reader != null) {
                try {
                    reader.close();
                } catch (IOException e) {
                    Log.e(LOG_TAG, e.toString());
                }
            }
        }
        return builder.toString().trim();
    }
}