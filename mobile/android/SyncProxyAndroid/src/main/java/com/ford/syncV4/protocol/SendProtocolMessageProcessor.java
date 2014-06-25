package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.ConsecutiveFrameProcessor;
import com.ford.syncV4.util.BitConverter;
import com.ford.syncV4.util.logger.Logger;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
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
    //private final ExecutorService heartbeatExecutor = Executors.newCachedThreadPool();
    /**
     * Executor service to process heart beat ack messages.
     */
    //private final ExecutorService heartbeatAckExecutor = Executors.newCachedThreadPool();

    private ISendProtocolMessageProcessor callback;

    public void setCallback(ISendProtocolMessageProcessor value) {
        callback = value;
    }

    /**
     * Process data into {@link com.ford.syncV4.protocol.ProtocolMessage}
     *
     * @param serviceType           type of the service
     * @param protocolVersionToSend protocol version
     * @param data                  byte array
     * @param maxDataSize           maximum size of the data
     * @param sessionID             id of the session
     * @param messageId             id of the message
     */
    public void process(final ServiceType serviceType, final byte protocolVersionToSend,
                        final byte[] data, final int maxDataSize,
                        final byte sessionID, final int messageId) {

        if (data == null) {
            callback.onProtocolFrameToSendError(ERROR_TYPE.DATA_NPE, "Data is NULL");
            return;
        }

        //Logger.d(LOG_TAG + " Process, serviceType:" + serviceType + ", data:" + data.length);

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
                                    if (callback != null) {
                                        callback.onProtocolFrameToSend(header, data, offset, length);
                                    }
                                }
                            }
                    );
                }
            });
        } else if (serviceType == ServiceType.Heartbeat) {
            // Move logic to separate methods
            // processHeartbeat, processHeartbeatAck
        } else {
            final ProtocolFrameHeader header =
                    ProtocolFrameHeaderFactory.createSingleSendData(serviceType,
                            sessionID, data.length, messageId, protocolVersionToSend);

            if (serviceType == ServiceType.Audio_Service) {
                if (audioExecutor.isTerminated()) {
                    return;
                }
                if (audioExecutor.isShutdown()) {
                    return;
                }
                audioExecutor.submit(new Runnable() {

                                         @Override
                                         public void run() {
                                             if (callback != null) {
                                                 callback.onProtocolFrameToSend(header, data, 0,
                                                         data.length);
                                             }
                                         }
                                     }
                );
            } else if (serviceType == ServiceType.Mobile_Nav) {
                if (mobileNaviExecutor.isTerminated()) {
                    return;
                }
                if (mobileNaviExecutor.isShutdown()) {
                    return;
                }
                mobileNaviExecutor.submit(new Runnable() {

                                              @Override
                                              public void run() {
                                                  if (callback != null) {
                                                      callback.onProtocolFrameToSend(header, data, 0,
                                                              data.length);
                                                  }
                                              }
                                          }
                );
            } else {
                //Logger.d(LOG_TAG + " RPC");
                singleMessageExecutor.submit(new Runnable() {

                                                 @Override
                                                 public void run() {
                                                     if (callback != null) {
                                                         callback.onProtocolFrameToSend(header, data, 0,
                                                                 data.length);
                                                     }
                                                 }
                                             }
                );
            }
        }
    }

    /**
     * Process Heart beat message to send
     *
     * @param protocolVersionToSend protocol version
     * @param sessionID             id of the session
     */
    public void processHeartbeat(byte protocolVersionToSend, byte sessionID) {
        //Logger.d(LOG_TAG + " HEART_BEAT");
        final ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createHeartbeat(ServiceType.Heartbeat,
                        protocolVersionToSend);
        header.setSessionID(sessionID);

        /*heartbeatExecutor.submit(new Runnable() {

                                     @Override
                                     public void run() {
                                         if (callback != null) {
                                             callback.onProtocolFrameToSend(header, null, 0, 0);
                                         }
                                     }
                                 }
        );*/

        if (callback != null) {
            callback.onProtocolFrameToSend(header, null, 0, 0);
        }
    }

    /**
     * Process Heart beat Ack message to send
     *
     * @param protocolVersionToSend protocol version
     * @param sessionId             id of the session
     */
    public void processHeartbeatAck(byte protocolVersionToSend, byte sessionId) {
        //Logger.d(LOG_TAG + " HEART_BEAT_ACK");
        final ProtocolFrameHeader header =
                ProtocolFrameHeaderFactory.createHeartbeatACK(ServiceType.Heartbeat,
                        protocolVersionToSend);
        header.setSessionID(sessionId);

        /*heartbeatAckExecutor.submit(new Runnable() {

                                        @Override
                                        public void run() {
                                            if (callback != null) {
                                                callback.onProtocolFrameToSend(header, null, 0, 0);
                                            }
                                        }
                                    }
        );*/

        if (callback != null) {
            callback.onProtocolFrameToSend(header, null, 0, 0);
        }
    }

    /**
     * Process Start session message to send
     *
     * @param protocolVersionToSend protocol version
     * @param sessionId             id of the session
     */
    public void processStartSession(final byte protocolVersionToSend, final byte sessionId) {
        Logger.d(LOG_TAG + " Start Session, prtcl ver:" + protocolVersionToSend +
                ", sesId:" + sessionId);
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (callback != null) {
                    ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(
                            ServiceType.RPC, sessionId, protocolVersionToSend);
                    callback.onProtocolFrameToSend(header, null, 0, 0);
                }
            }
        };
        singleMessageExecutor.submit(runnable);
    }

    /**
     *  Process Start session ack message to send
     *
     * @param serviceType           type of the service
     * @param protocolVersionToSend protocol version
     * @param sessionId             id of the session
     */
    public void processStartSessionAck(final ServiceType serviceType,
                                       final byte protocolVersionToSend, final byte sessionId) {
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (callback != null) {
                    ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSessionACK(
                            serviceType, sessionId, 0x00, protocolVersionToSend);
                    callback.onProtocolFrameToSend(header, null, 0, 0);
                }
            }
        };
        singleMessageExecutor.submit(runnable);
    }

    /**
     * Process End session message to send
     *
     * @param serviceType           type of the service
     * @param hashId                hash Id
     * @param protocolVersionToSend protocol version
     * @param sessionId             id of the session
     */
    public void processEndService(final ServiceType serviceType, final int hashId,
                                  final byte protocolVersionToSend, final byte sessionId) {

        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (callback != null) {
                    byte[] data = BitConverter.intToByteArray(hashId);
                    ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createEndSession(
                            serviceType, sessionId, hashId, protocolVersionToSend, data.length);
                    callback.onProtocolFrameToSend(header, data, 0, data.length);
                }
            }
        };
        singleMessageExecutor.submit(runnable);
    }

    /**
     * Process Start service message to send
     *
     * @param protocolVersionToSend protocol version
     * @param sessionId             id of the session
     */
    public void processStartService(final ServiceType serviceType, final byte protocolVersionToSend,
                                    final byte sessionId) {
        Logger.d(LOG_TAG + " Start Service:" + serviceType);
        Runnable runnable = new Runnable() {
            @Override
            public void run() {
                if (callback != null) {
                    ProtocolFrameHeader header = ProtocolFrameHeaderFactory.createStartSession(
                            serviceType, sessionId, protocolVersionToSend);
                    callback.onProtocolFrameToSend(header, null, 0, 0);
                }
            }
        };
        singleMessageExecutor.submit(runnable);
    }

    /**
     * Shut down audio executor
     * @throws InterruptedException
     */
    public void shutdownAudioExecutors() throws InterruptedException {
        audioExecutor.shutdownNow();
        audioExecutor.awaitTermination(1, TimeUnit.SECONDS);
    }

    /**
     * Shut down Navi executor
     * @throws InterruptedException
     */
    public void shutdownNaviExecutors() throws InterruptedException {
        mobileNaviExecutor.shutdownNow();
        mobileNaviExecutor.awaitTermination(1, TimeUnit.SECONDS);
    }

    /**
     * Shut down all executors
     *
     * @throws InterruptedException
     */
    public void shutdownAllExecutors() throws InterruptedException {
        // This will make the executor accept no new threads
        // and finish all existing threads in the queue

        shutdownAudioExecutors();

        shutdownNaviExecutors();

        bulkDataDataExecutor.shutdown();
        singleMessageExecutor.shutdown();
        // Wait until all threads are finish
        bulkDataDataExecutor.awaitTermination(1, TimeUnit.SECONDS);
        singleMessageExecutor.awaitTermination(1, TimeUnit.SECONDS);
    }
}