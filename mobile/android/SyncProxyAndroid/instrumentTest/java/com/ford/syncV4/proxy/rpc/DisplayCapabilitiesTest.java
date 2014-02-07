package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;

import junit.framework.TestCase;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Arrays;
import java.util.Vector;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for DisplayCapabilities struct (new parameters only).
 *
 * Created by enikolsky on 2014-02-07.
 */
public class DisplayCapabilitiesTest extends TestCase {
    private static final String IMAGE_FIELDS = "imageFields";
    private static final String GRAPHIC_SUPPORTED = "graphicSupported";

    public void testMessageShouldBeCreated() {
        DisplayCapabilities msg = new DisplayCapabilities();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        final Vector<ImageField> imageFields =
                new Vector<ImageField>(Arrays.asList(new ImageField()));
        final boolean graphicSupported = true;

        msg.setImageFields(imageFields);
        msg.setGraphicSupported(graphicSupported);

        JSONObject jsonObject = msg.serializeJSON();
        final JSONArray jsonArray = jsonObject.getJSONArray(IMAGE_FIELDS);
        assertThat(jsonArray.length(), is(imageFields.size()));
        assertThat(jsonObject.getBoolean(GRAPHIC_SUPPORTED),
                is(graphicSupported));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getImageFields(), nullValue());
        assertThat(msg.getGraphicSupported(), nullValue());
    }

    public void testImageFieldsGetterShouldReturnSetValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        final Vector<ImageField> imageFields =
                new Vector<ImageField>(Arrays.asList(new ImageField()));
        msg.setImageFields(imageFields);

        assertThat(msg.getImageFields(), is(imageFields));
    }

    public void testSettingNullImageFieldsShouldRemoveValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        msg.setImageFields(
                new Vector<ImageField>(Arrays.asList(new ImageField())));
        msg.setImageFields(null);

        assertThat(msg.getImageFields(), nullValue());
    }

    public void testDeserializedImageFieldsAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<ImageField> imageFields =
                new Vector<ImageField>(Arrays.asList(new ImageField()));
        jsonObject.put(IMAGE_FIELDS, imageFields);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageFields(), is(imageFields));
    }

    public void testDeserializedImageFieldsAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(IMAGE_FIELDS, "image");

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageFields(), nullValue());
    }

    public void testDeserializedImageFieldsAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(IMAGE_FIELDS, 1);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageFields(), nullValue());
    }

    public void testGraphicSupportedGetterShouldReturnSetValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        final boolean graphicSupported = true;
        msg.setGraphicSupported(graphicSupported);

        assertThat(msg.getGraphicSupported(), is(graphicSupported));
    }

    public void testSettingNullGraphicSupportedShouldRemoveValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        msg.setGraphicSupported(true);
        msg.setGraphicSupported(null);

        assertThat(msg.getGraphicSupported(), nullValue());
    }

    public void testDeserializedGraphicSupportedAsBooleanShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final boolean graphicSupported = true;
        jsonObject.put(GRAPHIC_SUPPORTED, graphicSupported);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getGraphicSupported(), is(graphicSupported));
    }

    public void testDeserializedGraphicSupportedAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(GRAPHIC_SUPPORTED, "true");

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getGraphicSupported(), nullValue());
    }

    public void testDeserializedGraphicSupportedAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(GRAPHIC_SUPPORTED, 1);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getGraphicSupported(), nullValue());
    }
}
