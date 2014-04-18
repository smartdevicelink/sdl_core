package com.ford.syncV4.protocol.heartbeat;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.proxy.rpc.TestCommon;

import org.mockito.Matchers;
import org.mockito.Mockito;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.verify;

/**
 * Tests for HeartbeatMonitor class.
 *
 * Created by enikolsky on 2013-12-26.
 */
public class HeartbeatMonitorTest extends InstrumentationTestCase {
    private static final int MAX_TIMER_DRIFT = 20;
    private static final int HEARTBEAT_TEST_INTERVAL = 30;
    private IHeartbeatMonitorListener listenerMock;
    private HeartbeatMonitor heartbeatMonitor;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        listenerMock = Mockito.mock(IHeartbeatMonitorListener.class);
        heartbeatMonitor = new HeartbeatMonitor();
    }

    @Override
    public void tearDown() throws Exception {
        listenerMock = null;
        heartbeatMonitor = null;

        super.tearDown();
    }

    public void testListenerShouldBeSet() {
        heartbeatMonitor.setListener(listenerMock);
        assertThat(heartbeatMonitor.getListener(), is(listenerMock));
    }

    public void testStartedMonitorShouldAskToSendHeartbeatAfterTimeout()
            throws InterruptedException {
        heartbeatMonitor.setListener(listenerMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();

        verify(listenerMock, timeout(HEARTBEAT_TEST_INTERVAL + MAX_TIMER_DRIFT))
                .sendHeartbeat(heartbeatMonitor);
        heartbeatMonitor.stop();
    }

    public void testMonitorShouldTimeoutOnSecondHeartbeatAndNoACK()
            throws InterruptedException {
        heartbeatMonitor.setListener(listenerMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();

        verify(listenerMock, timeout(HEARTBEAT_TEST_INTERVAL + MAX_TIMER_DRIFT))
                .sendHeartbeat(heartbeatMonitor);

        verify(listenerMock, timeout(HEARTBEAT_TEST_INTERVAL + MAX_TIMER_DRIFT))
                .heartbeatTimedOut(heartbeatMonitor);

        heartbeatMonitor.stop();
    }

    public void testStartedAndStoppedMonitorShouldNotSendHeartbeats()
            throws InterruptedException {
        heartbeatMonitor.setListener(listenerMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();
        Thread.sleep(5);
        heartbeatMonitor.stop();

        // this does not work:
//        verify(listenerMock, timeout(HEARTBEAT_TEST_INTERVAL * 3 +
//                MAX_TIMER_DRIFT).never()).sendHeartbeat(
//                Matchers.<IHeartbeatMonitor>any());

        Thread.sleep(HEARTBEAT_TEST_INTERVAL * 2 + MAX_TIMER_DRIFT);
        verify(listenerMock, never()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
    }

    public void testStartedAndStoppedImmediatelyMonitorShouldNotSendHeartbeats()
            throws InterruptedException {
        heartbeatMonitor.setListener(listenerMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();
        heartbeatMonitor.stop();

        Thread.sleep(HEARTBEAT_TEST_INTERVAL * 2 + MAX_TIMER_DRIFT);
        verify(listenerMock, never()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
    }

    public void testTransportActivityShouldResetHeartbeat()
            throws InterruptedException {
        heartbeatMonitor.setListener(listenerMock);

        final int INTERVAL = 50;
        heartbeatMonitor.setInterval(INTERVAL);
        heartbeatMonitor.start();

        final int ACTIVITY_DELAY = 40;
        Thread.sleep(ACTIVITY_DELAY);
        heartbeatMonitor.notifyTransportOutputActivity();

        Thread.sleep(INTERVAL - ACTIVITY_DELAY + 10);
        verify(listenerMock, never()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
        verify(listenerMock, timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);
        heartbeatMonitor.stop();
    }

    public void testReceivedHeartbeatACKShouldNotResetHeartbeat()
            throws InterruptedException {
        heartbeatMonitor.setListener(listenerMock);

        final int INTERVAL = 50;
        heartbeatMonitor.setInterval(INTERVAL);
        heartbeatMonitor.start();

        final int ACK_DELAY = 40;
        Thread.sleep(ACK_DELAY);
        heartbeatMonitor.heartbeatACKReceived();

        Thread.sleep(INTERVAL - ACK_DELAY + 10);
        verify(listenerMock, atLeastOnce()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
        verify(listenerMock, timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);
        heartbeatMonitor.stop();
    }

    public void testReceivedHeartbeatACKShouldResetHeartbeatThenTimeout()
            throws InterruptedException {
        heartbeatMonitor.setListener(listenerMock);

        final int INTERVAL = 50;
        heartbeatMonitor.setInterval(INTERVAL);
        heartbeatMonitor.start();

        verify(listenerMock, timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);

        IHeartbeatMonitorListener listenerMock2 =
                Mockito.mock(IHeartbeatMonitorListener.class);
        heartbeatMonitor.setListener(listenerMock2);

        verify(listenerMock2, never()).heartbeatTimedOut(
                Matchers.<IHeartbeatMonitor>any());

        final int ACK_DELAY = 10;
        Thread.sleep(ACK_DELAY);
        heartbeatMonitor.heartbeatACKReceived();

        verify(listenerMock2,
                timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);

        // NOTE: I had to decrease the ACK_DELAY from 40 ms. because sometimes
        // the test had failed here. Probably due to imprecision of scheduling,
        // heartbeatTimedOut() was called before heartbeatACKReceived().
        verify(listenerMock2, never()).heartbeatTimedOut(
                Matchers.<IHeartbeatMonitor>any());
        verify(listenerMock2,
                timeout(INTERVAL + MAX_TIMER_DRIFT)).heartbeatTimedOut(
                heartbeatMonitor);

        heartbeatMonitor.stop();
    }

}
