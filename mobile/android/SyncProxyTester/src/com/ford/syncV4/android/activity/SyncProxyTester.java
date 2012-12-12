package com.ford.syncV4.android.activity;

import java.util.Arrays;
import java.util.Set;
import java.util.Vector;

import org.json.JSONException;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.IBinder;
import android.text.ClipboardManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.view.WindowManager;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CheckedTextView;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Toast;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.adapters.logAdapter;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.android.module.ModuleTest;
import com.ford.syncV4.android.policies.PoliciesTest;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;

public class SyncProxyTester extends Activity implements OnClickListener {
	private static final String VERSION = "$Version:$";
	
	private static final String logTag = "SyncProxyTester";

    private static SyncProxyTester _activity;
	private static logAdapter _msgAdapter;
	private ProxyService _applinkService;
	private ModuleTest _testerMain;
	
	private TextView _console = null;
	private ScrollView _scroller = null;
	private ListView _listview = null;
	
	private ArrayAdapter<SyncSubMenu> _submenuAdapter = null;
	private ArrayAdapter<Integer> _commandAdapter = null;
	private ArrayAdapter<Integer> _choiceSetAdapter = null;

	private int autoIncCorrId = 101;
	private int autoIncChoiceSetId = 1;
	private int autoIncChoiceSetIdCmdId = 1;
	private int itemcmdID = 1;
	private int submenucmdID = 1000;

	private static final String PREFS_NAME = "SyncProxyTesterPrefs";
	private ArrayAdapter<ButtonName> _buttonAdapter = null;
	private boolean[] isButtonSubscribed = null;

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
    	
    	_activity = this;

		//_msgAdapter.logMessage("SyncProxyTester activity was started.", Log.DEBUG);

		setContentView(R.layout.main);
		_console = (TextView) findViewById(R.id.console);
		_scroller = (ScrollView) findViewById(R.id.scrollConsole);

		((Button) findViewById(R.id.btnSendMessage)).setOnClickListener(this);
		((Button) findViewById(R.id.btnPlayPause)).setOnClickListener(this);

		// set up storage for subscription records
		isButtonSubscribed = new boolean[ButtonName.values().length];
		_buttonAdapter = new ArrayAdapter<ButtonName>(this,
				android.R.layout.select_dialog_multichoice, ButtonName.values()) {
			public View getView(int position, View convertView, ViewGroup parent) {
				CheckedTextView ret = (CheckedTextView) super
						.getView(position, convertView, parent);
				ret.setChecked(isButtonSubscribed[position]);
				return ret;
			}
		};

		_listview = (ListView) findViewById(R.id.messageList);
		_msgAdapter = new logAdapter(logTag, false, this, R.layout.row);
		_submenuAdapter = new ArrayAdapter<SyncSubMenu>(this, android.R.layout.select_dialog_item);
		_submenuAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		// Add top level menu with parent ID zero
		SyncSubMenu sm = new SyncSubMenu();
		sm.setName("Top Level Menu");
		sm.setSubMenuId(0);
		addSubMenuToList(sm);

		_commandAdapter = new ArrayAdapter<Integer>(this, android.R.layout.select_dialog_item);
		_commandAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		_choiceSetAdapter = new ArrayAdapter<Integer>(this, android.R.layout.select_dialog_item);
		_choiceSetAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		_listview.setClickable(true);
		_listview.setAdapter(_msgAdapter);
		_listview.setTranscriptMode(ListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
		_listview.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				Object listObj = parent.getItemAtPosition(position);
				if (listObj instanceof RPCMessage) {
					String rawJSON = "";
					try {
						rawJSON = ((RPCMessage) listObj).serializeJSON().toString(2);
					} catch (JSONException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
					AlertDialog.Builder builder = new AlertDialog.Builder(SyncProxyTester.this);
					builder.setTitle("Raw JSON");
					builder.setMessage(rawJSON);
					builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int id) {
							dialog.cancel();
						}
					});
					AlertDialog ad = builder.create();
					ad.show();
				} else if (listObj instanceof String){
					AlertDialog.Builder builder = new AlertDialog.Builder(SyncProxyTester.this);
					builder.setMessage(listObj.toString());
					builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int id) {
							dialog.cancel();
						}
					});
					AlertDialog ad = builder.create();
					ad.show();
				}
			}
		});

		// Publish an SDP record and create a SYNC proxy.
		//startSyncProxyService();		
		_applinkService = new ProxyService();
		if (ProxyService.getInstance() == null) {
			Intent startIntent = new Intent(this, ProxyService.class);
			startService(startIntent);
			//bindService(startIntent, this, Context.BIND_AUTO_CREATE);
		} else {
			// need to get the instance and add myself as a listener
			ProxyService.getInstance().setCurrentActivity(this);
		}
	}
	
	public void startSyncProxyService() {
    	// Get the local Bluetooth adapter
        BluetoothAdapter mBtAdapter = BluetoothAdapter.getDefaultAdapter();

        //BT Adapter exists, is enabled, and there are paired devices with the name SYNC
		//Ideally start service and start proxy if already connected to sync
		//but, there is no way to tell if a device is currently connected (pre OS 4.0)
        
        if (mBtAdapter != null)
		{
			if ((mBtAdapter.isEnabled() && mBtAdapter.getBondedDevices().isEmpty() != true)) 
			{
				// Get a set of currently paired devices
				Set<BluetoothDevice> pairedDevices = mBtAdapter.getBondedDevices();
		
				boolean isSYNCpaired = false;
				// Check if there is a paired device with the name "SYNC"
		        if (pairedDevices.size() > 0) {
		            for (BluetoothDevice device : pairedDevices) {
		               if (device.getName().toString().equals("SYNC")) {
		            	   isSYNCpaired  = true;
		            	   break;
		               }
		            }
		        } else {
		        	Log.i("TAG", "A No Paired devices with the name sync");
		        }
		        
		        if (isSYNCpaired == true) { 		        	
		        	_applinkService = new ProxyService();
		    		if (ProxyService.getInstance() == null) {
		    			Intent startIntent = new Intent(this, ProxyService.class);
		    			startService(startIntent);
		    			//bindService(startIntent, this, Context.BIND_AUTO_CREATE);
		    		} else {
		    			// need to get the instance and add myself as a listener
		    			ProxyService.getInstance().setCurrentActivity(this);
		    		}
		        }
			}
		}
	}
    
    public static SyncProxyTester getInstance() {
		return _activity;
	}
    
    public static logAdapter getMessageAdapter() {
		return _msgAdapter;
	}
    
	public void setTesterMain(ModuleTest _instance) {
		this._testerMain = _instance;
	}

	protected void onDestroy() {
		super.onDestroy();
		endSyncProxyInstance();
		_activity = null;
		ProxyService service = ProxyService.getInstance();
		if (service != null) {
			service.setCurrentActivity(null);
		}
	}

	//upon onDestroy(), dispose current proxy and create a new one to enable auto-start
	//call resetProxy() to do so
	public void endSyncProxyInstance() {	
		ProxyService serviceInstance = ProxyService.getInstance();
		if (serviceInstance != null){
			SyncProxyALM proxyInstance = serviceInstance.getProxyInstance();
			//if proxy exists, reset it
			if(proxyInstance != null){			
				serviceInstance.reset();
			//if proxy == null create proxy
			} else {
				serviceInstance.startProxy();
			}
		}
	}
	
	public Dialog onCreateDialog(int id) {
		Dialog dialog = null;
		AlertDialog.Builder builder = new AlertDialog.Builder(this);
		switch (id) {
			case 1:
				builder.setTitle("Raw JSON");
				builder.setMessage("This is the raw JSON message here");
				builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int id) {
						dialog.cancel();
					}
				});
				AlertDialog dialog1 = builder.create();
				dialog = dialog1;
				break;
			case 2:
				break;
			default:
				dialog = null;
		}
		return dialog;
	}
	private final int PROXY_START = 5;
	private final int XML_TEST = 7;
	private final int POLICIES_TEST = 8;
	private final int MNU_TOGGLE_CONSOLE = 9;
	private final int MNU_CLEAR = 10;
	private final int MNU_EXIT = 11;
	private final int MNU_TOGGLE_MEDIA = 12;
	private final int MNU_VERSION = 13;
	private final int MNU_UNREGISTER = 14;

	
	/* Creates the menu items */
	public boolean onCreateOptionsMenu(Menu menu) {
		boolean result = super.onCreateOptionsMenu(menu);
		if (result) {
			menu.add(0, PROXY_START, 0, "Proxy Start");
			menu.add(0, XML_TEST, 0, "XML Test");
			menu.add(0, POLICIES_TEST, 0, "Policies Test");
			menu.add(0, MNU_TOGGLE_CONSOLE, 0, "Toggle Console");
			menu.add(0, MNU_CLEAR, 0, "Clear Messages");
			menu.add(0, MNU_EXIT, 0, "Exit");
			menu.add(0, MNU_TOGGLE_MEDIA, 0, "Toggle Media");
			menu.add(0, MNU_VERSION, 0, "Version");
			menu.add(0, MNU_UNREGISTER, 0, "Unregister");
			return true;
		} else {
			return false;
		}
	}

	/* Handles item selections */
	public boolean onOptionsItemSelected(MenuItem item) {

		switch (item.getItemId()) {
        case PROXY_START:
	        BluetoothAdapter mBtAdapter = BluetoothAdapter.getDefaultAdapter();
	        if (!mBtAdapter.isEnabled()) mBtAdapter.enable();
	        
	        if (_applinkService == null) _applinkService = new ProxyService();
	        
	        if (ProxyService.getInstance() == null) {
                Intent startIntent = new Intent(this, ProxyService.class);
                startService(startIntent);
	        } else {
                ProxyService.getInstance().setCurrentActivity(this);
	        }
	        
	        if (ProxyService.getInstance().getProxyInstance() != null) {
                try {
                        ProxyService.getInstance().getProxyInstance().resetProxy();
                } catch (SyncException e) {}
	        }
	        
	        if (!mBtAdapter.isDiscovering()) {
                Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
                startActivity(discoverableIntent);
	        }
			return true;

		case XML_TEST:
			if (_testerMain != null) {
				_testerMain.restart();
				Toast.makeText(getApplicationContext(), "start your engines", Toast.LENGTH_SHORT).show();
			}else {
				Toast.makeText(getApplicationContext(), "Start the app on SYNC first", Toast.LENGTH_LONG).show();
			}
			break;
		case POLICIES_TEST:
			PoliciesTest.runPoliciesTest();
			break;
		case MNU_EXIT:
			super.finish();
			break;
		case MNU_VERSION:
			{SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
			int versionN = settings.getInt("VersionNumber", 1);
			SharedPreferences.Editor editor = settings.edit();
			int newVersion = versionN == 1 ? 2:1;
			editor.putInt("VersionNumber", newVersion);
			editor.commit();
			/*new AlertDialog.Builder(this)  
			       .setMessage(VERSION)
			       .setNeutralButton("Copy", new DialogInterface.OnClickListener() {
						public void onClick(DialogInterface dialog, int which) {
							ClipboardManager clipboard = 
							      (ClipboardManager) getSystemService(CLIPBOARD_SERVICE); 
							clipboard.setText(VERSION);
						}
					})
			       .setNegativeButton("Close", null)
			       .show();*/
			break;}
		case MNU_TOGGLE_CONSOLE:
			if (_scroller.getVisibility() == ScrollView.VISIBLE) {
				_scroller.setVisibility(ScrollView.GONE);
				_listview.setVisibility(ListView.VISIBLE);
			} else {
				_scroller.setVisibility(ScrollView.VISIBLE);
				_listview.setVisibility(ListView.GONE);
			}
			return true;
		case MNU_CLEAR:
			_msgAdapter.clear();
			return true;
		case MNU_TOGGLE_MEDIA:
			SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
			boolean isMediaApp = settings.getBoolean("isMediaApp", false);
			SharedPreferences.Editor editor = settings.edit();
			editor.putBoolean("isMediaApp", !isMediaApp);

			// Don't forget to commit your edits!!!
			editor.commit();
			//super.finish();
			return true;
		case MNU_UNREGISTER:
			/*
			endSyncProxyInstance();
        	startSyncProxyService();
        	*/		       
	        if (_applinkService == null) { 		        	
	        	_applinkService = new ProxyService();
	    		if (ProxyService.getInstance() == null) {
	    			Intent startIntent = new Intent(this, ProxyService.class);
	    			startService(startIntent);
	    			//bindService(startIntent, this, Context.BIND_AUTO_CREATE);
	    		} else {
	    			// need to get the instance and add myself as a listener
	    			ProxyService.getInstance().setCurrentActivity(this);
	    		}
	        }
	        if (ProxyService.getInstance().getProxyInstance() != null) {
				try {
					ProxyService.getInstance().getProxyInstance().resetProxy();
				} catch (SyncException e) {}
	        }
			return true;
		}
		return false;
	}

	public void onClick(View v) {
		if (v == findViewById(R.id.btnSendMessage)) {
			final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.select_dialog_item);
			adapter.add("Alert");
			adapter.add("Speak");
			adapter.add("Show");
			adapter.add("ButtonSubscriptions");
			adapter.add("AddCommand");
			adapter.add("DeleteCommand");
			adapter.add("AddSubMenu");
			adapter.add("DeleteSubMenu");
			adapter.add("SetGlobalProperties");
			adapter.add("SetMediaClockTimer");
			adapter.add("CreateChoiceSet");
			adapter.add("DeleteChoiceSet");
			adapter.add("PerformInteraction");
			adapter.add("Slider");
			adapter.add("ReadDID");
			adapter.add("GetDTC");
			adapter.add("ScrollableMessage");
			
			new AlertDialog.Builder(this)  
		       .setTitle("Pick a Function")
		       .setAdapter(adapter, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						if(adapter.getItem(which) == "Alert"){
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.alert, null);
							final EditText txtSpeak = (EditText) layout.findViewById(R.id.txtSpeak);
							final EditText txtAlertField1 = (EditText) layout.findViewById(R.id.txtAlertField1);
							final EditText txtAlertField2 = (EditText) layout.findViewById(R.id.txtAlertField2);
							final EditText txtAlertField3 = (EditText) layout.findViewById(R.id.txtAlertField3);
							final EditText txtDuration = (EditText) layout.findViewById(R.id.txtDuration);
							final CheckBox chkPlayTone = (CheckBox) layout.findViewById(R.id.chkPlayTone);
							final CheckBox chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									String toSpeak = txtSpeak.getText().toString();
									try {
										Alert msg = new Alert();
										msg.setCorrelationID(autoIncCorrId++);
										msg.setAlertText1(txtAlertField1.getText().toString());
										msg.setAlertText2(txtAlertField2.getText().toString());
										msg.setAlertText3(txtAlertField3.getText().toString());
										msg.setDuration(Integer.parseInt(txtDuration.getText().toString()));
										msg.setPlayTone(chkPlayTone.isChecked());
										if (chkIncludeSoftButtons.isChecked()) {
											SoftButton sb1 = new SoftButton();
											sb1.setSoftButtonID(5400);
											sb1.setText("ReRoute");
											sb1.setType(SoftButtonType.SBT_TEXT);
											sb1.setIsHighlighted(false);
											sb1.setSystemAction(SystemAction.STEAL_FOCUS);
											SoftButton sb2 = new SoftButton();
											sb2.setSoftButtonID(5399);
											sb2.setText("Close");
											sb2.setType(SoftButtonType.SBT_TEXT);
											sb2.setIsHighlighted(false);
											sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
											Vector<SoftButton> sbarray = new Vector<SoftButton>();
											sbarray.add(sb1);
											sbarray.add(sb2);
											msg.setSoftButtons(sbarray);
										}
										_msgAdapter.logMessage(msg, true);
										ProxyService.getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();	
						} else if (adapter.getItem(which) == "ReadDID") {
							AlertDialog.Builder builder;
							AlertDialog dlg;
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.readdid, null);
							final EditText txtECUName = (EditText) layout.findViewById(R.id.txtECUName);
							final EditText txtDIDLocation = (EditText) layout.findViewById(R.id.txtDIDLocation);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										ReadDID msg = new ReadDID();
										//TODO: This needs to be a string, or something else.
										//This will fail, as it's not an INT
										msg.setEcuName(Integer.parseInt(txtECUName.getText().toString()));
										//This will fail, as it's not an INT
										Vector<Integer> didlocations = new Vector<Integer>();
										didlocations.add(Integer.parseInt(txtDIDLocation.getText().toString()));
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == "GetDTC") { 
							AlertDialog.Builder builder;
							AlertDialog dlg;
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.getdtcs, null);
							final EditText txtECUName = (EditText) layout.findViewById(R.id.txtECUName);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										GetDTCs msg = new GetDTCs();
										//TODO: This needs to be a string, or something else.
										//This will fail, as it's not an INT
										msg.setEcuName(Integer.parseInt(txtECUName.getText().toString()));
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == "ScrollableMessage") {
							//something
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.scrollablemessage, null);
							final EditText txtScrollableMessageBody = (EditText) layout.findViewById(R.id.txtScrollableMessageBody);
							final CheckBox chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										ScrollableMessage msg = new ScrollableMessage();
										msg.setCorrelationID(autoIncCorrId++);
										msg.setScrollableMessageBody(txtScrollableMessageBody.getEditableText().toString());
										if (chkIncludeSoftButtons.isChecked()) {
											SoftButton sb1 = new SoftButton();
											sb1.setSoftButtonID(5400);
											sb1.setText("Reply");
											sb1.setType(SoftButtonType.SBT_TEXT);
											sb1.setIsHighlighted(false);
											sb1.setSystemAction(SystemAction.STEAL_FOCUS);
											SoftButton sb2 = new SoftButton();
											sb2.setSoftButtonID(5399);
											sb2.setText("Close");
											sb2.setType(SoftButtonType.SBT_TEXT);
											sb2.setIsHighlighted(false);
											sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
											Vector<SoftButton> sbarray = new Vector<SoftButton>();
											sbarray.add(sb1);
											sbarray.add(sb2);
											msg.setSoftButtons(sbarray);
										}
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "Slider") {
							//something
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.slider, null);
							final EditText txtNumTicks = (EditText) layout.findViewById(R.id.txtNumTicks);
							final EditText txtSliderHeader = (EditText) layout.findViewById(R.id.txtSliderHeader);
							final EditText txtSliderFooter = (EditText) layout.findViewById(R.id.txtSliderFooter);
							final EditText txtTimeout = (EditText) layout.findViewById(R.id.txtTimeout);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Slider msg = new Slider();
									msg.setTimeout(Integer.parseInt(txtTimeout.getText().toString()));
									msg.setNumTicks(Integer.parseInt(txtNumTicks.getText().toString()));
									msg.setSliderHeader(txtSliderHeader.getText().toString());
									Vector<String> footerelements = new Vector<String>();
									footerelements.add(txtSliderFooter.getText().toString());
									msg.setSliderFooter(footerelements);
									msg.setCorrelationID(autoIncCorrId++);
									_msgAdapter.logMessage(msg, true);
									try {
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "Speak") {
							//something
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.speak, null);
							final EditText txtSpeakText1 = (EditText) layout.findViewById(R.id.txtSpeakText1);
							final EditText txtSpeakText2 = (EditText) layout.findViewById(R.id.txtSpeakText2);
							final EditText txtSpeakText3 = (EditText) layout.findViewById(R.id.txtSpeakText3);
							final EditText txtSpeakText4 = (EditText) layout.findViewById(R.id.txtSpeakText4);
							
							final Spinner spnSpeakType1 = (Spinner) layout.findViewById(R.id.spnSpeakType1);
							final Spinner spnSpeakType2 = (Spinner) layout.findViewById(R.id.spnSpeakType2);
							final Spinner spnSpeakType3 = (Spinner) layout.findViewById(R.id.spnSpeakType3);
							final Spinner spnSpeakType4 = (Spinner) layout.findViewById(R.id.spnSpeakType4);
							
							ArrayAdapter<SpeechCapabilities> speechSpinnerAdapter = new ArrayAdapter<SpeechCapabilities>(adapter.getContext(), android.R.layout.simple_spinner_item, SpeechCapabilities.values()); 
							spnSpeakType1.setAdapter(speechSpinnerAdapter);
							spnSpeakType2.setAdapter(speechSpinnerAdapter);
							spnSpeakType2.setSelection(3);
							spnSpeakType3.setAdapter(speechSpinnerAdapter);
							spnSpeakType4.setAdapter(speechSpinnerAdapter);
							spnSpeakType4.setSelection(1);
							spnSpeakType4.setEnabled(false);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									String speak1 = txtSpeakText1.getText().toString();
									String speak2 = txtSpeakText2.getText().toString();
									String speak3 = txtSpeakText3.getText().toString();
									String speak4 = txtSpeakText4.getText().toString();
									Vector<TTSChunk> chunks = new Vector<TTSChunk>();

									if (speak1.length() > 0) {
										chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities)spnSpeakType1.getSelectedItem(), speak1));
										
									}
									if (speak2.length() > 0) {
										chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities)spnSpeakType2.getSelectedItem(), speak2));
										
									}
									if (speak3.length() > 0) {
										chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities)spnSpeakType3.getSelectedItem(), speak3));
										
									}
									if (speak4.length() > 0) {
										chunks.add(TTSChunkFactory.createChunk(SpeechCapabilities.SAPI_PHONEMES, speak4));
										
									}
									try {
										ProxyService.getInstance().getProxyInstance().speak(chunks, autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "Show") {
							//something
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.show, null);
							final EditText txtShowField1 = (EditText) layout.findViewById(R.id.txtShowField1);
							final EditText txtShowField2 = (EditText) layout.findViewById(R.id.txtShowField2);
							final EditText statusBar = (EditText) layout.findViewById(R.id.txtStatusBar);
							final EditText mediaClock = (EditText) layout.findViewById(R.id.txtMediaClock);
							final EditText mediaTrack = (EditText) layout.findViewById(R.id.txtMediaTrack);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										//
										SoftButton sb1 = new SoftButton();
										sb1.setSoftButtonID(5432);
										sb1.setText("KeepContext");
										sb1.setType(SoftButtonType.SBT_TEXT);
										sb1.setIsHighlighted(false);
										sb1.setSystemAction(SystemAction.KEEP_CONTEXT);
										SoftButton sb2 = new SoftButton();
										sb2.setSoftButtonID(5431);
										sb2.setText("StealFocus");
										sb2.setType(SoftButtonType.SBT_TEXT);
										sb2.setIsHighlighted(false);
										sb2.setSystemAction(SystemAction.STEAL_FOCUS);
										SoftButton sb3 = new SoftButton();
										sb3.setSoftButtonID(5430);
										sb3.setText("Default");
										sb3.setType(SoftButtonType.SBT_TEXT);
										sb3.setIsHighlighted(false);
										sb3.setSystemAction(SystemAction.DEFAULT_ACTION);
										Vector<SoftButton> sbarray = new Vector<SoftButton>();
										sbarray.add(sb1);
										sbarray.add(sb2);
										sbarray.add(sb3);
										Show msg = new Show();
										msg.setCorrelationID(autoIncCorrId++);
										msg.setMainField1(txtShowField1.getText().toString());
										msg.setMainField2(txtShowField2.getText().toString());
										msg.setMainField3("MainField3_test");
										msg.setMainField4("MainField4_test");
										msg.setStatusBar(statusBar.getText().toString());
										msg.setMediaClock(mediaClock.getText().toString());
										msg.setMediaTrack(mediaTrack.getText().toString());
										msg.setSoftButtons(sbarray);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "ButtonSubscriptions") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_buttonAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
								boolean needToSubscribe = !isButtonSubscribed[which];
									try {
										if (needToSubscribe) 
											ProxyService.getInstance().getProxyInstance().subscribeButton(ButtonName.values()[which], autoIncCorrId++);
										else ProxyService.getInstance().getProxyInstance().unsubscribeButton(ButtonName.values()[which], autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									isButtonSubscribed[which] = !isButtonSubscribed[which];
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "AddCommand") {
							//something
							AlertDialog.Builder builder;
							AlertDialog addCommandDialog;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.addcommand,
									(ViewGroup) findViewById(R.id.itemRoot));

							final EditText er = (EditText) layout.findViewById(R.id.command);
							final Spinner s = (Spinner) layout.findViewById(R.id.availableSubmenus);
							s.setAdapter(_submenuAdapter);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									String itemText = er.getText().toString();
									SyncSubMenu sm = new SyncSubMenu();
									sm = (SyncSubMenu) s.getSelectedItem();
									
									Vector<String> vrCommands = new Vector<String>();
									vrCommands.add(itemText);
									int cmdID = itemcmdID++;
									try {
										ProxyService.getInstance().getProxyInstance().addCommand(cmdID, itemText, sm.getSubMenuId(), 0, vrCommands, autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									_commandAdapter.add(cmdID);
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							addCommandDialog = builder.create();
							addCommandDialog.show();
						}
						else if (adapter.getItem(which) == "DeleteCommand") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_commandAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									int cmdID = _commandAdapter.getItem(which);
									try {
										//ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
										ProxyService.getInstance().getProxyInstance().deleteCommand(cmdID, autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									_commandAdapter.remove(cmdID);
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "AddSubMenu") {
							//something
							AlertDialog.Builder builder;
							AlertDialog addSubMenuDialog;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.addsubmenu,
									(ViewGroup) findViewById(R.id.submenu_Root));

							final EditText subMenu = (EditText) layout.findViewById(R.id.submenu_item);

							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									SyncSubMenu sm = new SyncSubMenu();
									sm.setName(subMenu.getText().toString());
									sm.setSubMenuId(submenucmdID++);
									addSubMenuToList(sm);
									try {
										ProxyService.getInstance().getProxyInstance().addSubMenu(sm.getSubMenuId(), sm.getName(), autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							addSubMenuDialog = builder.create();
							addSubMenuDialog.show();
						}
						else if (adapter.getItem(which) == "DeleteSubMenu") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_submenuAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									SyncSubMenu menu = _submenuAdapter.getItem(which);
									try {
										ProxyService.getInstance().getProxyInstance().deleteSubMenu(menu.getSubMenuId(), autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}

									_submenuAdapter.remove(menu);
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "SetGlobalProperties") {
							try{
								Vector<TTSChunk> help = new Vector<TTSChunk>();
								help.add(TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, "This is the help prompt"));
								Vector<TTSChunk> timeout = new Vector<TTSChunk>();
								timeout.add(TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, "This is the timeout prompt"));
								ProxyService.getInstance().getProxyInstance().setGlobalProperties(help, timeout, autoIncCorrId++);	
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
							
						}
						else if (adapter.getItem(which) == "SetMediaClockTimer") {
							//something
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.setmediaclock, null);
							final EditText txtHours = (EditText) layout.findViewById(R.id.txtHours);
							final EditText txtMinutes = (EditText) layout.findViewById(R.id.txtMinutes);
							final EditText txtSeconds = (EditText) layout.findViewById(R.id.txtSeconds);
							final Spinner spnUpdateMode = (Spinner) layout.findViewById(R.id.spnUpdateMode);
							ArrayAdapter<UpdateMode> spinnerAdapter = new ArrayAdapter<UpdateMode>(adapter.getContext(),
									android.R.layout.simple_spinner_item, UpdateMode.values());
							spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnUpdateMode.setAdapter(spinnerAdapter);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									UpdateMode updateMode =  (UpdateMode)spnUpdateMode.getSelectedItem();
									int corrID = autoIncCorrId++;
									String strHours = txtHours.getText().toString();
									String strMinutes = txtMinutes.getText().toString();
									String strSeconds = txtSeconds.getText().toString();
									
									try {
										if (strHours.length() > 0 && strMinutes.length() > 0 && strSeconds.length() > 0) {
											ProxyService.getInstance().getProxyInstance().setMediaClockTimer(
													Integer.parseInt(strHours), 
													Integer.parseInt(strMinutes), Integer.parseInt(strSeconds), 
													updateMode, 
													corrID);
										} else {
											ProxyService.getInstance().getProxyInstance().setMediaClockTimer(null, null, null, updateMode, corrID);
										}
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "CreateChoiceSet") {
							//something
							AlertDialog.Builder builder;
							AlertDialog createCommandSet;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.createinteractionchoices,
									(ViewGroup) findViewById(R.id.createcommands_Root));

							final EditText command1 = (EditText) layout.findViewById(R.id.createcommands_command1);
							final EditText command2 = (EditText) layout.findViewById(R.id.createcommands_command2);
							final EditText command3 = (EditText) layout.findViewById(R.id.createcommands_command3);

							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<Choice> commands = new Vector<Choice>();
									Choice one = new Choice();
									one.setChoiceID(autoIncChoiceSetIdCmdId++);
									one.setMenuName(command1.getText().toString());
									one.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command1.getText().toString(),
											"Command one" })));
									commands.add(one);
									Choice two = new Choice();
									two.setChoiceID(autoIncChoiceSetIdCmdId++);
									two.setMenuName(command2.getText().toString());
									two.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command2.getText().toString(),
											"Command two" })));
									commands.add(two);
									Choice three = new Choice();
									three.setChoiceID(autoIncChoiceSetIdCmdId++);
									three.setMenuName(command3.getText().toString());
									three.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command3.getText().toString(),
											"Command three" })));
									commands.add(three);
									try {
										int choiceSetID = autoIncChoiceSetId++;
										ProxyService.getInstance().getProxyInstance().createInteractionChoiceSet(commands, choiceSetID, autoIncCorrId++);
										_choiceSetAdapter.add(choiceSetID);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									dialog.cancel();
								}
							});
							builder.setView(layout);
							createCommandSet = builder.create();
							createCommandSet.show();
						}
						else if (adapter.getItem(which) == "DeleteChoiceSet") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_choiceSetAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									int commandSetID = _choiceSetAdapter.getItem(which);
									try {
										ProxyService.getInstance().getProxyInstance().deleteInteractionChoiceSet(commandSetID, autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}

									_choiceSetAdapter.remove(commandSetID);
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						}
						else if (adapter.getItem(which) == "PerformInteraction") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_choiceSetAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									Vector<Integer> interactionChoiceSetIDs = new Vector<Integer>();
									interactionChoiceSetIDs.add(_choiceSetAdapter.getItem(which));
									try {
										ProxyService.getInstance().getProxyInstance().performInteraction(
															"Pick a command",
															"Pick number:",
															interactionChoiceSetIDs,
															"help me, I'm melting",
															"hurry it up",
															InteractionMode.BOTH, 
															10000,
															autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						}
						}
				})
		       .setNegativeButton("Close", null)
		       .show();			
		} else if (v == findViewById(R.id.btnPlayPause)) {
			ProxyService.getInstance().playPauseAnnoyingRepetitiveAudio();
		}
	}

	public void addSubMenuToList(final SyncSubMenu sm) {
		runOnUiThread(new Runnable() {
			public void run() {
				_submenuAdapter.add(sm);
			}
		});
	}

	/*
	public void appendMessageToList(final Object m) {
		runOnUiThread(new Runnable() {
			public void run() {
				_msgAdapter.add(m);
			}
		});
	}

	public void appendConsole(final Object toAppend) {
		runOnUiThread(new Runnable() {
			public void run() {
				_console.append("\n" + toAppend);
				_scroller.postDelayed(new Runnable() {
					public void run() {
						_scroller.fullScroll(ScrollView.FOCUS_DOWN);
					}
				}, 250);
			}
		});
	}

	public void appendLine(String line) {
		appendConsole(line);
	}
	*/
	
	/*public void finish() {
		try {
			unbindService(this);
		}
		catch (Exception e) {
			_msgAdapter.logMessage("Failed to unbind from ProxyService.", Log.ERROR, e);
		}
		super.finish();
	}

	public void onServiceConnected(ComponentName name, IBinder service) {
		if (ProxyService.getInstance() != null) {
			ProxyService.getInstance().setCurrentActivity(this);
			if (ProxyService.getInstance().getProxyInstance() == null) {
				ProxyService.getInstance().startProxy();
			}
		}
	}

	public void onServiceDisconnected(ComponentName name) {
		_msgAdapter.logMessage("Service Disconnected: " + name, Log.DEBUG);
	}*/
	
	@Override
	public void onBackPressed() {
		super.onBackPressed();
	}
	
	@Override
	public void onPause() {
		super.onPause();
	}
}

