package com.ford.syncV4.android.manager;

import android.bluetooth.BluetoothAdapter;

import com.ford.syncV4.android.receivers.IBluetoothReceiver;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 12/26/13
 * Time: 5:47 PM
 */
public class BluetoothDeviceManager implements IBluetoothReceiver {

    private boolean mIsBluetoothEnabled = false;
    private IBluetoothDeviceManager iBluetoothDeviceManagerCallback;

    public void setBluetoothDeviceManagerCallback(IBluetoothDeviceManager iBluetoothDeviceManagerCallback) {
        this.iBluetoothDeviceManagerCallback = iBluetoothDeviceManagerCallback;
    }

    public void initState() {
        BluetoothAdapter mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            // Bluetooth is not supported by device
        } else {
            if (!mBluetoothAdapter.isEnabled()) {
                // Bluetooth is not enabled
                mIsBluetoothEnabled = false;
                //Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                //startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            } else {
                // Bluetooth is enabled
                mIsBluetoothEnabled = true;
            }
        }
    }

    @Override
    public void onBluetoothOn() {
        if (!mIsBluetoothEnabled) {
            if (iBluetoothDeviceManagerCallback != null) {
                iBluetoothDeviceManagerCallback.onBluetoothDeviceRestoreConnection();
            }
        }
        mIsBluetoothEnabled = true;
    }

    @Override
    public void onBluetoothOff() {
        mIsBluetoothEnabled = false;
    }

    @Override
    public void onBluetoothTurningOff() {
        if (mIsBluetoothEnabled) {
            if (iBluetoothDeviceManagerCallback != null) {
                iBluetoothDeviceManagerCallback.onBluetoothDeviceTurningOff();
            }
        }
    }
}