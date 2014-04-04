package com.ford.syncV4.android.receivers;

import android.bluetooth.BluetoothAdapter;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.view.KeyEvent;

import com.ford.syncV4.util.logger.Logger;

public class SyncReceiver extends BroadcastReceiver {

    private IBluetoothReceiver mBluetoothReceiverCallback;
    private ISyncReceiver mSyncReceiverCallback;

    public void setBluetoothReceiverCallback(IBluetoothReceiver bluetoothReceiverCallback) {
        mBluetoothReceiverCallback = bluetoothReceiverCallback;
    }

    public void setSyncReceiver(ISyncReceiver syncReceiverCallback) {
        mSyncReceiverCallback = syncReceiverCallback;
    }

    public void onReceive(Context context, Intent intent) {
        Logger.i("SyncReceiver.onReceive()");

        String action = intent.getAction();

        Logger.i("Received Intent with action: " + action);

        if (action == null) {
            return;
        }

        if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
            int bluetoothState = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1);
            switch (bluetoothState) {
                case BluetoothAdapter.STATE_TURNING_OFF :
                    Logger.i("Bluetooth state STATE_TURNING_OFF");
                    if (mBluetoothReceiverCallback != null) {
                        mBluetoothReceiverCallback.onBluetoothTurningOff();
                    }
                    break;
                case BluetoothAdapter.STATE_TURNING_ON :
                    Logger.i("Bluetooth state STATE_TURNING_ON");
                    break;
                case BluetoothAdapter.STATE_CONNECTED :
                    Logger.i("Bluetooth state STATE_CONNECTED");
                    break;
                case BluetoothAdapter.STATE_CONNECTING :
                    Logger.i("Bluetooth state STATE_CONNECTING");
                    break;
                case BluetoothAdapter.STATE_DISCONNECTED :
                    Logger.i("Bluetooth state STATE_DISCONNECTED");
                    break;
                case BluetoothAdapter.STATE_DISCONNECTING :
                    Logger.i("Bluetooth state STATE_DISCONNECTING");
                    break;
                case BluetoothAdapter.STATE_OFF :
                    Logger.i("Bluetooth state STATE_OFF");
                    if (mBluetoothReceiverCallback != null) {
                        mBluetoothReceiverCallback.onBluetoothOff();
                    }
                    break;
                case BluetoothAdapter.STATE_ON :
                    Logger.i("Bluetooth state STATE_ON");
                    if (mBluetoothReceiverCallback != null) {
                        mBluetoothReceiverCallback.onBluetoothOn();
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
            if (mSyncReceiverCallback != null) {
                mSyncReceiverCallback.onReceive();
            }
        }
    }
}