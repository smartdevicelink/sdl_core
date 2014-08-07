package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;

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
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 07.08.14
 * Time: 15:35
 */
public class SendLocationTest extends TestCase {

    private static final String PHONE_NUMBER = "123-456-789";
    private static final String LOCATION_DESCRIPTION = "Location Description";
    private static final String LOCATION_NAME = "Location Name";
    private static final double LATITUDE_DEGREES = 88.25;
    private static final double LONGITUDE_DEGREES = 91.46;
    private static final Vector<String> ADDRESS_LINES =
            new Vector<String>(Arrays.asList("Address_Line_1", "Address_Line_2"));
    private static final Image LOCATION_IMAGE = new Image();

    private SendLocation mSendLocation;

    @Override
    public void setUp() throws Exception {
        super.setUp();

        mSendLocation = new SendLocation();
    }

    public void testSendLocationShouldBeCreated() {
        assertThat(mSendLocation, notNullValue());
    }

    public void testSerializedSendLocationShouldContainAllSetFields() throws JSONException {
        mSendLocation.setAddressLines(ADDRESS_LINES);
        mSendLocation.setLatitudeDegrees(LATITUDE_DEGREES);
        mSendLocation.setLocationDescription(LOCATION_DESCRIPTION);
        mSendLocation.setLocationImage(LOCATION_IMAGE);
        mSendLocation.setLocationName(LOCATION_NAME);
        mSendLocation.setLongitudeDegrees(LONGITUDE_DEGREES);
        mSendLocation.setPhoneNumber(PHONE_NUMBER);

        JSONObject jsonObject = mSendLocation.serializeJSON(ProtocolConstants.PROTOCOL_VERSION_TWO);

        assertThat(jsonObject.getString(Names.locationDescription), is(LOCATION_DESCRIPTION));
        assertThat(jsonObject.getString(Names.locationName), is(LOCATION_NAME));
        assertThat(jsonObject.getString(Names.phoneNumber), is(PHONE_NUMBER));
        final JSONArray messageDataJsonArray = jsonObject.getJSONArray(Names.addressLines);
        assertThat(messageDataJsonArray.length(), is(ADDRESS_LINES.size()));
        assertThat(messageDataJsonArray.getString(0), is(ADDRESS_LINES.get(0)));
        assertThat(messageDataJsonArray.getString(1), is(ADDRESS_LINES.get(1)));
        assertThat(jsonObject.getDouble(Names.latitudeDegrees), is(LATITUDE_DEGREES));
        assertThat(jsonObject.getDouble(Names.longitudeDegrees), is(LONGITUDE_DEGREES));
        assertThat(mSendLocation.getLocationImage(), is(LOCATION_IMAGE));
    }

    public void testDeserializedSendLocationWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLocationImage(), nullValue());
        assertThat(mSendLocation.getAddressLines(), nullValue());
        assertThat(mSendLocation.getLatitudeDegrees(), nullValue());
        assertThat(mSendLocation.getLocationDescription(), nullValue());
        assertThat(mSendLocation.getLocationName(), nullValue());
        assertThat(mSendLocation.getLongitudeDegrees(), nullValue());
        assertThat(mSendLocation.getPhoneNumber(), nullValue());
    }

    /**
     * PhoneNumber
     */

    public void testGetPhoneNumberShouldReturnSetValue() {
        mSendLocation.setPhoneNumber(PHONE_NUMBER);

        assertThat(mSendLocation.getPhoneNumber(), is(PHONE_NUMBER));
    }

    public void testSettingNullPhoneNumberShouldRemoveValue() {
        mSendLocation.setPhoneNumber(PHONE_NUMBER);
        mSendLocation.setPhoneNumber(null);

        assertThat(mSendLocation.getPhoneNumber(), nullValue());
    }

    public void testDeserializedPhoneNumberAsStringShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.phoneNumber, PHONE_NUMBER);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getPhoneNumber(), is(PHONE_NUMBER));
    }

    public void testDeserializedPhoneNumberAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.phoneNumber, 123456789);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getPhoneNumber(), nullValue());
    }

    /**
     * LongitudeDegrees
     */

    public void testGetLongitudeDegreesShouldReturnSetValue() {
        mSendLocation.setLongitudeDegrees(LONGITUDE_DEGREES);

        assertThat(mSendLocation.getLongitudeDegrees(), is(LONGITUDE_DEGREES));
    }

    public void testSettingNullLongitudeDegreesShouldRemoveValue() {
        mSendLocation.setLongitudeDegrees(LONGITUDE_DEGREES);
        mSendLocation.setLongitudeDegrees(null);

        assertThat(mSendLocation.getLongitudeDegrees(), nullValue());
    }

    public void testDeserializedLongitudeDegreesAsDoubleShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.longitudeDegrees, LONGITUDE_DEGREES);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLongitudeDegrees(), is(LONGITUDE_DEGREES));
    }

    public void testDeserializedLongitudeDegreesAsStringShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.longitudeDegrees, "123456789");

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLongitudeDegrees(), nullValue());
    }

    /**
     * LatitudeDegrees
     */

    public void testGetLatitudeDegreesShouldReturnSetValue() {
        mSendLocation.setLatitudeDegrees(LATITUDE_DEGREES);

        assertThat(mSendLocation.getLatitudeDegrees(), is(LATITUDE_DEGREES));
    }

    public void testSettingNullLatitudeDegreesShouldRemoveValue() {
        mSendLocation.setLatitudeDegrees(LATITUDE_DEGREES);
        mSendLocation.setLatitudeDegrees(null);

        assertThat(mSendLocation.getLatitudeDegrees(), nullValue());
    }

    public void testDeserializedLatitudeDegreesAsDoubleShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.latitudeDegrees, LATITUDE_DEGREES);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLatitudeDegrees(), is(LATITUDE_DEGREES));
    }

    public void testDeserializedLatitudeDegreesAsStringShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.latitudeDegrees, "123456789");

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLatitudeDegrees(), nullValue());
    }

    /**
     * LocationName
     */

    public void testGetLocationNameShouldReturnSetValue() {
        mSendLocation.setLocationName(LOCATION_NAME);

        assertThat(mSendLocation.getLocationName(), is(LOCATION_NAME));
    }

    public void testSettingNullLocationNameShouldRemoveValue() {
        mSendLocation.setLocationName(LOCATION_NAME);
        mSendLocation.setLocationName(null);

        assertThat(mSendLocation.getLocationName(), nullValue());
    }

    public void testDeserializedLocationNameAsStringShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.locationName, LOCATION_NAME);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLocationName(), is(LOCATION_NAME));
    }

    public void testDeserializedLocationNameDegreesAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.locationName, 123456789);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLocationName(), nullValue());
    }

    /**
     * LocationDescription
     */

    public void testGetLocationDescriptionShouldReturnSetValue() {
        mSendLocation.setLocationDescription(LOCATION_DESCRIPTION);

        assertThat(mSendLocation.getLocationDescription(), is(LOCATION_DESCRIPTION));
    }

    public void testSettingNullLocationDescriptionShouldRemoveValue() {
        mSendLocation.setLocationDescription(LOCATION_DESCRIPTION);
        mSendLocation.setLocationDescription(null);

        assertThat(mSendLocation.getLocationDescription(), nullValue());
    }

    public void testDeserializedLocationDescriptionAsStringShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.locationDescription, LOCATION_DESCRIPTION);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLocationDescription(), is(LOCATION_DESCRIPTION));
    }

    public void testDeserializedLocationDescriptionDegreesAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.locationDescription, 123456789);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLocationDescription(), nullValue());
    }

    /**
     * LocationImage
     */

    public void testGetLocationImageShouldReturnSetValue() {
        mSendLocation.setLocationImage(LOCATION_IMAGE);

        assertThat(mSendLocation.getLocationImage(), is(LOCATION_IMAGE));
    }

    public void testSettingNullLocationImageShouldRemoveValue() {
        mSendLocation.setLocationImage(LOCATION_IMAGE);
        mSendLocation.setLocationImage(null);

        assertThat(mSendLocation.getLocationImage(), nullValue());
    }

    public void testDeserializedLocationImageAsImageShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.locationImage, LOCATION_IMAGE);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLocationImage(), is(LOCATION_IMAGE));
    }

    public void testDeserializedLocationLocationImageAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.locationImage, 123456789);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getLocationImage(), nullValue());
    }

    /**
     * AddressLines
     */

    public void testGetAddressLinesShouldReturnSetValue() {
        mSendLocation.setAddressLines(ADDRESS_LINES);

        assertThat(mSendLocation.getAddressLines(), is(ADDRESS_LINES));
    }

    public void testSettingNullAddressLinesShouldRemoveValue() {
        mSendLocation.setAddressLines(ADDRESS_LINES);
        mSendLocation.setAddressLines(null);

        assertThat(mSendLocation.getAddressLines(), nullValue());
    }

    public void testDeserializedAddressLinesAsVectorShouldContainValue() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.addressLines, ADDRESS_LINES);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getAddressLines(), is(ADDRESS_LINES));
    }

    public void testDeserializedLocationAddressAsIntShouldBeNull() throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(Names.addressLines, 123456789);

        mSendLocation = new SendLocation(TestCommon.deserializeJSONRequestObject(jsonObject));

        assertThat(mSendLocation, notNullValue());
        assertThat(mSendLocation.getAddressLines(), nullValue());
    }
}