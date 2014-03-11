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
 * Tests for ImageResolution struct.
 *
 * Created by enikolsky on 2014-02-07.
 */
public class ImageResolutionTest extends TestCase {
    private static final String RESOLUTION_WIDTH = "resolutionWidth";
    private static final String RESOLUTION_HEIGHT = "resolutionHeight";

    public void testMessageShouldBeCreated() {
        ImageResolution msg = new ImageResolution();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        ImageResolution msg = new ImageResolution();

        final Integer width = 100;
        final Integer height = 200;

        msg.setResolutionWidth(width);
        msg.setResolutionHeight(height);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.getInt(RESOLUTION_WIDTH), is(width));
        assertThat(jsonObject.getInt(RESOLUTION_HEIGHT), is(height));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        ImageResolution msg = new ImageResolution(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getResolutionWidth(), nullValue());
        assertThat(msg.getResolutionHeight(), nullValue());
    }

    public void testResolutionWidthGetterShouldReturnSetValue()
            throws JSONException {
        ImageResolution msg = new ImageResolution();

        final Integer width = 100;
        msg.setResolutionWidth(width);

        assertThat(msg.getResolutionWidth(), is(width));
    }

    public void testSettingNullResolutionWidthShouldRemoveValue()
            throws JSONException {
        ImageResolution msg = new ImageResolution();

        msg.setResolutionWidth(1);
        msg.setResolutionWidth(null);

        assertThat(msg.getResolutionWidth(), nullValue());
    }

    public void testDeserializedResolutionWidthAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Integer width = 100;
        jsonObject.put(RESOLUTION_WIDTH, width);

        ImageResolution msg = new ImageResolution(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResolutionWidth(), is(width));
    }

    public void testDeserializedResolutionWidthAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(RESOLUTION_WIDTH, "1");

        ImageResolution msg = new ImageResolution(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResolutionWidth(), nullValue());
    }

    public void testResolutionHeightGetterShouldReturnSetValue()
            throws JSONException {
        ImageResolution msg = new ImageResolution();

        final Integer height = 100;
        msg.setResolutionHeight(height);

        assertThat(msg.getResolutionHeight(), is(height));
    }

    public void testSettingNullResolutionHeightShouldRemoveValue()
            throws JSONException {
        ImageResolution msg = new ImageResolution();

        msg.setResolutionHeight(1);
        msg.setResolutionHeight(null);

        assertThat(msg.getResolutionHeight(), nullValue());
    }

    public void testDeserializedResolutionHeightAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Integer Height = 100;
        jsonObject.put(RESOLUTION_HEIGHT, Height);

        ImageResolution msg = new ImageResolution(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResolutionHeight(), is(Height));
    }

    public void testDeserializedResolutionHeightAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(RESOLUTION_HEIGHT, "1");

        ImageResolution msg = new ImageResolution(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResolutionHeight(), nullValue());
    }
}
