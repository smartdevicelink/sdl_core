package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.rpc.enums.AudioType;
import com.ford.syncV4.proxy.rpc.enums.BitsPerSample;
import com.ford.syncV4.proxy.rpc.enums.SamplingRate;

import junit.framework.TestCase;

import org.json.JSONException;
import org.json.JSONObject;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.CoreMatchers.notNullValue;
import static org.hamcrest.CoreMatchers.nullValue;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for AudioPassThruCapabilities struct.
 *
 * Created by enikolsky on 2014-02-14.
 */
public class AudioPassThruCapabilitiesTest extends TestCase {
    private static final String AUDIO_TYPE = "audioType";
    private static final String BITS_PER_SAMPLE = "bitsPerSample";
    private static final String SAMPLING_RATE = "samplingRate";

    public void testMessageShouldBeCreated() {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();
        assertThat(msg, notNullValue());
    }

    public void testSerializedMessageShouldContainAllSetFields()
            throws JSONException {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();

        final SamplingRate samplingRate = SamplingRate._22KHZ;
        final BitsPerSample bitsPerSample = BitsPerSample._16_BIT;
        final AudioType audioType = AudioType.PCM;

        msg.setSamplingRate(samplingRate);
        msg.setBitsPerSample(bitsPerSample);
        msg.setAudioType(audioType);

        JSONObject jsonObject = msg.serializeJSON();
        assertThat(jsonObject.getString(SAMPLING_RATE),
                is(samplingRate.toString()));
        assertThat(jsonObject.getString(BITS_PER_SAMPLE),
                is(bitsPerSample.toString()));
        assertThat(jsonObject.getString(AUDIO_TYPE), is(audioType.toString()));
    }

    public void testDeserializedMessageWithoutParamsShouldContainNullFields()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));

        assertThat(msg, notNullValue());
        assertThat(msg.getSamplingRate(), nullValue());
        assertThat(msg.getBitsPerSample(), nullValue());
        assertThat(msg.getAudioType(), nullValue());
    }

    /// samplingRate
    public void testSamplingRateGetterShouldReturnSetValue()
            throws JSONException {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();

        final SamplingRate samplingRate = SamplingRate._22KHZ;
        msg.setSamplingRate(samplingRate);

        assertThat(msg.getSamplingRate(), is(samplingRate));
    }

    public void testSettingNullSamplingRateShouldRemoveValue()
            throws JSONException {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();

        msg.setSamplingRate(SamplingRate._8KHZ);
        msg.setSamplingRate(null);

        assertThat(msg.getSamplingRate(), nullValue());
    }

    public void testDeserializedSamplingRateAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final SamplingRate samplingRate = SamplingRate._22KHZ;
        jsonObject.put(SAMPLING_RATE, samplingRate.toString());

        AudioPassThruCapabilities msg = new AudioPassThruCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getSamplingRate(), is(samplingRate));
    }

    public void testDeserializedSamplingRateAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(SAMPLING_RATE, 0);

        AudioPassThruCapabilities msg = new AudioPassThruCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getSamplingRate(), nullValue());
    }

    /// bitsPerSample
    public void testBitsPerSampleGetterShouldReturnSetValue()
            throws JSONException {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();

        final BitsPerSample bitsPerSample = BitsPerSample._8_BIT;
        msg.setBitsPerSample(bitsPerSample);

        assertThat(msg.getBitsPerSample(), is(bitsPerSample));
    }

    public void testSettingNullBitsPerSampleShouldRemoveValue()
            throws JSONException {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();

        msg.setBitsPerSample(BitsPerSample._8_BIT);
        msg.setBitsPerSample(null);

        assertThat(msg.getBitsPerSample(), nullValue());
    }

    public void testDeserializedBitsPerSampleAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final BitsPerSample bitsPerSample = BitsPerSample._16_BIT;
        jsonObject.put(BITS_PER_SAMPLE, bitsPerSample.toString());

        AudioPassThruCapabilities msg = new AudioPassThruCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getBitsPerSample(), is(bitsPerSample));
    }

    public void testDeserializedBitsPerSampleAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(BITS_PER_SAMPLE, 0);

        AudioPassThruCapabilities msg = new AudioPassThruCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getBitsPerSample(), nullValue());
    }

    /// audioType
    public void testAudioTypeGetterShouldReturnSetValue() throws JSONException {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();

        final AudioType audioType = AudioType.PCM;
        msg.setAudioType(audioType);

        assertThat(msg.getAudioType(), is(audioType));
    }

    public void testSettingNullAudioTypeShouldRemoveValue()
            throws JSONException {
        AudioPassThruCapabilities msg = new AudioPassThruCapabilities();

        msg.setAudioType(AudioType.PCM);
        msg.setAudioType(null);

        assertThat(msg.getAudioType(), nullValue());
    }

    public void testDeserializedAudioTypeAsStringShouldContainValue()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        final AudioType audioType = AudioType.PCM;
        jsonObject.put(AUDIO_TYPE, audioType.toString());

        AudioPassThruCapabilities msg = new AudioPassThruCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getAudioType(), is(audioType));
    }

    public void testDeserializedAudioTypeAsIntShouldBeNull()
            throws JSONException {
        JSONObject jsonObject = new JSONObject();
        jsonObject.put(AUDIO_TYPE, 0);

        AudioPassThruCapabilities msg = new AudioPassThruCapabilities(
                JsonRPCMarshaller.deserializeJSONObject(jsonObject));
        assertThat(msg, notNullValue());
        assertThat(msg.getAudioType(), nullValue());
    }
}
