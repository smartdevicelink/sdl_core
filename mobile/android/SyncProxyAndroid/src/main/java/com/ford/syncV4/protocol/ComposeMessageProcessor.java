package com.ford.syncV4.protocol;

import java.util.Arrays;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.PriorityBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 22.04.14
 * Time: 11:36
 */
public class ComposeMessageProcessor {

    public interface IComposeMessageProcessor {

        public void onMessageBytesToSend(byte[] bytesToSend, int offset, int length);
    }

    private final static PriorityBlockingQueue<Runnable> BLOCKING_QUEUE =
            new PriorityBlockingQueue<Runnable>(20, new CompareMessagesPriority());
    private static final ExecutorService MESSAGES_EXECUTOR_SERVICE =
            new ThreadPoolExecutor(1, 10, 10, TimeUnit.SECONDS, BLOCKING_QUEUE);

    private static int sCorrelationIdCounter = 0;

    /**
     *
     * @param header
     * @param data
     * @param offset
     * @param length
     * @param callback
     */
    public void process(final ProtocolFrameHeader header, final byte[] data, final int offset,
                        final int length, final IComposeMessageProcessor callback) {
        MESSAGES_EXECUTOR_SERVICE.execute(new RunnableWithPriority(
                header.getServiceType().getValue(), getNextCorrelationId()) {
            @Override
            public void run() {
                if (data != null) {
                    byte[] commonArray = appendDataToProtocolHeader(header, data, offset, length);
                    callback.onMessageBytesToSend(commonArray, 0, commonArray.length);
                } else {
                    byte[] frameHeader = header.assembleHeaderBytes();
                    callback.onMessageBytesToSend(frameHeader, 0, frameHeader.length);
                }
            }
        });

    }

    private byte[] appendDataToProtocolHeader(ProtocolFrameHeader header, byte[] data, int offset,
                                              int length) {
        if (offset >= data.length) {
            throw new IllegalArgumentException("offset should not be more then length");
        }
        byte[] dataChunk;
        if (offset + length >= data.length) {
            dataChunk = Arrays.copyOfRange(data, offset, data.length);
        } else {
            dataChunk = Arrays.copyOfRange(data, offset, offset + length);
        }

        byte[] frameHeader = header.assembleHeaderBytes();
        byte[] commonArray = new byte[frameHeader.length + dataChunk.length];
        System.arraycopy(frameHeader, 0, commonArray, 0, frameHeader.length);
        System.arraycopy(dataChunk, 0, commonArray, frameHeader.length, dataChunk.length);

        return commonArray;
    }

    private static int getNextCorrelationId() {
        return sCorrelationIdCounter++;
    }
}