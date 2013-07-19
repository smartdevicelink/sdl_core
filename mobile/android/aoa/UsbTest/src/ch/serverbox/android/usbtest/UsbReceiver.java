package ch.serverbox.android.usbtest;

import com.android.future.usb.UsbAccessory;
import com.android.future.usb.UsbManager;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class UsbReceiver extends BroadcastReceiver{

	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (UsbManager.ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
				UsbAccessory accessory = UsbManager.getAccessory(intent);
				Log.d("USB","Attached!");
				if (intent.getBooleanExtra(
						UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
					//openAccessory(accessory);
				} else {
					Log.d("USB", "permission denied for accessory "
							+ accessory);
				}
		} else if (UsbManager.ACTION_USB_ACCESSORY_DETACHED.equals(action)) {
			UsbAccessory accessory = UsbManager.getAccessory(intent);
		}
	}		
}


