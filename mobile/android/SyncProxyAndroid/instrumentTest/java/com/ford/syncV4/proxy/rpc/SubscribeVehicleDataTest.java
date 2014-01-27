package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for SubscribeVehicleData request (new params only).
 *
 * Created by enikolsky on 2014-01-27.
 */
public class SubscribeVehicleDataTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String STEERING_WHEEL_ANGLE = "steeringWheelAngle";

    public void testMessageShouldBeCreated() {
        SubscribeVehicleData msg = new SubscribeVehicleData();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        SubscribeVehicleData msg = new SubscribeVehicleData();

        final boolean swa = true;

        msg.setSteeringWheelAngle(swa);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getBoolean(STEERING_WHEEL_ANGLE), is(swa));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        SubscribeVehicleData msg = new SubscribeVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testSteeringWheelAngleGetterShouldReturnSetValue()
            throws JSONException {
        SubscribeVehicleData msg = new SubscribeVehicleData();

        final boolean swa = true;
        msg.setSteeringWheelAngle(swa);

        assertThat(msg.getSteeringWheelAngle(), is(swa));
    }

    public void testSettingNullSteeringWheelAngleShouldRemoveValue()
            throws JSONException {
        SubscribeVehicleData msg = new SubscribeVehicleData();

        msg.setSteeringWheelAngle(true);
        msg.setSteeringWheelAngle(null);

        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testDeserializedSteeringWheelAngleShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final boolean swa = true;
        jsonObject.put(STEERING_WHEEL_ANGLE, swa);

        SubscribeVehicleData msg = new SubscribeVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), is(swa));
    }

    public void testDeserializedSteeringWheelAngleAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(STEERING_WHEEL_ANGLE, 42);

        SubscribeVehicleData msg = new SubscribeVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testDeserializedSteeringWheelAngleAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(STEERING_WHEEL_ANGLE, "true");

        SubscribeVehicleData msg = new SubscribeVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }
}
