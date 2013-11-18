package com.ford.syncV4.proxy.rpc;

import android.test.AndroidTestCase;

import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Vector;

/**
 * Created by Andrew Batutin on 11/18/13.
 */
public class ResetGlobalPropertiesTest extends AndroidTestCase{

    public void testSerialization() throws JSONException {
        ResetGlobalProperties msg = new ResetGlobalProperties();

        Vector<GlobalProperty> properties = new Vector<GlobalProperty>();
        assertNotNull(msg);
        properties.add(GlobalProperty.HELPPROMPT);
        properties.add(GlobalProperty.MENUICON);

        msg.setProperties(properties);

        JSONObject jsonObject = msg.serializeJSON((byte) 2);
        assertEquals(1, jsonObject.length());
        JSONArray result = jsonObject.getJSONArray("properties");
        assertEquals(GlobalProperty.HELPPROMPT, result.get(0));
        assertEquals(GlobalProperty.MENUICON, result.get(1));
    }

}
