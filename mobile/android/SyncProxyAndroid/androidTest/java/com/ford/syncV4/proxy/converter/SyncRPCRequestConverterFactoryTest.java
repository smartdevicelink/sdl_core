package com.ford.syncV4.proxy.converter;

import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.Show;

import junit.framework.TestCase;

import static org.hamcrest.CoreMatchers.instanceOf;
import static org.hamcrest.CoreMatchers.sameInstance;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for SyncRPCRequestConverterFactory class.
 *
 * Created by enikolsky on 2014-01-21.
 */
public class SyncRPCRequestConverterFactoryTest extends TestCase {
    private SyncRPCRequestConverterFactory factory;

    @Override
    public void setUp() throws Exception {
        super.setUp();

        factory = new SyncRPCRequestConverterFactory();
    }

    public void testGetConverterShouldReturnDefaultConverterForUnknownRequest() {
        AwesomeRequest msg = new AwesomeRequest();
        assertThat(factory.getConverterForRequest(msg),
                instanceOf(DefaultRPCRequestConverter.class));
    }

    public void testGetConverterShouldReturnDefaultConverterForKnownRequest() {
        Show msg = new Show();
        assertThat(factory.getConverterForRequest(msg),
                instanceOf(DefaultRPCRequestConverter.class));
    }

    public void testGetConverterShouldReturnDefaultConverterForDefaultPutFile() {
        PutFile msg = RPCRequestFactory.buildPutFile();
        assertThat(factory.getConverterForRequest(msg),
                instanceOf(DefaultRPCRequestConverter.class));
    }

    public void testGetConverterShouldReturnDefaultConverterForNotSystemPutFile() {
        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setSystemFile(false);
        assertThat(factory.getConverterForRequest(msg),
                instanceOf(DefaultRPCRequestConverter.class));
    }

    public void testGetConverterShouldReturnSystemPutFileConverterForSystemPutFile() {
        PutFile msg = RPCRequestFactory.buildPutFile();
        msg.setSystemFile(true);
        assertThat(factory.getConverterForRequest(msg),
                instanceOf(SystemPutFileRPCRequestConverter.class));
    }

    public void testGetConverterShouldCacheDefaultConverterForKnownRequest() {
        final RPCRequest msg = new Show();
        final IRPCRequestConverter converter =
                factory.getConverterForRequest(msg);
        assertThat(factory.getConverterForRequest(msg),
                sameInstance(converter));
    }

    public void testGetConverterShouldCacheSystemPutFileConverter() {
        final PutFile msg1 = RPCRequestFactory.buildPutFile();
        msg1.setSystemFile(true);

        final PutFile msg2 = RPCRequestFactory.buildPutFile();
        msg2.setSystemFile(true);

        final IRPCRequestConverter converter =
                factory.getConverterForRequest(msg1);
        assertThat("SystemPutFileRPCRequestConverter isn't cached",
                factory.getConverterForRequest(msg2), sameInstance(converter));
    }

    public void testGetConverterShouldThrowNullPointerExceptionWhenRequestIsNull() {
        try {
            factory.getConverterForRequest(null);
            fail("Should have thrown NullPointerException");
        } catch (NullPointerException e) {
            // success
        }
    }

    private class AwesomeRequest extends RPCRequest {
        public AwesomeRequest() {
            super("AwesomeRequest");
        }
    }
}
