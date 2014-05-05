package com.ford.syncV4.protocol;

import android.test.AndroidTestCase;

import com.ford.syncV4.protocol.enums.ServiceType;
import com.ford.syncV4.proxy.constants.ProtocolConstants;

import java.util.Arrays;

/**
 * Created by Andrew Batutin on 8/21/13.
 */
public class ProtocolMessageConverterTest extends AndroidTestCase {

    public ProtocolMessageConverterTest() {
    }

    public void  testMobileNavDataPayloadCreation() throws Exception {
        byte [] frame = new byte[]{1,2,3,4,5,7};
        ProtocolMessage message = new ProtocolMessage();
        message.setVersion(ProtocolConstants.PROTOCOL_VERSION_THREE);
        message.setData(frame);
        message.setServiceType(ServiceType.Mobile_Nav);
        ProtocolMessageConverter converter = new ProtocolMessageConverter(message, ProtocolConstants.PROTOCOL_VERSION_THREE);
        byte[] result = converter.generate().getData();
        assertTrue("for mobile nav data should be same as ProtocolMessage.data", Arrays.equals(result, frame));
    }

    public void  testAudioServiceDataPayloadCreation() throws Exception {
        byte [] frame = new byte[]{1,2,3,4,5,7};
        ProtocolMessage message = new ProtocolMessage();
        message.setVersion(ProtocolConstants.PROTOCOL_VERSION_THREE);
        message.setData(frame);
        message.setServiceType(ServiceType.Audio_Service);
        ProtocolMessageConverter converter = new ProtocolMessageConverter(message, ProtocolConstants.PROTOCOL_VERSION_THREE);
        byte[] result = converter.generate().getData();
        assertTrue("for audio service data should be same as ProtocolMessage.data", Arrays.equals(result, frame));
    }
}
