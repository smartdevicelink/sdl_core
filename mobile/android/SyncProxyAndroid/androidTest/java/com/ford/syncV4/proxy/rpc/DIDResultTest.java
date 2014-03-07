package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for DIDResult struct.
 *
 * Created by enikolsky on 2014-02-13.
 */
public class DIDResultTest extends TestCase {
    private static final String RESULT_CODE = "resultCode";
    private static final String DID_LOCATION = "didLocation";
    private static final String DATA = "data";

    public void testMessageShouldBeCreated() {
        DIDResult msg = new DIDResult();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        DIDResult msg = new DIDResult();

        final VehicleDataResultCode code =
                VehicleDataResultCode.DATA_ALREADY_SUBSCRIBED;
        final int didLocation = 1234;
        final String data = "Atlas Shrugged";

        msg.setResultCode(code);
        msg.setDidLocation(didLocation);
        msg.setData(data);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.getString(RESULT_CODE), is(code.toString()));
        assertThat(jsonObject.getInt(DID_LOCATION), is(didLocation));
        assertThat(jsonObject.getString(DATA), is(data));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        DIDResult msg = new DIDResult(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getResultCode(), nullValue());
        assertThat(msg.getDidLocation(), nullValue());
        assertThat(msg.getData(), nullValue());
    }

    /// resultCode
    public void testResultCodeGetterShouldReturnSetValue()
            throws JSONException {
        DIDResult msg = new DIDResult();

        final VehicleDataResultCode code = VehicleDataResultCode.IGNORED;
        msg.setResultCode(code);

        assertThat(msg.getResultCode(), is(code));
    }

    public void testSettingNullResultCodeShouldRemoveValue()
            throws JSONException {
        DIDResult msg = new DIDResult();

        msg.setResultCode(VehicleDataResultCode.DISALLOWED);
        msg.setResultCode(null);

        assertThat(msg.getResultCode(), nullValue());
    }

    public void testDeserializedResultCodeAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final VehicleDataResultCode code =
                VehicleDataResultCode.DATA_ALREADY_SUBSCRIBED;
        jsonObject.put(RESULT_CODE, code.toString());

        DIDResult msg = new DIDResult(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResultCode(), is(code));
    }

    public void testDeserializedResultCodeAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(RESULT_CODE, 4);

        DIDResult msg = new DIDResult(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResultCode(), nullValue());
    }

    /// didLocation
    public void testDidLocationGetterShouldReturnSetValue()
            throws JSONException {
        DIDResult msg = new DIDResult();

        final int didLocation = 1234;
        msg.setDidLocation(didLocation);

        assertThat(msg.getDidLocation(), is(didLocation));
    }

    public void testSettingNullDidLocationShouldRemoveValue()
            throws JSONException {
        DIDResult msg = new DIDResult();

        msg.setDidLocation(12345);
        msg.setDidLocation(null);

        assertThat(msg.getDidLocation(), nullValue());
    }

    public void testDeserializedDidLocationAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int didLocation = 1234;
        jsonObject.put(DID_LOCATION, didLocation);

        DIDResult msg = new DIDResult(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getDidLocation(), is(didLocation));
    }

    public void testDeserializedDidLocationAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(DID_LOCATION, "123");

        DIDResult msg = new DIDResult(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getDidLocation(), nullValue());
    }

    /// data
    public void testDataGetterShouldReturnSetValue() throws JSONException {
        DIDResult msg = new DIDResult();

        final String data = "Atlas Shrugged";
        msg.setData(data);

        assertThat(msg.getData(), is(data));
    }

    public void testSettingNullDataShouldRemoveValue() throws JSONException {
        DIDResult msg = new DIDResult();

        msg.setData("You Will Obey");
        msg.setData(null);

        assertThat(msg.getData(), nullValue());
    }

    public void testDeserializedDataAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final String data = "Atlas Shrugged";
        jsonObject.put(DATA, data);

        DIDResult msg = new DIDResult(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getData(), is(data));
    }

    public void testDeserializedDataAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(DATA, 432);

        DIDResult msg = new DIDResult(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getData(), nullValue());
    }
}
