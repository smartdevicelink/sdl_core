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
 * Tests for SetMediaClockTimer request.
 *
 * Created by enikolsky on 2014-02-12.
 */
public class SetMediaClockTimerTest extends TestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final String END_TIME = "endTime";

    public void testMessageShouldBeCreated() {
        SetMediaClockTimer msg = new SetMediaClockTimer();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        SetMediaClockTimer msg = new SetMediaClockTimer();

        final StartTime endTime = new StartTime();

        msg.setEndTime(endTime);

        JSONObject jsonObject = msg.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.has(END_TIME), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        SetMediaClockTimer msg = new SetMediaClockTimer(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));

        assertThat(msg, notNullValue());
        assertThat(msg.getEndTime(), nullValue());
    }

    public void testEndTimeGetterShouldReturnSetValue() throws JSONException {
        SetMediaClockTimer msg = new SetMediaClockTimer();

        final StartTime endTime = new StartTime();
        msg.setEndTime(endTime);

        assertThat(msg.getEndTime(), is(endTime));
    }

    public void testSettingNullEndTimeShouldRemoveValue() throws JSONException {
        SetMediaClockTimer msg = new SetMediaClockTimer();

        msg.setEndTime(new StartTime());
        msg.setEndTime(null);

        assertThat(msg.getEndTime(), nullValue());
    }

    public void testDeserializedEndTimeAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final StartTime endTime = new StartTime();
        jsonObject.put(END_TIME, endTime);

        SetMediaClockTimer msg = new SetMediaClockTimer(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getEndTime(), is(endTime));
    }

    public void testDeserializedEndTimeAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(END_TIME, 4);

        SetMediaClockTimer msg = new SetMediaClockTimer(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getEndTime(), nullValue());
    }

    public void testDeserializedEndTimeAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(END_TIME, "4.0f");

        SetMediaClockTimer msg = new SetMediaClockTimer(
                JsonRPCMarshaller.deserializeJSONObject(
                        TestCommon.paramsToRequestObject(jsonObject)));
        assertThat(msg, notNullValue());
        assertThat(msg.getEndTime(), nullValue());
    }
}
