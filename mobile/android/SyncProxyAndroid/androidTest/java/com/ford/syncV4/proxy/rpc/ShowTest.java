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
 * Tests for Show request (new params only).
 *
 * Created by enikolsky on 2014-02-13.
 */
public class ShowTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String SECONDARY_GRAPHIC = "secondaryGraphic";

    public void testMessageShouldBeCreated() {
        Show msg = new Show();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        Show msg = new Show();

        final Image graphic = new Image();

        msg.setSecondaryGraphic(graphic);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.has(SECONDARY_GRAPHIC), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        Show msg = new Show(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getSecondaryGraphic(), nullValue());
    }

    // secondaryGraphic
    public void testSecondaryGraphicGetterShouldReturnSetValue()
            throws JSONException {
        Show msg = new Show();

        final Image graphic = new Image();
        msg.setSecondaryGraphic(graphic);

        assertThat(msg.getSecondaryGraphic(), is(graphic));
    }

    public void testSettingNullSecondaryGraphicShouldRemoveValue()
            throws JSONException {
        Show msg = new Show();

        msg.setSecondaryGraphic(new Image());
        msg.setSecondaryGraphic(null);

        assertThat(msg.getSecondaryGraphic(), nullValue());
    }

    public void testDeserializedSecondaryGraphicAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Image graphic = new Image();
        jsonObject.put(SECONDARY_GRAPHIC, graphic);

        Show msg = new Show(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSecondaryGraphic(), is(graphic));
    }

    public void testDeserializedSecondaryGraphicAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SECONDARY_GRAPHIC, 42);

        Show msg = new Show(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSecondaryGraphic(), nullValue());
    }

    public void testDeserializedSecondaryGraphicAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SECONDARY_GRAPHIC, "true");

        Show msg = new Show(JsonRPCMarshaller.deserializeJSONObject(
                TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getSecondaryGraphic(), nullValue());
    }
}
