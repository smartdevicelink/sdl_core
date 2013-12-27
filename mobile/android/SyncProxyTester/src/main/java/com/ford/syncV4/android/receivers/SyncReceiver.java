package com.ford.syncV4.android.receivers;

import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.KeyEvent;

import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.util.DebugTool;

public class SyncReceiver extends BroadcastReceiver {

    private static final String TAG = "SyncProxyTester";

    private IBluetoothReceiver iBluetoothReceiverCallback;

    public void setBluetoothReceiverCallback(IBluetoothReceiver iBluetoothReceiver) {
        this.iBluetoothReceiverCallback = iBluetoothReceiver;
    }

    public void onReceive(Context context, Intent intent) {
        DebugTool.logInfo("SyncReceiver.onReceive()");

        String action = intent.getAction();

        DebugTool.logInfo("Received Intent with action: " + action);
        Log.i(TAG, "Received Intent with action: " + action);

        if (action == null) {
            return;
        }

        ProxyService serviceInstance = ProxyService.getInstance();

        if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
            int bluetoothState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1);
            switch (bluetoothState) {
                case BluetoothAdapter.STATE_TURNING_OFF :
                    Log.i(TAG, "Bluetooth state STATE_TURNING_OFF");
                    if (iBluetoothReceiverCallback != null) {
                        iBluetoothReceiverCallback.onBluetoothTurningOff();
                    }
                    break;
                case BluetoothAdapter.STATE_TURNING_ON :
                    Log.i(TAG, "Bluetooth state STATE_TURNING_ON");
                    break;
                case BluetoothAdapter.STATE_CONNECTED :
                    Log.i(TAG, "Bluetooth state STATE_CONNECTED");
                    break;
                case BluetoothAdapter.STATE_CONNECTING :
                    Log.i(TAG, "Bluetooth state STATE_CONNECTING");
                    break;
                case BluetoothAdapter.STATE_DISCONNECTED :
                    Log.i(TAG, "Bluetooth state STATE_DISCONNECTED");
                    break;
                case BluetoothAdapter.STATE_DISCONNECTING :
                    Log.i(TAG, "Bluetooth state STATE_DISCONNECTING");
                    break;
                case BluetoothAdapter.STATE_OFF :
                    Log.i(TAG, "Bluetooth state STATE_OFF");
                    if (iBluetoothReceiverCallback != null) {
                        iBluetoothReceiverCallback.onBluetoothOff();
                    }
                    break;
                case BluetoothAdapter.STATE_ON :
                    Log.i(TAG, "Bluetooth state STATE_ON");
                    if (iBluetoothReceiverCallback != null) {
                        iBluetoothReceiverCallback.onBluetoothOn();
                    }
                    break;
            }
        }

        if (action.compareTo(Intent.ACTION_MEDIA_BUTTON) == 0) {
            KeyEvent event = intent.getParcelableExtra(Intent.EXTRA_KEY_EVENT);
            if (event != null && event.getKeyCode() == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE) {
                abortBroadcast();
            }
        }

        if (action.equals(android.media.AudioManager.ACTION_AUDIO_BECOMING_NOISY)) {
            // signal your service to stop playback
            if (serviceInstance != null) {
                serviceInstance.pauseAnnoyingRepetitiveAudio();
            }
        }
    }
}