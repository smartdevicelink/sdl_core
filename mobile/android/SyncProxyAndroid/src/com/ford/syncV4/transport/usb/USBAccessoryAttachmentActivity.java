package com.ford.syncV4.transport.usb;

import android.app.Activity;
import android.content.Intent;
import android.hardware.usb.UsbManager;
import android.util.Log;

/**
 * The USBAccessoryAttachmentActivity is a proxy to listen for
 * USB_ACCESSORY_ATTACHED intents.
 *
 * Unfortunately, the USB_ACCESSORY_ATTACHED intent can only be sent to an
 * activity. So this class is a workaround to get that intent.
 *
 * Some reference: http://stackoverflow.com/questions/6981736/android-3-1-usb-host-broadcastreceiver-does-not-receive-usb-device-attached/9814826#9814826
 *
 * Inspired by OpenXC-Android: https://github.com/openxc/openxc-android
 *
 * <strong>NOTA BENE:</strong> An application that wants to use USB transport
 * must make the following changes to AndroidManifest.xml:
 *
 * 1. add these lines to <manifest>…</manifest>:
 * <!-- Required to use the USB Accessory mode -->
 * <uses-feature android:name="android.hardware.usb.accessory"/>
 *
 * 2. add these lines to <application>…</application>:
 * <activity android:name="com.ford.syncV4.transport.usb.USBAccessoryAttachmentActivity">
 *     <intent-filter>
 *         <action android:name="android.hardware.usb.action.USB_ACCESSORY_ATTACHED"/>
 *     </intent-filter>
 *     <meta-data
 *         android:name="android.hardware.usb.action.USB_ACCESSORY_ATTACHED"
 *         android:resource="@xml/accessory_filter"/>
 * </activity>
 *
 * 3. set minimum SDK version to 12:
 * <uses-sdk android:minSdkVersion="12"/>
 */
public class USBAccessoryAttachmentActivity extends Activity {
    private static final String TAG =
            USBAccessoryAttachmentActivity.class.getSimpleName();

    @Override
    protected void onResume() {
        super.onResume();

        final Intent intent = getIntent();
        String action = intent.getAction();
        Log.d(TAG, "Resumed with action: " + action);

        if (UsbManager.ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
            Intent usbAccessoryAttachedIntent =
                    new Intent(USBTransport.ACTION_USB_ACCESSORY_ATTACHED);
            usbAccessoryAttachedIntent.putExtra(UsbManager.EXTRA_ACCESSORY,
                    intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY));
            sendBroadcast(usbAccessoryAttachedIntent);
        }

        finish();
    }
}
