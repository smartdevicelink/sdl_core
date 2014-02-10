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
 * Tests for ScreenParams struct.
 *
 * Created by enikolsky on 2014-02-10.
 */
public class ScreenParamsTest extends TestCase {
    private static final String RESOLUTION = "resolution";
    private static final String TOUCH_EVENT_AVAILABLE = "touchEventAvailable";

    public void testMessageShouldBeCreated() {
        ScreenParams msg = new ScreenParams();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        ScreenParams msg = new ScreenParams();

        final ImageResolution resolution = new ImageResolution();
        final TouchEventCapabilities capabilities =
                new TouchEventCapabilities();

        msg.setResolution(resolution);
        msg.setTouchEventAvailable(capabilities);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.has(RESOLUTION), is(true));
        assertThat(jsonObject.has(TOUCH_EVENT_AVAILABLE), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        ScreenParams msg = new ScreenParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getResolution(), nullValue());
        assertThat(msg.getTouchEventAvailable(), nullValue());
    }

    public void testResolutionGetterShouldReturnSetValue()
            throws JSONException {
        ScreenParams msg = new ScreenParams();

        final ImageResolution resolution = new ImageResolution();
        msg.setResolution(resolution);

        assertThat(msg.getResolution(), is(resolution));
    }

    public void testSettingNullResolutionShouldRemoveValue()
            throws JSONException {
        ScreenParams msg = new ScreenParams();

        msg.setResolution(new ImageResolution());
        msg.setResolution(null);

        assertThat(msg.getResolution(), nullValue());
    }

    public void testDeserializedResolutionAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ImageResolution resolution = new ImageResolution();
        jsonObject.put(RESOLUTION, resolution);

        ScreenParams msg = new ScreenParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResolution(), is(resolution));
    }

    public void testDeserializedResolutionAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(RESOLUTION, "4");

        ScreenParams msg = new ScreenParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResolution(), nullValue());
    }

    public void testDeserializedResolutionAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(RESOLUTION, 4);

        ScreenParams msg = new ScreenParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getResolution(), nullValue());
    }

    public void testTouchEventAvailableGetterShouldReturnSetValue()
            throws JSONException {
        ScreenParams msg = new ScreenParams();

        final TouchEventCapabilities capabilities =
                new TouchEventCapabilities();
        msg.setTouchEventAvailable(capabilities);

        assertThat(msg.getTouchEventAvailable(), is(capabilities));
    }

    public void testSettingNullTouchEventAvailableShouldRemoveValue()
            throws JSONException {
        ScreenParams msg = new ScreenParams();

        msg.setTouchEventAvailable(new TouchEventCapabilities());
        msg.setTouchEventAvailable(null);

        assertThat(msg.getTouchEventAvailable(), nullValue());
    }

    public void testDeserializedTouchEventAvailableAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final TouchEventCapabilities capabilities =
                new TouchEventCapabilities();
        jsonObject.put(TOUCH_EVENT_AVAILABLE, capabilities);

        ScreenParams msg = new ScreenParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTouchEventAvailable(), is(capabilities));
    }

    public void testDeserializedTouchEventAvailableAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(TOUCH_EVENT_AVAILABLE, "4");

        ScreenParams msg = new ScreenParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTouchEventAvailable(), nullValue());
    }

    public void testDeserializedTouchEventAvailableAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(TOUCH_EVENT_AVAILABLE, 4);

        ScreenParams msg = new ScreenParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTouchEventAvailable(), nullValue());
    }
}
