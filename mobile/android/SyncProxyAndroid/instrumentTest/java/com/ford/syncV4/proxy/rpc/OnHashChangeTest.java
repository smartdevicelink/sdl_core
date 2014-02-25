package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.constants.Names;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/25/14
 * Time: 1:50 PM
 */
public class OnHashChangeTest extends TestCase {

    private static final byte PROTOCOL_VERSION = (byte) 2;

    private String mLegalHashId = "1234567890qwertyuiop";
    private String mIllegalHashId = "dG8f5887951261W2a578o4L65X484eVl38tWr1n5474727m9iF6n2p" +
            "620335PM18z322B28Q3XG38257kY5Pn8n3584596rU7J8ldG8f5887951261W2a578o4L65X484eVl" +
            "38tWr1n5474727m9iF6n2p620335PM18z322B28Q3XG38257kY5Pn8n3584596rU7J8l";

    public void testNotificationShouldBeCreated() {
        OnHashChange onHashChange = new OnHashChange();
        assertThat(onHashChange, notNullValue());
    }

    public void testSerializedNotificationShouldContainAllSetFields() throws JSONException {
        OnHashChange onHashChange = new OnHashChange();
        onHashChange.setHashID(mLegalHashId);

        JSONObject jsonObject = onHashChange.serializeJSON(PROTOCOL_VERSION);
        assertThat(jsonObject.getString(Names.hashID), is(mLegalHashId));
    }

    public void testDeserializedNotificationWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        OnHashChange onHashChange = new OnHashChange(TestCommon.deserializeJSONRequestObject(
                jsonObject));

        assertThat(onHashChange, notNullValue());
        assertThat(onHashChange.getHashID(), nullValue());
    }

    public void testGetHashIdShouldReturnSetValue() throws JSONException {
        OnHashChange onHashChange = new OnHashChange();
        onHashChange.setHashID(mLegalHashId);
        assertThat(onHashChange.getHashID(), is(mLegalHashId));
    }

    public void testSettingNullHashIdShouldRemoveValue() throws JSONException {
        OnHashChange onHashChange = new OnHashChange();
        onHashChange.setHashID(mLegalHashId);
        onHashChange.setHashID(null);
        assertThat(onHashChange.getHashID(), nullValue());
    }

    public void testDeserializedHashIdShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hashID, mLegalHashId);

        OnHashChange onHashChange = new OnHashChange(TestCommon.deserializeJSONRequestObject(
                jsonObject));
        assertThat(onHashChange, notNullValue());
        assertThat(onHashChange.getHashID(), is(mLegalHashId));
    }

    public void testDeserializedHashIdAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.hashID, 1);

        OnHashChange onHashChange = new OnHashChange(TestCommon.deserializeJSONRequestObject(
                jsonObject));
        assertThat(onHashChange, notNullValue());
        assertThat(onHashChange.getHashID(), nullValue());
    }

    public void testSetIllegalHashId() {
        OnHashChange onHashChange = new OnHashChange();
        try {
            onHashChange.setHashID(mIllegalHashId);
            fail();
        } catch (IllegalArgumentException e) {
            /* expected */
        }
    }
}