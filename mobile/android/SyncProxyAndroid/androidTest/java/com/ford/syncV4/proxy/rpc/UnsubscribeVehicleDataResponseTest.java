package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataResultCode;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for UnsubscribeVehicleDataResponse response (new params only).
 *
 * Created by enikolsky on 2014-01-27.
 */
public class UnsubscribeVehicleDataResponseTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String STEERING_WHEEL_ANGLE = "steeringWheelAngle";

    public void testMessageShouldBeCreated() {
        UnsubscribeVehicleDataResponse msg =
                new UnsubscribeVehicleDataResponse();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        UnsubscribeVehicleDataResponse msg =
                new UnsubscribeVehicleDataResponse();

        msg.setSteeringWheelAngle(new VehicleDataResult());

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.has(STEERING_WHEEL_ANGLE), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        UnsubscribeVehicleDataResponse msg = new UnsubscribeVehicleDataResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testSteeringWheelAngleGetterShouldReturnSetValue()
            throws JSONException {
        UnsubscribeVehicleDataResponse msg =
                new UnsubscribeVehicleDataResponse();

        final VehicleDataResult swa = new VehicleDataResult();
        swa.setResultCode(VehicleDataResultCode.DATA_ALREADY_SUBSCRIBED);
        msg.setSteeringWheelAngle(swa);

        assertThat(msg.getSteeringWheelAngle(), is(swa));
    }

    public void testSettingNullSteeringWheelAngleShouldRemoveValue()
            throws JSONException {
        UnsubscribeVehicleDataResponse msg =
                new UnsubscribeVehicleDataResponse();

        msg.setSteeringWheelAngle(new VehicleDataResult());
        msg.setSteeringWheelAngle(null);

        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testDeserializedSteeringWheelAngleShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final VehicleDataResult swa = new VehicleDataResult();
        swa.setResultCode(VehicleDataResultCode.DATA_NOT_AVAILABLE);
        swa.setDataType(VehicleDataType.VEHICLEDATA_STEERINGWHEEL);
        jsonObject.put(STEERING_WHEEL_ANGLE, swa);

        UnsubscribeVehicleDataResponse msg = new UnsubscribeVehicleDataResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), is(swa));
    }

    public void testDeserializedSteeringWheelAngleAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(STEERING_WHEEL_ANGLE, 42);

        UnsubscribeVehicleDataResponse msg = new UnsubscribeVehicleDataResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }

    public void testDeserializedSteeringWheelAngleAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(STEERING_WHEEL_ANGLE, "true");

        UnsubscribeVehicleDataResponse msg = new UnsubscribeVehicleDataResponse(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
    }
}
