package com.ford.syncV4.android.receivers;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;
import android.view.KeyEvent;

import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.util.DebugTool;

public class SyncReceiver extends BroadcastReceiver {
	static final String TAG = "SyncProxyTester";
	
	public void onReceive(Context context, Intent intent) {
		DebugTool.logInfo("SyncReceiver.onReceive()");
		DebugTool.logInfo("Received Intent with action: " + intent.getAction());
		Log.i(TAG, "Received Intent with action: " + intent.getAction());
		final BluetoothDevice bluetoothDevice = (BluetoothDevice) intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
		BluetoothAdapter mBtAdapter;
		ProxyService serviceInstance = ProxyService.getInstance();
		
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
		if (intent.getAction().equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
				if ((intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1) == (BluetoothAdapter.STATE_TURNING_OFF))){
					if (serviceInstance != null){
						Log.i(TAG, "Bt off stop service");
						Intent stopIntent = new Intent(context, ProxyService.class);
						stopIntent.putExtras(intent);
						context.stopService(stopIntent);
					}
				} else if ((intent.getIntExtra(BluetoothAdapter.EXTRA_STATE, -1) == (BluetoothAdapter.STATE_TURNING_ON))){
						Log.i(TAG, "Bt on");
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
		
		if (intent.getAction().compareTo(Intent.ACTION_MEDIA_BUTTON) == 0) {
			KeyEvent event = (KeyEvent) intent.getParcelableExtra(Intent.EXTRA_KEY_EVENT);
			if (event.getKeyCode() == KeyEvent.KEYCODE_MEDIA_PLAY_PAUSE) {
				abortBroadcast();
			}
			
		}

		if (intent.getAction().equals(android.media.AudioManager.ACTION_AUDIO_BECOMING_NOISY)) {
			// signal your service to stop playback
			if (serviceInstance != null){
				serviceInstance.pauseAnnoyingRepetitiveAudio();
			}	
		}
	}
}

