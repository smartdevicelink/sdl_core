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
 * Tests for OnVehicleData notification (new params only).
 *
 * Created by enikolsky on 2014-01-27.
 */
public class OnVehicleDataTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String STEERING_WHEEL_ANGLE = "steeringWheelAngle";

    public void testMessageShouldBeCreated() {
        OnVehicleData msg = new OnVehicleData();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final float swa = 1.0f;

        msg.setSteeringWheelAngle(swa);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getDouble(STEERING_WHEEL_ANGLE),
                is((double) swa));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testSteeringWheelAngleGetterShouldReturnSetValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final float swa = 2.0f;
        msg.setSteeringWheelAngle(swa);

        assertThat(msg.getSteeringWheelAngle(), is(swa));
    }

    public void testSettingNullSteeringWheelAngleShouldRemoveValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        msg.setSteeringWheelAngle(-3.14f);
        msg.setSteeringWheelAngle(null);

        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testDeserializedSteeringWheelAngleShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final float swa = 4.0f;
        jsonObject.put(STEERING_WHEEL_ANGLE, swa);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), is(swa));
    }

    public void testDeserializedSteeringWheelAngleAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(STEERING_WHEEL_ANGLE, "4.0f");

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }
}
