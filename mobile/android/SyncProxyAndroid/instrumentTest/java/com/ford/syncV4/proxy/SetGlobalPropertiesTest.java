package com.ford.syncV4.proxy;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.KeyboardProperties;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

/**
 * Created by enikolsky on 2013-10-02.
 */
public class SetGlobalPropertiesTest extends TestCase {
    private static final String MENU_TITLE = "menuTitle";
    private static final String MENU_ICON = "menuIcon";
    private static final String KEYBOARD_PROPERTIES = "keyboardProperties";

    public void testSerialization() throws JSONException {
        SetGlobalProperties msg = new SetGlobalProperties();
        assertNotNull(msg);

        final String menuTitle = "title";
        final Image menuImage = new Image();
        final KeyboardProperties kbdProperties = new KeyboardProperties();

        msg.setMenuTitle(menuTitle);
        msg.setMenuIcon(menuImage);
        msg.setKeyboardProperties(kbdProperties);

        JSONObject jsonObject = msg.serializeJSON((byte) 2);
        assertEquals(3, jsonObject.length());
        assertEquals(menuTitle, jsonObject.getString(MENU_TITLE));
        assertTrue(jsonObject.has(MENU_ICON));
        assertTrue(jsonObject.has(KEYBOARD_PROPERTIES));
    }

    public void testNullDeserialization() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        SetGlobalProperties msg = new SetGlobalProperties(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertNull(msg.getMenuTitle());
        assertNull(msg.getMenuIcon());
        assertNull(msg.getKeyboardProperties());
    }

    public void testMenuTitleAPI() {
        SetGlobalProperties msg = new SetGlobalProperties();

        final String menuTitle = "111";
        msg.setMenuTitle(menuTitle);

        assertEquals(menuTitle, msg.getMenuTitle());
    }

    public void testRemoveMenuTitle() {
        SetGlobalProperties msg = new SetGlobalProperties();
        msg.setMenuTitle("42");
        msg.setMenuTitle(null);
        assertNull(msg.getMenuTitle());
    }

    public void testGetMenuTitle() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final String menuTitle = "asdf";
        jsonObject.put(MENU_TITLE, menuTitle);

        SetGlobalProperties msg = new SetGlobalProperties(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertEquals(menuTitle, msg.getMenuTitle());
    }

    public void testGetMenuTitleIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MENU_TITLE, 42);

        SetGlobalProperties msg = new SetGlobalProperties(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertNull(msg.getMenuTitle());
    }

    private JSONObject paramsToRequestObject(JSONObject paramsObject)
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        JSONObject requestObject = new JSONObject();
        jsonObject.put("request", requestObject);
        requestObject.put("parameters", paramsObject);
        return jsonObject;
    }

    public void testMenuIconAPI() {
        SetGlobalProperties msg = new SetGlobalProperties();

        final Image menuIcon = new Image();
        msg.setMenuIcon(menuIcon);

        assertEquals(menuIcon, msg.getMenuIcon());
    }

    public void testRemoveMenuIcon() {
        SetGlobalProperties msg = new SetGlobalProperties();
        msg.setMenuIcon(new Image());
        msg.setMenuIcon(null);
        assertNull(msg.getMenuIcon());
    }

    public void testGetMenuIcon() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Image menuIcon = new Image();
        jsonObject.put(MENU_ICON, menuIcon);

        SetGlobalProperties msg = new SetGlobalProperties(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertEquals(menuIcon, msg.getMenuIcon());
    }

    public void testGetMenuIconIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(MENU_ICON, 42);

        SetGlobalProperties msg = new SetGlobalProperties(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertNull(msg.getMenuIcon());
    }

    public void testKeyboardPropertiesAPI() {
        SetGlobalProperties msg = new SetGlobalProperties();

        final KeyboardProperties keyboardProperties = new KeyboardProperties();
        msg.setKeyboardProperties(keyboardProperties);

        assertEquals(keyboardProperties, msg.getKeyboardProperties());
    }

    public void testRemoveKeyboardProperties() {
        SetGlobalProperties msg = new SetGlobalProperties();
        msg.setKeyboardProperties(new KeyboardProperties());
        msg.setKeyboardProperties(null);
        assertNull(msg.getKeyboardProperties());
    }

    public void testGetKeyboardProperties() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final KeyboardProperties keyboardProperties = new KeyboardProperties();
        jsonObject.put(KEYBOARD_PROPERTIES, keyboardProperties);

        SetGlobalProperties msg = new SetGlobalProperties(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertEquals(keyboardProperties, msg.getKeyboardProperties());
    }

    public void testGetKeyboardPropertiesIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(KEYBOARD_PROPERTIES, 42);

        SetGlobalProperties msg = new SetGlobalProperties(JsonRPCMarshaller
                .deserializeJSONObject(paramsToRequestObject(jsonObject)));
        assertNotNull(msg);
        assertNull(msg.getKeyboardProperties());
    }
}
