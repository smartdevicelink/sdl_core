package com.ford.syncV4.util;

import android.bluetooth.BluetoothAdapter;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 05.06.14
 * Time: 13:35
 */
public class BTHelper {

    /**
     * @return whether or not Bluetooth Adapter is available on the device
     */
    public static boolean isBTAdapterAvailable() {
        return BluetoothAdapter.getDefaultAdapter() != null;
    }

    /**
     * @return Return true if Bluetooth is currently enabled and ready for use.
     * Equivalent to: getBluetoothState() == STATE_ON
     */
    public static boolean isBTEnabled() {
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        return bluetoothAdapter != null && bluetoothAdapter.isEnabled();
    }
}