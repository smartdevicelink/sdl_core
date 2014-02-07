package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.enums.AmbientLightStatus;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for HeadLampStatus struct.
 *
 * Created by enikolsky on 2014-02-07.
 */
public class HeadLampStatusTest extends TestCase {
    private static final String AMBIENT_LIGHT_SENSOR_STATUS =
            "ambientLightSensorStatus";

    public void testMessageShouldBeCreated() {
        HeadLampStatus msg = new HeadLampStatus();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        HeadLampStatus msg = new HeadLampStatus();

        final AmbientLightStatus als = AmbientLightStatus.NIGHT;

        msg.setAmbientLightSensorStatus(als);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.getString(AMBIENT_LIGHT_SENSOR_STATUS),
                is(als.toString()));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        HeadLampStatus msg = new HeadLampStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getAmbientLightSensorStatus(), nullValue());
    }

    public void testAmbientLightSensorStatusGetterShouldReturnSetValue()
            throws JSONException {
        HeadLampStatus msg = new HeadLampStatus();

        final AmbientLightStatus als = AmbientLightStatus.INVALID;
        msg.setAmbientLightSensorStatus(als);

        assertThat(msg.getAmbientLightSensorStatus(), is(als));
    }

    public void testSettingNullAmbientLightSensorStatusShouldRemoveValue()
            throws JSONException {
        HeadLampStatus msg = new HeadLampStatus();

        msg.setAmbientLightSensorStatus(AmbientLightStatus.NIGHT);
        msg.setAmbientLightSensorStatus(null);

        assertThat(msg.getAmbientLightSensorStatus(), nullValue());
    }

    public void testDeserializedAmbientLightSensorStatusShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final AmbientLightStatus als = AmbientLightStatus.TWILIGHT_1;
        jsonObject.put(AMBIENT_LIGHT_SENSOR_STATUS, als);

        HeadLampStatus msg = new HeadLampStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getAmbientLightSensorStatus(), is(als));
    }

    public void testDeserializedAmbientLightSensorStatusAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final AmbientLightStatus als = AmbientLightStatus.TWILIGHT_2;
        jsonObject.put(AMBIENT_LIGHT_SENSOR_STATUS, als.toString());

        HeadLampStatus msg = new HeadLampStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getAmbientLightSensorStatus(), is(als));
    }

    public void testDeserializedAmbientLightSensorStatusAsFloatShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(AMBIENT_LIGHT_SENSOR_STATUS, 4.0f);

        HeadLampStatus msg = new HeadLampStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getAmbientLightSensorStatus(), nullValue());
    }
}
