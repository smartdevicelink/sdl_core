package ch.serverbox.android.usbtest;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.hardware.usb.UsbAccessory;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.ByteArrayOutputStream;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.Arrays;

public class UsbTest extends Activity {
    private static final String TAG = UsbTest.class.getSimpleName();
    private static final String ACTION_USB_PERMISSION = "ch.serverbox.android.usbtest.USBPERMISSION";
    private static final String ACCESSORY_MANUFACTURER = "Nexus-Computing GmbH";

	private Button mBtSend = null;
    private Button mBtWriteBenchmark = null;

    private UsbAccessory mAccessory = null;
	private FileOutputStream mFout = null;
    private FileInputStream mFin = null;

    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);

        mBtSend = (Button)(findViewById(R.id.btSend));
        mBtSend.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String s = ((EditText)findViewById(R.id.editText1)).getText().toString();
                queueWrite(s);
            }
        });
        mBtWriteBenchmark = (Button) findViewById(R.id.btWriteBenchmark);
        mBtWriteBenchmark.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                writeBenchmark();
            }
        });

        IntentFilter i = new IntentFilter();
//        i.addAction(UsbManager.ACTION_USB_ACCESSORY_ATTACHED);
        i.addAction(UsbManager.ACTION_USB_ACCESSORY_DETACHED);
        i.addAction(ACTION_USB_PERMISSION);
        registerReceiver(mUsbReceiver,i);

		Intent intent = getIntent();
        final String action = intent.getAction();
        Log.d(TAG, "Starting intent: " + action);
        if(action.equals(UsbManager.ACTION_USB_ACCESSORY_ATTACHED)){
        	UsbAccessory accessory = intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
            openAccessory(accessory);
        } else {
            discoverAccessories();
        }
    }

/*    @Override
    protected void onResume() {
        super.onResume();
        Intent intent = getIntent();
        Log.d(TAG, "Resuming intent: " + intent.getAction());
    }*/

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        final String action = intent.getAction();
        Log.d(TAG, "onNewIntent: " + action);

        if (UsbManager.ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
            UsbAccessory accessory = intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
            openAccessory(accessory);
        }
    }

    @Override
    protected void onDestroy() {
    	unregisterReceiver(mUsbReceiver);
    	super.onDestroy();
    }
    
    public void queueWrite(final String data){
    	if(mAccessory == null){
    		return;
    	}
    	new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					Log.d(TAG, "Writing length "+data.length());
					mFout.write(new byte[]{(byte)data.length()});
					Log.d(TAG, "Writing data: "+data);
					mFout.write(data.getBytes());
					Log.d(TAG,"Done writing");

                    queueRead();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}).start();
    }

    public void queueRead() {
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    Log.d(TAG, "Reading");

                    byte[] lenBuffer = new byte[500];
                    int bytesRead = mFin.read(lenBuffer);
                    Log.d(TAG, "1: Bytes read: " + bytesRead);

                    byte[] buffer = new byte[100000];
                    bytesRead = mFin.read(buffer);
                    byte[] outBuffer = Arrays.copyOf(buffer, bytesRead);
                    final String out = new String(outBuffer);
                    Log.d(TAG, "2: Bytes read: " + bytesRead + "\n" + out);

                    UsbTest.this.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            Toast.makeText(getApplicationContext(), out, Toast.LENGTH_LONG).show();
                        }
                    });
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }).start();
    }

    private void writeBenchmark() {
        // write a whole bunch of data non-stop and measure how long it'll take
        final int ALL_BYTES_LENGTH = 256;
        byte[] allByteValues = new byte[ALL_BYTES_LENGTH];
        for (int i = 0; i < ALL_BYTES_LENGTH; ++i) {
            allByteValues[i] = (byte) i;
        }

        // 4 KB in one chunk
        final int BUFFER_LENGTH = 4096;
        assert BUFFER_LENGTH % ALL_BYTES_LENGTH == 0;
        final int numCopies = BUFFER_LENGTH / ALL_BYTES_LENGTH;

        ByteArrayOutputStream bas = new ByteArrayOutputStream(BUFFER_LENGTH);
        for (int i = 0; i < numCopies; ++i) {
            try {
                bas.write(allByteValues);
            } catch (IOException e) {
                e.printStackTrace();
                return;
            }
        }
        final byte[] buffer = bas.toByteArray();

        new Thread(new Runnable() {
            @Override
            public void run() {
                // 4 MB total
                final int writeCount = 10000;
                final int reportPeriod = 1000;
                int blocksWritten = 0;
                Log.d(TAG, "Benchmark start");
                long startTime = System.currentTimeMillis();

                try {
                    for (blocksWritten = 0; blocksWritten < writeCount;
                         ++blocksWritten) {
                        if ((blocksWritten + 1) % reportPeriod == 0) {
                            Log.d(TAG, "tick");
                        }
                        mFout.write(buffer);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                }

                final long runTime = System.currentTimeMillis() - startTime;
                final int totalBytesWritten = blocksWritten * BUFFER_LENGTH;
                final double avgSpeed = totalBytesWritten / (runTime / 1000.0);
                final String msg = "Benchmark finish; run time = " + runTime +
                        " ms. for " + totalBytesWritten + " bytes, which is " +
                        "about " + avgSpeed + " B/s";
                Log.d(TAG, msg);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(UsbTest.this, msg, Toast.LENGTH_LONG)
                                .show();
                    }
                });
            }
        }, "BenchmarkThread").start();
    }
    
	private final BroadcastReceiver mUsbReceiver = new BroadcastReceiver() {
		@Override
		public void onReceive(Context context, Intent intent) {
			String action = intent.getAction();
            Log.d(TAG, "Receiver action: " + action);
/*            // this ACTION_USB_ACCESSORY_ATTACHED is never called in a BroadcastReceiver
            // http://stackoverflow.com/questions/6981736/android-3-1-usb-host-broadcastreceiver-does-not-receive-usb-device-attached/9814826#9814826

			if (UsbManager.ACTION_USB_ACCESSORY_ATTACHED.equals(action)) {
					UsbAccessory accessory = (UsbAccessory) intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
					Log.d(TAG,"Attached!");
					if (intent.getBooleanExtra(
							UsbManager.EXTRA_PERMISSION_GRANTED, false)) {
                        openAccessory(accessory);
					} else {
						Log.d(TAG, "permission denied for accessory "
								+ accessory);
					}
			} else*/
			if (UsbManager.ACTION_USB_ACCESSORY_DETACHED.equals(action)) {
				UsbAccessory accessory = intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
                closeAccessory(accessory);
            }else if(ACTION_USB_PERMISSION.equals(action)){
				l("permission answered");
                UsbAccessory accessory = intent.getParcelableExtra(UsbManager.EXTRA_ACCESSORY);
                openAccessory(accessory);
/*				if(intent.getBooleanExtra(UsbManager.EXTRA_PERMISSION_GRANTED, false)){
		        	UsbAccessory[] accessories = ((UsbManager) getSystemService(Context.USB_SERVICE)).getAccessoryList();
		        	for(UsbAccessory a : accessories){
		        		l("accessory: "+a.getManufacturer());
		        		if(a.getManufacturer().equals(ACCESSORY_MANUFACTURER)){
                            openAccessory(a);
		        			l("added accessory");
		        			break;
		        		}
		        	}
				}*/
			}
		}
	};

    private void openAccessory(UsbAccessory accessory) {
        final String manufacturer = accessory.getManufacturer();
        if (!manufacturer.equals(ACCESSORY_MANUFACTURER)) {
            Log.w(TAG, "Unsupported manufacturer: " + manufacturer + "; connecting anyway");
        }

        mAccessory = accessory;
        FileDescriptor fd;
        try {
            fd = ((UsbManager) getSystemService(Context.USB_SERVICE)).openAccessory(accessory).getFileDescriptor();
            mFout = new FileOutputStream(fd);
            mFin = new FileInputStream(fd);
            setButtonsEnabled(true);

            l("Accessory " + manufacturer + " opened!");
        } catch (IllegalArgumentException e) {
            Log.e(TAG, "openAccessory error", e);
        } catch (NullPointerException e) {
            Log.e(TAG, "openAccessory error", e);
        }
    }

    private void closeAccessory(UsbAccessory accessory) {
        if ((accessory != null) && accessory.equals(mAccessory)) {
            if (mFout != null) {
                try {
                    mFout.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (mFin != null) {
                try {
                    mFin.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            mAccessory = null;
            setButtonsEnabled(false);
        }
    }

    private void setButtonsEnabled(boolean enabled) {
        mBtSend.setEnabled(enabled);
        mBtWriteBenchmark.setEnabled(enabled);
    }

    private void discoverAccessories() {
        UsbAccessory[] accessories = ((UsbManager) getSystemService(Context.USB_SERVICE)).getAccessoryList();
        if (accessories != null) {
            for (UsbAccessory a : accessories) {
                l("accessory: " + a.getManufacturer());
                if (a.getManufacturer().equals(ACCESSORY_MANUFACTURER)) {
                    PendingIntent mPermissionIntent =
                            PendingIntent.getBroadcast(this, 0, new Intent(ACTION_USB_PERMISSION), 0);
                    final UsbManager usbManager = (UsbManager) getSystemService(Context.USB_SERVICE);
                    if (usbManager.hasPermission(a)) {
                        openAccessory(a);
                    } else {
                        usbManager.requestPermission(a, mPermissionIntent);
                        l("permission requested");
                    }
                    break;
                }
            }
        } else {
            l("accessories not found");
        }
    }

    private void l(String l){
		Log.d(TAG, l);
	}
}