package com.ford.syncV4.protocol.heartbeat;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.proxy.rpc.TestCommon;

import org.mockito.Matchers;
import org.mockito.Mockito;

import static com.ford.syncV4.protocol.heartbeat.IHeartbeatMonitor.IHeartbeatMonitorDelegate;
import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;
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
    private IHeartbeatMonitorDelegate delegateMock;
    private HeartbeatMonitor heartbeatMonitor;

    @Override
    public void setUp() throws Exception {
        super.setUp();
        TestCommon.setupMocking(this);

        delegateMock = Mockito.mock(IHeartbeatMonitorDelegate.class);
        heartbeatMonitor = new HeartbeatMonitor();
    }

    @Override
    public void tearDown() throws Exception {
        delegateMock = null;
        heartbeatMonitor = null;

        super.tearDown();
    }

    public void testDelegateShouldBeSet() {
        heartbeatMonitor.setDelegate(delegateMock);
        assertThat(heartbeatMonitor.getDelegate(), is(delegateMock));
    }

    public void testStartedMonitorShouldAskToSendHeartbeatAfterTimeout()
            throws InterruptedException {
        heartbeatMonitor.setDelegate(delegateMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();

        verify(delegateMock, timeout(HEARTBEAT_TEST_INTERVAL + MAX_TIMER_DRIFT))
                .sendHeartbeat(heartbeatMonitor);
        heartbeatMonitor.stop();
    }

    public void testMonitorShouldTimeoutOnSecondHeartbeatAndNoACK()
            throws InterruptedException {
        heartbeatMonitor.setDelegate(delegateMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();

        verify(delegateMock, timeout(HEARTBEAT_TEST_INTERVAL + MAX_TIMER_DRIFT))
                .sendHeartbeat(heartbeatMonitor);

        verify(delegateMock, timeout(HEARTBEAT_TEST_INTERVAL + MAX_TIMER_DRIFT))
                .heartbeatTimedOut(heartbeatMonitor);

        heartbeatMonitor.stop();
    }

    public void testStartedAndStoppedMonitorShouldNotSendHeartbeats()
            throws InterruptedException {
        heartbeatMonitor.setDelegate(delegateMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();
        Thread.sleep(5);
        heartbeatMonitor.stop();

        // this does not work:
//        verify(delegateMock, timeout(HEARTBEAT_TEST_INTERVAL * 3 +
//                MAX_TIMER_DRIFT).never()).sendHeartbeat(
//                Matchers.<IHeartbeatMonitor>any());

        Thread.sleep(HEARTBEAT_TEST_INTERVAL * 2 + MAX_TIMER_DRIFT);
        verify(delegateMock, never()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
    }

    public void testStartedAndStoppedImmediatelyMonitorShouldNotSendHeartbeats()
            throws InterruptedException {
        heartbeatMonitor.setDelegate(delegateMock);

        heartbeatMonitor.setInterval(HEARTBEAT_TEST_INTERVAL);
        heartbeatMonitor.start();
        heartbeatMonitor.stop();

        Thread.sleep(HEARTBEAT_TEST_INTERVAL * 2 + MAX_TIMER_DRIFT);
        verify(delegateMock, never()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
    }

    public void testTransportActivityShouldResetHeartbeat()
            throws InterruptedException {
        heartbeatMonitor.setDelegate(delegateMock);

        final int INTERVAL = 50;
        heartbeatMonitor.setInterval(INTERVAL);
        heartbeatMonitor.start();

        final int ACTIVITY_DELAY = 40;
        Thread.sleep(ACTIVITY_DELAY);
        heartbeatMonitor.notifyTransportActivity();

        Thread.sleep(INTERVAL - ACTIVITY_DELAY + 10);
        verify(delegateMock, never()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
        verify(delegateMock, timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);
        heartbeatMonitor.stop();
    }

    public void testReceivedHeartbeatACKShouldResetHeartbeat()
            throws InterruptedException {
        heartbeatMonitor.setDelegate(delegateMock);

        final int INTERVAL = 50;
        heartbeatMonitor.setInterval(INTERVAL);
        heartbeatMonitor.start();

        final int ACK_DELAY = 40;
        Thread.sleep(ACK_DELAY);
        heartbeatMonitor.heartbeatACKReceived();

        Thread.sleep(INTERVAL - ACK_DELAY + 10);
        verify(delegateMock, never()).sendHeartbeat(
                Matchers.<IHeartbeatMonitor>any());
        verify(delegateMock, timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);
        heartbeatMonitor.stop();
    }

    public void testReceivedHeartbeatACKShouldResetHeartbeatThenTimeout()
            throws InterruptedException {
        heartbeatMonitor.setDelegate(delegateMock);

        final int INTERVAL = 50;
        heartbeatMonitor.setInterval(INTERVAL);
        heartbeatMonitor.start();

        verify(delegateMock, timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);


        IHeartbeatMonitorDelegate delegateMock2 =
                Mockito.mock(IHeartbeatMonitorDelegate.class);
        heartbeatMonitor.setDelegate(delegateMock2);

        final int ACK_DELAY = 40;
        Thread.sleep(ACK_DELAY);
        heartbeatMonitor.heartbeatACKReceived();

        verify(delegateMock2,
                timeout(INTERVAL + MAX_TIMER_DRIFT)).sendHeartbeat(
                heartbeatMonitor);

        verify(delegateMock2, never()).heartbeatTimedOut(
                Matchers.<IHeartbeatMonitor>any());
        verify(delegateMock2,
                timeout(INTERVAL + MAX_TIMER_DRIFT)).heartbeatTimedOut(
                heartbeatMonitor);

        heartbeatMonitor.stop();
    }
}
