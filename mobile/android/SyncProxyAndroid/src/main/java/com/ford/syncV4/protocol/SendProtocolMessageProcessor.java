package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.ConsecutiveFrameProcessor;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.PriorityBlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.04.14
 * Time: 16:07
 */
public class SendProtocolMessageProcessor {

    public interface ISendProtocolMessageProcessor {

        public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset,
                                          int length);
    }

    private final static PriorityBlockingQueue<Runnable> BLOCKING_QUEUE =
            new PriorityBlockingQueue<Runnable>(20, new CompareMessagesPriority());
    private static final ExecutorService MESSAGES_CACHED_EXECUTOR_SERVICE =
            Executors.newCachedThreadPool();

    /**
     * Executor service to process single frame commands.
     * If the number of threads is greater than the 10, set the the maximum time that excess
     * idle threads will wait for new tasks before terminating to 10 seconds
     */
    private static final ExecutorService MESSAGES_EXECUTOR_SERVICE =
            new ThreadPoolExecutor(1, 10, 10, TimeUnit.SECONDS, BLOCKING_QUEUE);

    public void process(final ServiceType serviceType, final byte protocolVersionToSend,
                        final byte[] data, final int maxDataSize,
                        final byte sessionID, final int messageId, final int correlationId,
                        final ISendProtocolMessageProcessor callback) {
        if (data.length > maxDataSize) {

            MESSAGES_CACHED_EXECUTOR_SERVICE.submit(new Runnable() {
                @Override
                public void run() {

                    ConsecutiveFrameProcessor consecutiveFrameProcessor =
                            new ConsecutiveFrameProcessor();

                    consecutiveFrameProcessor.process(data, sessionID, messageId,
                            serviceType,
                            protocolVersionToSend, maxDataSize,
                            new ConsecutiveFrameProcessor.IConsecutiveFrameProcessor() {

                                @Override
                                public void onProtocolFrameToSend(ProtocolFrameHeader header,
                                                                  byte[] data, int offset,
                                                                  int length) {
                                    callback.onProtocolFrameToSend(header, data, offset, length);
                                }
                            }
                    );
                }
            });
        } else {

            MESSAGES_EXECUTOR_SERVICE.execute(
                    new RunnableWithPriority(serviceType.getValue(), correlationId) {

                        @Override
                        public void run() {

                            ProtocolFrameHeader header =
                                    ProtocolFrameHeaderFactory.createSingleSendData(serviceType,
                                            sessionID, data.length, messageId,
                                            protocolVersionToSend);
                            callback.onProtocolFrameToSend(header, data, 0, data.length);
                        }
                    }
            );
        }
    }
}