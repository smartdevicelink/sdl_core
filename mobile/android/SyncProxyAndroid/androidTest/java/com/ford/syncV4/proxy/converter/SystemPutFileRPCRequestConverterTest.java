package com.ford.syncV4.proxy.converter;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.protocol.enums.MessageType;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.FileType;

import org.hamcrest.MatcherAssert;
import org.json.JSONException;
import org.json.JSONObject;

import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.List;

import static org.hamcrest.MatcherAssert.assertThat;
import static org.hamcrest.Matchers.greaterThan;
import static org.hamcrest.Matchers.greaterThanOrEqualTo;
import static org.hamcrest.Matchers.nullValue;
import static org.hamcrest.core.Is.is;
import static org.hamcrest.core.IsNull.notNullValue;

/**
 * Tests for SystemPutFileRPCRequestConverter class.
 *
 * Created by enikolsky on 2014-01-21.
 */
public class SystemPutFileRPCRequestConverterTest
        extends InstrumentationTestCase {
    private static final byte PROTOCOL_VERSION = (byte) 2;
    private static final int PUTFILE_FUNCTIONID = 32;
    private static final String NUMBER_OF_OBJECTS_IS_INCORRECT =
            "Number of objects is incorrect";
    private static final String OFFSET = "offset";
    private static final String LENGTH = "length";
    private static final String SYNC_FILENAME = "syncFileName";
    private static final String SYSTEM_FILE = "systemFile";
    private static final String FILE_TYPE = "fileType";
    private SystemPutFileRPCRequestConverter converter;
    private JsonRPCMarshaller marshaller;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        converter = new SystemPutFileRPCRequestConverter();
        marshaller = new JsonRPCMarshaller();
    }

    public void testDefaultMaxDataSizeShouldBeLargeEnough() {
        // how much large is enough?
        final int minMaxDataSize = 10000;

        MatcherAssert.assertThat(converter.getMaxDataSize(),
                greaterThanOrEqualTo(minMaxDataSize));
    }

    public void testGetProtocolMessagesShouldReturnEmptyListIfRequestIsNotPutFile() {
        assertThat(
                converter.getProtocolMessages(new Show(), (byte) 0, marshaller,
                        PROTOCOL_VERSION).size(), is(0));
    }

    public void testGetProtocolMessagesForSmallPutFileShouldReturnCorrectProtocolMessage()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;
        final byte[] data = TestCommon.getRandomBytes(32);

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setSyncFileName("file");
        msg.setBulkData(data);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        ProtocolMessage pm = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm);
        assertThat(pm.getData(), notNullValue());
        assertThat(pm.getJsonSize(), greaterThan(0));
        assertThat(pm.getSessionID(), is(sessionID));
        assertThat(pm.getCorrID(), is(correlationID));
        assertThat(pm.getBulkData(), is(data));
    }

    // or should it?
    public void testGetProtocolMessagesForPutFileWithoutBulkDataShouldReturnCorrectProtocolMessage()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setSyncFileName("file");
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        ProtocolMessage pm = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm);
        assertThat(pm.getData(), notNullValue());
        assertThat(pm.getJsonSize(), greaterThan(0));
        assertThat(pm.getSessionID(), is(sessionID));
        assertThat(pm.getCorrID(), is(correlationID));
        assertThat(pm.getBulkData(), nullValue());
    }

    public void testMediumPutFileShouldBeSplitIntoTwoProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        converter.setMaxDataSize(100);

        final int extraDataSize = 10;
        final int maxDataSize = converter.getMaxDataSize();
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String fileName = "file2";
        final FileType fileType = FileType.AUDIO_MP3;

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setBulkData(data);
        msg.setSyncFileName(fileName);
        msg.setFileType(fileType);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(2));

        ProtocolMessage pm0 = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm0);
        final byte[] json0 = pm0.getData();
        checkSystemPutFileJSON(json0, 0, maxDataSize, fileName, fileType);
        assertThat(pm0.getJsonSize(), is(json0.length));
        assertThat(pm0.getSessionID(), is(sessionID));
        assertThat(pm0.getCorrID(), is(correlationID));
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));

        ProtocolMessage pm1 = protocolMessages.get(1);
        commonPutFileProtocolMessageAsserts(pm1);
        final byte[] json1 = pm1.getData();
        checkSystemPutFileJSON(json1, maxDataSize, dataSize - maxDataSize,
                fileName, fileType);
        assertThat(pm1.getJsonSize(), is(json1.length));
        assertThat(pm1.getSessionID(), is(sessionID));
        assertThat(pm1.getCorrID(), is(correlationID));
        final byte[] data1 = Arrays.copyOfRange(data, maxDataSize, dataSize);
        assertThat(pm1.getBulkData(), is(data1));
    }

    // check a corner case when "data size" == "max data size" * N
    public void testPutFileWithDivisibleBulkDataSizeShouldBeSplitIntoTwoProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        converter.setMaxDataSize(100);

        final int maxDataSize = converter.getMaxDataSize();
        final int N = 2;
        final int dataSize = maxDataSize * N;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String fileName = "file2";
        final FileType fileType = FileType.AUDIO_MP3;

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setBulkData(data);
        msg.setSyncFileName(fileName);
        msg.setFileType(fileType);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(2));

        ProtocolMessage pm0 = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm0);
        final byte[] json0 = pm0.getData();
        checkSystemPutFileJSON(json0, 0, maxDataSize, fileName, fileType);
        assertThat(pm0.getJsonSize(), is(json0.length));
        assertThat(pm0.getSessionID(), is(sessionID));
        assertThat(pm0.getCorrID(), is(correlationID));
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));

        ProtocolMessage pm1 = protocolMessages.get(1);
        commonPutFileProtocolMessageAsserts(pm1);
        final byte[] json1 = pm1.getData();
        checkSystemPutFileJSON(json1, maxDataSize, dataSize - maxDataSize,
                fileName, fileType);
        assertThat(pm1.getJsonSize(), is(json1.length));
        assertThat(pm1.getSessionID(), is(sessionID));
        assertThat(pm1.getCorrID(), is(correlationID));
        final byte[] data1 = Arrays.copyOfRange(data, maxDataSize, dataSize);
        assertThat(pm1.getBulkData(), is(data1));
    }

    public void testBigPutFileShouldBeSplitIntoMultipleProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        final int maxDataSize = converter.getMaxDataSize();
        final int dataCopies = 12;
        final int extraDataSize = 20;
        final int dataSize = (maxDataSize * dataCopies) + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String fileName = "file2";
        final FileType fileType = FileType.AUDIO_MP3;

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setBulkData(data);
        msg.setSyncFileName(fileName);
        msg.setFileType(fileType);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(dataCopies + 1));

        for (int i = 0; i < dataCopies; ++i) {
            final ProtocolMessage pm = protocolMessages.get(i);
            final byte[] json = pm.getData();
            checkSystemPutFileJSON(json, i * maxDataSize, maxDataSize, fileName,
                    fileType);
        }

        final ProtocolMessage pm = protocolMessages.get(dataCopies);
        final byte[] json = pm.getData();
        checkSystemPutFileJSON(json, dataCopies * maxDataSize, extraDataSize,
                fileName, fileType);
    }

    public void testGetProtocolMessagesForSmallPutFileWithOffsetShouldReturnCorrectProtocolMessage()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;
        final byte[] data = TestCommon.getRandomBytes(32);

        final int offset = 4000;
        final String fileName = "file";

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setSyncFileName(fileName);
        msg.setBulkData(data);
        msg.setOffset(offset);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(1));

        ProtocolMessage pm = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm);
        final byte[] data1 = pm.getData();
        checkSystemPutFileJSON(data1, offset, data.length, fileName, null);
        assertThat(pm.getJsonSize(), is(data1.length));
        assertThat(pm.getSessionID(), is(sessionID));
        assertThat(pm.getCorrID(), is(correlationID));
        assertThat(pm.getBulkData(), is(data));
    }

    public void testMediumPutFileWithOffsetShouldBeSplitIntoTwoProtocolMessages()
            throws Exception {
        final int correlationID = 1;
        final byte sessionID = (byte) 0;

        converter.setMaxDataSize(100);

        final int extraDataSize = 10;
        final int maxDataSize = converter.getMaxDataSize();
        final int dataSize = maxDataSize + extraDataSize;
        final byte[] data = TestCommon.getRandomBytes(dataSize);

        final String fileName = "file2";
        final FileType fileType = FileType.AUDIO_MP3;
        final int offset = 4000;

        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setBulkData(data);
        msg.setSyncFileName(fileName);
        msg.setFileType(fileType);
        msg.setOffset(offset);
        msg.setCorrelationID(correlationID);

        final List<ProtocolMessage> protocolMessages =
                converter.getProtocolMessages(msg, sessionID, marshaller,
                        PROTOCOL_VERSION);

        assertThat(NUMBER_OF_OBJECTS_IS_INCORRECT, protocolMessages.size(),
                is(2));

        ProtocolMessage pm0 = protocolMessages.get(0);
        commonPutFileProtocolMessageAsserts(pm0);
        final byte[] json0 = pm0.getData();
        checkSystemPutFileJSON(json0, offset, maxDataSize, fileName, fileType);
        assertThat(pm0.getJsonSize(), is(json0.length));
        assertThat(pm0.getSessionID(), is(sessionID));
        assertThat(pm0.getCorrID(), is(correlationID));
        final byte[] data0 = Arrays.copyOfRange(data, 0, maxDataSize);
        assertThat(pm0.getBulkData(), is(data0));

        ProtocolMessage pm1 = protocolMessages.get(1);
        commonPutFileProtocolMessageAsserts(pm1);
        final byte[] json1 = pm1.getData();
        checkSystemPutFileJSON(json1, offset + maxDataSize,
                dataSize - maxDataSize, fileName, fileType);
        assertThat(pm1.getJsonSize(), is(json1.length));
        assertThat(pm1.getSessionID(), is(sessionID));
        assertThat(pm1.getCorrID(), is(correlationID));
        final byte[] data1 = Arrays.copyOfRange(data, maxDataSize, dataSize);
        assertThat(pm1.getBulkData(), is(data1));
    }

    private void checkSystemPutFileJSON(byte[] data, int offset, int length,
                                        String filename, FileType fileType)
            throws JSONException {
        assertThat("JSON data must not be null", data, notNullValue());

        JSONObject jsonObject =
                new JSONObject(new String(data, Charset.defaultCharset()));
        assertThat("offset doesn't match", jsonObject.getInt(OFFSET),
                is(offset));
        assertThat("length doesn't match", jsonObject.getInt(LENGTH),
                is(length));
        assertThat("filename must be set", jsonObject.getString(SYNC_FILENAME),
                is(filename));
        assertThat("systemFile must be true",
                jsonObject.getBoolean(SYSTEM_FILE), is(true));

        if (fileType != null) {
            assertThat("fileType must be set", jsonObject.getString(FILE_TYPE),
                    is(fileType.toString()));
        }
    }

    private void commonPutFileProtocolMessageAsserts(
            ProtocolMessage protocolMessage) {
        assertThat(protocolMessage.getVersion(), is(PROTOCOL_VERSION));
        assertThat(protocolMessage.getFunctionID(), is(PUTFILE_FUNCTIONID));
        assertThat(protocolMessage.getMessageType(), is(MessageType.RPC));
    }
}
