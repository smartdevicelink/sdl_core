package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.enums.WarningLightStatus;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for TireStatus struct (only new parameters).
 *
 * Created by enikolsky on 2014-02-07.
 */
public class TireStatusTest extends TestCase {
    private static final String PRESSURE_TELLTALE = "pressureTelltale";

    public void testMessageShouldBeCreated() {
        TireStatus msg = new TireStatus();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        TireStatus msg = new TireStatus();

        final WarningLightStatus wls = WarningLightStatus.FLASH;

        msg.setPressureTelltale(wls);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.getString(PRESSURE_TELLTALE), is(wls.toString()));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        TireStatus msg = new TireStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getPressureTelltale(), nullValue());
    }

    public void testPressureTelltaleGetterShouldReturnSetValue()
            throws JSONException {
        TireStatus msg = new TireStatus();

        final WarningLightStatus wls = WarningLightStatus.FLASH;
        msg.setPressureTelltale(wls);

        assertThat(msg.getPressureTelltale(), is(wls));
    }

    public void testSettingNullPressureTelltaleShouldRemoveValue()
            throws JSONException {
        TireStatus msg = new TireStatus();

        msg.setPressureTelltale(WarningLightStatus.NOT_USED);
        msg.setPressureTelltale(null);

        assertThat(msg.getPressureTelltale(), nullValue());
    }

    public void testDeserializedPressureTelltaleShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final WarningLightStatus wls = WarningLightStatus.FLASH;
        jsonObject.put(PRESSURE_TELLTALE, wls);

        TireStatus msg = new TireStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getPressureTelltale(), is(wls));
    }

    public void testDeserializedPressureTelltaleAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final WarningLightStatus wls = WarningLightStatus.FLASH;
        jsonObject.put(PRESSURE_TELLTALE, wls.toString());

        TireStatus msg = new TireStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getPressureTelltale(), is(wls));
    }

    public void testDeserializedPressureTelltaleAsFloatShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(PRESSURE_TELLTALE, 4.0f);

        TireStatus msg = new TireStatus(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getPressureTelltale(), nullValue());
    }
}
