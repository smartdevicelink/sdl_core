package com.ford.syncV4.protocol;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.service.ConsecutiveFrameProcessor;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

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
    private final ExecutorService heartbeatExecutor = Executors.newFixedThreadPool(1);

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
        } else {
            final ProtocolFrameHeader header =
                    ProtocolFrameHeaderFactory.createSingleSendData(serviceType,
                            sessionID, data.length, messageId, protocolVersionToSend);

            if (serviceType == ServiceType.Audio_Service) {
                audioExecutor.submit(new Runnable() {

                                         @Override
                                         public void run() {
                                             callback.onProtocolFrameToSend(header, data, 0,
                                                     data.length);
                                         }
                                     }
                );
            } else if (serviceType == ServiceType.Mobile_Nav) {
                mobileNaviExecutor.submit(new Runnable() {

                                              @Override
                                              public void run() {
                                                  callback.onProtocolFrameToSend(header, data, 0,
                                                          data.length);
                                              }
                                          }
                );
            } else if (serviceType == ServiceType.Heartbeat) {
                heartbeatExecutor.submit(new Runnable() {

                                             @Override
                                             public void run() {
                                                 callback.onProtocolFrameToSend(header, data, 0,
                                                         data.length);
                                             }
                                         }
                );
            } else {
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
}