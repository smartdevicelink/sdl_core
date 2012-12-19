package com.ford.syncV4.android.activity;

import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Set;
import java.util.Vector;

import org.json.JSONException;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CheckedTextView;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.Toast;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.adapters.logAdapter;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.android.module.ModuleTest;
import com.ford.syncV4.android.policies.PoliciesTest;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuver;
import com.ford.syncV4.proxy.rpc.ChangeRegistration;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DialNumber;
import com.ford.syncV4.proxy.rpc.EndAudioPassThru;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.ListFiles;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.Turn;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.enums.AudioCaptureQuality;
import com.ford.syncV4.proxy.rpc.enums.AudioType;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.SamplingRate;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;

public class SyncProxyTester extends Activity implements OnClickListener {
	private static final String VERSION = "$Version:$";
	
	private static final String logTag = "SyncProxyTester";

    private static SyncProxyTester _activity;
    private static ArrayList<Object> _logMessages = new ArrayList<Object>();
	private static logAdapter _msgAdapter;
	private ProxyService _applinkService;
	private ModuleTest _testerMain;
	
	private ScrollView _scroller = null;
	private ListView _listview = null;
	
	private ArrayAdapter<SyncSubMenu> _submenuAdapter = null;
	private ArrayAdapter<Integer> _commandAdapter = null;
	private ArrayAdapter<Integer> _choiceSetAdapter = null;
	private ArrayAdapter<String> _putFileAdapter = null;

	private int autoIncCorrId = 101;
	private int autoIncChoiceSetId = 1;
	private int autoIncChoiceSetIdCmdId = 1;
	private int itemcmdID = 1;
	private int submenucmdID = 1000;

	private static final String PREFS_NAME = "SyncProxyTesterPrefs";
	private ArrayAdapter<ButtonName> _buttonAdapter = null;
	private boolean[] isButtonSubscribed = null;

	private ArrayAdapter<VehicleDataType> _vehicleDataType = null;
	private boolean[] isVehicleDataSubscribed = null;

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
    	
    	_activity = this;

		//_msgAdapter.logMessage("SyncProxyTester activity was started.", Log.DEBUG);

		setContentView(R.layout.main);
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

		isVehicleDataSubscribed = new boolean[VehicleDataType.values().length];
		_vehicleDataType = new ArrayAdapter<VehicleDataType>(this,
				android.R.layout.select_dialog_multichoice, VehicleDataType.values()) {
			public View getView(int position, View convertView, ViewGroup parent) {
				CheckedTextView ret = (CheckedTextView) super
						.getView(position, convertView, parent);
				ret.setChecked(isVehicleDataSubscribed[position]);
				return ret;
			}
		};

		_listview = (ListView) findViewById(R.id.messageList);
		_msgAdapter = new logAdapter(logTag, false, this, R.layout.row, _logMessages);
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

		_putFileAdapter = new ArrayAdapter<String>(this, android.R.layout.select_dialog_item);
		_putFileAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		_listview.setClickable(true);
		_listview.setAdapter(_msgAdapter);
		_listview.setTranscriptMode(ListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
		_listview.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				Object listObj = parent.getItemAtPosition(position);
				if (listObj instanceof RPCMessage) {
					String rawJSON = "";
					try {
						rawJSON = ((RPCMessage) listObj).serializeJSON(
								ProxyService.getInstance().getProxyInstance().getWiProVersion()).toString(2);
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

	protected void onDestroy() {
		super.onDestroy();
		endSyncProxyInstance();
		_activity = null;
		ProxyService service = ProxyService.getInstance();
		if (service != null) {
			service.setCurrentActivity(null);
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
	private final int MNU_TOGGLE_PROTOCOL_VERSION = 13;
	private final int MNU_UNREGISTER = 14;
	private final int MNU_APP_VERSION = 15;

	
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
			menu.add(0, MNU_TOGGLE_PROTOCOL_VERSION, 0,
					"Toggle Protocol Ver. (cur " + getCurrentProtocolVersion()
							+ ")");
			menu.add(0, MNU_UNREGISTER, 0, "Unregister");
			menu.add(0, MNU_APP_VERSION, 0, "App version");
			return true;
		} else {
			return false;
		}
	}
	
	private String getCurrentProtocolVersion() {
		return String.valueOf(getSharedPreferences(PREFS_NAME, 0).getInt(
			"VersionNumber", 1));
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
		case MNU_TOGGLE_PROTOCOL_VERSION:
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
		case MNU_APP_VERSION: {
			showAppVersion();
			break;
		}
		}
		
		return false;
	}
	
	private String getAssetsContents(String filename, String defaultString) {
		StringBuilder builder = new StringBuilder();
		BufferedReader reader = null;
		try {
			reader = new BufferedReader(new InputStreamReader(getAssets().open(
					filename)));
			String line;
			while ((line = reader.readLine()) != null) {
				builder.append(line + "\n");
			}
		} catch (IOException e) {
			Log.d(logTag, "Can't open file with build info", e);
		} finally {
			if (reader != null) {
				try {
					reader.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
		return builder.length() > 0 ? builder.toString().trim() : defaultString;
	}

	private void showAppVersion() {
		String appVersion = getAssetsContents("appVersion", "Unknown");
		String buildInfo = getAssetsContents("build.info",
				"Build info not available");
		String changelog = getAssetsContents("CHANGELOG.txt",
				"Changelog not available");

		new AlertDialog.Builder(this)
				.setTitle("App version")
				.setMessage(
						appVersion + ", " + buildInfo + "\n\nCHANGELOG:\n"
								+ changelog)
				.setNeutralButton(android.R.string.ok, null).create().show();
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
			adapter.add("ScrollableMessage");
			adapter.add("ChangeRegistration");
			adapter.add("PutFile");
			adapter.add("DeleteFile");
			adapter.add("ListFiles");
			adapter.add("SetAppIcon");
			adapter.add("PerformAudioPassThru");
			adapter.add("EndAudioPassThru");
			adapter.add("SubscribeVehicleData");
			adapter.add("UnsubscribeVehicleData");
			adapter.add("GetVehicleData");
			adapter.add("ReadDID");
			adapter.add("GetDTCs");
			adapter.add("ShowConstantTBT");
			adapter.add("AlertManeuver");
			adapter.add("UpdateTurnList");
			adapter.add("DialNumber");
			
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
						} else if (adapter.getItem(which) == "Speak") {
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
										_msgAdapter.logMessage("Speak (request)", true);
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
						} else if (adapter.getItem(which) == "Show") {
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
						} else if (adapter.getItem(which) == "ButtonSubscriptions") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_buttonAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
								boolean needToSubscribe = !isButtonSubscribed[which];
									try {
										_msgAdapter.logMessage("ButtonSubscriptions (request)", true);
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
						} else if (adapter.getItem(which) == "AddCommand") {
							//something
							AlertDialog.Builder builder;
							AlertDialog addCommandDialog;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.addcommand,
									(ViewGroup) findViewById(R.id.itemRoot));

							final EditText er = (EditText) layout.findViewById(R.id.command);
							final EditText editVrSynonym = (EditText) layout.findViewById(R.id.command2);
							final Spinner s = (Spinner) layout.findViewById(R.id.availableSubmenus);
							s.setAdapter(_submenuAdapter);
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									String itemText = er.getText().toString();
									SyncSubMenu sm = new SyncSubMenu();
									sm = (SyncSubMenu) s.getSelectedItem();
									
									String vrSynonym = editVrSynonym.getText().toString();
									Vector<String> vrCommands = null;
									if (vrSynonym.length() > 0) {
										vrCommands = new Vector<String>();
										vrCommands.add(vrSynonym);
									}
									int cmdID = itemcmdID++;
									try {
										_msgAdapter.logMessage("AddCommand (request)", true);
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
						} else if (adapter.getItem(which) == "DeleteCommand") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_commandAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									int cmdID = _commandAdapter.getItem(which);
									try {
										_msgAdapter.logMessage("DeleteCommand (request)", true);
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
						} else if (adapter.getItem(which) == "AddSubMenu") {
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
										_msgAdapter.logMessage("AddSubMenu (request)", true);
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
						} else if (adapter.getItem(which) == "DeleteSubMenu") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_submenuAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									SyncSubMenu menu = _submenuAdapter.getItem(which);
									try {
										_msgAdapter.logMessage("DeleteSubMenu (request)", true);
										ProxyService.getInstance().getProxyInstance().deleteSubMenu(menu.getSubMenuId(), autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}

									_submenuAdapter.remove(menu);
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == "SetGlobalProperties") {
							try{
								Vector<TTSChunk> help = new Vector<TTSChunk>();
								help.add(TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, "This is the help prompt"));
								Vector<TTSChunk> timeout = new Vector<TTSChunk>();
								timeout.add(TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, "This is the timeout prompt"));
								_msgAdapter.logMessage("SetGlobalProperties (request)", true);
								ProxyService.getInstance().getProxyInstance().setGlobalProperties(help, timeout, autoIncCorrId++);
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
							
						} else if (adapter.getItem(which) == "SetMediaClockTimer") {
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
										_msgAdapter.logMessage("SetMediaClockTimer (request)", true);
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
						} else if (adapter.getItem(which) == "CreateChoiceSet") {
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
							final EditText vr1 = (EditText) layout.findViewById(R.id.createcommands_vr1);
							final EditText vr2 = (EditText) layout.findViewById(R.id.createcommands_vr2);
							final EditText vr3 = (EditText) layout.findViewById(R.id.createcommands_vr3);
							final CheckBox choice1 = (CheckBox) layout.findViewById(R.id.createcommands_choice1);
							final CheckBox choice2 = (CheckBox) layout.findViewById(R.id.createcommands_choice2);
							final CheckBox choice3 = (CheckBox) layout.findViewById(R.id.createcommands_choice3);

							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<Choice> commands = new Vector<Choice>();
									
									if (choice1.isChecked()) {
										Choice one = new Choice();
										one.setChoiceID(autoIncChoiceSetIdCmdId++);
										one.setMenuName(command1.getText().toString());
										one.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command1.getText().toString(),
												vr1.getText().toString() })));
										commands.add(one);
									}
									
									if (choice2.isChecked()) {
										Choice two = new Choice();
										two.setChoiceID(autoIncChoiceSetIdCmdId++);
										two.setMenuName(command2.getText().toString());
										two.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command2.getText().toString(),
												vr2.getText().toString() })));
										commands.add(two);
									}
									
									if (choice3.isChecked()) {
										Choice three = new Choice();
										three.setChoiceID(autoIncChoiceSetIdCmdId++);
										three.setMenuName(command3.getText().toString());
										three.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command3.getText().toString(),
												vr3.getText().toString() })));
										commands.add(three);
									}
									
									if (!commands.isEmpty()) {
										try {
											_msgAdapter.logMessage("CreateChoiceSet (request)", true);
											int choiceSetID = autoIncChoiceSetId++;
											ProxyService.getInstance().getProxyInstance().createInteractionChoiceSet(commands, choiceSetID, autoIncCorrId++);
											_choiceSetAdapter.add(choiceSetID);
										} catch (SyncException e) {
											_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
										}
									} else {
										Toast.makeText(getApplicationContext(), "No commands to set", Toast.LENGTH_SHORT).show();
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
						} else if (adapter.getItem(which) == "DeleteChoiceSet") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_choiceSetAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									int commandSetID = _choiceSetAdapter.getItem(which);
									try {
										_msgAdapter.logMessage("DeleteChoiceSet (request)", true);
										ProxyService.getInstance().getProxyInstance().deleteInteractionChoiceSet(commandSetID, autoIncCorrId++);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}

									_choiceSetAdapter.remove(commandSetID);
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == "PerformInteraction") {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_choiceSetAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									Vector<Integer> interactionChoiceSetIDs = new Vector<Integer>();
									interactionChoiceSetIDs.add(_choiceSetAdapter.getItem(which));
									try {
										_msgAdapter.logMessage("PerformInteraction (request)", true);
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
						} else if (adapter.getItem(which) == "Slider") {
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
						} else if (adapter.getItem(which) == "ChangeRegistration") {
							//ChangeRegistration
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.changeregistration, null);
							
							final Spinner spnLanguage = (Spinner) layout.findViewById(R.id.spnLanguage);
							ArrayAdapter<Language> spinnerAdapterLanguage = new ArrayAdapter<Language>(adapter.getContext(),
									android.R.layout.simple_spinner_item, Language.values());
							spinnerAdapterLanguage.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnLanguage.setAdapter(spinnerAdapterLanguage);
							
							final Spinner spnHmiDisplayLanguage = (Spinner) layout.findViewById(R.id.spnHmiDisplayLanguage);
							ArrayAdapter<Language> spinnerAdapterHmiDisplayLanguage = new ArrayAdapter<Language>(adapter.getContext(),
									android.R.layout.simple_spinner_item, Language.values());
							spinnerAdapterHmiDisplayLanguage.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnHmiDisplayLanguage.setAdapter(spinnerAdapterHmiDisplayLanguage);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										ChangeRegistration msg = new ChangeRegistration();
										msg.setLanguage((Language) spnLanguage.getSelectedItem());
										msg.setHmiDisplayLanguage((Language) spnHmiDisplayLanguage.getSelectedItem());
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
						} else if (adapter.getItem(which) == "PutFile") {
							//PutFile
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.putfile, null);
							
							final EditText txtSyncFileName = (EditText) layout.findViewById(R.id.syncFileName);
							
							final Spinner spnFileType = (Spinner) layout.findViewById(R.id.spnFileType);
							ArrayAdapter<FileType> spinnerAdapter = new ArrayAdapter<FileType>(adapter.getContext(),
									android.R.layout.simple_spinner_item, FileType.values());
							spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnFileType.setAdapter(spinnerAdapter);
							
							final CheckBox chkPersistentFile = (CheckBox) layout.findViewById(R.id.chkPersistentFile);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									String syncFileName = txtSyncFileName.getText().toString();
									try {
										PutFile msg = new PutFile();
										msg.setSyncFileName(syncFileName);
										msg.setFileType((FileType) spnFileType.getSelectedItem());
										msg.setPersistentFile(chkPersistentFile.isChecked());
										msg.setCorrelationID(autoIncCorrId++);
										
									    Bitmap photo = BitmapFactory.decodeResource(getResources(), R.drawable.fiesta);
								        ByteArrayOutputStream bas = new ByteArrayOutputStream();
									    photo.compress(CompressFormat.JPEG, 100, bas);
								        byte[] data = new byte[bas.toByteArray().length];
								        data = bas.toByteArray();
								        
								        msg.setBulkData(data);
										
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									_putFileAdapter.add(syncFileName);
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
						} else if (adapter.getItem(which) == "DeleteFile") {
							//DeleteFile
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_putFileAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									String syncFileName = _putFileAdapter.getItem(which);
									
									try {
										DeleteFile msg = new DeleteFile();
										msg.setSyncFileName(syncFileName);
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									_putFileAdapter.remove(syncFileName);
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == "ListFiles") {
							//ListFiles
							try {
								ListFiles msg = new ListFiles();
								msg.setCorrelationID(autoIncCorrId++);
								_msgAdapter.logMessage(msg, true);
								ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
						} else if (adapter.getItem(which) == "SetAppIcon") {
							//SetAppIcon
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.setappicon, null);
							
							final EditText txtSyncFileName = (EditText) layout.findViewById(R.id.syncFileNameIcon);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									String syncFileName = txtSyncFileName.getText().toString();
									try {
										SetAppIcon msg = new SetAppIcon();
										msg.setSyncFileName(syncFileName);
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
						} else if (adapter.getItem(which) == "PerformAudioPassThru") {
							//PerformAudioPassThru
							AlertDialog.Builder builder;
							AlertDialog dlg;

							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.performaudiopassthru, null);

							final EditText txtInitialPrompt = (EditText) layout.findViewById(R.id.txtInitialPrompt);

							final EditText txtAudioPassThruDisplayText1 = (EditText) layout.findViewById(R.id.txtAudioPassThruDisplayText1);

							final EditText txtAudioPassThruDisplayText2 = (EditText) layout.findViewById(R.id.txtAudioPassThruDisplayText2);
							
							final Spinner spnSamplingRate = (Spinner) layout.findViewById(R.id.spnSamplingRate);
							ArrayAdapter<SamplingRate> spinnerAdapterSamplingRate = new ArrayAdapter<SamplingRate>(adapter.getContext(),
									android.R.layout.simple_spinner_item, SamplingRate.values());
							spinnerAdapterSamplingRate.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnSamplingRate.setAdapter(spinnerAdapterSamplingRate);

							final EditText txtMaxDuration = (EditText) layout.findViewById(R.id.txtMaxDuration);
							
							final Spinner spnBitsPerSample = (Spinner) layout.findViewById(R.id.spnBitsPerSample);
							ArrayAdapter<AudioCaptureQuality> spinnerAdapterBitsPerSample = new ArrayAdapter<AudioCaptureQuality>(adapter.getContext(),
									android.R.layout.simple_spinner_item, AudioCaptureQuality.values());
							spinnerAdapterBitsPerSample.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnBitsPerSample.setAdapter(spinnerAdapterBitsPerSample);

							final Spinner spnAudioType = (Spinner) layout.findViewById(R.id.spnAudioType);
							ArrayAdapter<AudioType> spinnerAdapterAudioType = new ArrayAdapter<AudioType>(adapter.getContext(),
									android.R.layout.simple_spinner_item, AudioType.values());
							spinnerAdapterAudioType.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnAudioType.setAdapter(spinnerAdapterAudioType);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<TTSChunk> initChunks = TTSChunkFactory
											.createSimpleTTSChunks(txtInitialPrompt.getText().toString());
									try {
										PerformAudioPassThru msg = new PerformAudioPassThru();
										msg.setInitialPrompt(initChunks);
										msg.setAudioPassThruDisplayText1(txtAudioPassThruDisplayText1.getText().toString());
										msg.setAudioPassThruDisplayText2(txtAudioPassThruDisplayText2.getText().toString());
										msg.setSamplingRate((SamplingRate) spnSamplingRate.getSelectedItem());
										msg.setMaxDuration(Integer.parseInt(txtMaxDuration.getText().toString()));
										msg.setBitsPerSample((AudioCaptureQuality) spnBitsPerSample.getSelectedItem());
										msg.setAudioType((AudioType) spnAudioType.getSelectedItem());
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
						} else if (adapter.getItem(which) == "EndAudioPassThru") {
							//EndAudioPassThru
							try {
								EndAudioPassThru msg = new EndAudioPassThru();
								msg.setCorrelationID(autoIncCorrId++);
								_msgAdapter.logMessage(msg, true);
								ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
						} else if (adapter.getItem(which) == "SubscribeVehicleData") {
							//SubscribeVehicleData
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.subscribevehicledata, null);
							
							final CheckBox chkVEHICLEDATA_GPS = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_GPS);
							final CheckBox chkVEHICLEDATA_SPEED = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_SPEED);
							final CheckBox chkVEHICLEDATA_FUELLEVEL = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_FUELLEVEL);
							final CheckBox chkVEHICLEDATA_FUELECONOMY = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_FUELECONOMY);
							final CheckBox chkVEHICLEDATA_ENGINERPM = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_ENGINERPM);
							final CheckBox chkVEHICLEDATA_BATTVOLTS = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTVOLTS);
							final CheckBox chkVEHICLEDATA_RAINSENSOR = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_RAINSENSOR);
							final CheckBox chkVEHICLEDATA_ODOMETER = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_ODOMETER);
							final CheckBox chkVEHICLEDATA_VIN = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_VIN);
							final CheckBox chkVEHICLEDATA_EXTERNTEMP = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_EXTERNTEMP);
							final CheckBox chkVEHICLEDATA_PRNDLSTATUS = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_PRNDLSTATUS);
							final CheckBox chkVEHICLEDATA_TIREPRESSURE = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_TIREPRESSURE);
							final CheckBox chkVEHICLEDATA_BATTERYPACKVOLTAGE = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTERYPACKVOLTAGE);
							final CheckBox chkVEHICLEDATA_BATTERYCURRENT = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTERYCURRENT);
							final CheckBox chkVEHICLEDATA_BATTERYTEMPERATURE = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTERYTEMPERATURE);
							final CheckBox chkVEHICLEDATA_SATESN = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_SATESN);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<VehicleDataType> dataType = new Vector<VehicleDataType>();
									if (chkVEHICLEDATA_GPS.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_GPS);
									else if (chkVEHICLEDATA_SPEED.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_SPEED);
									else if (chkVEHICLEDATA_FUELLEVEL.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_FUELLEVEL);
									else if (chkVEHICLEDATA_FUELECONOMY.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_FUELECONOMY);
									else if (chkVEHICLEDATA_ENGINERPM.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_ENGINERPM);
									else if (chkVEHICLEDATA_BATTVOLTS.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTVOLTS);
									else if (chkVEHICLEDATA_RAINSENSOR.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_RAINSENSOR);
									else if (chkVEHICLEDATA_ODOMETER.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_ODOMETER);
									else if (chkVEHICLEDATA_VIN.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_VIN);
									else if (chkVEHICLEDATA_EXTERNTEMP.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_EXTERNTEMP);
									else if (chkVEHICLEDATA_PRNDLSTATUS.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_PRNDLSTATUS);
									else if (chkVEHICLEDATA_TIREPRESSURE.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_TIREPRESSURE);
									else if (chkVEHICLEDATA_BATTERYPACKVOLTAGE.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTERYPACKVOLTAGE);
									else if (chkVEHICLEDATA_BATTERYCURRENT.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTERYCURRENT);
									else if (chkVEHICLEDATA_BATTERYTEMPERATURE.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTERYTEMPERATURE);
									else if (chkVEHICLEDATA_SATESN.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_SATESN);
									
									try {
										SubscribeVehicleData msg = new SubscribeVehicleData();
										msg.setDataType(dataType);
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
						} else if (adapter.getItem(which) == "UnsubscribeVehicleData") {
							//UnsubscribeVehicleData
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.subscribevehicledata, null);
							
							final CheckBox chkVEHICLEDATA_GPS = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_GPS);
							final CheckBox chkVEHICLEDATA_SPEED = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_SPEED);
							final CheckBox chkVEHICLEDATA_FUELLEVEL = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_FUELLEVEL);
							final CheckBox chkVEHICLEDATA_FUELECONOMY = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_FUELECONOMY);
							final CheckBox chkVEHICLEDATA_ENGINERPM = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_ENGINERPM);
							final CheckBox chkVEHICLEDATA_BATTVOLTS = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTVOLTS);
							final CheckBox chkVEHICLEDATA_RAINSENSOR = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_RAINSENSOR);
							final CheckBox chkVEHICLEDATA_ODOMETER = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_ODOMETER);
							final CheckBox chkVEHICLEDATA_VIN = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_VIN);
							final CheckBox chkVEHICLEDATA_EXTERNTEMP = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_EXTERNTEMP);
							final CheckBox chkVEHICLEDATA_PRNDLSTATUS = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_PRNDLSTATUS);
							final CheckBox chkVEHICLEDATA_TIREPRESSURE = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_TIREPRESSURE);
							final CheckBox chkVEHICLEDATA_BATTERYPACKVOLTAGE = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTERYPACKVOLTAGE);
							final CheckBox chkVEHICLEDATA_BATTERYCURRENT = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTERYCURRENT);
							final CheckBox chkVEHICLEDATA_BATTERYTEMPERATURE = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_BATTERYTEMPERATURE);
							final CheckBox chkVEHICLEDATA_SATESN = (CheckBox) layout.findViewById(R.id.chkVEHICLEDATA_SATESN);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<VehicleDataType> dataType = new Vector<VehicleDataType>();
									if (chkVEHICLEDATA_GPS.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_GPS);
									else if (chkVEHICLEDATA_SPEED.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_SPEED);
									else if (chkVEHICLEDATA_FUELLEVEL.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_FUELLEVEL);
									else if (chkVEHICLEDATA_FUELECONOMY.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_FUELECONOMY);
									else if (chkVEHICLEDATA_ENGINERPM.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_ENGINERPM);
									else if (chkVEHICLEDATA_BATTVOLTS.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTVOLTS);
									else if (chkVEHICLEDATA_RAINSENSOR.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_RAINSENSOR);
									else if (chkVEHICLEDATA_ODOMETER.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_ODOMETER);
									else if (chkVEHICLEDATA_VIN.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_VIN);
									else if (chkVEHICLEDATA_EXTERNTEMP.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_EXTERNTEMP);
									else if (chkVEHICLEDATA_PRNDLSTATUS.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_PRNDLSTATUS);
									else if (chkVEHICLEDATA_TIREPRESSURE.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_TIREPRESSURE);
									else if (chkVEHICLEDATA_BATTERYPACKVOLTAGE.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTERYPACKVOLTAGE);
									else if (chkVEHICLEDATA_BATTERYCURRENT.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTERYCURRENT);
									else if (chkVEHICLEDATA_BATTERYTEMPERATURE.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_BATTERYTEMPERATURE);
									else if (chkVEHICLEDATA_SATESN.isChecked()) dataType.add(VehicleDataType.VEHICLEDATA_SATESN);
									
									try {
										UnsubscribeVehicleData msg = new UnsubscribeVehicleData();
										msg.setDataType(dataType);
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
						} else if (adapter.getItem(which) == "GetVehicleData") {
							//GetVehicleData
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_vehicleDataType, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									boolean needToSubscribe = !isVehicleDataSubscribed[which];
									try {
										GetVehicleData msg = new GetVehicleData();
										msg.setDataType(VehicleDataType.values()[which]);
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									isVehicleDataSubscribed[which] = !isVehicleDataSubscribed[which];
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == "ReadDID") {
							//ReadDID
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.readdid, null);
							
							final EditText txtECUNameDID = (EditText) layout.findViewById(R.id.txtECUNameDID);
							final EditText txtDIDLocation = (EditText) layout.findViewById(R.id.txtDIDLocation);
							final CheckBox chkEncryptedDID = (CheckBox) layout.findViewById(R.id.chkEncryptedDID);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<Integer> didlocations = new Vector<Integer>();
									didlocations.add(Integer.parseInt(txtDIDLocation.getText().toString()));
									try {
										ReadDID msg = new ReadDID();
										msg.setEcuName(Integer.parseInt(txtECUNameDID.getText().toString()));
										msg.setDidLocation(didlocations);
										msg.setEncrypted(chkEncryptedDID.isChecked());
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
						} else if (adapter.getItem(which) == "GetDTCs") { 
							//GetDTCs
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.getdtcs, null);
							
							final EditText txtECUNameDTC = (EditText) layout.findViewById(R.id.txtECUNameDTC);
							final CheckBox chkEncryptedDTC = (CheckBox) layout.findViewById(R.id.chkEncryptedDTC);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										GetDTCs msg = new GetDTCs();
										msg.setEcuName(Integer.parseInt(txtECUNameDTC.getText().toString()));
										msg.setEncrypted(chkEncryptedDTC.isChecked());
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
						} else if (adapter.getItem(which) == "ShowConstantTBT") {
							//ShowConstantTBT
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.showconstanttbt, null);
							
							final EditText txtNavigationText1 = (EditText) layout.findViewById(R.id.txtNavigationText1);
							final EditText txtNavigationText2 = (EditText) layout.findViewById(R.id.txtNavigationText2);
							final EditText txtEta = (EditText) layout.findViewById(R.id.txtEta);
							final EditText txtTotalDistance = (EditText) layout.findViewById(R.id.txtTotalDistance);
							
							final EditText txtDistanceToManeuver = (EditText) layout.findViewById(R.id.txtDistanceToManeuver);
							final EditText txtDistanceToManeuverScale = (EditText) layout.findViewById(R.id.txtDistanceToManeuverScale);
							
							final CheckBox chkManeuverComplete = (CheckBox) layout.findViewById(R.id.chkManeuverComplete);
							final CheckBox chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Image turnIcon = new Image();
									turnIcon.setValue("Value");
									turnIcon.setImageType(ImageType.STATIC);
									try {
										ShowConstantTBT msg = new ShowConstantTBT();
										msg.setNavigationText1(txtNavigationText1.getText().toString());
										msg.setNavigationText2(txtNavigationText2.getText().toString());
										msg.setEta(txtEta.getText().toString());
										msg.setTotalDistance(txtTotalDistance.getText().toString());
										msg.setTurnIcon(turnIcon);
										msg.setDistanceToManeuver((float) Integer.parseInt(txtDistanceToManeuver.getText().toString()));
										msg.setDistanceToManeuverScale((float) Integer.parseInt(txtDistanceToManeuverScale.getText().toString()));
										msg.setManeuverComplete(chkManeuverComplete.isChecked());
										msg.setCorrelationID(autoIncCorrId++);
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
						} else if (adapter.getItem(which) == "AlertManeuver") {
							//AlertManeuver
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.alertmaneuver, null);

							final EditText txtTtsChunks = (EditText) layout.findViewById(R.id.txtTtsChunks);
							
							final CheckBox chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<TTSChunk> ttsChunks = TTSChunkFactory
											.createSimpleTTSChunks(txtTtsChunks.getText().toString());
									try {
										AlertManeuver msg = new AlertManeuver();
										msg.setTtsChunks(ttsChunks);
										msg.setCorrelationID(autoIncCorrId++);
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
						} else if (adapter.getItem(which) == "UpdateTurnList") {
							UpdateTurnList msg = new UpdateTurnList();
							
							Turn t1 = new Turn();
							t1.setNavigationText("Turn 1");
							Image ti1 = new Image();
							ti1.setValue("Turn1");
							ti1.setImageType(ImageType.STATIC);
							t1.setTurnIcon(ti1);
							Turn t2 = new Turn();
							t2.setNavigationText("Turn 2");
							Image ti2 = new Image();
							ti2.setValue("Turn2");
							ti2.setImageType(ImageType.STATIC);
							t2.setTurnIcon(ti2);
							Vector<Turn> tarray = new Vector<Turn>();
							tarray.add(t1);
							tarray.add(t2);
							msg.setTurnList(tarray);
							
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
							
							_msgAdapter.logMessage(msg, true);
							
							try {
								ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
						} else if (adapter.getItem(which) == "DialNumber") {
							DialNumber msg = new DialNumber();
							msg.setNumber("3138675309");
							
							_msgAdapter.logMessage(msg, true);
							
							try {
								ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
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
    
    public static SyncProxyTester getInstance() {
		return _activity;
	}
    
    public static logAdapter getMessageAdapter() {
		return _msgAdapter;
	}
    
	public void setTesterMain(ModuleTest _instance) {
		this._testerMain = _instance;
	}
	
	@Override
	public void onBackPressed() {
		super.onBackPressed();
	}
	
	@Override
	public void onPause() {
		super.onPause();
	}
}

