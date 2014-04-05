package com.ford.syncV4.proxy.rpc;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.WiProProtocol;
import com.ford.syncV4.protocol.enums.FunctionID;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.syncConnection.SyncConnection;
import com.ford.syncV4.test.TestConfig;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Hashtable;
import java.util.Random;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

/**
 * Common methods for testing classes.
 *
 * Created by enikolsky on 2013-10-29.
 */
public class TestCommon {

    public static JSONObject paramsToRequestObject(JSONObject paramsObject)
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        JSONObject requestObject = new JSONObject();
        jsonObject.put(Names.request, requestObject);
        requestObject.put(Names.parameters, paramsObject);
        return jsonObject;
    }

    public static void setupMocking(InstrumentationTestCase testCase) {
        System.setProperty("dexmaker.dexcache", testCase.getInstrumentation()
                                                        .getTargetContext()
                                                        .getCacheDir()
                                                        .getPath());
    }

    public static SyncProxyALM getSyncProxyALMNoTransport(
            IProxyListenerALM proxyListener) throws SyncException {
        SyncConnection connectionMock = mock(SyncConnection.class);
        when(connectionMock.getIsConnected()).thenReturn(true);
        WiProProtocol protocolMock = mock(WiProProtocol.class);
        when(connectionMock.getWiProProtocol()).thenReturn(protocolMock);

        return new SyncProxyALM(proxyListener, null, "!", null, null, true,
                null, null, null, null, null, null, false, false,
                ProtocolConstants.PROTOCOL_VERSION_TWO, null, connectionMock, new TestConfig());
    }

    public static byte[] getRandomBytes(int dataSize) {
        // to simplify matching if bytes mess up
        final byte[] data = new byte[dataSize];
        new Random().nextBytes(data);
        return data;
    }

    public static Hashtable<String, Object> deserializeJSONRequestObject(
            JSONObject jsonObject) throws JSONException {
        return JsonRPCMarshaller.deserializeJSONObject(
                paramsToRequestObject(jsonObject));
    }

    public static ProtocolMessage createProtocolMessage(
            final String functionName, final Hashtable<String, Object> params,
            final byte rpcType, final int corrID) throws JSONException {
        ProtocolMessage pm = new ProtocolMessage();
        pm.setVersion(ProtocolConstants.PROTOCOL_VERSION_MAX);
        pm.setCorrID(corrID);

        if (params != null) {
            JSONObject paramsObject =
                    JsonRPCMarshaller.serializeHashtable(params);
            byte[] paramsData = paramsObject.toString().getBytes();
            pm.setData(paramsData, paramsData.length);
            pm.setJsonSize(paramsData.length);
        }

        pm.setFunctionID(FunctionID.getFunctionID(functionName));
        pm.setRPCType(rpcType);

        return pm;
    }
}
