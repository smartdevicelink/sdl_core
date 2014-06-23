package com.ford.syncV4.protocol.heartbeat;

import android.test.InstrumentationTestCase;

import com.ford.syncV4.session.SessionTest;

import org.mockito.Mockito;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 19.06.14
 * Time: 14:11
 */
public class HeartbeatMonitorsManagerTest extends InstrumentationTestCase {

    private static final int TIME_OUT = 5000;

    private HeartbeatMonitorsManager manager;
    private IHeartbeatMonitor monitorSpy;

    @Override
    public void setUp() throws Exception {
        super.setUp();

        System.setProperty("dexmaker.dexcache",
                getInstrumentation().getTargetContext().getCacheDir().getPath());

        manager = new HeartbeatMonitorsManager();
        IHeartbeatMonitor monitor = new HeartbeatMonitor(SessionTest.SESSION_ID);
        monitorSpy = Mockito.spy(monitor);
    }

    public void testAddMonitor() {
        manager.addMonitor(SessionTest.SESSION_ID, TIME_OUT, true);
        assertNotNull(manager.hasMonitor(SessionTest.SESSION_ID));
    }

    public void testStartMonitor() {
        manager.addMonitor(monitorSpy);
        manager.startMonitor(SessionTest.SESSION_ID, Mockito.mock(IHeartbeatMonitorListener.class));
        Mockito.verify(monitorSpy, Mockito.times(1)).start();
    }

    public void testStopMonitor() {
        manager.addMonitor(monitorSpy);
        manager.stopMonitor(SessionTest.SESSION_ID);
        Mockito.verify(monitorSpy, Mockito.times(1)).stop();
    }

    public void testStopAllMonitors() {
        final int count = 100;
        final IHeartbeatMonitor[] monitorsSpies = new IHeartbeatMonitor[count];
        IHeartbeatMonitor monitor;
        IHeartbeatMonitor monitorSpy;
        byte sessionId = SessionTest.SESSION_ID;
        for (int i = 0; i < count; i++) {
            monitor = new HeartbeatMonitor(sessionId++);
            monitorSpy = Mockito.spy(monitor);

            monitorsSpies[i] = monitorSpy;

            manager.addMonitor(monitorSpy);
        }
        manager.stopAllMonitors();
        for (int i = 0; i < count; i++) {
            Mockito.verify(monitorsSpies[i], Mockito.times(1)).stop();
        }
    }

    public void testHeartbeatACKReceived() {
        manager.addMonitor(monitorSpy);
        manager.startMonitor(SessionTest.SESSION_ID, Mockito.mock(IHeartbeatMonitorListener.class));
        manager.heartbeatACKReceived(SessionTest.SESSION_ID);
        Mockito.verify(monitorSpy, Mockito.times(1)).heartbeatACKReceived();
    }

    public void testHeartbeatReceived() {
        manager.addMonitor(monitorSpy);
        manager.startMonitor(SessionTest.SESSION_ID, Mockito.mock(IHeartbeatMonitorListener.class));
        manager.heartbeatReceived(SessionTest.SESSION_ID);
        Mockito.verify(monitorSpy, Mockito.times(1)).heartbeatReceived();
    }

    public void testNotifyTransportOutputActivity() {
        manager.addMonitor(monitorSpy);
        manager.notifyTransportOutputActivity(SessionTest.SESSION_ID);
        Mockito.verify(monitorSpy, Mockito.times(1)).notifyTransportOutputActivity();
    }

    public void testDisposeMonitors() {
        final int count = 100;
        final IHeartbeatMonitor[] monitorsSpies = new IHeartbeatMonitor[count];
        IHeartbeatMonitor monitor;
        IHeartbeatMonitor monitorSpy;
        byte sessionId = SessionTest.SESSION_ID;
        for (int i = 0; i < count; i++) {
            monitor = new HeartbeatMonitor(sessionId++);
            monitorSpy = Mockito.spy(monitor);

            monitorsSpies[i] = monitorSpy;

            manager.addMonitor(monitorSpy);
        }
        manager.dispose();
        for (int i = 0; i < count; i++) {
            Mockito.verify(monitorsSpies[i], Mockito.times(1)).stop();
        }
        for (int i = 0; i < count; i++) {
            assertFalse(manager.hasMonitor(monitorsSpies[i].getSessionId()));
        }
    }
}