package com.ford.syncV4.proxy;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.exception.SyncExceptionCause;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.enums.SyncDisconnectedReason;
import com.ford.syncV4.trace.SyncTrace;
import com.ford.syncV4.util.DebugTool;

class ProxyHeartBeat {
    private static final String SYNC_LIB_TRACE_KEY = "42baba60-eb57-11df-98cf-0800200c9a66";

    private final String THREAD_NAME = "SyncHeartbeat";
    private final long POLLING_INTERVAL = 1000; // 1 second

    private SyncProxyBase syncProxyBase;

    private boolean _heartBeatEnabled = false;
    private boolean _isHalted = false;

    private Thread _heartbeatThread = null;
    private boolean _heartbeatActive = false;
    private int _pendingRequestCount = 0;
    private long _interfaceMostRecentActivityTimestamp_ms = 0;
    private long _mostRecentHeartbeatSentTimestamp_ms = 0;
    private boolean _heartbeatRequestInProgress = false;

    private long _interfaceIdleTimeLimit = 3000; // 3 seconds
    private long _heartbeatResponsePastDueTimeLimit = 2000; // 2 seconds

    public ProxyHeartBeat(SyncProxyBase syncProxyBase) {
        this.syncProxyBase = syncProxyBase;

        // Set initial value of heart beat members
        _heartbeatRequestInProgress = false;
        _heartbeatActive = false;
        _interfaceMostRecentActivityTimestamp_ms = 0;
        _mostRecentHeartbeatSentTimestamp_ms = 0;
        _pendingRequestCount = 0;

        _heartbeatThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (!_isHalted) {
                        heartbeatProcessing();
                    }
                } catch (InterruptedException e) {
                    SyncTrace.logProxyEvent("Heartbeat thread interupted.",
                            SYNC_LIB_TRACE_KEY);
                }
            }
        });
        _heartbeatThread.setName(THREAD_NAME);
        _heartbeatThread.setDaemon(true);
        _heartbeatActive = true;
    }

    // Method to start the heart beat
    public void startHeartBeat() {
        _heartbeatThread.start();
    }

    // Set heart beat active
    public void setHeartBeatActive(boolean isActive) {
        if (_heartBeatEnabled) {
            _heartbeatActive = isActive;
        }
    }

    // Manage most-recent activity timestamp for pseudo-heartbeat
    // TODO Remove on next release if proven unnecessary
    @Deprecated
    synchronized void changePendingRequestCount(boolean incrementCount) {
        if (_heartBeatEnabled) {
            if (incrementCount) {
                _pendingRequestCount++;
            } else {
                _pendingRequestCount--;
            }
        }
    }

    synchronized void recordMostRecentHeartbeatSentTimestamp() {
        if (_heartBeatEnabled) {
            _mostRecentHeartbeatSentTimestamp_ms = System.currentTimeMillis();
        }
    }

    synchronized void recordMostRecentIncomingProtocolInterfaceActivity() {
        if (_heartBeatEnabled) {
            _interfaceMostRecentActivityTimestamp_ms =
                    System.currentTimeMillis();
            if (_heartbeatRequestInProgress) {
                cancelHeartbeatRequest();
            }
        }
    }

    private synchronized boolean heartbeatRequired() {
        long idleTime = System.currentTimeMillis() - _interfaceMostRecentActivityTimestamp_ms;

        return (idleTime >= _interfaceIdleTimeLimit);
    }

    private synchronized boolean heartbeatResponsePastDue() {
        long heartbeatElapsedTime = System.currentTimeMillis() - _mostRecentHeartbeatSentTimestamp_ms;

        return (heartbeatElapsedTime >= _heartbeatResponsePastDueTimeLimit);
    }

    private void cancelHeartbeatRequest() {
        _heartbeatRequestInProgress = false;
    }

    private void issueHeartbeatRequest() {
        // FIXME: construct HB according to the updated protocol
        SetGlobalProperties req = new SetGlobalProperties();
        req.setCorrelationID(syncProxyBase.HEARTBEAT_CORRELATION_ID);

        try {
            syncProxyBase.sendRPCRequestPrivate(req);
            _heartbeatRequestInProgress = true;
        } catch (Exception ex) {
            // If sending the heartbeat request fails, cancel the heartbeat monitoring
            DebugTool.logError(
                    "Failure issuing heartbeat request: " + ex.toString(), ex);
            halt();
            _heartBeatEnabled = false;
        } // end-catch
    } // end-method

    public void receivedHeartBeatResponse() {
        _heartbeatRequestInProgress = false;
    }

    private void heartbeatProcessing() throws InterruptedException {
        // This function polls to manage an RPC-level heartbeat to more quickly detect
        // a lost connection to SYNC.

        Thread.sleep(POLLING_INTERVAL);

        if (_heartbeatRequestInProgress) {
            if (heartbeatResponsePastDue()) {
                DebugTool.logError("HEARTBEAT PAST DUE (i.e. not received within " +  _heartbeatResponsePastDueTimeLimit + "ms)");
                if (syncProxyBase._advancedLifecycleManagementEnabled) {
                    // Heartbeat past due and connection lost, interface no longer valid
                    syncProxyBase._appInterfaceRegisterd = false;
                    synchronized(SyncProxyBase.APP_INTERFACE_REGISTERED_LOCK) {
                        SyncProxyBase.APP_INTERFACE_REGISTERED_LOCK.notify();
                    }

                    // Cycle the proxy with HEARTBEAT_PAST_DUE as the disconnect reason
                    syncProxyBase.cycleProxy(
                            SyncDisconnectedReason.HEARTBEAT_PAST_DUE);
                } else {
                    syncProxyBase.notifyProxyClosed("HeartbeatPastDue",
                            new SyncException("Heartbeat past due.",
                                    SyncExceptionCause.HEARTBEAT_PAST_DUE));
                }
                return;
            } // end-if
        } else {
            // There's not a heartbeat request in-flight at the moment ...
            //
            // If heartbeat is deactivated (for whatever reason), then just
            // iterate loop.
            if (!_heartbeatActive) {
                return; // Do not need to request a heartbeat
            } // end-if

            //if (heartbeatRequired()) && m_pendingRequestCount == 0) {
            if (heartbeatRequired()) {
                issueHeartbeatRequest();
            } // end-if
        } // end-if
    } // end-method

    void halt() {
        _isHalted = true;

        if (_heartbeatThread != null) {
            try	{
                _heartbeatThread.interrupt();
            } catch (Exception ex) {
                DebugTool.logError("Failure interrupting heartbeat thread: " + ex.toString(), ex);
            }
            _heartbeatThread = null;
        } // end-if
    }
}
