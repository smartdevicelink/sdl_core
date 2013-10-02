package com.ford.syncV4.proxy;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.KeyboardProperties;
import com.ford.syncV4.proxy.rpc.enums.KeyboardLayout;
import com.ford.syncV4.proxy.rpc.enums.Language;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Vector;

/**
 * Created by enikolsky on 2013-10-02.
 */
public class KeyboardPropertiesTest extends TestCase {
    private static final String LANGUAGE = "language";
    private static final String KEYBOARD_LAYOUT = "keyboardLayout";
    private static final String SEND_DYNAMIC_ENTRY = "sendDynamicEntry";
    private static final String LIMITED_CHARACTER_LIST = "limitedCharacterList";
    private static final String AUTO_COMPLETE_TEXT = "autoCompleteText";

    public void testSerialization() throws JSONException {
        KeyboardProperties msg = new KeyboardProperties();
        assertNotNull(msg);

        final Language lang = Language.AR_SA;
        final KeyboardLayout kbdLayout = KeyboardLayout.QWERTZ;
        final boolean sendDynamicEntry = true;
        final Vector<String> charList = new Vector<String>() {{
            add("A");
            add("B");
        }};
        final String autoCompleteText = "1234";

        msg.setLanguage(lang);
        msg.setKeyboardLayout(kbdLayout);
        msg.setSendDynamicEntry(sendDynamicEntry);
        msg.setLimitedCharacterList(charList);
        msg.setAutoCompleteText(autoCompleteText);

        JSONObject jsonObject = msg.serializeJSON();
        assertEquals(5, jsonObject.length());
        assertEquals(lang.toString(), jsonObject.getString(LANGUAGE));
        assertEquals(kbdLayout.toString(),
                jsonObject.getString(KEYBOARD_LAYOUT));
        assertEquals(sendDynamicEntry,
                jsonObject.getBoolean(SEND_DYNAMIC_ENTRY));
        assertTrue(jsonObject.has(LIMITED_CHARACTER_LIST));
        assertEquals(autoCompleteText,
                jsonObject.getString(AUTO_COMPLETE_TEXT));
    }

    public void testNullDeserialization() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertNull(msg.getLanguage());
        assertNull(msg.getKeyboardLayout());
        assertNull(msg.getSendDynamicEntry());
        assertNull(msg.getLimitedCharacterList());
        assertNull(msg.getAutoCompleteText());
    }

    public void testLanguageAPI() {
        KeyboardProperties msg = new KeyboardProperties();

        final Language lang = Language.CS_CZ;
        msg.setLanguage(lang);

        assertEquals(lang, msg.getLanguage());
    }

    public void testRemoveLanguage() {
        KeyboardProperties msg = new KeyboardProperties();
        msg.setLanguage(Language.DA_DK);
        msg.setLanguage(null);
        assertNull(msg.getLanguage());
    }

    public void testGetLanguage() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Language lang = Language.DE_DE;
        jsonObject.put(LANGUAGE, lang);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertEquals(lang, msg.getLanguage());
    }

    public void testGetLanguageFromString() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Language lang = Language.DE_DE;
        jsonObject.put(LANGUAGE, lang.toString());

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertEquals(lang, msg.getLanguage());
    }

    public void testGetLanguageIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(LANGUAGE, 42);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertNull(msg.getLanguage());
    }

    public void testKeyboardLayoutAPI() {
        KeyboardProperties msg = new KeyboardProperties();

        final KeyboardLayout keyboardLayout = KeyboardLayout.AZERTY;
        msg.setKeyboardLayout(keyboardLayout);

        assertEquals(keyboardLayout, msg.getKeyboardLayout());
    }

    public void testRemoveKeyboardLayout() {
        KeyboardProperties msg = new KeyboardProperties();
        msg.setKeyboardLayout(KeyboardLayout.QWERTY);
        msg.setKeyboardLayout(null);
        assertNull(msg.getKeyboardLayout());
    }

    public void testGetKeyboardLayout() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final KeyboardLayout keyboardLayout = KeyboardLayout.QWERTZ;
        jsonObject.put(KEYBOARD_LAYOUT, keyboardLayout);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertEquals(keyboardLayout, msg.getKeyboardLayout());
    }

    public void testGetKeyboardLayoutFromString() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final KeyboardLayout keyboardLayout = KeyboardLayout.AZERTY;
        jsonObject.put(KEYBOARD_LAYOUT, keyboardLayout.toString());

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertEquals(keyboardLayout, msg.getKeyboardLayout());
    }

    public void testGetKeyboardLayoutIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(KEYBOARD_LAYOUT, 42);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertNull(msg.getKeyboardLayout());
    }

    public void testSendDynamicEntryAPI() {
        KeyboardProperties msg = new KeyboardProperties();

        final Boolean sendDynamicEntry = true;
        msg.setSendDynamicEntry(sendDynamicEntry);

        assertEquals(sendDynamicEntry, msg.getSendDynamicEntry());
    }

    public void testRemoveSendDynamicEntry() {
        KeyboardProperties msg = new KeyboardProperties();
        msg.setSendDynamicEntry(true);
        msg.setSendDynamicEntry(null);
        assertNull(msg.getSendDynamicEntry());
    }

    public void testGetSendDynamicEntry() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Boolean sendDynamicEntry = false;
        jsonObject.put(SEND_DYNAMIC_ENTRY, sendDynamicEntry);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertEquals(sendDynamicEntry, msg.getSendDynamicEntry());
    }

    public void testGetSendDynamicEntryFromString() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SEND_DYNAMIC_ENTRY, "true");

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertNull(msg.getSendDynamicEntry());
    }

    public void testGetSendDynamicEntryIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SEND_DYNAMIC_ENTRY, 42);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertNull(msg.getSendDynamicEntry());
    }

    public void testLimitedCharacterListAPI() {
        KeyboardProperties msg = new KeyboardProperties();

        final Vector<String> limitedCharacterList = new Vector<String>() {{
            add("A");
            add("B");
        }};
        msg.setLimitedCharacterList(limitedCharacterList);

        assertEquals(limitedCharacterList, msg.getLimitedCharacterList());
    }

    public void testRemoveLimitedCharacterList() {
        KeyboardProperties msg = new KeyboardProperties();
        final Vector<String> charList = new Vector<String>() {{
            add("A");
            add("B");
        }};
        msg.setLimitedCharacterList(charList);
        msg.setLimitedCharacterList(null);
        assertNull(msg.getLimitedCharacterList());
    }

    public void testGetLimitedCharacterList() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final Vector<String> limitedCharacterList = new Vector<String>() {{
            add("q");
            add("w");
        }};
        jsonObject.put(LIMITED_CHARACTER_LIST, limitedCharacterList);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertEquals(limitedCharacterList, msg.getLimitedCharacterList());
    }

    public void testGetLimitedCharacterListIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(LIMITED_CHARACTER_LIST, 42);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertNull(msg.getLimitedCharacterList());
    }

    public void testAutoCompleteTextAPI() {
        KeyboardProperties msg = new KeyboardProperties();

        final String autoCompleteText = "123456";
        msg.setAutoCompleteText(autoCompleteText);

        assertEquals(autoCompleteText, msg.getAutoCompleteText());
    }

    public void testRemoveAutoCompleteText() {
        KeyboardProperties msg = new KeyboardProperties();
        msg.setAutoCompleteText("asdf");
        msg.setAutoCompleteText(null);
        assertNull(msg.getAutoCompleteText());
    }

    public void testGetAutoCompleteText() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final String autoCompleteText = "qwer";
        jsonObject.put(AUTO_COMPLETE_TEXT, autoCompleteText);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertEquals(autoCompleteText, msg.getAutoCompleteText());
    }

    public void testGetAutoCompleteTextIncorrect() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(AUTO_COMPLETE_TEXT, 42);

        KeyboardProperties msg = new KeyboardProperties(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertNotNull(msg);
        assertNull(msg.getAutoCompleteText());
    }
}
