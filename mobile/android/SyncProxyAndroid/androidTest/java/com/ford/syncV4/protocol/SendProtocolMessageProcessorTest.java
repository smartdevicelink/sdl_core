package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.service.ConsecutiveFrameProcessor;
import com.ford.syncV4.util.CommonUtils;
import com.ford.syncV4.util.logger.Logger;

import junit.framework.TestCase;

import java.util.ArrayList;
import java.util.Collection;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 05.05.14
 * Time: 10:31
 */
public class SendProtocolMessageProcessorTest extends TestCase {

    @SuppressWarnings("unused")
    private static final String TAG = SendProtocolMessageProcessorTest.class.getSimpleName();

    private final static byte SESSION_ID = 1;
    private final static int MAX_DATA_SIZE = AbstractProtocol.MTU_SIZE -
            ProtocolConstants.PROTOCOL_FRAME_HEADER_SIZE_V_2;
    private final static int MESSAGES_COUNTER = 100;
    
    private int mCallbackCounter;
    private int mExpectedCallbackCounter;
    private CountDownLatch mCountDownLatch;
    private SendProtocolMessageProcessor mProcessor;

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        
        mCallbackCounter = 0;
        mProcessor = new SendProtocolMessageProcessor();

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        mCallbackCounter++;
                        mCountDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        mProcessor.setCallback(callback);
    }

    /**
     * These tests are almost the same (Copy | Paste) except of the Service Type.
     * Probably there is a way to improve them, but because luck of the time I leave them like
     * they are
     */

    public void testHeartbeatMessagesExecuted() throws InterruptedException {
        mExpectedCallbackCounter = MESSAGES_COUNTER;
        mCountDownLatch = new CountDownLatch(mExpectedCallbackCounter);

        for (int i = 0; i < MESSAGES_COUNTER; i++) {
            mProcessor.processHeartbeat(ProtocolConstants.PROTOCOL_VERSION_TWO, SESSION_ID);
        }

        mCountDownLatch.await(1, TimeUnit.SECONDS);

        mProcessor.shutdownAllExecutors();

        assertEquals(mExpectedCallbackCounter, mCallbackCounter);
    }

    public void testHeartbeatAckMessagesExecuted() throws InterruptedException {
        mExpectedCallbackCounter = MESSAGES_COUNTER;
        mCountDownLatch = new CountDownLatch(mExpectedCallbackCounter);

        for (int i = 0; i < MESSAGES_COUNTER; i++) {
            mProcessor.processHeartbeatAck(ProtocolConstants.PROTOCOL_VERSION_TWO, SESSION_ID);
        }

        mCountDownLatch.await(1, TimeUnit.SECONDS);

        mProcessor.shutdownAllExecutors();

        assertEquals(mExpectedCallbackCounter, mCallbackCounter);
    }

    public void testRPCMessagesExecuted() throws InterruptedException {
        mExpectedCallbackCounter = MESSAGES_COUNTER;
        mCountDownLatch = new CountDownLatch(mExpectedCallbackCounter);

        for (int i = 0; i < MESSAGES_COUNTER; i++) {
            mProcessor.process(ServiceType.RPC, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    false, new byte[0], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        mCountDownLatch.await(1, TimeUnit.SECONDS);

        mProcessor.shutdownAllExecutors();

        assertEquals(mExpectedCallbackCounter, mCallbackCounter);
    }

    public void testMobileNaviMessagesExecuted() throws InterruptedException {
        mExpectedCallbackCounter = MESSAGES_COUNTER;
        mCountDownLatch = new CountDownLatch(mExpectedCallbackCounter);

        for (int i = 0; i < MESSAGES_COUNTER; i++) {
            mProcessor.process(ServiceType.Mobile_Nav, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    false, new byte[0], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        mCountDownLatch.await(1, TimeUnit.SECONDS);

        mProcessor.shutdownAllExecutors();

        assertEquals(mExpectedCallbackCounter, mCallbackCounter);
    }

    public void testAudioMessagesExecuted() throws InterruptedException {
        mExpectedCallbackCounter = MESSAGES_COUNTER;
        mCountDownLatch = new CountDownLatch(mExpectedCallbackCounter);

        for (int i = 0; i < MESSAGES_COUNTER; i++) {
            mProcessor.process(ServiceType.Audio_Service, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    false, new byte[0], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        mCountDownLatch.await(1, TimeUnit.SECONDS);

        mProcessor.shutdownAllExecutors();

        assertEquals(mExpectedCallbackCounter, mCallbackCounter);
    }

    public void testBulkMessagesExecuted() throws InterruptedException {
        final int dataSize = 192354;
        mExpectedCallbackCounter = MESSAGES_COUNTER *
                ConsecutiveFrameProcessor.getFrameCount(dataSize, MAX_DATA_SIZE) +
                MESSAGES_COUNTER;
        mCountDownLatch = new CountDownLatch(mExpectedCallbackCounter);

        final byte[] data = new byte[dataSize];
        for (int i = 0; i < MESSAGES_COUNTER; i++) {
            mProcessor.process(ServiceType.RPC, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    false, data, MAX_DATA_SIZE, SESSION_ID, i);

        }

        mCountDownLatch.await(25, TimeUnit.SECONDS);

        mProcessor.shutdownAllExecutors();

        assertEquals(mExpectedCallbackCounter, mCallbackCounter);
    }

    /**
     * Interferential test
     */

    public void testEveryMessageOfDifferentServiceTypesExecuted() throws Exception {
        for (int i = 0; i < 10; i++) {
            setUp();
            everyMessageOfDifferentServiceTypesExecuted();
        }
    }

    private void everyMessageOfDifferentServiceTypesExecuted() throws InterruptedException {
        ServiceType serviceType;
        int bulkDataSize = 168495;
        int bulkMessagesCount = 0;
        byte[] data;
        byte[] zeroData = new byte[0];
        byte[] bulkData = new byte[bulkDataSize];
        // Store messages in the collection in order to calculate expected amount of the messages
        Collection<Message> messages = new ArrayList<Message>(MESSAGES_COUNTER);
        for (int i = 0; i < MESSAGES_COUNTER; i++) {
            switch (CommonUtils.randInt(0, 3)) {
                case 0:
                    serviceType = ServiceType.RPC;
                    data = zeroData;
                    break;
                case 1:
                    serviceType = ServiceType.Mobile_Nav;
                    data = zeroData;
                    break;
                case 2:
                    serviceType = ServiceType.Audio_Service;
                    data = zeroData;
                    break;
                default:
                    serviceType = ServiceType.RPC;
                    bulkMessagesCount++;
                    data = bulkData;
                    break;
            }
            messages.add(new Message(serviceType, data));

        }

        mExpectedCallbackCounter = MESSAGES_COUNTER - bulkMessagesCount +
                (bulkMessagesCount *
                        ConsecutiveFrameProcessor.getFrameCount(bulkDataSize, MAX_DATA_SIZE) +
                        bulkMessagesCount);
        mCountDownLatch = new CountDownLatch(mExpectedCallbackCounter);

        int counter = 0;
        // Iterate over stored messages
        for (Message message : messages) {
            mProcessor.process(message.mServiceType, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    false, message.mData, MAX_DATA_SIZE, SESSION_ID, counter++);
        }

        mCountDownLatch.await(30, TimeUnit.SECONDS);

        mProcessor.shutdownAllExecutors();

        assertEquals(mExpectedCallbackCounter, mCallbackCounter);
    }

    // Helper class to keep a data related to a single "message"
    private class Message {

        private ServiceType mServiceType;
        private byte[] mData;

        private Message(ServiceType serviceType, byte[] data) {
            mServiceType = serviceType;
            mData = data;
        }
    }
}