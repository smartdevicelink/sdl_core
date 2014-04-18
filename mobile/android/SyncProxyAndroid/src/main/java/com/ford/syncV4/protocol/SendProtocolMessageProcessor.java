package com.ford.syncV4.protocol;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.04.14
 * Time: 16:07
 */
public class SendProtocolMessageProcessor {

    public interface IProtocolMessageProcessor {

        public void onProtocolFrameToSend(ProtocolFrameHeader header, byte[] data, int offset,
                                          int length);
    }

}