package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.service.ConsecutiveFrameProcessor;
import com.ford.syncV4.util.CommonUtils;

import junit.framework.TestCase;

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

    /**
     * These tests are almost the same (Copy | Paste) except of the Service Type.
     * Probably there is a way to improve them, but because luck of the time I leave them like
     * they are
     */

    public void testHeartbeatMessagesExecuted() throws InterruptedException {
        final int messagesCount = 10;
        final int[] callbackCounter = {0};
        SendProtocolMessageProcessor processor = new SendProtocolMessageProcessor();
        final CountDownLatch countDownLatch = new CountDownLatch(messagesCount);

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        callbackCounter[0]++;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        processor.setCallback(callback);

        for (int i = 0; i < messagesCount; i++) {
            processor.processHeartbeat(ProtocolConstants.PROTOCOL_VERSION_TWO, SESSION_ID);
        }

        countDownLatch.await(1, TimeUnit.SECONDS);

        processor.shutdownAllExecutors();

        assertEquals(messagesCount, callbackCounter[0]);
    }

    public void testHeartbeatAckMessagesExecuted() throws InterruptedException {
        final int messagesCount = 10;
        final int[] callbackCounter = {0};
        SendProtocolMessageProcessor processor = new SendProtocolMessageProcessor();
        final CountDownLatch countDownLatch = new CountDownLatch(messagesCount);

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        callbackCounter[0]++;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        processor.setCallback(callback);

        for (int i = 0; i < messagesCount; i++) {
            processor.processHeartbeatAck(ProtocolConstants.PROTOCOL_VERSION_TWO, SESSION_ID);
        }

        countDownLatch.await(1, TimeUnit.SECONDS);

        processor.shutdownAllExecutors();

        assertEquals(messagesCount, callbackCounter[0]);
    }

    public void testRPCMessagesExecuted() throws InterruptedException {
        final int messagesCount = 10;
        final int[] callbackCounter = {0};
        SendProtocolMessageProcessor processor = new SendProtocolMessageProcessor();
        final CountDownLatch countDownLatch = new CountDownLatch(messagesCount);

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        callbackCounter[0]++;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        processor.setCallback(callback);

        for (int i = 0; i < messagesCount; i++) {
            processor.process(ServiceType.RPC, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    new byte[0], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        countDownLatch.await(1, TimeUnit.SECONDS);

        processor.shutdownAllExecutors();

        assertEquals(messagesCount, callbackCounter[0]);
    }

    public void testMobileNaviMessagesExecuted() throws InterruptedException {
        final int messagesCount = 10;
        final int[] callbackCounter = {0};
        SendProtocolMessageProcessor processor = new SendProtocolMessageProcessor();
        final CountDownLatch countDownLatch = new CountDownLatch(messagesCount);

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        callbackCounter[0]++;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        processor.setCallback(callback);

        for (int i = 0; i < messagesCount; i++) {
            processor.process(ServiceType.Mobile_Nav, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    new byte[0], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        countDownLatch.await(1, TimeUnit.SECONDS);

        processor.shutdownAllExecutors();

        assertEquals(messagesCount, callbackCounter[0]);
    }

    public void testAudioMessagesExecuted() throws InterruptedException {
        final int messagesCount = 10;
        final int[] callbackCounter = {0};
        SendProtocolMessageProcessor processor = new SendProtocolMessageProcessor();
        final CountDownLatch countDownLatch = new CountDownLatch(messagesCount);

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        callbackCounter[0]++;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        processor.setCallback(callback);

        for (int i = 0; i < messagesCount; i++) {
            processor.process(ServiceType.Audio_Service, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    new byte[0], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        countDownLatch.await(1, TimeUnit.SECONDS);

        processor.shutdownAllExecutors();

        assertEquals(messagesCount, callbackCounter[0]);
    }

    public void testBulkMessagesExecuted() throws InterruptedException {
        final int messagesCount = 10;
        final int[] callbackCounter = {0};
        int dataSize = 192354;
        SendProtocolMessageProcessor processor = new SendProtocolMessageProcessor();
        final CountDownLatch countDownLatch = new CountDownLatch(messagesCount);

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        callbackCounter[0]++;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        processor.setCallback(callback);

        for (int i = 0; i < messagesCount; i++) {
            processor.process(ServiceType.RPC, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    new byte[dataSize], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        countDownLatch.await(1, TimeUnit.SECONDS);

        processor.shutdownAllExecutors();

        assertEquals(messagesCount *
                        ConsecutiveFrameProcessor.getFrameCount(dataSize, MAX_DATA_SIZE) +
                        messagesCount,
                callbackCounter[0]);
    }

    /**
     * Interferential test
     */

    public void testEveryMessageOfDifferentServiceTypesExecuted() throws InterruptedException {
        final int messagesCount = 100;
        final int[] callbackCounter = {0};
        SendProtocolMessageProcessor processor = new SendProtocolMessageProcessor();
        final CountDownLatch countDownLatch = new CountDownLatch(messagesCount);

        SendProtocolMessageProcessor.ISendProtocolMessageProcessor callback =
                new SendProtocolMessageProcessor.ISendProtocolMessageProcessor() {
                    @Override
                    public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data,
                                                      int offset, int length) {
                        callbackCounter[0]++;
                        countDownLatch.countDown();
                    }

                    @Override
                    public void onProtocolFrameToSendError(SendProtocolMessageProcessor.ERROR_TYPE errorType,
                                                           String message) {

                    }
                };

        processor.setCallback(callback);

        ServiceType serviceType;
        int dataSize;
        int bulkDataSize = 168495;
        int bulkMessagesCount = 0;
        for (int i = 0; i < messagesCount; i++) {
            switch (CommonUtils.randInt(0, 3)) {
                case 0:
                    serviceType = ServiceType.RPC;
                    dataSize = 0;
                    break;
                case 1:
                    serviceType = ServiceType.Mobile_Nav;
                    dataSize = 0;
                    break;
                case 2:
                    serviceType = ServiceType.Audio_Service;
                    dataSize = 0;
                    break;
                default:
                    serviceType = ServiceType.Bulk_Data;
                    dataSize = bulkDataSize;
                    bulkMessagesCount++;
                    break;
            }
            processor.process(serviceType, ProtocolConstants.PROTOCOL_VERSION_TWO,
                    new byte[dataSize], MAX_DATA_SIZE, SESSION_ID, 1);

        }

        countDownLatch.await(3, TimeUnit.SECONDS);

        processor.shutdownAllExecutors();

        assertEquals(messagesCount - bulkMessagesCount +
                (bulkMessagesCount *
                ConsecutiveFrameProcessor.getFrameCount(bulkDataSize, MAX_DATA_SIZE) +
                bulkMessagesCount), callbackCounter[0]);
    }
}