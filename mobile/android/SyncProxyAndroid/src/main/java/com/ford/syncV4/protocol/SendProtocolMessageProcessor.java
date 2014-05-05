package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.ConsecutiveFrameProcessor;
import com.ford.syncV4.util.logger.Logger;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadFactory;
import java.util.concurrent.TimeUnit;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.04.14
 * Time: 16:07
 */

/**
 * This class is response for the messages processing in parallel threads to avoid blocking of the
 * messages flow
 */
public class SendProtocolMessageProcessor {

    @SuppressWarnings("unused")
    private static final String LOG_TAG = SendProtocolMessageProcessor.class.getSimpleName();

    /**
     * Interface to provide protocol messages processor's lifecycle callbacks
     */
    public interface ISendProtocolMessageProcessor {

        public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset,
                                          int length);

        public void onProtocolFrameToSendError(ERROR_TYPE errorType, String message);
    }

    public static enum ERROR_TYPE {
        DATA_NPE
    }

    /**
     * Executor service to process messages with bulk data.
     */
    private final ExecutorService bulkDataDataExecutor = Executors.newFixedThreadPool(1);
    /**
     * Executor service to process single messages.
     */
    private final ExecutorService singleMessageExecutor = Executors.newFixedThreadPool(1);
    /**
     * Executor service to process mobile navi messages.
     */
    private final ExecutorService mobileNaviExecutor = Executors.newFixedThreadPool(1);
    /**
     * Executor service to process audio messages.
     */
    private final ExecutorService audioExecutor = Executors.newFixedThreadPool(1);
    /**
     * Executor service to process heart beat messages.
     */
    private final ExecutorService heartbeatExecutor = Executors.newSingleThreadExecutor();

    /**
     * Process data into {@link com.ford.syncV4.protocol.ProtocolMessage}
     *
     * @param serviceType           type of the service
     * @param protocolVersionToSend protocol version
     * @param data                  byte array
     * @param maxDataSize           maximum size of the data
     * @param sessionID             id of the session
     * @param messageId             id of the message
     * @param callback              callback reference
     */
    public void process(final ServiceType serviceType, final byte protocolVersionToSend,
                        final byte[] data, final int maxDataSize,
                        final byte sessionID, final int messageId,
                        final ISendProtocolMessageProcessor callback) {

        if (data == null) {
            callback.onProtocolFrameToSendError(ERROR_TYPE.DATA_NPE, "Data is NULL");
            return;
        }

        if (data.length > maxDataSize) {
            //Logger.d(LOG_TAG + " BULK_DATA");
            bulkDataDataExecutor.submit(new Runnable() {

                @Override
                public void run() {

                    ConsecutiveFrameProcessor consecutiveFrameProcessor =
                            new ConsecutiveFrameProcessor();

                    consecutiveFrameProcessor.process(data, sessionID, messageId,
                            serviceType, protocolVersionToSend, maxDataSize,
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
        } else if (serviceType == ServiceType.Heartbeat) {
            //Logger.d(LOG_TAG + " HEART_BEAT");
            final ProtocolFrameHeader header =
                    ProtocolFrameHeaderFactory.createHeartbeat(serviceType, protocolVersionToSend);

            heartbeatExecutor.submit(new Runnable() {

                                         @Override
                                         public void run() {
                                             callback.onProtocolFrameToSend(header, null, 0, 0);
                                         }
                                     }
            );
        } else {
            final ProtocolFrameHeader header =
                    ProtocolFrameHeaderFactory.createSingleSendData(serviceType,
                            sessionID, data.length, messageId, protocolVersionToSend);

            if (serviceType == ServiceType.Audio_Service) {
                //Logger.d(LOG_TAG + " AUDIO");
                audioExecutor.submit(new Runnable() {

                                         @Override
                                         public void run() {
                                             callback.onProtocolFrameToSend(header, data, 0,
                                                     data.length);
                                         }
                                     }
                );
            } else if (serviceType == ServiceType.Mobile_Nav) {
                //Logger.d(LOG_TAG + " MOBILE_NAVI");
                mobileNaviExecutor.submit(new Runnable() {

                                              @Override
                                              public void run() {
                                                  callback.onProtocolFrameToSend(header, data, 0,
                                                          data.length);
                                              }
                                          }
                );
            } else {
                //Logger.d(LOG_TAG + " RPC");
                singleMessageExecutor.submit(new Runnable() {

                                             @Override
                                             public void run() {
                                                 callback.onProtocolFrameToSend(header, data, 0,
                                                         data.length);
                                             }
                                         }
                );
            }
        }
    }

    public void shutdownAllExecutors() throws InterruptedException {
        // This will make the executor accept no new threads
        // and finish all existing threads in the queue
        bulkDataDataExecutor.shutdown();
        heartbeatExecutor.shutdown();
        audioExecutor.shutdown();
        mobileNaviExecutor.shutdown();
        singleMessageExecutor.shutdown();
        // Wait until all threads are finish
        bulkDataDataExecutor.awaitTermination(1, TimeUnit.SECONDS);
        heartbeatExecutor.awaitTermination(1, TimeUnit.SECONDS);
        audioExecutor.awaitTermination(1, TimeUnit.SECONDS);
        mobileNaviExecutor.awaitTermination(1, TimeUnit.SECONDS);
        singleMessageExecutor.awaitTermination(1, TimeUnit.SECONDS);
    }
}