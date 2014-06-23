package com.ford.syncV4.proxy;

import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.util.CommonUtils;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Hashtable;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 12/13/13
 * Time: 12:18 PM
 */
public class RPCMessageTest extends TestCase {

    private static final int CORRELATION_ID = 123;
    private static final String FUNCTION_NAME = Names.OnAudioPassThru;
    private static final byte[] BULK_DATA = new byte[Byte.MAX_VALUE];
    private Hashtable<String, Object> parametersHashTable;
    private Hashtable<Object, Object> messageHashTable;
    private Hashtable<Object, Object> dataHashTable;

    @Override
    protected void setUp() throws Exception {
        super.setUp();

        if (parametersHashTable != null) {
            parametersHashTable.clear();
        } else {
            parametersHashTable = new Hashtable<String, Object>();
        }
        parametersHashTable.put(Names.name, Names.OnAudioPassThru);
        parametersHashTable.put(Names.correlationID, CORRELATION_ID);

        if (messageHashTable != null) {
            messageHashTable.clear();
        } else {
            messageHashTable = new Hashtable<Object, Object>();
        }

        if (dataHashTable != null) {
            dataHashTable.clear();
        } else {
            dataHashTable = new Hashtable<Object, Object>();
        }
    }

    public void testCreateMessage_SetRequestTypeFirst() {
        messageHashTable.put(Names.request, getRandomData());
        messageHashTable.put(Names.bulkData, BULK_DATA);

        RPCMessage message = new RPCMessage(messageHashTable);
        assertNotNull(message);
    }

    public void testCreateMessage_SetResponseTypeFirst() {
        messageHashTable.put(Names.response, getRandomData());
        messageHashTable.put(Names.bulkData, BULK_DATA);

        RPCMessage message = new RPCMessage(messageHashTable);
        assertNotNull(message);
    }

    public void testCreateMessage_SetNotificationTypeFirst() {
        messageHashTable.put(Names.notification, getRandomData());
        messageHashTable.put(Names.bulkData, BULK_DATA);

        RPCMessage message = new RPCMessage(messageHashTable);
        assertNotNull(message);
    }

    public void testCreateMessage_SetBulkDataFirst_NotificationTypeSecond() {
        messageHashTable.put(Names.bulkData, BULK_DATA);
        messageHashTable.put(Names.notification, getRandomData());

        RPCMessage message = new RPCMessage(messageHashTable);
        assertNotNull(message);
    }

    public void testCreateMessage_SetBulkDataFirst_ResponseTypeSecond() {
        messageHashTable.put(Names.bulkData, BULK_DATA);
        messageHashTable.put(Names.response, getRandomData());

        RPCMessage message = new RPCMessage(messageHashTable);
        assertNotNull(message);
    }

    public void testCreateMessage_SetBulkDataFirst_RequestTypeSecond() {
        messageHashTable.put(Names.bulkData, BULK_DATA);
        messageHashTable.put(Names.request, getRandomData());

        RPCMessage message = new RPCMessage(messageHashTable);
        assertNotNull(message);
    }

    public void testSerialize() throws JSONException {
        messageHashTable.put(Names.bulkData, BULK_DATA);
        messageHashTable.put(Names.request, getData());

        RPCMessage message = new RPCMessage(messageHashTable);
        assertNotNull(message);

        JSONObject jsonObject = message.serializeJSON((byte) 2);
        assertEquals(2, jsonObject.length());
        assertEquals(CORRELATION_ID, jsonObject.getInt(Names.correlationID));
        assertEquals(FUNCTION_NAME, jsonObject.getString(Names.name));
    }

    private Hashtable<Object, Object> getRandomData() {
        // The 3-rd one is empty
        switch (CommonUtils.randInt(0, 3)) {
            case 0:
                getData();
                break;
            case 1:
                dataHashTable.put(Names.correlationID, CORRELATION_ID);
                dataHashTable.put(Names.function_name, FUNCTION_NAME);
                break;
            case 2:
                dataHashTable.put(Names.function_name, FUNCTION_NAME);
                break;
        }
        return dataHashTable;
    }

    private Hashtable<Object, Object> getData() {
        dataHashTable.put(Names.correlationID, CORRELATION_ID);
        dataHashTable.put(Names.parameters, parametersHashTable);
        dataHashTable.put(Names.function_name, FUNCTION_NAME);
        return dataHashTable;
    }

    public void testRECMessageCopyConstructor() throws Exception {
        messageHashTable.put(Names.bulkData, BULK_DATA);
        messageHashTable.put(Names.request, getRandomData());
        RPCMessage message = new RPCMessage(messageHashTable);
        RPCMessage real = new RPCMessage(message);
        assertEquals(message.getFunctionName(), real.getFunctionName());
        assertEquals(message.getMessageType(), real.getMessageType());
    }
}