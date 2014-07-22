package com.ford.syncV4.service;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.util.BitConverter;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 17.04.14
 * Time: 17:19
 */

/**
 * This class provides functionality to process single message with a data into a sequence of
 * small messages
 */
public class ConsecutiveFrameProcessor {

    /**
     * Interface to provide {@link com.ford.syncV4.service.ConsecutiveFrameProcessor}'s
     * lifecycle callbacks
     */
    public interface IConsecutiveFrameProcessor {

        public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset,
                                          int length);
    }

    /**
     * Process a data into a sequence of small messages
     *  @param data                  byte array of the data
     * @param sessionID             id of the session
     * @param messageID             id of the message
     * @param serviceType           type of the service
     * @param protocolVersionToSend version of current protocol
     * @param encrypted
     * @param maxDataSize           maximum data size
     * @param callback              lifecycle callback
     */
    public void process(byte[] data, byte sessionID, final int messageID, ServiceType serviceType,
                        byte protocolVersionToSend, boolean encrypted, int maxDataSize,
                        IConsecutiveFrameProcessor callback) {
        ProtocolFrameHeader firstHeader =
                ProtocolFrameHeaderFactory.createMultiSendDataFirst(serviceType,
                        sessionID, messageID, protocolVersionToSend);
        firstHeader.setEncrypted(encrypted);
        // Assemble first frame.
        int frameCount = getFrameCount(data.length, maxDataSize);
        byte[] firstFrameData = new byte[8];
        // First four bytes are data size.
        System.arraycopy(BitConverter.intToByteArray(data.length), 0, firstFrameData, 0, 4);
        // Second four bytes are frame count.
        System.arraycopy(BitConverter.intToByteArray(frameCount), 0, firstFrameData, 4, 4);

        callback.onProtocolFrameToSend(firstHeader, firstFrameData, 0, firstFrameData.length);

        int currentOffset = 0;
        byte frameSequenceNumber = 0;

        for (int i = 0; i < frameCount; i++) {
            if (i < (frameCount - 1)) {
                ++frameSequenceNumber;
                if (frameSequenceNumber ==
                        ProtocolFrameHeader.FrameDataFinalConsecutiveFrame) {
                    // we can't use 0x00 as frameSequenceNumber, because
                    // it's reserved for the last frame
                    ++frameSequenceNumber;
                }
            } else {
                frameSequenceNumber = ProtocolFrameHeader.FrameDataFinalConsecutiveFrame;
            } // end-if

            int bytesToWrite = data.length - currentOffset;
            if (bytesToWrite > maxDataSize) {
                bytesToWrite = maxDataSize;
            }

            ProtocolFrameHeader consecHeader =
                    ProtocolFrameHeaderFactory.createMultiSendDataRest(serviceType,
                            sessionID, bytesToWrite, frameSequenceNumber,
                            messageID, protocolVersionToSend);
            consecHeader.setEncrypted(encrypted);
            callback.onProtocolFrameToSend(consecHeader, data, currentOffset, bytesToWrite);
            currentOffset += bytesToWrite;
        }
    }

    /**
     * Calculates frames number based on data size
     *
     * @param dataLength  incoming data length
     * @param maxDataSize maximum size of the data in single frame
     * @return frame count
     */
    public static int getFrameCount(int dataLength, int maxDataSize) {
        int frameCount = dataLength / maxDataSize;
        if (dataLength % maxDataSize > 0) {
            frameCount++;
        }
        return frameCount;
    }
}