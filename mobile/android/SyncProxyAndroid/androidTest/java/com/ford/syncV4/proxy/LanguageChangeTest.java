package com.ford.syncV4.proxy;

import android.test.InstrumentationTestCase;
import android.util.Log;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.protocol.ProtocolMessage;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.constants.ProtocolConstants;
import com.ford.syncV4.proxy.interfaces.IProxyListenerALM;
import com.ford.syncV4.proxy.rpc.OnLanguageChange;
import com.ford.syncV4.proxy.rpc.TestCommon;
import com.ford.syncV4.proxy.rpc.enums.AppInterfaceUnregisteredReason;
import com.ford.syncV4.proxy.rpc.enums.Language;

import org.json.JSONException;
import org.mockito.ArgumentCaptor;

import java.util.Hashtable;

import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * Tests for the case when the app is unregistered due to language change.
 *
 * Created by enikolsky on 2013-11-14.
 */
public class LanguageChangeTest extends InstrumentationTestCase {

    private static final int CALLBACK_WAIT_TIMEOUT = 500;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);
    }

    public void testCorrectLanguageChange()
            throws SyncException, JSONException {
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        SyncProxyALM proxy =
                TestCommon.getSyncProxyALMNoTransport(proxyListenerMock);
        assertNotNull(proxy);

        // send OnLanguageChange first
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        final Language lang = Language.AR_SA;
        params.put(Names.language, lang);
        final Language hmiLang = Language.CS_CZ;
        params.put(Names.hmiDisplayLanguage, hmiLang);
        proxy.dispatchIncomingMessage(
                TestCommon.createProtocolMessage(Names.OnLanguageChange, params,
                        ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        // send OnAppInterfaceUnregistered second
        params = new Hashtable<String, Object>();
        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.LANGUAGE_CHANGE;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 2));

        ArgumentCaptor<OnLanguageChange> argument =
                ArgumentCaptor.forClass(OnLanguageChange.class);
        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredAfterLanguageChange(
                ProtocolConstants.PROTOCOL_VERSION_TWO,
                argument.capture());
        assertEquals(lang, argument.getValue().getLanguage());
        assertEquals(hmiLang, argument.getValue().getHmiDisplayLanguage());
    }

    public void testLanguageChangeDifferentReason()
            throws SyncException, JSONException {
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        SyncProxyALM proxy =
                TestCommon.getSyncProxyALMNoTransport(proxyListenerMock);
        assertNotNull(proxy);
        //proxy.mWiproVersion = 2;

        // send OnLanguageChange first
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        final Language lang = Language.AR_SA;
        params.put(Names.language, lang);
        final Language hmiLang = Language.CS_CZ;
        params.put(Names.hmiDisplayLanguage, hmiLang);
        proxy.dispatchIncomingMessage(
                TestCommon.createProtocolMessage(Names.OnLanguageChange, params,
                        ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        // send OnAppInterfaceUnregistered second
        params = new Hashtable<String, Object>();
        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.MASTER_RESET;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 2));

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT).never()).onAppUnregisteredAfterLanguageChange(
                ProtocolConstants.PROTOCOL_VERSION_TWO,
                null);
    }

    public void testAppUnregisteredWithoutLanguageChange()
            throws SyncException, JSONException {
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        SyncProxyALM proxy =
                TestCommon.getSyncProxyALMNoTransport(proxyListenerMock);
        assertNotNull(proxy);
        //proxy.mWiproVersion = 2;

        // send OnAppInterfaceUnregistered
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.LANGUAGE_CHANGE;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        verify(proxyListenerMock,
                timeout(CALLBACK_WAIT_TIMEOUT)).onAppUnregisteredAfterLanguageChange(
                ProtocolConstants.PROTOCOL_VERSION_TWO,
                null);
    }

    public void testMessagesBetweenLanguageChangeAndAppUnregistered()
            throws JSONException, SyncException {
        IProxyListenerALM proxyListenerMock = mock(IProxyListenerALM.class);
        SyncProxyALM proxy =
                TestCommon.getSyncProxyALMNoTransport(proxyListenerMock);
        assertNotNull(proxy);
        //proxy.mWiproVersion = 2;

        // send OnLanguageChange first
        Hashtable<String, Object> params = new Hashtable<String, Object>();
        final Language lang = Language.AR_SA;
        params.put(Names.language, lang);
        final Language hmiLang = Language.CS_CZ;
        params.put(Names.hmiDisplayLanguage, hmiLang);
        proxy.dispatchIncomingMessage(
                TestCommon.createProtocolMessage(Names.OnLanguageChange, params,
                        ProtocolMessage.RPCTYPE_NOTIFICATION, 1));

        // send something in between
        proxy.dispatchIncomingMessage(
                TestCommon.createProtocolMessage(Names.OnCommand, null,
                        ProtocolMessage.RPCTYPE_NOTIFICATION, 2));
        proxy.dispatchIncomingMessage(
                TestCommon.createProtocolMessage(Names.Alert, null,
                        ProtocolMessage.RPCTYPE_RESPONSE, 3));

        // send OnAppInterfaceUnregistered last
        params = new Hashtable<String, Object>();
        final AppInterfaceUnregisteredReason reason =
                AppInterfaceUnregisteredReason.LANGUAGE_CHANGE;
        params.put(Names.reason, reason);
        proxy.dispatchIncomingMessage(TestCommon.createProtocolMessage(
                Names.OnAppInterfaceUnregistered, params,
                ProtocolMessage.RPCTYPE_NOTIFICATION, 4));

        ArgumentCaptor<OnLanguageChange> argument =
                ArgumentCaptor.forClass(OnLanguageChange.class);
        verify(proxyListenerMock, timeout(CALLBACK_WAIT_TIMEOUT)).
                         onAppUnregisteredAfterLanguageChange(
                                 ProtocolConstants.PROTOCOL_VERSION_TWO,
                                 argument.capture());
        assertEquals(lang, argument.getValue().getLanguage());
        assertEquals(hmiLang, argument.getValue().getHmiDisplayLanguage());
    }
}