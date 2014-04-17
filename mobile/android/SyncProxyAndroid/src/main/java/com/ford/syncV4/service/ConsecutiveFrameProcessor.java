package com.ford.syncV4.service;

import com.ford.syncV4.protocol.ProtocolFrameHeader;
import com.ford.syncV4.protocol.ProtocolFrameHeaderFactory;
import com.ford.syncV4.util.BitConverter;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 17.04.14
 * Time: 17:19
 */
public class ConsecutiveFrameProcessor {

    public void execute() {
        ProtocolFrameHeader firstHeader =
                ProtocolFrameHeaderFactory.createMultiSendDataFirst(serviceType,
                        sessionID, messageID, protocolVersionToSend);

        // Assemble first frame.
        int frameCount = data.length / MAX_DATA_SIZE;
        if (data.length % MAX_DATA_SIZE > 0) {
            frameCount++;
        }
        //byte[] firstFrameData = new byte[PROTOCOL_FRAME_HEADER_SIZE];
        byte[] firstFrameData = new byte[8];
        // First four bytes are data size.
        System.arraycopy(BitConverter.intToByteArray(data.length), 0, firstFrameData, 0, 4);
        // Second four bytes are frame count.
        System.arraycopy(BitConverter.intToByteArray(frameCount), 0, firstFrameData, 4, 4);

        handleProtocolFrameToSend(firstHeader, firstFrameData, 0, firstFrameData.length);

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
            if (bytesToWrite > MAX_DATA_SIZE) {
                bytesToWrite = MAX_DATA_SIZE;
            }

            ProtocolFrameHeader consecHeader =
                    ProtocolFrameHeaderFactory.createMultiSendDataRest(serviceType,
                            sessionID, bytesToWrite, frameSequenceNumber,
                            messageID, protocolVersionToSend);
            handleProtocolFrameToSend(consecHeader, data, currentOffset, bytesToWrite);
            currentOffset += bytesToWrite;
        }
    }
}