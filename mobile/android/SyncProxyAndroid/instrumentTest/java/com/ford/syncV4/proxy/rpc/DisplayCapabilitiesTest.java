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
    private static final String TEMPLATES_AVAILABLE = "templatesAvailable";
    private static final String NUM_CUSTOM_PRESETS_AVAILABLE =
            "numCustomPresetsAvailable";
    private static final String SCREEN_PARAMS = "screenParams";

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
        final Vector<String> templatesAvailable =
                new Vector<String>(Arrays.asList("ABC", "ZYX"));
        final int numCustomPresetsAvailable = 45;
        final ScreenParams screenParams = new ScreenParams();

        msg.setImageFields(imageFields);
        msg.setGraphicSupported(graphicSupported);
        msg.setTemplatesAvailable(templatesAvailable);
        msg.setNumCustomPresetsAvailable(numCustomPresetsAvailable);
        msg.setScreenParams(screenParams);

        JSONObject jsonObject = msg.serializeJSON();
        final JSONArray imgsJsonArray = jsonObject.getJSONArray(IMAGE_FIELDS);
        assertThat(imgsJsonArray.length(), is(imageFields.size()));
        assertThat(jsonObject.getBoolean(GRAPHIC_SUPPORTED),
                is(graphicSupported));
        final JSONArray tmplsJsonArray =
                jsonObject.getJSONArray(TEMPLATES_AVAILABLE);
        assertThat(tmplsJsonArray.length(), is(templatesAvailable.size()));
        assertThat(jsonObject.getInt(NUM_CUSTOM_PRESETS_AVAILABLE),
                is(numCustomPresetsAvailable));
        assertThat(jsonObject.has(SCREEN_PARAMS), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getImageFields(), nullValue());
        assertThat(msg.getGraphicSupported(), nullValue());
        assertThat(msg.getTemplatesAvailable(), nullValue());
        assertThat(msg.getNumCustomPresetsAvailable(), nullValue());
        assertThat(msg.getScreenParams(), nullValue());
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

    public void testDeserializedImageFieldsAsVectorShouldContainValue()
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

    public void testTemplatesAvailableGetterShouldReturnSetValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        final Vector<String> templatesAvailable =
                new Vector<String>(Arrays.asList("ABC", "ZYX"));
        msg.setTemplatesAvailable(templatesAvailable);

        assertThat(msg.getTemplatesAvailable(), is(templatesAvailable));
    }

    public void testSettingNullTemplatesAvailableShouldRemoveValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        msg.setTemplatesAvailable(new Vector<String>(Arrays.asList("ASDF")));
        msg.setTemplatesAvailable(null);

        assertThat(msg.getTemplatesAvailable(), nullValue());
    }

    public void testDeserializedTemplatesAvailableAsVectorShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<String> templatesAvailable =
                new Vector<String>(Arrays.asList("ABC", "ZYX"));
        jsonObject.put(TEMPLATES_AVAILABLE, templatesAvailable);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTemplatesAvailable(), is(templatesAvailable));
    }

    public void testDeserializedTemplatesAvailableAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(TEMPLATES_AVAILABLE, "image");

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTemplatesAvailable(), nullValue());
    }

    public void testDeserializedTemplatesAvailableAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(TEMPLATES_AVAILABLE, 1);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getTemplatesAvailable(), nullValue());
    }

    public void testScreenParamsGetterShouldReturnSetValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        final ScreenParams screenParams = new ScreenParams();
        msg.setScreenParams(screenParams);

        assertThat(msg.getScreenParams(), is(screenParams));
    }

    public void testSettingNullScreenParamsShouldRemoveValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        msg.setScreenParams(new ScreenParams());
        msg.setScreenParams(null);

        assertThat(msg.getScreenParams(), nullValue());
    }

    public void testDeserializedScreenParamsAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ScreenParams screenParams = new ScreenParams();
        jsonObject.put(SCREEN_PARAMS, screenParams);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getScreenParams(), is(screenParams));
    }

    public void testDeserializedScreenParamsAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SCREEN_PARAMS, "image");

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getScreenParams(), nullValue());
    }

    public void testDeserializedScreenParamsAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SCREEN_PARAMS, 1);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getScreenParams(), nullValue());
    }

    public void testNumCustomPresetsAvailableGetterShouldReturnSetValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        final int numCustomPresetsAvailable = 45;
        msg.setNumCustomPresetsAvailable(numCustomPresetsAvailable);

        assertThat(msg.getNumCustomPresetsAvailable(),
                is(numCustomPresetsAvailable));
    }

    public void testSettingNullNumCustomPresetsAvailableShouldRemoveValue()
            throws JSONException {
        DisplayCapabilities msg = new DisplayCapabilities();

        msg.setNumCustomPresetsAvailable(30);
        msg.setNumCustomPresetsAvailable(null);

        assertThat(msg.getNumCustomPresetsAvailable(), nullValue());
    }

    public void testDeserializedNumCustomPresetsAvailableAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int numCustomPresetsAvailable = 45;
        jsonObject.put(NUM_CUSTOM_PRESETS_AVAILABLE, numCustomPresetsAvailable);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getNumCustomPresetsAvailable(),
                is(numCustomPresetsAvailable));
    }

    public void testDeserializedNumCustomPresetsAvailableAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(NUM_CUSTOM_PRESETS_AVAILABLE, "42");

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getNumCustomPresetsAvailable(), nullValue());
    }

    public void testDeserializedNumCustomPresetsAvailableAsBooleanShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(NUM_CUSTOM_PRESETS_AVAILABLE, true);

        DisplayCapabilities msg = new DisplayCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getNumCustomPresetsAvailable(), nullValue());
    }
}
