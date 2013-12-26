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
                /*case BluetoothAdapter.STATE_TURNING_OFF :
                    Log.i(TAG, "Bluetooth state STATE_TURNING_OFF");
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
                    break;*/
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

		/*//if SYNC connected to phone via bluetooth, start service (which starts proxy)
		if (intent.getAction().compareTo(BluetoothDevice.ACTION_ACL_CONNECTED) == 0) {
			if(bluetoothDevice.getName() != null) {
				if (bluetoothDevice.getName().contains("SYNC")) {
					DebugTool.logInfo("ACL Event: " + bluetoothDevice.getName() + ", "
							+ bluetoothDevice.getAddress());
					if (serviceInstance == null){
						Intent startIntent = new Intent(context, ProxyService.class);  
						startIntent.putExtras(intent);
						context.startService(startIntent);
					}
				}
			}
			
		//if SYNC is disconnected from phone or BT disabled, stop service (and thus the proxy)
		} else if (intent.getAction().equals(BluetoothDevice.ACTION_ACL_DISCONNECTED)) {
			if (bluetoothDevice.getName().contains("SYNC")) {
				DebugTool.logInfo("ACL Event: " + bluetoothDevice.getName() + ", "
						+ bluetoothDevice.getAddress());
				if (serviceInstance != null){
					Intent stopIntent = new Intent(context, ProxyService.class);
					stopIntent.putExtras(intent);
					context.stopService(stopIntent);
				}
			}*/

        //Listen for STATE_CHANGED as double-check when BT turned off
        //} else
        if (action.equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
            if ((intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1) == BluetoothAdapter.STATE_TURNING_OFF)) {
                if (serviceInstance != null) {
                    //Log.i(TAG, "Bt off stop service");
                    //Intent stopIntent = new Intent(context, ProxyService.class);
                    //stopIntent.putExtras(intent);
                    //context.stopService(stopIntent);
                }
            } else if ((intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1) == BluetoothAdapter.STATE_TURNING_ON)) {
                //Log.i(TAG, "Bt on");
						/*if (serviceInstance == null){
							Log.i(TAG, "Bt on start service");
							Intent startIntent = new Intent(context, ProxyService.class);  
							startIntent.putExtras(intent);
							context.startService(startIntent);
						}*/
            }
            //Listen for phone reboot and start service
/*		} else if (intent.getAction().equals(Intent.ACTION_BOOT_COMPLETED)) {
			mBtAdapter = BluetoothAdapter.getDefaultAdapter();
			if (mBtAdapter != null)
			{
				if (mBtAdapter.isEnabled()){
					Intent startIntent = new Intent(context, ProxyService.class);  
					startIntent.putExtras(intent);
					context.startService(startIntent);
				}
			}*/
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