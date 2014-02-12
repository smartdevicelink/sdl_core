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
    private static final String HEAD_LAMP_STATUS = "headLampStatus";
    private static final String E_CALL_INFO = "eCallInfo";
    private static final String AIRBAG_STATUS = "airbagStatus";
    private static final String EMERGENCY_EVENT = "emergencyEvent";
    private static final String CLUSTER_MODE_STATUS = "clusterModeStatus";
    private static final String MY_KEY = "myKey";

    public void testMessageShouldBeCreated() {
        OnVehicleData msg = new OnVehicleData();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final float swa = 1.0f;
        final HeadLampStatus status = new HeadLampStatus();
        final ECallInfo eCallInfo = new ECallInfo();
        final AirbagStatus airbagStatus = new AirbagStatus();
        final EmergencyEvent emergencyEvent = new EmergencyEvent();
        final ClusterModeStatus clusterModeStatus = new ClusterModeStatus();
        final MyKey myKey = new MyKey();

        msg.setSteeringWheelAngle(swa);
        msg.setHeadLampStatus(status);
        msg.setECallInfo(eCallInfo);
        msg.setAirbagStatus(airbagStatus);
        msg.setEmergencyEvent(emergencyEvent);
        msg.setClusterModeStatus(clusterModeStatus);
        msg.setMyKey(myKey);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getDouble(STEERING_WHEEL_ANGLE),
                is((double) swa));
        assertThat(jsonObject.has(HEAD_LAMP_STATUS), is(true));
        assertThat(jsonObject.has(E_CALL_INFO), is(true));
        assertThat(jsonObject.has(AIRBAG_STATUS), is(true));
        assertThat(jsonObject.has(EMERGENCY_EVENT), is(true));
        assertThat(jsonObject.has(CLUSTER_MODE_STATUS), is(true));
        assertThat(jsonObject.has(MY_KEY), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getSteeringWheelAngle(), nullValue());
        assertThat(msg.getHeadLampStatus(), nullValue());
        assertThat(msg.getECallInfo(), nullValue());
        assertThat(msg.getAirbagStatus(), nullValue());
        assertThat(msg.getEmergencyEvent(), nullValue());
        assertThat(msg.getClusterModeStatus(), nullValue());
        assertThat(msg.getMyKey(), nullValue());
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

    public void testHeadLampStatusGetterShouldReturnSetValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final HeadLampStatus status = new HeadLampStatus();
        msg.setHeadLampStatus(status);

        assertThat(msg.getHeadLampStatus(), is(status));
    }

    public void testSettingNullHeadLampStatusShouldRemoveValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        msg.setHeadLampStatus(new HeadLampStatus());
        msg.setHeadLampStatus(null);

        assertThat(msg.getHeadLampStatus(), nullValue());
    }

    public void testDeserializedHeadLampStatusAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final HeadLampStatus status = new HeadLampStatus();
        jsonObject.put(HEAD_LAMP_STATUS, status);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getHeadLampStatus(), is(status));
    }

    public void testDeserializedHeadLampStatusAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(HEAD_LAMP_STATUS, "4.0f");

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getHeadLampStatus(), nullValue());
    }

    public void testDeserializedHeadLampStatusAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(HEAD_LAMP_STATUS, 32);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getHeadLampStatus(), nullValue());
    }

    /////////// ECallInfo
    public void testECallInfoGetterShouldReturnSetValue() throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final ECallInfo eCallInfo = new ECallInfo();
        msg.setECallInfo(eCallInfo);

        assertThat(msg.getECallInfo(), is(eCallInfo));
    }

    public void testSettingNullECallInfoShouldRemoveValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        msg.setECallInfo(new ECallInfo());
        msg.setECallInfo(null);

        assertThat(msg.getECallInfo(), nullValue());
    }

    public void testDeserializedECallInfoAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ECallInfo eCallInfo = new ECallInfo();
        jsonObject.put(E_CALL_INFO, eCallInfo);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getECallInfo(), is(eCallInfo));
    }

    public void testDeserializedECallInfoAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(E_CALL_INFO, "4.0f");

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getECallInfo(), nullValue());
    }

    public void testDeserializedECallInfoAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(E_CALL_INFO, 32);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getECallInfo(), nullValue());
    }

    /////////// AirbagStatus
    public void testAirbagStatusGetterShouldReturnSetValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final AirbagStatus airbagStatus = new AirbagStatus();
        msg.setAirbagStatus(airbagStatus);

        assertThat(msg.getAirbagStatus(), is(airbagStatus));
    }

    public void testSettingNullAirbagStatusShouldRemoveValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        msg.setAirbagStatus(new AirbagStatus());
        msg.setAirbagStatus(null);

        assertThat(msg.getAirbagStatus(), nullValue());
    }

    public void testDeserializedAirbagStatusAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final AirbagStatus airbagStatus = new AirbagStatus();
        jsonObject.put(AIRBAG_STATUS, airbagStatus);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getAirbagStatus(), is(airbagStatus));
    }

    public void testDeserializedAirbagStatusAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(AIRBAG_STATUS, "4.0f");

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getAirbagStatus(), nullValue());
    }

    public void testDeserializedAirbagStatusAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(AIRBAG_STATUS, 32);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getAirbagStatus(), nullValue());
    }

    /////////// EmergencyEvent
    public void testEmergencyEventGetterShouldReturnSetValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final EmergencyEvent emergencyEvent = new EmergencyEvent();
        msg.setEmergencyEvent(emergencyEvent);

        assertThat(msg.getEmergencyEvent(), is(emergencyEvent));
    }

    public void testSettingNullEmergencyEventShouldRemoveValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        msg.setEmergencyEvent(new EmergencyEvent());
        msg.setEmergencyEvent(null);

        assertThat(msg.getEmergencyEvent(), nullValue());
    }

    public void testDeserializedEmergencyEventAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final EmergencyEvent emergencyEvent = new EmergencyEvent();
        jsonObject.put(EMERGENCY_EVENT, emergencyEvent);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getEmergencyEvent(), is(emergencyEvent));
    }

    public void testDeserializedEmergencyEventAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(EMERGENCY_EVENT, "4.0f");

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getEmergencyEvent(), nullValue());
    }

    public void testDeserializedEmergencyEventAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(EMERGENCY_EVENT, 32);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getEmergencyEvent(), nullValue());
    }

    /////////// ClusterModeStatus
    public void testClusterModeStatusGetterShouldReturnSetValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final ClusterModeStatus clusterModeStatus = new ClusterModeStatus();
        msg.setClusterModeStatus(clusterModeStatus);

        assertThat(msg.getClusterModeStatus(), is(clusterModeStatus));
    }

    public void testSettingNullClusterModeStatusShouldRemoveValue()
            throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        msg.setClusterModeStatus(new ClusterModeStatus());
        msg.setClusterModeStatus(null);

        assertThat(msg.getClusterModeStatus(), nullValue());
    }

    public void testDeserializedClusterModeStatusAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ClusterModeStatus clusterModeStatus = new ClusterModeStatus();
        jsonObject.put(CLUSTER_MODE_STATUS, clusterModeStatus);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getClusterModeStatus(), is(clusterModeStatus));
    }

    public void testDeserializedClusterModeStatusAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(CLUSTER_MODE_STATUS, "4.0f");

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getClusterModeStatus(), nullValue());
    }

    public void testDeserializedClusterModeStatusAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(CLUSTER_MODE_STATUS, 32);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getClusterModeStatus(), nullValue());
    }

    /////////// MyKey
    public void testMyKeyGetterShouldReturnSetValue() throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        final MyKey myKey = new MyKey();
        msg.setMyKey(myKey);

        assertThat(msg.getMyKey(), is(myKey));
    }

    public void testSettingNullMyKeyShouldRemoveValue() throws JSONException {
        OnVehicleData msg = new OnVehicleData();

        msg.setMyKey(new MyKey());
        msg.setMyKey(null);

        assertThat(msg.getMyKey(), nullValue());
    }

    public void testDeserializedMyKeyAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final MyKey myKey = new MyKey();
        jsonObject.put(MY_KEY, myKey);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getMyKey(), is(myKey));
    }

    public void testDeserializedMyKeyAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MY_KEY, "4.0f");

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getMyKey(), nullValue());
    }

    public void testDeserializedMyKeyAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MY_KEY, 32);

        OnVehicleData msg = new OnVehicleData(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getMyKey(), nullValue());
    }
}
