package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.ImageFieldName;

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
 * Tests for ImageField struct.
 *
 * Created by enikolsky on 2014-02-07.
 */
public class ImageFieldTest extends TestCase {
    private static final String NAME = "name";
    private static final String IMAGE_TYPE_SUPPORTED = "imageTypeSupported";
    private static final String IMAGE_RESOLUTION = "imageResolution";

    public void testMessageShouldBeCreated() {
        ImageField msg = new ImageField();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        ImageField msg = new ImageField();

        final ImageFieldName name = ImageFieldName.choiceImage;
        final FileType fileType = FileType.AUDIO_MP3;
        final Vector<FileType> fileTypes =
                new Vector<FileType>(Arrays.asList(fileType));
        final ImageResolution resolution = new ImageResolution();

        msg.setName(name);
        msg.setImageTypeSupported(fileTypes);
        msg.setImageResolution(resolution);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.getString(NAME), is(name.toString()));
        final JSONArray jsonArray =
                jsonObject.getJSONArray(IMAGE_TYPE_SUPPORTED);
        assertThat(jsonArray.length(), is(fileTypes.size()));
        assertThat(jsonArray.getString(0), is(fileType.toString()));
        assertThat(jsonObject.has(IMAGE_RESOLUTION), is(true));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getName(), nullValue());
        assertThat(msg.getImageTypeSupported(), nullValue());
        assertThat(msg.getImageResolution(), nullValue());
    }

    public void testNameGetterShouldReturnSetValue() throws JSONException {
        ImageField msg = new ImageField();

        final ImageFieldName name = ImageFieldName.choiceImage;
        msg.setName(name);

        assertThat(msg.getName(), is(name));
    }

    public void testSettingNullNameShouldRemoveValue() throws JSONException {
        ImageField msg = new ImageField();

        msg.setName(ImageFieldName.softButtonImage);
        msg.setName(null);

        assertThat(msg.getName(), nullValue());
    }

    public void testDeserializedNameAsEnumShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ImageFieldName name = ImageFieldName.choiceImage;
        jsonObject.put(NAME, name);

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getName(), is(name));
    }

    public void testDeserializedNameAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ImageFieldName name = ImageFieldName.choiceImage;
        jsonObject.put(NAME, name.toString());

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getName(), is(name));
    }

    public void testDeserializedNameAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(NAME, 1);

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getName(), nullValue());
    }

    public void testImageTypeSupportedGetterShouldReturnSetValue()
            throws JSONException {
        ImageField msg = new ImageField();

        final FileType fileType = FileType.AUDIO_MP3;
        final Vector<FileType> fileTypes =
                new Vector<FileType>(Arrays.asList(fileType));
        msg.setImageTypeSupported(fileTypes);

        assertThat(msg.getImageTypeSupported(), is(fileTypes));
    }

    public void testSettingNullImageTypeSupportedShouldRemoveValue()
            throws JSONException {
        ImageField msg = new ImageField();

        msg.setImageTypeSupported(new Vector<FileType>());
        msg.setImageTypeSupported(null);

        assertThat(msg.getImageTypeSupported(), nullValue());
    }

    public void testDeserializedImageTypeSupportedAsEnumShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final FileType fileType = FileType.AUDIO_MP3;
        final Vector<FileType> fileTypes =
                new Vector<FileType>(Arrays.asList(fileType));
        jsonObject.put(IMAGE_TYPE_SUPPORTED, fileTypes);

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageTypeSupported(), is(fileTypes));
    }

    public void testDeserializedImageTypeSupportedAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(IMAGE_TYPE_SUPPORTED, "IMAGE");

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageTypeSupported(), nullValue());
    }

    public void testDeserializedImageTypeSupportedAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(IMAGE_TYPE_SUPPORTED, 1);

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageTypeSupported(), nullValue());
    }

    public void testImageResolutionGetterShouldReturnSetValue()
            throws JSONException {
        ImageField msg = new ImageField();

        final ImageResolution resolution = new ImageResolution();
        resolution.setResolutionWidth(100);
        resolution.setResolutionHeight(122);
        msg.setImageResolution(resolution);

        assertThat(msg.getImageResolution(), is(resolution));
    }

    public void testSettingNullImageResolutionShouldRemoveValue()
            throws JSONException {
        ImageField msg = new ImageField();

        msg.setImageResolution(new ImageResolution());
        msg.setImageResolution(null);

        assertThat(msg.getImageResolution(), nullValue());
    }

    public void testDeserializedImageResolutionAsStructShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ImageResolution resolution = new ImageResolution();
        resolution.setResolutionWidth(100);
        resolution.setResolutionHeight(122);
        jsonObject.put(IMAGE_RESOLUTION, resolution);

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageResolution(), is(resolution));
    }

    public void testDeserializedImageResolutionAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final ImageResolution resolution = new ImageResolution();
        resolution.setResolutionWidth(100);
        resolution.setResolutionHeight(122);
        jsonObject.put(IMAGE_RESOLUTION, resolution.toString());

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageResolution(), nullValue());
    }

    public void testDeserializedImageResolutionAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(IMAGE_RESOLUTION, 1);

        ImageField msg = new ImageField(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getImageResolution(), nullValue());
    }
}
