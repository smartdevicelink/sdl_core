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
 * Tests for MenuParams struct.
 *
 * Created by enikolsky on 2014-02-12.
 */
public class MenuParamsTest extends TestCase {
    private static final String PARENT_ID = "parentID";
    private static final String POSITION = "position";
    private static final String MENU_NAME = "menuName";

    public void testMessageShouldBeCreated() {
        MenuParams msg = new MenuParams();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        MenuParams msg = new MenuParams();

        final int parentID = 1;
        final int position = 34;
        final String menuName = "abcd";

        msg.setParentID(parentID);
        msg.setPosition(position);
        msg.setMenuName(menuName);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.getInt(PARENT_ID), is(parentID));
        assertThat(jsonObject.getInt(POSITION), is(position));
        assertThat(jsonObject.getString(MENU_NAME), is(menuName));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        MenuParams msg = new MenuParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getParentID(), nullValue());
        assertThat(msg.getPosition(), nullValue());
        assertThat(msg.getMenuName(), nullValue());
    }

    /// parentID
    public void testParentIDGetterShouldReturnSetValue() throws JSONException {
        MenuParams msg = new MenuParams();

        final int parentID = 1;
        msg.setParentID(parentID);

        assertThat(msg.getParentID(), is(parentID));
    }

    public void testSettingNullParentIDShouldRemoveValue()
            throws JSONException {
        MenuParams msg = new MenuParams();

        msg.setParentID(2);
        msg.setParentID(null);

        assertThat(msg.getParentID(), nullValue());
    }

    public void testDeserializedParentIDAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int parentID = 1;
        jsonObject.put(PARENT_ID, parentID);

        MenuParams msg = new MenuParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getParentID(), is(parentID));
    }

    public void testDeserializedParentIDAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(PARENT_ID, "4");

        MenuParams msg = new MenuParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getParentID(), nullValue());
    }

    /// position
    public void testPositionGetterShouldReturnSetValue() throws JSONException {
        MenuParams msg = new MenuParams();

        final int position = 34;
        msg.setPosition(position);

        assertThat(msg.getPosition(), is(position));
    }

    public void testSettingNullPositionShouldRemoveValue()
            throws JSONException {
        MenuParams msg = new MenuParams();

        msg.setPosition(2);
        msg.setPosition(null);

        assertThat(msg.getPosition(), nullValue());
    }

    public void testDeserializedPositionAsIntShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final int position = 34;
        jsonObject.put(POSITION, position);

        MenuParams msg = new MenuParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getPosition(), is(position));
    }

    public void testDeserializedPositionAsStringShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(POSITION, "4");

        MenuParams msg = new MenuParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getPosition(), nullValue());
    }

    /// menuName
    public void testMenuNameGetterShouldReturnSetValue() throws JSONException {
        MenuParams msg = new MenuParams();

        final String menuName = "abcd";
        msg.setMenuName(menuName);

        assertThat(msg.getMenuName(), is(menuName));
    }

    public void testSettingNullMenuNameShouldRemoveValue()
            throws JSONException {
        MenuParams msg = new MenuParams();

        msg.setMenuName("123");
        msg.setMenuName(null);

        assertThat(msg.getMenuName(), nullValue());
    }

    public void testDeserializedMenuNameAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final String menuName = "abcd";
        jsonObject.put(MENU_NAME, menuName);

        MenuParams msg = new MenuParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMenuName(), is(menuName));
    }

    public void testDeserializedMenuNameAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MENU_NAME, 123);

        MenuParams msg = new MenuParams(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getMenuName(), nullValue());
    }
}
