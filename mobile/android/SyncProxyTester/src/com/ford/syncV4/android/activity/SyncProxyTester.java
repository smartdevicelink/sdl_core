package com.ford.syncV4.android.activity;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnMultiChoiceClickListener;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager.NameNotFoundException;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnFocusChangeListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CheckedTextView;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.RadioGroup;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.Toast;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.adapters.logAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.android.module.ModuleTest;
import com.ford.syncV4.android.policies.PoliciesTesterActivity;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.SyncProxyALM;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuver;
import com.ford.syncV4.proxy.rpc.ChangeRegistration;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.DialNumber;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.EndAudioPassThru;
import com.ford.syncV4.proxy.rpc.GenericResponse;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.ListFiles;
import com.ford.syncV4.proxy.rpc.MenuParams;
import com.ford.syncV4.proxy.rpc.OnAudioPassThru;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.SubscribeButton;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.Turn;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.VrHelpItem;
import com.ford.syncV4.proxy.rpc.enums.AudioType;
import com.ford.syncV4.proxy.rpc.enums.BitsPerSample;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.FileType;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.rpc.enums.SamplingRate;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.Base64;
import com.lamerman.FileDialog;
import com.lamerman.SelectionMode;

public class SyncProxyTester extends Activity implements OnClickListener {
	private static final String VERSION = "$Version:$";
	
	private static final String logTag = "SyncProxyTester";
	
	private static final String ButtonSubscriptions = "ButtonSubscriptions";
	private static final String VehicleDataSubscriptions = "VehicleDataSubscriptions";
	/** Name of a generic request. See {@link GenericRequest}. */
	private static final String GenericRequest = "GenericRequest";

	/**
	 * The name of the file where all the data coming with
	 * {@link OnAudioPassThru} notifications is saved. The root directory is the
	 * external storage.
	 */
	private static final String AUDIOPASSTHRU_OUTPUT_FILE = "audiopassthru.wav";

	private static final int ALERT_MAXSOFTBUTTONS = 4;
	private static final int SCROLLABLEMESSAGE_MAXSOFTBUTTONS = 8;
	private static final int SHOW_MAXSOFTBUTTONS = 8;
	private static final int ALERTMANEUVER_MAXSOFTBUTTONS = 3;
	private static final int SHOWCONSTANTTBT_MAXSOFTBUTTONS = 3;
	private static final int UPDATETURNLIST_MAXSOFTBUTTONS = 1;

	private static final int REQUEST_FILE_OPEN = 50;
	private final static int REQUEST_CHOOSE_XML_TEST = 51;
	
	private static final int PUTFILE_MAXFILESIZE = 4 * 1024 * 1024; // 4MB

    private static SyncProxyTester _activity;
    private static ArrayList<Object> _logMessages = new ArrayList<Object>();
	private static logAdapter _msgAdapter;
	private ModuleTest _testerMain;
	private static byte[] _ESN;
	
	private ScrollView _scroller = null;
	private ListView _listview = null;
	
	private ArrayAdapter<SyncSubMenu> _submenuAdapter = null;
	private ArrayAdapter<Integer> _commandAdapter = null;
	private Map<Integer, Integer> _commandIdToParentSubmenuMap = null;
	private ArrayAdapter<Integer> _choiceSetAdapter = null;
	private ArrayAdapter<String> _putFileAdapter = null;
	
	private static final int CHOICESETID_UNSET = -1;
	/**
	 * Latest choiceSetId, required to add it to the adapter when a successful
	 * CreateInteractionChoiceSetResponse comes.
	 */
	private int _latestCreateChoiceSetId = CHOICESETID_UNSET;
	/**
	 * Latest choiceSetId, required to delete it from the adapter when a
	 * successful DeleteInteractionChoiceSetResponse comes.
	 */
	private int _latestDeleteChoiceSetId = CHOICESETID_UNSET;
	/**
	 * Latest SyncSubMenu, required to delete the submenu from the adapter
	 * when a successful DeleteSubMenuResponse comes.
	 */
	private SyncSubMenu _latestDeleteSubmenu = null;
	/**
	 * Latest SyncSubMenu, required to add the submenu from the adapter when a
	 * successful AddSubMenuResponse comes.
	 */
	private SyncSubMenu _latestAddSubmenu = null;
	
	/** List of function names supported in protocol v1. */
	private Vector<String> v1Functions = null;

	private int autoIncCorrId = 101;
	private int autoIncChoiceSetId = 1;
	private int autoIncChoiceSetIdCmdId = 1;
	private int itemcmdID = 1;
	private int submenucmdID = 1000;

	private ArrayAdapter<ButtonName> _buttonAdapter = null;
	private boolean[] isButtonSubscribed = null;

	private ArrayAdapter<VehicleDataType> _vehicleDataType = null;
	private boolean[] isVehicleDataSubscribed = null;
	
	/**
	 * List of soft buttons for current function. Passed between
	 * {@link SoftButtonsListActivity} and this activity.
	 */
	private Vector<SoftButton> currentSoftButtons;
	/**
	 * The Include Soft Buttons checkbox in the current dialog. Kept here to
	 * check it when the user has explicitly set the soft buttons.
	 */
	private CheckBox chkIncludeSoftButtons;
	/**
	 * Reference to PutFile dialog's local filename text field, so that the
	 * filename is set after choosing.
	 */
	private EditText txtLocalFileName;
	
	/**
	 * Stores the number of selections of each message to sort them by
	 * most-popular usage.
	 */
	private Map<String, Integer> messageSelectCount;
	private static final String MSC_PREFIX = "msc_";

	/** The output stream to write audioPassThru data. */
	private OutputStream audioPassThruOutStream = null;
	/** Media player for the stream of audio pass thru. */
	private MediaPlayer audioPassThruMediaPlayer = null;
	/**
	 * The most recent sent PerformAudioPassThru message, saved in case we need
	 * to retry the request.
	 */
	private PerformAudioPassThru latestPerformAudioPassThruMsg = null;

	/** Autoincrementing id for new softbuttons. */
	private static int autoIncSoftButtonId = 5500;

	/**
	 * In onCreate() specifies if it is the first time the activity is created
	 * during this app launch.
	 */
	private static boolean isFirstActivityRun = true;
	
	/**
	 * A custom {@link RPCRequest} subclass that is not present in the
	 * specification. Used to test that the response is {@link GenericResponse}.
	 */
	class GenericRequest extends RPCRequest {
		public GenericRequest() {
			super("GenericRequest");
		}
		
		public GenericRequest(Hashtable hash) {
			super(hash);
		}
	}
	

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
    	
    	_activity = this;

		//_msgAdapter.logMessage("SyncProxyTester activity was started.", Log.DEBUG);

		setContentView(R.layout.main);
		_scroller = (ScrollView) findViewById(R.id.scrollConsole);

		((Button) findViewById(R.id.btnSendMessage)).setOnClickListener(this);
		((Button) findViewById(R.id.btnPlayPause)).setOnClickListener(this);
		
		resetAdapters();

		_vehicleDataType = new ArrayAdapter<VehicleDataType>(this,
				android.R.layout.simple_spinner_item, VehicleDataType.values());
		_vehicleDataType
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		_listview = (ListView) findViewById(R.id.messageList);
		_msgAdapter = new logAdapter(logTag, false, this, R.layout.row, _logMessages);
		
		_listview.setClickable(true);
		_listview.setAdapter(_msgAdapter);
		_listview.setTranscriptMode(ListView.TRANSCRIPT_MODE_ALWAYS_SCROLL);
		_listview.setOnItemClickListener(new OnItemClickListener() {
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				Object listObj = parent.getItemAtPosition(position);
				if (listObj instanceof RPCMessage) {
					AlertDialog.Builder builder = new AlertDialog.Builder(SyncProxyTester.this);
					String rawJSON = "";
					
					Integer corrId = -1;
					if (listObj instanceof RPCRequest) {
						corrId = ((RPCRequest) listObj).getCorrelationID();
					} else if (listObj instanceof RPCResponse) {
						corrId = ((RPCResponse) listObj).getCorrelationID();
					}

					try {
						rawJSON = ((RPCMessage) listObj).serializeJSON(
								ProxyService.getInstance().getProxyInstance().getWiProVersion()).toString(2);
						builder.setTitle("Raw JSON" + (corrId != -1 ? " (Corr ID " + corrId + ")" : ""));
					} catch (Exception e) {
						try {rawJSON = ((RPCMessage) listObj).getFunctionName() + 
								" (" + ((RPCMessage) listObj).getMessageType() + ")";
						} catch (Exception e1) {rawJSON = "Undefined";}
					}
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
		
		if (isFirstActivityRun) {
			selectProtocolUI();
		} else {
			showProtocolPropertiesInTitle();
			startSyncProxy();
		}

		loadMessageSelectCount();
		
		isFirstActivityRun = false;
	}

	private void loadMessageSelectCount() {
		SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, 0);
		messageSelectCount = new Hashtable<String, Integer>();
		for (Entry<String, ?> entry : prefs.getAll().entrySet()) {
			if (entry.getKey().startsWith(MSC_PREFIX)) {
				messageSelectCount.put(
						entry.getKey().substring(MSC_PREFIX.length()),
						(Integer) entry.getValue());
			}
		}
	}

	private void saveMessageSelectCount() {
		if (messageSelectCount == null) {
			return;
		}

		SharedPreferences.Editor editor = getSharedPreferences(
				Const.PREFS_NAME, 0).edit();
		for (Entry<String, Integer> entry : messageSelectCount.entrySet()) {
			editor.putInt(MSC_PREFIX + entry.getKey(), entry.getValue());
		}
		editor.commit();
	}

	private void clearMessageSelectCount() {
		messageSelectCount = new Hashtable<String, Integer>();
		SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, 0);
		SharedPreferences.Editor editor = prefs.edit();
		for (Entry<String, ?> entry : prefs.getAll().entrySet()) {
			if (entry.getKey().startsWith(MSC_PREFIX)) {
				editor.remove(entry.getKey());
			}
		}
		editor.commit();
	}

	/**
	 * Shows a dialog where the user can select connection features (protocol
	 * version, media flag, app name, language, HMI language, and transport
	 * settings). Starts the proxy after selecting.
	 */
	private void selectProtocolUI() {
		Context context = this;
		LayoutInflater inflater = (LayoutInflater) context
				.getSystemService(LAYOUT_INFLATER_SERVICE);
		View view = inflater.inflate(R.layout.selectprotocol,
				(ViewGroup) findViewById(R.id.selectprotocol_Root));

		ArrayAdapter<Language> langAdapter = new ArrayAdapter<Language>(this,
				android.R.layout.simple_spinner_item, Language.values());
		langAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		final RadioGroup protocolVersionGroup = (RadioGroup) view
				.findViewById(R.id.selectprotocol_radioGroupProtocolVersion);
		final CheckBox mediaCheckBox = (CheckBox) view
				.findViewById(R.id.selectprotocol_checkMedia);
		final EditText appNameEditText = (EditText) view
				.findViewById(R.id.selectprotocol_appName);
		final Spinner langSpinner = (Spinner) view
				.findViewById(R.id.selectprotocol_lang);
		final Spinner hmiLangSpinner = (Spinner) view
				.findViewById(R.id.selectprotocol_hmiLang);
		final RadioGroup transportGroup = (RadioGroup) view
				.findViewById(R.id.selectprotocol_radioGroupTransport);
		final EditText ipAddressEditText = (EditText) view
				.findViewById(R.id.selectprotocol_ipAddr);
		final EditText tcpPortEditText = (EditText) view
				.findViewById(R.id.selectprotocol_tcpPort);
		final CheckBox autoReconnectCheckBox = (CheckBox) view
				.findViewById(R.id.selectprotocol_checkAutoReconnect);
		final CheckBox autoSetAppIconCheckBox = (CheckBox) view
				.findViewById(R.id.selectprotocol_checkAutoSetAppIcon);

		ipAddressEditText.setEnabled(false);
		tcpPortEditText.setEnabled(false);
		autoReconnectCheckBox.setEnabled(false);

		transportGroup
				.setOnCheckedChangeListener(new RadioGroup.OnCheckedChangeListener() {
					@Override
					public void onCheckedChanged(RadioGroup group, int checkedId) {
						boolean transportOptionsEnabled = checkedId == R.id.selectprotocol_radioWiFi;
						ipAddressEditText.setEnabled(transportOptionsEnabled);
						tcpPortEditText.setEnabled(transportOptionsEnabled);
						autoReconnectCheckBox
								.setEnabled(transportOptionsEnabled);
					}
				});

		langSpinner.setAdapter(langAdapter);
		hmiLangSpinner.setAdapter(langAdapter);

		// display current configs
		final SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME,
				0);
		int protocolVersion = prefs.getInt(Const.PREFS_KEY_PROTOCOLVERSION,
				Const.PREFS_DEFAULT_PROTOCOLVERSION);
		boolean isMedia = prefs.getBoolean(Const.PREFS_KEY_ISMEDIAAPP,
				Const.PREFS_DEFAULT_ISMEDIAAPP);
		String appName = prefs.getString(Const.PREFS_KEY_APPNAME,
				Const.PREFS_DEFAULT_APPNAME);
		Language lang = Language.valueOf(prefs.getString(Const.PREFS_KEY_LANG,
				Const.PREFS_DEFAULT_LANG));
		Language hmiLang = Language.valueOf(prefs.getString(
				Const.PREFS_KEY_HMILANG, Const.PREFS_DEFAULT_HMILANG));
		int transportType = prefs.getInt(
				Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
				Const.Transport.PREFS_DEFAULT_TRANSPORT_TYPE);
		String ipAddress = prefs.getString(
				Const.Transport.PREFS_KEY_TRANSPORT_IP,
				Const.Transport.PREFS_DEFAULT_TRANSPORT_IP);
		int tcpPort = prefs.getInt(Const.Transport.PREFS_KEY_TRANSPORT_PORT,
				Const.Transport.PREFS_DEFAULT_TRANSPORT_PORT);
		boolean autoReconnect = prefs.getBoolean(
				Const.Transport.PREFS_KEY_TRANSPORT_RECONNECT,
				Const.Transport.PREFS_DEFAULT_TRANSPORT_RECONNECT_DEFAULT);
		boolean autoSetAppIcon = prefs.getBoolean(
				Const.PREFS_KEY_AUTOSETAPPICON,
				Const.PREFS_DEFAULT_AUTOSETAPPICON);

		int radioButtonId = R.id.selectprotocol_radioV1;
		switch (protocolVersion) {
		case 2:
			radioButtonId = R.id.selectprotocol_radioV2;
			break;

		case 1:
			// 1 is the default
			break;

		default:
			radioButtonId = -1;
			break;
		}
		protocolVersionGroup.check(radioButtonId);
		mediaCheckBox.setChecked(isMedia);
		appNameEditText.setText(appName);
		langSpinner.setSelection(langAdapter.getPosition(lang));
		hmiLangSpinner.setSelection(langAdapter.getPosition(hmiLang));
		transportGroup
				.check(transportType == Const.Transport.KEY_TCP ? R.id.selectprotocol_radioWiFi
						: R.id.selectprotocol_radioBT);
		ipAddressEditText.setText(ipAddress);
		tcpPortEditText.setText(String.valueOf(tcpPort));
		autoReconnectCheckBox.setChecked(autoReconnect);
		autoSetAppIconCheckBox.setChecked(autoSetAppIcon);

		new AlertDialog.Builder(context)
				.setTitle("Please select protocol properties")
				.setCancelable(false)
				.setPositiveButton("OK", new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						int protocolVersion = 1;
						switch (protocolVersionGroup.getCheckedRadioButtonId()) {
						case R.id.selectprotocol_radioV2:
							protocolVersion = 2;
							break;

						case R.id.selectprotocol_radioV1:
							// 1 by default
						default:
							break;
						}

						boolean isMedia = mediaCheckBox.isChecked();
						String appName = appNameEditText.getText().toString();
						String lang = ((Language) langSpinner.getSelectedItem())
								.name();
						String hmiLang = ((Language) hmiLangSpinner
								.getSelectedItem()).name();
						int transportType = transportGroup
								.getCheckedRadioButtonId() == R.id.selectprotocol_radioWiFi ? Const.Transport.KEY_TCP
								: Const.Transport.KEY_BLUETOOTH;
						String ipAddress = ipAddressEditText.getText()
								.toString();
						int tcpPort = Integer.parseInt(tcpPortEditText
								.getText().toString());
						boolean autoReconnect = autoReconnectCheckBox
								.isChecked();
						boolean autoSetAppIcon = autoSetAppIconCheckBox
								.isChecked();

						// save the configs
						boolean success = prefs
								.edit()
								.putInt(Const.PREFS_KEY_PROTOCOLVERSION,
										protocolVersion)
								.putBoolean(Const.PREFS_KEY_ISMEDIAAPP, isMedia)
								.putString(Const.PREFS_KEY_APPNAME, appName)
								.putString(Const.PREFS_KEY_LANG, lang)
								.putString(Const.PREFS_KEY_HMILANG, hmiLang)
								.putInt(Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
										transportType)
								.putString(
										Const.Transport.PREFS_KEY_TRANSPORT_IP,
										ipAddress)
								.putInt(Const.Transport.PREFS_KEY_TRANSPORT_PORT,
										tcpPort)
								.putBoolean(
										Const.Transport.PREFS_KEY_TRANSPORT_RECONNECT,
										autoReconnect)
								.putBoolean(Const.PREFS_KEY_AUTOSETAPPICON,
										autoSetAppIcon).commit();
						if (!success) {
							Log.w(logTag,
									"Can't save selected protocol properties");
						}

						showProtocolPropertiesInTitle();
						startSyncProxy();
					}
				}).setView(view).show();
	}

	/** Starts the sync proxy at startup after selecting protocol features. */
	private void startSyncProxy() {
		// Publish an SDP record and create a SYNC proxy.
		// startSyncProxyService();
		if (ProxyService.getInstance() == null) {
			Intent startIntent = new Intent(SyncProxyTester._activity, ProxyService.class);
			startService(startIntent);
			// bindService(startIntent, this, Context.BIND_AUTO_CREATE);
		} else {
			// need to get the instance and add myself as a listener
			ProxyService.getInstance().setCurrentActivity(SyncProxyTester._activity);
		}
	}

	/**
	 * Initializes/resets the adapters keeping created submenus, interaction
	 * choice set ids, etc.
	 */
	private void resetAdapters() {
		// set up storage for subscription records
		// ignoring the last CUSTOM_NAME value
		List<ButtonName> subscribableButtonNames = Arrays.asList(ButtonName.values()).
				subList(0, ButtonName.values().length - 1);
		isButtonSubscribed = new boolean[subscribableButtonNames.size()];
		_buttonAdapter = new ArrayAdapter<ButtonName>(this,
				android.R.layout.select_dialog_multichoice, subscribableButtonNames) {
			public View getView(int position, View convertView, ViewGroup parent) {
				CheckedTextView ret = (CheckedTextView) super.getView(position,
						convertView, parent);
				ret.setChecked(isButtonSubscribed[position]);
				return ret;
			}
		};

		isVehicleDataSubscribed = new boolean[VehicleDataType.values().length];

		_submenuAdapter = new ArrayAdapter<SyncSubMenu>(this,
				android.R.layout.select_dialog_item);
		_submenuAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		// Add top level menu with parent ID zero
		SyncSubMenu sm = new SyncSubMenu();
		sm.setName("Top Level Menu");
		sm.setSubMenuId(0);
		addSubMenuToList(sm);

		_commandAdapter = new ArrayAdapter<Integer>(this,
				android.R.layout.select_dialog_item);
		_commandAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
		
		_commandIdToParentSubmenuMap = new Hashtable<Integer, Integer>();

		_choiceSetAdapter = new ArrayAdapter<Integer>(this,
				android.R.layout.simple_spinner_item);
		_choiceSetAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

		_putFileAdapter = new ArrayAdapter<String>(this,
				android.R.layout.select_dialog_item);
		_putFileAdapter
				.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
	}

	/** Displays the current protocol properties in the activity's title. */
	private void showProtocolPropertiesInTitle() {
		final SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME,
				0);
		int protocolVersion = prefs.getInt(Const.PREFS_KEY_PROTOCOLVERSION,
				Const.PREFS_DEFAULT_PROTOCOLVERSION);
		boolean isMedia = prefs.getBoolean(Const.PREFS_KEY_ISMEDIAAPP,
				Const.PREFS_DEFAULT_ISMEDIAAPP);
		String transportType = prefs.getInt(
				Const.Transport.PREFS_KEY_TRANSPORT_TYPE,
				Const.Transport.PREFS_DEFAULT_TRANSPORT_TYPE) == Const.Transport.KEY_TCP ? "WiFi"
				: "BT";
		setTitle(getResources().getString(R.string.tester_app_name) + " (v"
				+ protocolVersion + ", " + (isMedia ? "" : "non-") + "media, "
				+ transportType + ")");
	}

	protected void onDestroy() {
		super.onDestroy();
		//endSyncProxyInstance();
		saveMessageSelectCount();
		_activity = null;
		ProxyService service = ProxyService.getInstance();
		if (service != null) {
			service.setCurrentActivity(null);
		}
		closeAudioPassThruStream();
		closeAudioPassThruMediaPlayer();
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
	private final int MNU_CLEAR_FUNCTIONS_USAGE = 16;
	private final int MNU_WAKELOCK = 17;

	
	/* Creates the menu items */
	public boolean onCreateOptionsMenu(Menu menu) {
		boolean result = super.onCreateOptionsMenu(menu);
		if (result) {
			menu.add(0, PROXY_START, 0, "Proxy Start");
			menu.add(0, MNU_TOGGLE_CONSOLE, 0, "Toggle Console");
			menu.add(0, MNU_CLEAR, 0, "Clear Messages");
			menu.add(0, MNU_EXIT, 0, "Exit");
/*			menu.add(0, MNU_TOGGLE_MEDIA, 0, "Toggle Media");
			menu.add(0, MNU_TOGGLE_PROTOCOL_VERSION, 0,
					"Toggle Protocol Ver. (cur " + String.valueOf(
							getCurrentProtocolVersion()) + ")");*/
			menu.add(0, MNU_APP_VERSION, 0, "App version");
			menu.add(0, MNU_UNREGISTER, 0, "Unregister");
			menu.add(0, MNU_CLEAR_FUNCTIONS_USAGE, 0, "Reset functions usage");
			menu.add(0, XML_TEST, 0, "XML Test");
			menu.add(0, POLICIES_TEST, 0, "Policies Test");
			MenuItem menuitem = menu.add(0, MNU_WAKELOCK, 0, "Lock screen while testing");
			menuitem.setCheckable(true);
			menuitem.setChecked(!getDisableLockFlag());
			return true;
		} else {
			return false;
		}
	}
	
	@Override
	public boolean onPrepareOptionsMenu(Menu menu) {
		MenuItem menuItem = menu.findItem(MNU_WAKELOCK);
		if (menuItem != null) {
			menuItem.setChecked(!getDisableLockFlag());
		}
		return super.onPrepareOptionsMenu(menu);
	}
	
	private int getCurrentProtocolVersion() {
		return getSharedPreferences(Const.PREFS_NAME, 0).getInt(
				Const.PREFS_KEY_PROTOCOLVERSION,
				Const.PREFS_DEFAULT_PROTOCOLVERSION);
	}

	private boolean getIsMedia() {
		return getSharedPreferences(Const.PREFS_NAME, 0).getBoolean(
				Const.PREFS_KEY_ISMEDIAAPP, Const.PREFS_DEFAULT_ISMEDIAAPP);
	}

	/* Handles item selections */
	public boolean onOptionsItemSelected(MenuItem item) {

		switch (item.getItemId()) {
        case PROXY_START:
	        BluetoothAdapter mBtAdapter = BluetoothAdapter.getDefaultAdapter();
	        if (!mBtAdapter.isEnabled()) mBtAdapter.enable();
	        
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
			xmlTest();
			break;
		case POLICIES_TEST:
			if(PoliciesTesterActivity.getInstance() == null) {
				Intent i = new Intent(this, PoliciesTesterActivity.class);
				i.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
				i.addFlags(Intent.FLAG_ACTIVITY_NO_USER_ACTION);
				startActivity(i);
				if (_ESN != null) PoliciesTesterActivity.setESN(_ESN);
			}	
			//PoliciesTest.runPoliciesTest();
			break;
		case MNU_EXIT:
			exitApp();
			break;
		case MNU_TOGGLE_PROTOCOL_VERSION:
			{SharedPreferences settings = getSharedPreferences(Const.PREFS_NAME, 0);
			int versionN = settings.getInt(Const.PREFS_KEY_PROTOCOLVERSION, Const.PREFS_DEFAULT_PROTOCOLVERSION);
			SharedPreferences.Editor editor = settings.edit();
			int newVersion = versionN == 1 ? 2:1;
			editor.putInt(Const.PREFS_KEY_PROTOCOLVERSION, newVersion);
			editor.commit();
			
			Toast.makeText(getApplicationContext(),
					"Please start the app again", Toast.LENGTH_LONG).show();
			exitApp();
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
			SharedPreferences settings = getSharedPreferences(Const.PREFS_NAME, 0);
			boolean isMediaApp = settings.getBoolean(Const.PREFS_KEY_ISMEDIAAPP, Const.PREFS_DEFAULT_ISMEDIAAPP);
			SharedPreferences.Editor editor = settings.edit();
			editor.putBoolean(Const.PREFS_KEY_ISMEDIAAPP, !isMediaApp);

			// Don't forget to commit your edits!!!
			editor.commit();
			//super.finish();
			return true;
		case MNU_UNREGISTER:
			/*
			endSyncProxyInstance();
        	startSyncProxyService();
        	*/		       
	    		if (ProxyService.getInstance() == null) {
	    			Intent startIntent = new Intent(this, ProxyService.class);
	    			startService(startIntent);
	    			//bindService(startIntent, this, Context.BIND_AUTO_CREATE);
	    		} else {
	    			// need to get the instance and add myself as a listener
	    			ProxyService.getInstance().setCurrentActivity(this);
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
		case MNU_CLEAR_FUNCTIONS_USAGE:
			clearMessageSelectCount();
			break;
		case MNU_WAKELOCK:
			toggleDisableLock();
			break;
		}
		
		return false;
	}

	private void xmlTest() {
		openXmlFilePathDialog();
	}
	
	private void xmlTestContinue(String filePath) {
		if (_testerMain != null) {
			_testerMain.restart(filePath);
			Toast.makeText(getApplicationContext(), "start your engines", Toast.LENGTH_SHORT).show();
		} else {
			ProxyService.getInstance().startModuleTest();
			_testerMain.restart(filePath);
			Toast.makeText(getApplicationContext(), "Start the app on SYNC first", Toast.LENGTH_LONG).show();
		}
	}

	/**
	 * Toggles the current state of the disable lock when testing flag, and
	 * writes it to the preferences.
	 */
	private void toggleDisableLock() {
		SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME, 0);
		boolean disableLock = prefs.getBoolean(
				Const.PREFS_KEY_DISABLE_LOCK_WHEN_TESTING,
				Const.PREFS_DEFAULT_DISABLE_LOCK_WHEN_TESTING);
		disableLock = !disableLock;
		prefs.edit()
				.putBoolean(Const.PREFS_KEY_DISABLE_LOCK_WHEN_TESTING,
						disableLock).commit();
	}

	/**
	 * Returns the current state of the disable lock when testing flag.
	 * 
	 * @return true if the screen lock is disabled
	 */
	public boolean getDisableLockFlag() {
		return getSharedPreferences(Const.PREFS_NAME, 0).getBoolean(
				Const.PREFS_KEY_DISABLE_LOCK_WHEN_TESTING,
				Const.PREFS_DEFAULT_DISABLE_LOCK_WHEN_TESTING);
	}

	/** Closes the activity and stops the proxy service. */
	private void exitApp() {
		stopService(new Intent(this, ProxyService.class));
		finish();
		saveMessageSelectCount();
		new Timer().schedule(new TimerTask() {
			@Override
			public void run() {
				android.os.Process.killProcess(android.os.Process.myPid());
			}
		}, 1000);
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
		String appVersion;
		try {
			appVersion = getPackageManager()
					.getPackageInfo(getPackageName(), 0).versionName;
		} catch (NameNotFoundException e) {
			Log.d(logTag, "Can't get package info", e);
			appVersion = "Unknown";
		}
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

	/**
	 * Adds the function name to the adapter if it is supported by the specified
	 * protocol version.
	 */
	private void addToFunctionsAdapter(ArrayAdapter<String> adapter,
			int protocolVersion, String functionName) {
		switch (protocolVersion) {
		case 1:
			if (v1Functions == null) {
				v1Functions = new Vector<String>();
				v1Functions.add(Names.RegisterAppInterface);
				v1Functions.add(Names.UnregisterAppInterface);
				v1Functions.add(Names.SetGlobalProperties);
				v1Functions.add(Names.ResetGlobalProperties);
				v1Functions.add(Names.AddCommand);
				v1Functions.add(Names.DeleteCommand);
				v1Functions.add(Names.AddSubMenu);
				v1Functions.add(Names.DeleteSubMenu);
				v1Functions.add(Names.CreateInteractionChoiceSet);
				v1Functions.add(Names.PerformInteraction);
				v1Functions.add(Names.DeleteInteractionChoiceSet);
				v1Functions.add(Names.Alert);
				v1Functions.add(Names.Show);
				v1Functions.add(Names.Speak);
				v1Functions.add(Names.SetMediaClockTimer);
				v1Functions.add(Names.EncodedSyncPData);
				v1Functions.add(Names.SubscribeButton);
				v1Functions.add(Names.UnsubscribeButton);
				// it's the name of the menu item for the last two commands
				v1Functions.add(ButtonSubscriptions);
				v1Functions.add(GenericRequest);
			}
			
			if (v1Functions.contains(functionName)) {
				adapter.add(functionName);
			}
			break;

		default:
			adapter.add(functionName);
			break;
		}
	}

	public void onClick(View v) {
		if (v == findViewById(R.id.btnSendMessage)) {
			final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.select_dialog_item);
			int protocolVersion = getCurrentProtocolVersion();
			addToFunctionsAdapter(adapter, protocolVersion, Names.Alert);
			addToFunctionsAdapter(adapter, protocolVersion, Names.Speak);
			addToFunctionsAdapter(adapter, protocolVersion, Names.Show);
			addToFunctionsAdapter(adapter, protocolVersion, ButtonSubscriptions);
			addToFunctionsAdapter(adapter, protocolVersion, Names.AddCommand);
			addToFunctionsAdapter(adapter, protocolVersion, Names.DeleteCommand);
			addToFunctionsAdapter(adapter, protocolVersion, Names.AddSubMenu);
			addToFunctionsAdapter(adapter, protocolVersion, Names.DeleteSubMenu);
			addToFunctionsAdapter(adapter, protocolVersion, Names.SetGlobalProperties);
			addToFunctionsAdapter(adapter, protocolVersion, Names.ResetGlobalProperties);
			addToFunctionsAdapter(adapter, protocolVersion, Names.SetMediaClockTimer);
			addToFunctionsAdapter(adapter, protocolVersion, Names.CreateInteractionChoiceSet);
			addToFunctionsAdapter(adapter, protocolVersion, Names.DeleteInteractionChoiceSet);
			addToFunctionsAdapter(adapter, protocolVersion, Names.PerformInteraction);
			addToFunctionsAdapter(adapter, protocolVersion, Names.EncodedSyncPData);
			addToFunctionsAdapter(adapter, protocolVersion, Names.Slider);
			addToFunctionsAdapter(adapter, protocolVersion, Names.ScrollableMessage);
			addToFunctionsAdapter(adapter, protocolVersion, Names.ChangeRegistration);
			addToFunctionsAdapter(adapter, protocolVersion, Names.PutFile);
			addToFunctionsAdapter(adapter, protocolVersion, Names.DeleteFile);
			addToFunctionsAdapter(adapter, protocolVersion, Names.ListFiles);
			addToFunctionsAdapter(adapter, protocolVersion, Names.SetAppIcon);
			addToFunctionsAdapter(adapter, protocolVersion, Names.PerformAudioPassThru);
			addToFunctionsAdapter(adapter, protocolVersion, Names.EndAudioPassThru);
			addToFunctionsAdapter(adapter, protocolVersion, VehicleDataSubscriptions);
			addToFunctionsAdapter(adapter, protocolVersion, Names.GetVehicleData);
			addToFunctionsAdapter(adapter, protocolVersion, Names.ReadDID);
			addToFunctionsAdapter(adapter, protocolVersion, Names.GetDTCs);
			addToFunctionsAdapter(adapter, protocolVersion, Names.ShowConstantTBT);
			addToFunctionsAdapter(adapter, protocolVersion, Names.AlertManeuver);
			addToFunctionsAdapter(adapter, protocolVersion, Names.UpdateTurnList);
			addToFunctionsAdapter(adapter, protocolVersion, Names.DialNumber);
			addToFunctionsAdapter(adapter, protocolVersion, GenericRequest);
			
			adapter.sort(new Comparator<String>() {
				@Override
				public int compare(String lhs, String rhs) {
					// compare based on the number of selections so far
					Integer lCount = messageSelectCount.get(lhs);
					if (lCount == null) {
						lCount = 0;
					}
					Integer rCount = messageSelectCount.get(rhs);
					if (rCount == null) {
						rCount = 0;
					}
					return rCount - lCount;
				}
			});
			
			new AlertDialog.Builder(this)  
		       .setTitle("Pick a Function (v" + protocolVersion + ")")
		       .setAdapter(adapter, new DialogInterface.OnClickListener() {
					final boolean v2Features = getCurrentProtocolVersion() >= 2;

					public void onClick(DialogInterface dialog, int which) {
						if(adapter.getItem(which) == Names.Alert){
							AlertDialog.Builder builder;
							AlertDialog dlg;

							final Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.alert, null);
							final EditText txtSpeak = (EditText) layout.findViewById(R.id.txtSpeak);
							final EditText txtAlertField1 = (EditText) layout.findViewById(R.id.txtAlertField1);
							final EditText txtAlertField2 = (EditText) layout.findViewById(R.id.txtAlertField2);
							final EditText txtAlertField3 = (EditText) layout.findViewById(R.id.txtAlertField3);
							final EditText txtDuration = (EditText) layout.findViewById(R.id.txtDuration);
							final CheckBox chkPlayTone = (CheckBox) layout.findViewById(R.id.chkPlayTone);
							
							if (v2Features) {
								chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);
	
								SoftButton sb1 = new SoftButton();
								sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
								sb1.setText("ReRoute");
								sb1.setType(SoftButtonType.SBT_TEXT);
								sb1.setIsHighlighted(false);
								sb1.setSystemAction(SystemAction.STEAL_FOCUS);
								SoftButton sb2 = new SoftButton();
								sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
								sb2.setText("Close");
								sb2.setType(SoftButtonType.SBT_TEXT);
								sb2.setIsHighlighted(false);
								sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
								currentSoftButtons = new Vector<SoftButton>();
								currentSoftButtons.add(sb1);
								currentSoftButtons.add(sb2);
	
								Button btnSoftButtons = (Button) layout.findViewById(R.id.alert_btnSoftButtons);
								btnSoftButtons.setOnClickListener(new OnClickListener() {
									@Override
									public void onClick(View v) {
										IntentHelper.addObjectForKey(currentSoftButtons,
												Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
										Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
										intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER,
												ALERT_MAXSOFTBUTTONS);
										startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
									}
								});
							} else {
								int visibility = android.view.View.GONE;
								View[] views = { txtAlertField3,
										layout.findViewById(R.id.alert_line3),
										layout.findViewById(R.id.alert_softButtonsLayout) };
								for (View view : views) {
									view.setVisibility(visibility);
								}
							}
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									String toSpeak = txtSpeak.getText().toString();
									try {
										Alert msg = new Alert();
										msg.setCorrelationID(autoIncCorrId++);
										msg.setAlertText1(txtAlertField1.getText().toString());
										msg.setAlertText2(txtAlertField2.getText().toString());
										if (v2Features) {
											msg.setAlertText3(txtAlertField3.getText().toString());
										}
										msg.setDuration(Integer.parseInt(txtDuration.getText().toString()));
										msg.setPlayTone(chkPlayTone.isChecked());
										if (toSpeak.length() > 0) {
											Vector<TTSChunk> ttsChunks = TTSChunkFactory.createSimpleTTSChunks(toSpeak);
											msg.setTtsChunks(ttsChunks);
										}
										if (v2Features &&
												chkIncludeSoftButtons.isChecked() &&
												(currentSoftButtons != null) &&
												(currentSoftButtons.size() > 0)) {
											msg.setSoftButtons(currentSoftButtons);
										}
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (NumberFormatException e) {
										Toast.makeText(mContext, "Couldn't parse number", Toast.LENGTH_LONG).show();
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									currentSoftButtons = null;
									chkIncludeSoftButtons = null;
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									currentSoftButtons = null;
									chkIncludeSoftButtons = null;
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();	
						} else if (adapter.getItem(which) == Names.Speak) {
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
							
							ArrayAdapter<SpeechCapabilities> speechSpinnerAdapter = new ArrayAdapter<SpeechCapabilities>(
									adapter.getContext(), android.R.layout.simple_spinner_item, SpeechCapabilities.values());
							int textCapabilityPos = speechSpinnerAdapter.getPosition(SpeechCapabilities.TEXT);
							spnSpeakType1.setAdapter(speechSpinnerAdapter);
							spnSpeakType1.setSelection(textCapabilityPos);
							spnSpeakType2.setAdapter(speechSpinnerAdapter);
							spnSpeakType2.setSelection(textCapabilityPos);
							spnSpeakType3.setAdapter(speechSpinnerAdapter);
							spnSpeakType3.setSelection(textCapabilityPos);
							spnSpeakType4.setAdapter(speechSpinnerAdapter);
							spnSpeakType4.setSelection(textCapabilityPos);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Speak msg = new Speak();
									msg.setCorrelationID(autoIncCorrId++);
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
									msg.setTtsChunks(chunks);
									try {
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
						} else if (adapter.getItem(which) == Names.Show) {
							sendShow();
						} else if (adapter.getItem(which) == ButtonSubscriptions) {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_buttonAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
								boolean needToSubscribe = !isButtonSubscribed[which];
									try {
										ButtonName buttonName = ButtonName.values()[which];
										int corrId = autoIncCorrId++;
										if (needToSubscribe) {
											SubscribeButton msg = new SubscribeButton();
											msg.setCorrelationID(corrId);
											msg.setButtonName(buttonName);
											_msgAdapter.logMessage(msg, true);
											ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
										} else {
											UnsubscribeButton msg = new UnsubscribeButton();
											msg.setCorrelationID(corrId);
											msg.setButtonName(buttonName);
											_msgAdapter.logMessage(msg, true);
											ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
										}
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									isButtonSubscribed[which] = !isButtonSubscribed[which];
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.AddCommand) {
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
							final CheckBox chkUseIcon = (CheckBox) layout.findViewById(R.id.addcommand_useIcon);
							final EditText editIconValue = (EditText) layout.findViewById(R.id.addcommand_iconValue);
							final Spinner spnIconType = (Spinner) layout.findViewById(R.id.addcommand_iconType);
							
							if (!v2Features) {
								int visibility = android.view.View.GONE;
								View[] views = { chkUseIcon, editIconValue, spnIconType };
								for (View view : views) {
									view.setVisibility(visibility);
								}
							}
							
							ArrayAdapter<ImageType> imageTypeAdapter = new ArrayAdapter<ImageType>(
									mContext, android.R.layout.simple_spinner_item, ImageType.values());
							imageTypeAdapter	.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnIconType.setAdapter(imageTypeAdapter);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									AddCommand msg = new AddCommand();
									msg.setCorrelationID(autoIncCorrId++);
									String itemText = er.getText().toString();
									SyncSubMenu sm = new SyncSubMenu();
									sm = (SyncSubMenu) s.getSelectedItem();
									MenuParams menuParams = new MenuParams();
									menuParams.setMenuName(itemText);
									menuParams.setPosition(0);
									menuParams.setParentID(sm.getSubMenuId());
									msg.setMenuParams(menuParams);
									
									String vrSynonym = editVrSynonym.getText().toString();
									if (vrSynonym.length() > 0) {
										Vector<String> vrCommands = new Vector<String>();
										vrCommands.add(vrSynonym);
										msg.setVrCommands(vrCommands);
									}
									
									if (v2Features && chkUseIcon.isChecked()) {
										Image icon = new Image();
										icon.setValue(editIconValue.getText().toString());
										icon.setImageType((ImageType) spnIconType.getSelectedItem());
										msg.setCmdIcon(icon);
									}
									
									int cmdID = itemcmdID++;
									msg.setCmdID(cmdID);
									
									try {
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									addCommandToList(cmdID, menuParams.getParentID());
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
						} else if (adapter.getItem(which) == Names.DeleteCommand) {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_commandAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									DeleteCommand msg = new DeleteCommand();
									msg.setCorrelationID(autoIncCorrId++);
									int cmdID = _commandAdapter.getItem(which);
									msg.setCmdID(cmdID);
									try {
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									removeCommandFromList(cmdID);
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.AddSubMenu) {
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
									AddSubMenu msg = new AddSubMenu();
									msg.setCorrelationID(autoIncCorrId++);
									SyncSubMenu sm = new SyncSubMenu();
									sm.setName(subMenu.getText().toString());
									sm.setSubMenuId(submenucmdID++);
									msg.setMenuID(sm.getSubMenuId());
									msg.setMenuName(sm.getName());
									msg.setPosition(null);
									try {
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
									
									if (_latestAddSubmenu != null) {
										Log.w(logTag, "Latest addSubmenu should be null, but equals to " + _latestAddSubmenu);
									}
									_latestAddSubmenu = sm;
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
						} else if (adapter.getItem(which) == Names.DeleteSubMenu) {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_submenuAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									SyncSubMenu menu = _submenuAdapter.getItem(which);
									if (menu.getSubMenuId() != 0) {
										DeleteSubMenu msg = new DeleteSubMenu();
										msg.setCorrelationID(autoIncCorrId++);
										msg.setMenuID(menu.getSubMenuId());
										try {
											_msgAdapter.logMessage(msg, true);
											ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
										} catch (SyncException e) {
											_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
										}

										if (_latestDeleteSubmenu != null) {
											Log.w(logTag, "Latest deleteSubmenu should be null, but equals to " + _latestDeleteSubmenu);
										}
										_latestDeleteSubmenu = menu;
									} else {
										Toast.makeText(getApplicationContext(),
												"Sorry, can't delete top-level menu",
												Toast.LENGTH_LONG).show();
									}
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.SetGlobalProperties) {
							sendSetGlobalProperties();
						} else if (adapter.getItem(which) == Names.ResetGlobalProperties) {
							sendResetGlobalProperties();
						} else if (adapter.getItem(which) == Names.SetMediaClockTimer) {
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
							
							List<UpdateMode> updateModes = Arrays.asList(UpdateMode.values());
							if (!v2Features) {
								// CLEAR UpdateMode appeared in protocol v2
								updateModes = new ArrayList<UpdateMode>(updateModes);
								updateModes.remove(UpdateMode.CLEAR);
							}
							
							ArrayAdapter<UpdateMode> spinnerAdapter = new ArrayAdapter<UpdateMode>(adapter.getContext(),
									android.R.layout.simple_spinner_item, updateModes);
							spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
							spnUpdateMode.setAdapter(spinnerAdapter);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									SetMediaClockTimer msg = new SetMediaClockTimer();
									msg.setCorrelationID(autoIncCorrId++);
									UpdateMode updateMode =  (UpdateMode)spnUpdateMode.getSelectedItem();
									msg.setUpdateMode(updateMode);
									try {
										Integer hours = Integer.parseInt(txtHours.getText().toString());
										Integer minutes = Integer.parseInt(txtMinutes.getText().toString());
										Integer seconds = Integer.parseInt(txtSeconds.getText().toString());
										StartTime startTime = new StartTime();
										startTime.setHours(hours);
										startTime.setMinutes(minutes);
										startTime.setSeconds(seconds);
										msg.setStartTime(startTime);
									} catch (NumberFormatException e) {
										// skip setting start time if parsing failed
									}
									
									try {
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
						} else if (adapter.getItem(which) == Names.CreateInteractionChoiceSet) {
							sendCreateInteractionChoiceSet();
						} else if (adapter.getItem(which) == Names.DeleteInteractionChoiceSet) {
							//something
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_choiceSetAdapter, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									DeleteInteractionChoiceSet msg = new DeleteInteractionChoiceSet();
									msg.setCorrelationID(autoIncCorrId++);
									int commandSetID = _choiceSetAdapter.getItem(which);
									msg.setInteractionChoiceSetID(commandSetID);
									try {
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
										if (_latestDeleteChoiceSetId != CHOICESETID_UNSET) {
											Log.w(logTag, "Latest deleteChoiceSetId should be unset, but equals to " + _latestDeleteChoiceSetId);
										}
										_latestDeleteChoiceSetId = commandSetID;
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.PerformInteraction) {
							sendPerformInteraction();
						} else if (adapter.getItem(which) == Names.EncodedSyncPData) {
							sendEncodedSyncPData();
						} else if (adapter.getItem(which) == Names.Slider) {
							sendSlider();
						} else if (adapter.getItem(which) == Names.ScrollableMessage) {
							//something
							AlertDialog.Builder builder;
							AlertDialog dlg;

							final Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.scrollablemessage, null);
							final EditText txtScrollableMessageBody = (EditText) layout.findViewById(R.id.txtScrollableMessageBody);
							chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);
							final EditText txtTimeout = (EditText) layout.findViewById(R.id.scrollablemessage_editTimeout);

							SoftButton sb1 = new SoftButton();
							sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
							sb1.setText("Reply");
							sb1.setType(SoftButtonType.SBT_TEXT);
							sb1.setIsHighlighted(false);
							sb1.setSystemAction(SystemAction.STEAL_FOCUS);
							SoftButton sb2 = new SoftButton();
							sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
							sb2.setText("Close");
							sb2.setType(SoftButtonType.SBT_TEXT);
							sb2.setIsHighlighted(false);
							sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
							currentSoftButtons = new Vector<SoftButton>();
							currentSoftButtons.add(sb1);
							currentSoftButtons.add(sb2);

							Button btnSoftButtons = (Button) layout.findViewById(R.id.scrollablemessage_btnSoftButtons);
							btnSoftButtons.setOnClickListener(new OnClickListener() {
								@Override
								public void onClick(View v) {
									IntentHelper.addObjectForKey(currentSoftButtons,
											Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
									Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
									intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER,
											SCROLLABLEMESSAGE_MAXSOFTBUTTONS);
									startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
								}
							});
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										ScrollableMessage msg = new ScrollableMessage();
										msg.setCorrelationID(autoIncCorrId++);
										try {
											msg.setTimeout(Integer.parseInt(txtTimeout.getText().toString()));
										} catch (NumberFormatException e) {
											// do nothing, leave the default timeout
										}
										msg.setScrollableMessageBody(txtScrollableMessageBody.getEditableText().toString());
										if (chkIncludeSoftButtons.isChecked() &&
												(currentSoftButtons != null) &&
												(currentSoftButtons.size() > 0)) {
											msg.setSoftButtons(currentSoftButtons);
										}
										currentSoftButtons = null;
										chkIncludeSoftButtons = null;
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									currentSoftButtons = null;
									chkIncludeSoftButtons = null;
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.ChangeRegistration) {
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
						} else if (adapter.getItem(which) == Names.PutFile) {
							//PutFile
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							final Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.putfile, null);
							
							txtLocalFileName = (EditText) layout.findViewById(R.id.putfile_localFileName);
							final Button btnSelectLocalFile = (Button) layout.findViewById(R.id.putfile_selectFileButton);
							btnSelectLocalFile.setOnClickListener(new OnClickListener() {
								@Override
								public void onClick(View v) {
									// show Choose File dialog
									Intent intent = new Intent(mContext, FileDialog.class);
									intent.putExtra(FileDialog.START_PATH, "/sdcard");
									intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
									intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
									startActivityForResult(intent, REQUEST_FILE_OPEN);
								}
							});
							
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
									String filename = txtLocalFileName.getText().toString();
									long filesize = new File(filename).length();
									if (filesize <= PUTFILE_MAXFILESIZE) {
										byte[] data = contentsOfFile(filename);
										if (data != null) {
											String syncFileName = txtSyncFileName.getText().toString();
											try {
												PutFile msg = new PutFile();
												msg.setSyncFileName(syncFileName);
												msg.setFileType((FileType) spnFileType.getSelectedItem());
												msg.setPersistentFile(chkPersistentFile.isChecked());
												msg.setCorrelationID(autoIncCorrId++);
												msg.setBulkData(data);
												
												_msgAdapter.logMessage(msg, true);
												ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
											} catch (SyncException e) {
												_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
											}
											_putFileAdapter.add(syncFileName);
										} else {
											Toast.makeText(mContext, "Can't read data from file", Toast.LENGTH_LONG).show();
										}
									} else {
										Toast.makeText(mContext, "The size of the file exceeds the limit of " +
												(PUTFILE_MAXFILESIZE / (1024 * 1024)) + " MB", Toast.LENGTH_LONG).show();
									}
									txtLocalFileName = null;
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									txtLocalFileName = null;
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.DeleteFile) {
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
						} else if (adapter.getItem(which) == Names.ListFiles) {
							//ListFiles
							try {
								ListFiles msg = new ListFiles();
								msg.setCorrelationID(autoIncCorrId++);
								_msgAdapter.logMessage(msg, true);
								ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
						} else if (adapter.getItem(which) == Names.SetAppIcon) {
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
						} else if (adapter.getItem(which) == Names.PerformAudioPassThru) {
							//PerformAudioPassThru
							AlertDialog.Builder builder;
							AlertDialog dlg;

							final Context mContext = adapter.getContext();
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
							ArrayAdapter<BitsPerSample> spinnerAdapterBitsPerSample = new ArrayAdapter<BitsPerSample>(adapter.getContext(),
									android.R.layout.simple_spinner_item, BitsPerSample.values());
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
										msg.setBitsPerSample((BitsPerSample) spnBitsPerSample.getSelectedItem());
										msg.setAudioType((AudioType) spnAudioType.getSelectedItem());
										msg.setCorrelationID(autoIncCorrId++);
										latestPerformAudioPassThruMsg = msg;
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (NumberFormatException e) {
										Toast.makeText(mContext, "Couldn't parse number", Toast.LENGTH_LONG).show();
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
						} else if (adapter.getItem(which) == Names.EndAudioPassThru) {
							//EndAudioPassThru
							try {
								EndAudioPassThru msg = new EndAudioPassThru();
								msg.setCorrelationID(autoIncCorrId++);
								_msgAdapter.logMessage(msg, true);
								ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
							} catch (SyncException e) {
								_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
							}
						} else if (adapter.getItem(which) == VehicleDataSubscriptions) {
							sendVehicleDataSubscriptions();
						} else if (adapter.getItem(which) == Names.GetVehicleData) {
							//GetVehicleData
							AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
							builder.setAdapter(_vehicleDataType, new DialogInterface.OnClickListener() {

								public void onClick(DialogInterface dialog, int which) {
									try {
										GetVehicleData msg = new GetVehicleData();
										msg.setDataType(VehicleDataType.values()[which]);
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
							});
							AlertDialog dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.ReadDID) {
							//ReadDID
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							final Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.readdid, null);
							
							final EditText txtECUNameDID = (EditText) layout.findViewById(R.id.txtECUNameDID);
							final EditText txtDIDLocation = (EditText) layout.findViewById(R.id.txtDIDLocation);
							final CheckBox chkEncryptedDID = (CheckBox) layout.findViewById(R.id.chkEncryptedDID);
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									try {
										Vector<Integer> didlocations = new Vector<Integer>();
										didlocations.add(Integer.parseInt(txtDIDLocation.getText().toString()));
										
										ReadDID msg = new ReadDID();
										msg.setEcuName(Integer.parseInt(txtECUNameDID.getText().toString()));
										msg.setDidLocation(didlocations);
										msg.setEncrypted(chkEncryptedDID.isChecked());
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (NumberFormatException e) {
										Toast.makeText(mContext, "Couldn't parse number", Toast.LENGTH_LONG).show();
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
						} else if (adapter.getItem(which) == Names.GetDTCs) { 
							//GetDTCs
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							final Context mContext = adapter.getContext();
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
									} catch (NumberFormatException e) {
										Toast.makeText(mContext, "Couldn't parse number", Toast.LENGTH_LONG).show();
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
						} else if (adapter.getItem(which) == Names.ShowConstantTBT) {
							sendShowConstantTBT();
						} else if (adapter.getItem(which) == Names.AlertManeuver) {
							//AlertManeuver
							AlertDialog.Builder builder;
							AlertDialog dlg;
							
							final Context mContext = adapter.getContext();
							LayoutInflater inflater = (LayoutInflater) mContext
									.getSystemService(LAYOUT_INFLATER_SERVICE);
							View layout = inflater.inflate(R.layout.alertmaneuver, null);

							final EditText txtTtsChunks = (EditText) layout.findViewById(R.id.txtTtsChunks);
							
							SoftButton sb1 = new SoftButton();
							sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
							sb1.setText("Reply");
							sb1.setType(SoftButtonType.SBT_TEXT);
							sb1.setIsHighlighted(false);
							sb1.setSystemAction(SystemAction.STEAL_FOCUS);
							SoftButton sb2 = new SoftButton();
							sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
							sb2.setText("Close");
							sb2.setType(SoftButtonType.SBT_TEXT);
							sb2.setIsHighlighted(false);
							sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
							currentSoftButtons = new Vector<SoftButton>();
							currentSoftButtons.add(sb1);
							currentSoftButtons.add(sb2);
							
							Button btnSoftButtons = (Button) layout.findViewById(R.id.alertManeuver_btnSoftButtons);
							btnSoftButtons.setOnClickListener(new OnClickListener() {
								@Override
								public void onClick(View v) {
									IntentHelper.addObjectForKey(currentSoftButtons,
											Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
									Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
									intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER,
											ALERTMANEUVER_MAXSOFTBUTTONS);
									startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
								}
							});
							
							builder = new AlertDialog.Builder(mContext);
							builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									Vector<TTSChunk> ttsChunks = new Vector<TTSChunk>();
									// string to join/split ttsChunks string
									final String joinString = ",";
									
									String ttsChunksString = txtTtsChunks.getText().toString();
									for (String ttsChunk : ttsChunksString.split(joinString)) {
										TTSChunk chunk = TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, ttsChunk);
										ttsChunks.add(chunk);
									}

									if (!ttsChunks.isEmpty()) {
										try {
											AlertManeuver msg = new AlertManeuver();
											msg.setTtsChunks(ttsChunks);
											msg.setCorrelationID(autoIncCorrId++);
											if (currentSoftButtons != null) {
												msg.setSoftButtons(currentSoftButtons);
											} else {
												msg.setSoftButtons(new Vector<SoftButton>());
											}
											currentSoftButtons = null;
											_msgAdapter.logMessage(msg, true);
											ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
										} catch (SyncException e) {
											_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
										}
									} else {
										Toast.makeText(mContext, "No TTS Chunks entered", Toast.LENGTH_SHORT).show();
									}
								}
							});
							builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
								public void onClick(DialogInterface dialog, int id) {
									currentSoftButtons = null;
									dialog.cancel();
								}
							});
							builder.setView(layout);
							dlg = builder.create();
							dlg.show();
						} else if (adapter.getItem(which) == Names.UpdateTurnList) {
							sendUpdateTurnList();
						} else if (adapter.getItem(which) == Names.DialNumber) {
							sendDialNumber();
						} else if (adapter.getItem(which) == GenericRequest) {
							sendGenericRequest();
						}
						
						String function = adapter.getItem(which);
						Integer curCount = messageSelectCount.get(function);
						if (curCount == null) {
							curCount = 0;
						}
						messageSelectCount.put(function, curCount + 1);
					}

                   /**
                    * Opens the dialog for EncodedSyncPData message and sends it.
                    */
                   private void sendEncodedSyncPData() {
                       final Context mContext = adapter.getContext();
                       LayoutInflater inflater = (LayoutInflater) mContext
                               .getSystemService(LAYOUT_INFLATER_SERVICE);
                       View layout = inflater.inflate(R.layout.encodedsyncpdata, null);

                       txtLocalFileName = (EditText) layout.findViewById(R.id.encodedsyncpdata_localFileName);
                       final Button btnSelectLocalFile = (Button) layout.findViewById(R.id.encodedsyncpdata_selectFileButton);
                       btnSelectLocalFile.setOnClickListener(new OnClickListener() {
                           @Override
                           public void onClick(View v) {
                               // show Choose File dialog
                               Intent intent = new Intent(mContext, FileDialog.class);
                               intent.putExtra(FileDialog.START_PATH, "/sdcard");
                               intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
                               intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
                               startActivityForResult(intent, REQUEST_FILE_OPEN);
                           }
                       });

                       AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                       builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                           @Override
                           public void onClick(DialogInterface dialog, int id) {
                               String filename = txtLocalFileName.getText().toString();
                               byte[] data = contentsOfFile(filename);
                               if (data != null) {
                                   String base64Data = Base64.encode(data);
                                   EncodedSyncPData msg = new EncodedSyncPData();
                                   Vector<String> syncPData = new Vector<String>();
                                   syncPData.add(base64Data);
                                   msg.setData(syncPData);
                                   msg.setCorrelationID(autoIncCorrId++);

                                   _msgAdapter.logMessage(msg, true);

                                   try {
                                       ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
                                   } catch (SyncException e) {
                                       _msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
                                   }
                               } else {
                                   Toast.makeText(mContext, "Can't read data from file", Toast.LENGTH_LONG).show();
                               }
                               txtLocalFileName = null;
                           }
                       });
                       builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                           public void onClick(DialogInterface dialog, int id) {
                               txtLocalFileName = null;
                               dialog.cancel();
                           }
                       });
                       builder.setView(layout);
                       builder.show();
                   }

					/**
					 * Opens the dialog for ShowConstantTBT message and sends it.
					 */
					private void sendShowConstantTBT() {
						final Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.showconstanttbt,
								(ViewGroup) findViewById(R.id.showconstanttbt_Root));
						
						final EditText txtNavigationText1 = (EditText) layout.findViewById(R.id.showconstanttbt_txtNavigationText1);
						final EditText txtNavigationText2 = (EditText) layout.findViewById(R.id.showconstanttbt_txtNavigationText2);
						final EditText txtEta = (EditText) layout.findViewById(R.id.showconstanttbt_txtEta);
						final EditText txtTotalDistance = (EditText) layout.findViewById(R.id.showconstanttbt_txtTotalDistance);
						final CheckBox chkUseTurnIcon = (CheckBox) layout.findViewById(R.id.showconstanttbt_turnIconCheck);
						final Spinner spnTurnIconType = (Spinner) layout.findViewById(R.id.showconstanttbt_turnIconType);
						final EditText txtTurnIconValue = (EditText) layout.findViewById(R.id.showconstanttbt_turnIconValue);
						final EditText txtDistanceToManeuver = (EditText) layout.findViewById(R.id.showconstanttbt_txtDistanceToManeuver);
						final EditText txtDistanceToManeuverScale = (EditText) layout.findViewById(R.id.showconstanttbt_txtDistanceToManeuverScale);
						final CheckBox chkManeuverComplete = (CheckBox) layout.findViewById(R.id.showconstanttbt_chkManeuverComplete);
						
						final ArrayAdapter<ImageType> imageTypeAdapter = new ArrayAdapter<ImageType>(
								mContext, android.R.layout.simple_spinner_item, ImageType.values());
						imageTypeAdapter	.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
						
						if (v2Features) {
							spnTurnIconType.setAdapter(imageTypeAdapter);
						} else {
							int visibility = android.view.View.GONE;
							View[] views = { chkUseTurnIcon, spnTurnIconType, txtTurnIconValue };
							for (View view : views) {
								view.setVisibility(visibility);
							}
						}

						SoftButton sb1 = new SoftButton();
						sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
						sb1.setText("Reply");
						sb1.setType(SoftButtonType.SBT_TEXT);
						sb1.setIsHighlighted(false);
						sb1.setSystemAction(SystemAction.STEAL_FOCUS);
						SoftButton sb2 = new SoftButton();
						sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
						sb2.setText("Close");
						sb2.setType(SoftButtonType.SBT_TEXT);
						sb2.setIsHighlighted(false);
						sb2.setSystemAction(SystemAction.DEFAULT_ACTION);
						currentSoftButtons = new Vector<SoftButton>();
						currentSoftButtons.add(sb1);
						currentSoftButtons.add(sb2);

						Button btnSoftButtons = (Button) layout.findViewById(R.id.showconstanttbt_btnSoftButtons);
						btnSoftButtons.setOnClickListener(new OnClickListener() {
							@Override
							public void onClick(View v) {
								IntentHelper.addObjectForKey(currentSoftButtons,
										Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
								Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
								intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER,
										SHOWCONSTANTTBT_MAXSOFTBUTTONS);
								startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
							}
						});
						
						AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								try {
									ShowConstantTBT msg = new ShowConstantTBT();
									msg.setNavigationText1(txtNavigationText1.getText().toString());
									msg.setNavigationText2(txtNavigationText2.getText().toString());
									msg.setEta(txtEta.getText().toString());
									msg.setTotalDistance(txtTotalDistance.getText().toString());
									
									if (v2Features && chkUseTurnIcon.isChecked()) {
										Image image = new Image();
										image.setImageType(imageTypeAdapter.getItem(
												spnTurnIconType.getSelectedItemPosition()));
										image.setValue(txtTurnIconValue.getText().toString());
										msg.setTurnIcon(image);
									}

									msg.setDistanceToManeuver((float) Integer.parseInt(txtDistanceToManeuver.getText().toString()));
									msg.setDistanceToManeuverScale((float) Integer.parseInt(txtDistanceToManeuverScale.getText().toString()));
									msg.setManeuverComplete(chkManeuverComplete.isChecked());
									msg.setCorrelationID(autoIncCorrId++);
									if (currentSoftButtons != null) {
										msg.setSoftButtons(currentSoftButtons);
									} else {
										msg.setSoftButtons(new Vector<SoftButton>());
									}
									_msgAdapter.logMessage(msg, true);
									ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
								} catch (NumberFormatException e) {
									Toast.makeText(mContext, "Couldn't parse number", Toast.LENGTH_LONG).show();
								} catch (SyncException e) {
									_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
								}
								currentSoftButtons = null;
							}
						});
						builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								currentSoftButtons = null;
								dialog.cancel();
							}
						});
						builder.setView(layout);
						builder.show();
					}

					/**
					 * Opens the dialog for CreateInteractionChoiceSet message and sends it.
					 */
					private void sendCreateInteractionChoiceSet() {
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
						final CheckBox image1Check = (CheckBox) layout.findViewById(R.id.createinteractionchoiceset_image1Check);
						final CheckBox image2Check = (CheckBox) layout.findViewById(R.id.createinteractionchoiceset_image2Check);
						final CheckBox image3Check = (CheckBox) layout.findViewById(R.id.createinteractionchoiceset_image3Check);
						final Spinner image1Type = (Spinner) layout.findViewById(R.id.createinteractionchoiceset_image1Type);
						final Spinner image2Type = (Spinner) layout.findViewById(R.id.createinteractionchoiceset_image2Type);
						final Spinner image3Type = (Spinner) layout.findViewById(R.id.createinteractionchoiceset_image3Type);
						final EditText image1Value = (EditText) layout.findViewById(R.id.createinteractionchoiceset_image1Value);
						final EditText image2Value = (EditText) layout.findViewById(R.id.createinteractionchoiceset_image2Value);
						final EditText image3Value = (EditText) layout.findViewById(R.id.createinteractionchoiceset_image3Value);
						
						final ArrayAdapter<ImageType> imageTypeAdapter = new ArrayAdapter<ImageType>(
								mContext, android.R.layout.simple_spinner_item, ImageType.values());
						imageTypeAdapter	.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
						
						if (v2Features) {
							image1Type.setAdapter(imageTypeAdapter);
							image2Type.setAdapter(imageTypeAdapter);
							image3Type.setAdapter(imageTypeAdapter);
						} else {
							int visibility = android.view.View.GONE;
							View[] views = { image1Check, image2Check, image3Check,
									image1Type, image2Type, image3Type,
									image1Value, image2Value, image3Value };
							for (View view : views) {
								view.setVisibility(visibility);
							}
						}

						AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								Vector<Choice> commands = new Vector<Choice>();
								
								if (choice1.isChecked()) {
									Choice one = new Choice();
									one.setChoiceID(autoIncChoiceSetIdCmdId++);
									one.setMenuName(command1.getText().toString());
									one.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command1.getText().toString(),
											vr1.getText().toString() })));
									if (v2Features && image1Check.isChecked()) {
										Image image = new Image();
										image.setImageType(imageTypeAdapter.getItem(image1Type.getSelectedItemPosition()));
										image.setValue(image1Value.getText().toString());
										one.setImage(image);
									}
									commands.add(one);
								}
								
								if (choice2.isChecked()) {
									Choice two = new Choice();
									two.setChoiceID(autoIncChoiceSetIdCmdId++);
									two.setMenuName(command2.getText().toString());
									two.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command2.getText().toString(),
											vr2.getText().toString() })));
									if (v2Features && image2Check.isChecked()) {
										Image image = new Image();
										image.setImageType(imageTypeAdapter.getItem(image2Type.getSelectedItemPosition()));
										image.setValue(image2Value.getText().toString());
										two.setImage(image);
									}
									commands.add(two);
								}
								
								if (choice3.isChecked()) {
									Choice three = new Choice();
									three.setChoiceID(autoIncChoiceSetIdCmdId++);
									three.setMenuName(command3.getText().toString());
									three.setVrCommands(new Vector<String>(Arrays.asList(new String[] { command3.getText().toString(),
											vr3.getText().toString() })));
									if (v2Features && image3Check.isChecked()) {
										Image image = new Image();
										image.setImageType(imageTypeAdapter.getItem(image3Type.getSelectedItemPosition()));
										image.setValue(image3Value.getText().toString());
										three.setImage(image);
									}
									commands.add(three);
								}
								
								if (!commands.isEmpty()) {
									CreateInteractionChoiceSet msg = new CreateInteractionChoiceSet();
									msg.setCorrelationID(autoIncCorrId++);
									int choiceSetID = autoIncChoiceSetId++;
									msg.setInteractionChoiceSetID(choiceSetID);
									msg.setChoiceSet(commands);
									try {
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
										if (_latestCreateChoiceSetId != CHOICESETID_UNSET) {
											Log.w(logTag, "Latest createChoiceSetId should be unset, but equals to " + _latestCreateChoiceSetId);
										}
										_latestCreateChoiceSetId = choiceSetID;
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
						builder.show();
					}

					/**
					 * Opens the dialog for Show message and sends it.
					 */
					private void sendShow() {
						final Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.show, null);
						
						final CheckBox mainField1Check = (CheckBox) layout.findViewById(R.id.show_mainField1Check);
						final EditText mainField1 = (EditText) layout.findViewById(R.id.show_mainField1);
						final CheckBox mainField2Check = (CheckBox) layout.findViewById(R.id.show_mainField2Check);
						final EditText mainField2 = (EditText) layout.findViewById(R.id.show_mainField2);
						final CheckBox mainField3Check = (CheckBox) layout.findViewById(R.id.show_mainField3Check);
						final EditText mainField3 = (EditText) layout.findViewById(R.id.show_mainField3);
						final CheckBox mainField4Check = (CheckBox) layout.findViewById(R.id.show_mainField4Check);
						final EditText mainField4 = (EditText) layout.findViewById(R.id.show_mainField4);
						final CheckBox textAlignmentCheck = (CheckBox) layout.findViewById(R.id.show_textAlignmentCheck);
						final Spinner textAlignmentSpinner = (Spinner) layout.findViewById(R.id.show_textAlignmentSpinner);
						final CheckBox statusBarCheck = (CheckBox) layout.findViewById(R.id.show_statusBarCheck);
						final EditText statusBar = (EditText) layout.findViewById(R.id.show_statusBar);
						final CheckBox mediaClockCheck = (CheckBox) layout.findViewById(R.id.show_mediaClockCheck);
						final EditText mediaClock = (EditText) layout.findViewById(R.id.show_mediaClock);
						final CheckBox mediaTrackCheck = (CheckBox) layout.findViewById(R.id.show_mediaTrackCheck);
						final EditText mediaTrack = (EditText) layout.findViewById(R.id.show_mediaTrack);
						final CheckBox graphicCheck = (CheckBox) layout.findViewById(R.id.show_graphicCheck);
						final EditText graphic = (EditText) layout.findViewById(R.id.show_graphic);
						chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.show_chkIncludeSBs);
						final Button softButtons = (Button) layout.findViewById(R.id.show_btnSoftButtons);
						final CheckBox customPresetsCheck = (CheckBox) layout.findViewById(R.id.show_customPresetsCheck);
						final EditText customPresets = (EditText) layout.findViewById(R.id.show_customPresets);
						
						final ArrayAdapter<TextAlignment> textAlignmentAdapter = new ArrayAdapter<TextAlignment>(
								mContext, android.R.layout.simple_spinner_item, TextAlignment.values());
						textAlignmentAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
						textAlignmentSpinner.setAdapter(textAlignmentAdapter);
						textAlignmentSpinner.setSelection(textAlignmentAdapter.getPosition(TextAlignment.CENTERED));
						
						final boolean isMedia = getIsMedia();
						
						if (!isMedia) {
							int visibility = android.view.View.GONE;
							mediaClock.setVisibility(visibility);
							mediaTrack.setVisibility(visibility);
							mediaTrackCheck.setVisibility(visibility);
							mediaClockCheck.setVisibility(visibility);
						}
						
						if (!v2Features) {
							int visibility = android.view.View.GONE;
							mainField3Check.setVisibility(visibility);
							mainField3.setVisibility(visibility);
							mainField4Check.setVisibility(visibility);
							mainField4.setVisibility(visibility);
							graphicCheck.setVisibility(visibility);
							graphic.setVisibility(visibility);
							chkIncludeSoftButtons.setVisibility(visibility);
							softButtons.setVisibility(visibility);
							customPresetsCheck.setVisibility(visibility);
							customPresets.setVisibility(visibility);
						} else {
							SoftButton sb1 = new SoftButton();
							sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
							sb1.setText("KeepContext");
							sb1.setType(SoftButtonType.SBT_TEXT);
							sb1.setIsHighlighted(false);
							sb1.setSystemAction(SystemAction.KEEP_CONTEXT);
							SoftButton sb2 = new SoftButton();
							sb2.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
							sb2.setText("StealFocus");
							sb2.setType(SoftButtonType.SBT_TEXT);
							sb2.setIsHighlighted(false);
							sb2.setSystemAction(SystemAction.STEAL_FOCUS);
							SoftButton sb3 = new SoftButton();
							sb3.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
							sb3.setText("Default");
							sb3.setType(SoftButtonType.SBT_TEXT);
							sb3.setIsHighlighted(false);
							sb3.setSystemAction(SystemAction.DEFAULT_ACTION);
							currentSoftButtons = new Vector<SoftButton>();
							currentSoftButtons.add(sb1);
							currentSoftButtons.add(sb2);
							currentSoftButtons.add(sb3);

							Button btnSoftButtons = (Button) layout.findViewById(R.id.show_btnSoftButtons);
							btnSoftButtons.setOnClickListener(new OnClickListener() {
								@Override
								public void onClick(View v) {
									IntentHelper.addObjectForKey(currentSoftButtons,
											Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
									Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
									intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER,
											SHOW_MAXSOFTBUTTONS);
									startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
								}
							});
						}

						AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								try {
									Show msg = new Show();
									msg.setCorrelationID(autoIncCorrId++);
									
									if (mainField1Check.isChecked()) {
										msg.setMainField1(mainField1.getText().toString());
									}
									if (mainField2Check.isChecked()) {
										msg.setMainField2(mainField2.getText().toString());
									}
									if (v2Features && mainField3Check.isChecked()) {
										msg.setMainField3(mainField3.getText().toString());
									}
									if (v2Features && mainField4Check.isChecked()) {
										msg.setMainField4(mainField4.getText().toString());
									}
									if (textAlignmentCheck.isChecked()) {
										msg.setAlignment(textAlignmentAdapter.getItem(textAlignmentSpinner.getSelectedItemPosition()));
									}
									if (statusBarCheck.isChecked()) {
										msg.setStatusBar(statusBar.getText().toString());
									}
									if (isMedia) {
										if (mediaClockCheck.isChecked()) {
											msg.setMediaClock(mediaClock.getText().toString());
										}
										if (mediaTrackCheck.isChecked()) {
											msg.setMediaTrack(mediaTrack.getText().toString());
										}
									}
									if (v2Features && graphicCheck.isChecked()) {
										Image image = new Image();
										image.setImageType(ImageType.STATIC);
										image.setValue(graphic.getText().toString());
										msg.setGraphic(image);
									}
									if (v2Features &&
											chkIncludeSoftButtons.isChecked() &&
											(currentSoftButtons != null) &&
											(currentSoftButtons.size() > 0)) {
										msg.setSoftButtons(currentSoftButtons);
									}
									currentSoftButtons = null;
									chkIncludeSoftButtons = null;
									if (v2Features && customPresetsCheck.isChecked()) {
										String splitter = ",";
										String[] customPresetsList = customPresets.getText().
												toString().split(splitter);
										msg.setCustomPresets(new Vector<String>(Arrays.
												asList(customPresetsList)));
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
								currentSoftButtons = null;
								chkIncludeSoftButtons = null;
								dialog.cancel();
							}
						});
						builder.setView(layout);
						builder.show();
					}

					/**
					 * Opens the dialog for PerformInteraction message and sends it.
					 */
					private void sendPerformInteraction() {
						final Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.performinteraction,
								(ViewGroup) findViewById(R.id.performinteraction_Root));
						
						final EditText initialText = (EditText) layout.findViewById(R.id.performinteraction_initialText);
						final EditText initialPrompt = (EditText) layout.findViewById(R.id.performinteraction_initialPrompt);
						final Spinner interactionModeSpinner = (Spinner) layout.findViewById(R.id.performinteraction_interactionModeSpinner);
						final Button choiceSetIDs = (Button) layout.findViewById(R.id.performinteraction_choiceSetIDs);
						final CheckBox helpPromptCheck = (CheckBox) layout.findViewById(R.id.performinteraction_helpPromptCheck);
						final EditText helpPrompt = (EditText) layout.findViewById(R.id.performinteraction_helpPrompt);
						final CheckBox timeoutPromptCheck = (CheckBox) layout.findViewById(R.id.performinteraction_timeoutPromptCheck);
						final EditText timeoutPrompt = (EditText) layout.findViewById(R.id.performinteraction_timeoutPrompt);
						final CheckBox timeoutCheck = (CheckBox) layout.findViewById(R.id.performinteraction_timeoutCheck);
						final EditText timeout = (EditText) layout.findViewById(R.id.performinteraction_timeout);
						final CheckBox vrHelpItemCheck = (CheckBox) layout.findViewById(R.id.performinteraction_vrHelpItemCheck);
						final EditText vrHelpItemPos = (EditText) layout.findViewById(R.id.performinteraction_vrHelpItemPos);
						final EditText vrHelpItemText = (EditText) layout.findViewById(R.id.performinteraction_vrHelpItemText);
						final EditText vrHelpItemImage = (EditText) layout.findViewById(R.id.performinteraction_vrHelpItemImage);
						
						final ArrayAdapter<InteractionMode> interactionModeAdapter = new ArrayAdapter<InteractionMode>(
								mContext, android.R.layout.simple_spinner_item, InteractionMode.values());
						interactionModeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
						interactionModeSpinner.setAdapter(interactionModeAdapter);
						interactionModeSpinner.setSelection(interactionModeAdapter.getPosition(InteractionMode.BOTH));
						
						final String[] choiceSetIDStrings = new String[_choiceSetAdapter.getCount()];
						final boolean[] choiceSetIDSelections = new boolean[choiceSetIDStrings.length];
						
						for (int i = 0; i < _choiceSetAdapter.getCount(); ++i) {
							choiceSetIDStrings[i] = _choiceSetAdapter.getItem(i).toString();
						}
						
						choiceSetIDs.setOnClickListener(new OnClickListener() {
							@Override
							public void onClick(View v) {
								new AlertDialog.Builder(mContext).
									setMultiChoiceItems(choiceSetIDStrings, choiceSetIDSelections, new OnMultiChoiceClickListener() {
										@Override
										public void onClick(DialogInterface dialog, int which, boolean isChecked) {
										}
									}).
									setPositiveButton("OK", new DialogInterface.OnClickListener() {
										@Override
										public void onClick(DialogInterface dialog, int which) {
											dialog.dismiss();
										}
									}).
									show();
							}
						});
						
						final boolean vrHelpEnabled = getCurrentProtocolVersion() >= 2;
						if (!vrHelpEnabled) {
							// disable VR help title and VR help item
							int visibility = android.view.View.GONE;
							vrHelpItemCheck.setVisibility(visibility);
							vrHelpItemPos.setVisibility(visibility);
							vrHelpItemText.setVisibility(visibility);
							vrHelpItemImage.setVisibility(visibility);
						}
						
						AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							// string to join/split initial and help prompts, and VR helps
							private String joinString = ",";

							@Override
							public void onClick(DialogInterface dialog, int which) {
								// fail if no interaction choice set selected
								Vector<Integer> choiceSetIDs = new Vector<Integer>();
								for (int i = 0; i < choiceSetIDSelections.length; ++i) {
									if (choiceSetIDSelections[i]) {
										choiceSetIDs.add(_choiceSetAdapter.getItem(i));
									}
								}
								
								if (choiceSetIDs.size() > 0) {
									PerformInteraction msg = new PerformInteraction();
									msg.setCorrelationID(autoIncCorrId++);
									msg.setInitialText(initialText.getText().toString());
									msg.setInitialPrompt(ttsChunksFromString(initialPrompt.getText().toString()));
									msg.setInteractionMode(interactionModeAdapter.getItem(interactionModeSpinner.getSelectedItemPosition()));
									msg.setInteractionChoiceSetIDList(choiceSetIDs);
									
									if (helpPromptCheck.isChecked()) {
										msg.setHelpPrompt(ttsChunksFromString(helpPrompt.getText().toString()));
									}
									
									if (timeoutPromptCheck.isChecked()) {
										msg.setTimeoutPrompt(ttsChunksFromString(timeoutPrompt.getText().toString()));
									}
									
									if (timeoutCheck.isChecked()) {
										try {
											msg.setTimeout(Integer.parseInt(timeout.getText().toString()));
										} catch (NumberFormatException e) {
											// set default timeout
											msg.setTimeout(10000);
										}
									}
									
									if (vrHelpEnabled && vrHelpItemCheck.isChecked()) {
										Vector<VrHelpItem> vrHelpItems = new Vector<VrHelpItem>();
										VrHelpItem item = new VrHelpItem();
										item.setText(vrHelpItemText.getText().toString());
										
										try {
											item.setPosition(Integer.parseInt(vrHelpItemPos.getText().toString()));
										} catch (NumberFormatException e) {
											// set default position
											item.setPosition(1);
										}
										
										Image image = new Image();
										image.setValue(vrHelpItemImage.getText().toString());
										image.setImageType(ImageType.STATIC);
										item.setImage(image);
										
										vrHelpItems.add(item);
										msg.setVrHelp(vrHelpItems);
									}
									
									try {
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								} else {
									Toast.makeText(mContext, "No interaction choice set selected", Toast.LENGTH_LONG).show();
								}
							}

							/**
							 * Splits the string with a comma and returns a vector of TTSChunks.
							 */
							private Vector<TTSChunk> ttsChunksFromString(String string) {
								Vector<TTSChunk> chunks = new Vector<TTSChunk>();
								for (String stringChunk : string.split(joinString )) {
									TTSChunk chunk = TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, stringChunk);
									chunks.add(chunk);
								}
								return chunks;
							}
						});
						builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which) {
								dialog.cancel();
							}
						});
						
						builder.setView(layout);
						builder.show();
					}

					private void sendUpdateTurnList() {
						AlertDialog.Builder builder;

						final Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.updateturnlist, null);
						final EditText txtTurnList = (EditText) layout.findViewById(R.id.updateturnlist_txtTurnList);
						final EditText txtIconList = (EditText) layout.findViewById(R.id.updateturnlist_txtIconList);
						
						SoftButton sb1 = new SoftButton();
						sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
						sb1.setText("Close");
						sb1.setType(SoftButtonType.SBT_TEXT);
						sb1.setIsHighlighted(false);
						sb1.setSystemAction(SystemAction.DEFAULT_ACTION);
						currentSoftButtons = new Vector<SoftButton>();
						currentSoftButtons.add(sb1);

						Button btnSoftButtons = (Button) layout.findViewById(R.id.updateturnlist_btnSoftButtons);
						btnSoftButtons.setOnClickListener(new OnClickListener() {
							@Override
							public void onClick(View v) {
								IntentHelper.addObjectForKey(currentSoftButtons,
										Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
								Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
								intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER,
										UPDATETURNLIST_MAXSOFTBUTTONS);
								startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
							}
						});
						
						builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								/*
								 * the number of items to send is determined as max of turn items
								 * and icon items. only when the both fields are empty, we
								 * don't send anything.
								 */
								String turnListString = txtTurnList.getText().toString();
								String iconListString = txtIconList.getText().toString();
								if ((turnListString.length() > 0) || (iconListString.length() > 0)) {
									// string to split turnList/iconList strings
									final String joinString = ",";
									
									Vector<Turn> tarray = new Vector<Turn>();
									
									String[] iconNames = iconListString.split(joinString);
									String[] turnNames = turnListString.split(joinString);
									int turnCount = Math.max(iconNames.length, turnNames.length);
									
									for (int i = 0; i < turnCount; ++i) {
										Turn t = new Turn();
										t.setNavigationText((i < turnNames.length) ? turnNames[i] : "");
										Image ti = new Image();
										ti.setValue((i < iconNames.length) ? iconNames[i] : "");
										ti.setImageType(ImageType.STATIC);
										t.setTurnIcon(ti);
										tarray.add(t);
									}
									UpdateTurnList msg = new UpdateTurnList();
									msg.setCorrelationID(autoIncCorrId++);
									msg.setTurnList(tarray);
									if (currentSoftButtons != null) {
										msg.setSoftButtons(currentSoftButtons);
									} else {
										msg.setSoftButtons(new Vector<SoftButton>());
									}
									currentSoftButtons = null;
									
									_msgAdapter.logMessage(msg, true);
									
									try {
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								} else {
									Toast.makeText(mContext, "Both fields are empty, nothing to send",
											Toast.LENGTH_LONG).show();
								}
							}
						});
						builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								currentSoftButtons = null;
								dialog.cancel();
							}
						});
						builder.setView(layout);
						builder.show();
					}

					private void sendDialNumber() {
						AlertDialog.Builder builder;

						Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.dialnumber, null);
						final EditText txtPhoneNumber = (EditText) layout.findViewById(R.id.dialNumber_editPhoneNumber);
						
						builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								DialNumber msg = new DialNumber();
								msg.setNumber(txtPhoneNumber.getText().toString());
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
						builder.show();
					}
					
					private void updateDynamicFooter(EditText txtNumTicks,
							EditText txtSliderFooter, String joinString) {
						// set numTicks comma-separated strings
						int numTicks = 0;
						try {
							numTicks = Integer.parseInt(txtNumTicks.getText().toString());
						} catch (NumberFormatException e) {
							// do nothing, leave 0
						}
						if (numTicks > 0) {
							StringBuilder b = new StringBuilder();
							for (int i = 0; i < numTicks; ++i) {
								b.append(joinString).append(i + 1);
							}
							txtSliderFooter.setText(b.toString().substring(joinString.length()));
						} else {
							txtSliderFooter.setText("");
						}
					}

					private void sendSlider() {
						AlertDialog.Builder builder;

						final Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.slider, null);
						final EditText txtNumTicks = (EditText) layout.findViewById(R.id.txtNumTicks);
						final EditText txtPosititon = (EditText) layout.findViewById(R.id.txtPosition);
						final EditText txtSliderHeader = (EditText) layout.findViewById(R.id.txtSliderHeader);
						final EditText txtSliderFooter = (EditText) layout.findViewById(R.id.txtSliderFooter);
						final EditText txtTimeout = (EditText) layout.findViewById(R.id.txtTimeout);

						// string to join/split footer strings
						final String joinString = ",";

						final CheckBox chkDynamicFooter = (CheckBox) layout.findViewById(R.id.slider_chkDynamicFooter);
						chkDynamicFooter.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
							@Override
							public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
								if (!isChecked) {
									// set default static text
									txtSliderFooter.setText(R.string.slider_footer);
								} else {
									updateDynamicFooter(txtNumTicks, txtSliderFooter, joinString);
								}
							}
						});
						
						txtNumTicks.setOnFocusChangeListener(new OnFocusChangeListener() {
							@Override
							public void onFocusChange(View v, boolean hasFocus) {
								if ((!hasFocus) && chkDynamicFooter.isChecked()) {
									updateDynamicFooter(txtNumTicks, txtSliderFooter, joinString);
								}
							}
						});
						
						builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								try {
									if (chkDynamicFooter.isChecked()) {
										updateDynamicFooter(txtNumTicks, txtSliderFooter, joinString);
									}
									
									Slider msg = new Slider();
									msg.setTimeout(Integer.parseInt(txtTimeout.getText().toString()));
									msg.setNumTicks(Integer.parseInt(txtNumTicks.getText().toString()));
									msg.setSliderHeader(txtSliderHeader.getText().toString());
									
									Vector<String> footerelements = null;
									String footer = txtSliderFooter.getText().toString();
									if (chkDynamicFooter.isChecked()) {
										footerelements = new Vector<String>(Arrays.asList(footer.split(joinString)));
									} else {
										footerelements = new Vector<String>();
										footerelements.add(footer);
									}
									msg.setSliderFooter(footerelements);
									
									msg.setPosition(Integer.parseInt(txtPosititon.getText().toString()));
									msg.setCorrelationID(autoIncCorrId++);
									_msgAdapter.logMessage(msg, true);
									ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
								} catch (NumberFormatException e) {
									Toast.makeText(mContext, "Couldn't parse number", Toast.LENGTH_LONG).show();
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
						builder.show();
					}

					private void sendVehicleDataSubscriptions() {
						AlertDialog.Builder builder;
						final Context mContext = adapter.getContext();
						
						// the local copy of isVehicleDataSubscribed
						final boolean[] checkedVehicleDataTypes = isVehicleDataSubscribed.clone();
						
						builder = new AlertDialog.Builder(mContext);
						builder.setMultiChoiceItems(vehicleDataTypeNames(), checkedVehicleDataTypes, new OnMultiChoiceClickListener() {
							@Override
							public void onClick(DialogInterface dialog, int which, boolean isChecked) {
								/**
								 * NB! This method is intentionally left empty. If the 3rd
								 * parameter to setMultiChoiceItems() is null, the user's
								 * changes to checked items don't save.
								 **/
							}
						});
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								Vector<VehicleDataType> subscribeVehicleData = new Vector<VehicleDataType>();
								Vector<VehicleDataType> unsubscribeVehicleData = new Vector<VehicleDataType>();
								VehicleDataType[] dataTypes = VehicleDataType.values();
								
								// subscribe and unsubscribe to new checked/unchecked items only
								for (int i = 0; i < checkedVehicleDataTypes.length; i++) {
									boolean checked = checkedVehicleDataTypes[i];
									boolean wasChecked = isVehicleDataSubscribed[i];
									if (checked && !wasChecked) {
										subscribeVehicleData.add(dataTypes[i]);
									} else if (!checked && wasChecked) {
										unsubscribeVehicleData.add(dataTypes[i]);
									}
								}
								
								if (!subscribeVehicleData.isEmpty()) {
									try {
										SubscribeVehicleData msg = new SubscribeVehicleData();
										msg.setDataType(subscribeVehicleData);
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
								
								if (!unsubscribeVehicleData.isEmpty()) {
									try {
										UnsubscribeVehicleData msg = new UnsubscribeVehicleData();
										msg.setDataType(unsubscribeVehicleData);
										msg.setCorrelationID(autoIncCorrId++);
										_msgAdapter.logMessage(msg, true);
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								}
								isVehicleDataSubscribed = checkedVehicleDataTypes.clone();
								
								if (subscribeVehicleData.isEmpty() && unsubscribeVehicleData.isEmpty()) {
									Toast.makeText(mContext, "Nothing new here", Toast.LENGTH_LONG).show();
								}
							}
						});
						builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								dialog.cancel();
							}
						});
						builder.show();
					}

					private String[] vehicleDataTypeNames() {
						final String[] vehicleDataTypeNames = new String[] {
								"GPS", "Speed", "Fuel Level", "Fuel Economy",
								"Engine RPM", "Battery Voltage", "Rain Sensor",
								"Odometer", "VIN", "External Temp", "PRNDL Status",
								"Tire Pressure", "Battery Voltage", "Battery Current",
								"Battery Temperature", "Satellite ESN"
						};
						return vehicleDataTypeNames;
					}

					private void sendSetGlobalProperties() {
						AlertDialog.Builder builder;

						Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.setglobalproperties,
								(ViewGroup) findViewById(R.id.setglobalproperties_Root));

						final EditText helpPrompt = (EditText) layout.findViewById(R.id.setglobalproperties_helpPrompt);
						final EditText timeoutPrompt = (EditText) layout.findViewById(R.id.setglobalproperties_timeoutPrompt);
						final EditText vrHelpTitle = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpTitle);
						final EditText vrHelpItemText = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpItemText);
						final EditText vrHelpItemImage = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpItemImage);
						final EditText vrHelpItemPosition = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpItemPos);
						final CheckBox choiceHelpPrompt = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceHelpPrompt);
						final CheckBox choiceTimeoutPrompt = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceTimeoutPrompt);
						final CheckBox choiceVRHelpTitle = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceVRHelpTitle);
						final CheckBox choiceVRHelpItem = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceVRHelpItem);
						
						final boolean vrHelpEnabled = getCurrentProtocolVersion() >= 2;
						if (!vrHelpEnabled) {
							// disable VR help title and VR help item
							int visibility = android.view.View.GONE;
							choiceVRHelpTitle.setVisibility(visibility);
							vrHelpTitle.setVisibility(visibility);
							layout.findViewById(R.id.setglobalproperties_vrHelpItemLayout).setVisibility(visibility);
							vrHelpItemText.setVisibility(visibility);
							vrHelpItemImage.setVisibility(visibility);
						}

						builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								SetGlobalProperties msg = new SetGlobalProperties();
								int numberOfChoices = 0;
								
								// string to join/split help and timeout prompts
								final String joinString = ",";
								
								if (choiceHelpPrompt.isChecked()) {
									Vector<TTSChunk> help = new Vector<TTSChunk>();
									String helpString = helpPrompt.getText().toString();
									for (String ttsChunk : helpString.split(joinString)) {
										TTSChunk chunk = TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, ttsChunk);
										help.add(chunk);
									}
									msg.setHelpPrompt(help);
									++numberOfChoices;
								}

								if (choiceTimeoutPrompt.isChecked()) {
									Vector<TTSChunk> timeout = new Vector<TTSChunk>();
									String timeoutString = timeoutPrompt.getText().toString();
									for (String ttsChunk : timeoutString.split(joinString)) {
										TTSChunk chunk = TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, ttsChunk);
										timeout.add(chunk);
									}
									msg.setTimeoutPrompt(timeout);
									++numberOfChoices;
								}

								if (vrHelpEnabled && choiceVRHelpTitle.isChecked()) {
									msg.setVrHelpTitle(vrHelpTitle.getText().toString());
									++numberOfChoices;
								}
								
								if (vrHelpEnabled && choiceVRHelpItem.isChecked()) {
									Vector<VrHelpItem> vrHelp = new Vector<VrHelpItem>();
									
									VrHelpItem helpItem = new VrHelpItem();
									helpItem.setText(vrHelpItemText.getText().toString());
									try {
										helpItem.setPosition(Integer.parseInt(vrHelpItemPosition.getText().toString()));
									} catch (NumberFormatException e) {
										// set something default
										helpItem.setPosition(1);
									}
									Image image = new Image();
									image.setValue(vrHelpItemImage.getText().toString());
									image.setImageType(ImageType.STATIC);
									helpItem.setImage(image);
									vrHelp.add(helpItem);
									
									msg.setVrHelp(vrHelp);
									++numberOfChoices;
								}

								if (numberOfChoices > 0) {
									msg.setCorrelationID(autoIncCorrId++);
									_msgAdapter.logMessage(msg, true);
									try {
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								} else {
									Toast.makeText(getApplicationContext(), "No items selected", Toast.LENGTH_LONG).show();
								}
							}
						});
						builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								dialog.cancel();
							}
						});
						builder.setView(layout);
						builder.create().show();
					}

					private void sendResetGlobalProperties() {
						AlertDialog.Builder builder;

						Context mContext = adapter.getContext();
						LayoutInflater inflater = (LayoutInflater) mContext
								.getSystemService(LAYOUT_INFLATER_SERVICE);
						View layout = inflater.inflate(R.layout.resetglobalproperties,
								(ViewGroup) findViewById(R.id.resetglobalproperties_Root));

						final CheckBox choiceHelpPrompt = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceHelpPrompt);
						final CheckBox choiceTimeoutPrompt = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceTimeoutPrompt);
						final CheckBox choiceVRHelpTitle = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceVRHelpTitle);
						final CheckBox choiceVRHelpItem = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceVRHelpItems);

						final boolean vrHelpEnabled = getCurrentProtocolVersion() >= 2;
						if (!vrHelpEnabled) {
							// disable VR help title and VR help item
							int visibility = android.view.View.GONE;
							choiceVRHelpTitle.setVisibility(visibility);
							choiceVRHelpItem.setVisibility(visibility);
						}

						builder = new AlertDialog.Builder(mContext);
						builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								ResetGlobalProperties msg = new ResetGlobalProperties();
								Vector<GlobalProperty> properties = new Vector<GlobalProperty>();
								
								if (choiceHelpPrompt.isChecked()) {
									properties.add(GlobalProperty.HELPPROMPT);
								}

								if (choiceTimeoutPrompt.isChecked()) {
									properties.add(GlobalProperty.TIMEOUTPROMPT);
								}

								if (vrHelpEnabled && choiceVRHelpTitle.isChecked()) {
									properties.add(GlobalProperty.VRHELPTITLE);
								}

								if (vrHelpEnabled && choiceVRHelpItem.isChecked()) {
									properties.add(GlobalProperty.VRHELPITEMS);
								}

								if (!properties.isEmpty()) {
									msg.setProperties(properties);
									msg.setCorrelationID(autoIncCorrId++);
									_msgAdapter.logMessage(msg, true);
									try {
										ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
									} catch (SyncException e) {
										_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
									}
								} else {
									Toast.makeText(getApplicationContext(), "No items selected", Toast.LENGTH_LONG).show();
								}
							}
						});
						builder.setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int id) {
								dialog.cancel();
							}
						});
						builder.setView(layout);
						builder.create().show();
					}
					
					/**
					 * Sends a GenericRequest message.
					 */
					private void sendGenericRequest() {
						GenericRequest msg = new GenericRequest();
						msg.setCorrelationID(autoIncCorrId++);
						_msgAdapter.logMessage(msg, true);
						try {
							ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
						} catch (SyncException e) {
							_msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
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
	
	/**
	 * Adds command ID to the adapter, and maps it to its parent submenu.
	 * 
	 * @param cmdID
	 *            ID of the new command
	 * @param submenuID
	 *            ID of the command's parent submenu
	 */
	private void addCommandToList(int cmdID, int submenuID) {
		_commandAdapter.add(cmdID);
		_commandIdToParentSubmenuMap.put(cmdID, submenuID);
	}

	/**
	 * Removes command ID from the adapter.
	 * 
	 * @param cmdID
	 *            ID of the command
	 */
	private void removeCommandFromList(int cmdID) {
		_commandAdapter.remove(cmdID);
		_commandIdToParentSubmenuMap.remove(cmdID);
	}

/*	public void startSyncProxyService() {
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
	}*/

	//upon onDestroy(), dispose current proxy and create a new one to enable auto-start
	//call resetProxy() to do so
	public void endSyncProxyInstance() {	
		ProxyService serviceInstance = ProxyService.getInstance();
		if (serviceInstance != null){
			SyncProxyALM proxyInstance = serviceInstance.getProxyInstance();
			//if proxy exists, reset it
			if(proxyInstance != null){
				if (proxyInstance.getCurrentTransportType() == TransportType.BLUETOOTH) {
					serviceInstance.reset();
				} else {
					Log.e(logTag, "endSyncProxyInstance. No reset required if transport is TCP");
				}
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
	
	public static void setESN(byte[] ESN) {
		_ESN = ESN;
	}

	@Override
	public void onBackPressed() {
		super.onBackPressed();
		saveMessageSelectCount();
	}
	
	@Override
	public void onPause() {
		super.onPause();
		saveMessageSelectCount();
	}
	
	/**
	 * Called when a CreateChoiceSetResponse comes. If successful, add it to the
	 * adapter. In any case, remove the key from the map.
	 */
	public void onCreateChoiceSetResponse(boolean success) {
		if (_latestCreateChoiceSetId != CHOICESETID_UNSET) {
			if (success) {
				_choiceSetAdapter.add(_latestCreateChoiceSetId);
			}
			_latestCreateChoiceSetId = CHOICESETID_UNSET;
		} else {
			Log.w(logTag, "Latest createChoiceSetId is unset");
		}
	}
	
	/**
	 * Called when a DeleteChoiceSetResponse comes. If successful, remove it
	 * from the adapter.
	 */
	public void onDeleteChoiceSetResponse(boolean success) {
		if (_latestDeleteChoiceSetId != CHOICESETID_UNSET) {
			if (success) {
				_choiceSetAdapter.remove(_latestDeleteChoiceSetId);
			}
			_latestDeleteChoiceSetId = CHOICESETID_UNSET;
		} else {
			Log.w(logTag, "Latest deleteChoiceSetId is unset");
		}
	}
	
	/**
	 * Called when a DeleteSubMenuResponse comes. If successful, remove it from
	 * the adapter. We also need to delete all the commands that were added to
	 * this submenu.
	 */
	public void onDeleteSubMenuResponse(boolean success) {
		if (_latestDeleteSubmenu != null) {
			if (success) {
				_submenuAdapter.remove(_latestDeleteSubmenu);

				for (Iterator<Entry<Integer, Integer>> it = _commandIdToParentSubmenuMap
						.entrySet().iterator(); it.hasNext();) {
					Entry<Integer, Integer> entry = it.next();
					if (entry.getValue() == _latestDeleteSubmenu.getSubMenuId()) {
						_commandAdapter.remove(entry.getKey());
						it.remove();
					}
				}
			}
			_latestDeleteSubmenu = null;
		} else {
			Log.w(logTag, "Latest deleteSubMenu is unset");
		}
	}
	
	/**
	 * Called when a AddSubMenuResponse comes. If successful, add it to the
	 * adapter.
	 */
	public void onAddSubMenuResponse(boolean success) {
		if (_latestAddSubmenu != null) {
			if (success) {
				addSubMenuToList(_latestAddSubmenu);
			}
			_latestAddSubmenu = null;
		} else {
			Log.w(logTag, "Latest addSubMenu is unset");
		}
	}

	/**
	 * Called whenever an OnAudioPassThru notification comes. The aptData is the
	 * audio data sent in it.
	 */
	public void onAudioPassThru(byte[] aptData) {
		if (aptData == null) {
			Log.w(logTag, "onAudioPassThru aptData is null");
			return;
		}
		Log.i(logTag, "data len " + aptData.length);

		File outFile = audioPassThruOutputFile();
		try {
			if (audioPassThruOutStream == null) {
				audioPassThruOutStream = new BufferedOutputStream(
						new FileOutputStream(outFile, false));
			}
			audioPassThruOutStream.write(aptData);
			audioPassThruOutStream.flush();
		} catch (FileNotFoundException e) {
			logToConsoleAndUI(
					"Output file "
							+ (outFile != null ? outFile.toString()
									: "'unknown'")
							+ " can't be opened for writing", e);
		} catch (IOException e) {
			logToConsoleAndUI("Can't write to output file", e);
		}

		/*
		 * if there is current player, save the current position, stop and
		 * release it, so that we recreate it with the appended file and jump to
		 * that position, emulating seamless stream playing
		 */
		int audioPosition = -1;
		if (audioPassThruMediaPlayer != null) {
			audioPosition = audioPassThruMediaPlayer.getCurrentPosition();
			audioPassThruMediaPlayer.stop();
			audioPassThruMediaPlayer.reset();
			audioPassThruMediaPlayer.release();
			audioPassThruMediaPlayer = null;
		}

		audioPassThruMediaPlayer = new MediaPlayer();
		try {
			if (isExtStorageWritable()) {
				audioPassThruMediaPlayer.setDataSource(outFile.toString());
			} else {
				/*
				 * setDataSource with a filename on the internal storage throws
				 * "java.io.IOException: Prepare failed.: status=0x1", so we
				 * open the file with a special method
				 */
				audioPassThruMediaPlayer.setDataSource(openFileInput(
						AUDIOPASSTHRU_OUTPUT_FILE).getFD());
			}
			audioPassThruMediaPlayer.prepare();
			if (audioPosition != -1) {
				audioPassThruMediaPlayer.seekTo(audioPosition);
			}
			audioPassThruMediaPlayer.start();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	/**
	 * Called when a PerformAudioPassThru response comes. Save the file only if
	 * the result is success. If the result is retry, send the latest request
	 * again.
	 */
	public void onPerformAudioPassThruResponse(Result result) {
		closeAudioPassThruStream();
		closeAudioPassThruMediaPlayer();
		if (Result.SUCCESS != result) {
			File outFile = audioPassThruOutputFile();
			if ((outFile != null) && outFile.exists()) {
				if (!outFile.delete()) {
					logToConsoleAndUI("Failed to delete output file", null);
				}
			}

			if ((Result.RETRY == result)
					&& (latestPerformAudioPassThruMsg != null)) {
				latestPerformAudioPassThruMsg.setCorrelationID(autoIncCorrId++);
				try {
					_msgAdapter.logMessage(latestPerformAudioPassThruMsg, true);
					ProxyService.getInstance().getProxyInstance()
							.sendRPCRequest(latestPerformAudioPassThruMsg);
				} catch (SyncException e) {
					_msgAdapter.logMessage("Error sending message: " + e,
							Log.ERROR, e);
				}
			}
		}
	}

	/**
	 * Called when an EndAudioPassThru response comes. The logic is the same as
	 * when a PerformAudioPassThru response comes.
	 */
	public void onEndAudioPassThruResponse(Result result) {
		onPerformAudioPassThruResponse(result);
	}

	private void closeAudioPassThruStream() {
		if (audioPassThruOutStream != null) {
			Log.d(logTag, "closing audioPassThruOutStream");
			try {
				audioPassThruOutStream.flush();
				audioPassThruOutStream.close();
			} catch (IOException e) {
				Log.w(logTag, "Can't close output file", e);
			}
			audioPassThruOutStream = null;
		}
	}
	
	private void closeAudioPassThruMediaPlayer() {
		if (audioPassThruMediaPlayer == null) {
			return;
		}
		
		if (audioPassThruMediaPlayer.isPlaying()) {
			audioPassThruMediaPlayer.setOnCompletionListener(new OnCompletionListener() {
				@Override
				public void onCompletion(MediaPlayer mp) {
					Log.d(logTag, "mediaPlayer completed");
					audioPassThruMediaPlayer.reset();
					audioPassThruMediaPlayer.release();
					audioPassThruMediaPlayer = null;
				}
			});
		} else {
			// the player has stopped
			Log.d(logTag, "mediaPlayer is stopped");
			audioPassThruMediaPlayer.release();
			audioPassThruMediaPlayer = null;
		}
	}

	private File audioPassThruOutputFile() {
		File baseDir = isExtStorageWritable() ? Environment
				.getExternalStorageDirectory() : getFilesDir();
		File outFile = new File(baseDir, AUDIOPASSTHRU_OUTPUT_FILE);
		return outFile;
	}

	private void logToConsoleAndUI(String msg, Throwable thr) {
		Log.d(logTag, msg, thr);
		Toast.makeText(getApplicationContext(), msg, Toast.LENGTH_SHORT).show();
	}

	/** Returns whether the external storage is available for writing. */
	private boolean isExtStorageWritable() {
		String state = Environment.getExternalStorageState();
		return Environment.MEDIA_MOUNTED.equals(state);
	}

	/** Called when a connection to a SYNC device has been closed. */
	public void onProxyClosed() {
		resetAdapters();
		_msgAdapter.logMessage("Disconnected", true);
	}

	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		switch (requestCode) {
		case Const.REQUEST_LIST_SOFTBUTTONS:
			if (resultCode == RESULT_OK) {
				currentSoftButtons = (Vector<SoftButton>) IntentHelper.
						getObjectForKey(Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
				if (chkIncludeSoftButtons != null) {
					chkIncludeSoftButtons.setChecked(true);
				}
			}
			IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
			break;

		case REQUEST_FILE_OPEN:
			if (resultCode == RESULT_OK) {
				String fileName = data.getStringExtra(FileDialog.RESULT_PATH);
				if (txtLocalFileName != null) {
					txtLocalFileName.setText(fileName);
				}
			}
			break;
			
		case REQUEST_CHOOSE_XML_TEST:
			if (resultCode == RESULT_OK) {
				String filePath = data.getStringExtra(FileDialog.RESULT_PATH);
				if (filePath != null) {
					xmlTestContinue(filePath);
				}
			}
			break;
			
		default:
			Log.i(logTag, "Unknown request code: " + requestCode);
			break;
		}
	}
	
	public static int getNewSoftButtonId() {
		return autoIncSoftButtonId++;
	}
	
	/**
	 * Called when the app is acivated from HMI for the first time. ProxyService
	 * automatically subscribes to buttons, so we reflect that in the
	 * subscription list.
	 */
	public void buttonsSubscribed(Vector<ButtonName> buttons) {
		List<ButtonName> buttonNames = Arrays.asList(ButtonName.values());
		for (ButtonName buttonName : buttons) {
			isButtonSubscribed[buttonNames.indexOf(buttonName)] = true;
		}
	}

	/**
	 * Returns the file contents from the specified file.
	 * 
	 * @param filename
	 *            Name of the file to open.
	 * @return The file's contents or null in case of an error
	 */
	public static byte[] contentsOfFile(String filename) {
		InputStream is = null;
		try {
			is = new BufferedInputStream(new FileInputStream(filename));
			ByteArrayOutputStream os = new ByteArrayOutputStream(is.available());
			final int buffersize = 4096;
			final byte[] buffer = new byte[buffersize];
			int available = 0;
			while ((available = is.read(buffer)) >= 0) {
				os.write(buffer, 0, available);
			}
			return os.toByteArray();
		} catch (IOException e) {
			Log.w(logTag, "Can't read file " + filename, e);
			return null;
		} catch (OutOfMemoryError e) {
			Log.e(logTag, "File " + filename + " is too big", e);
			return null;
		} finally {
			if (is != null) {
				try {
					is.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}

	/**
	 * Opens a dialog so that the user can select an XML test or a directory
	 * with XML tests. The result will come to onActivityResult method.
	 */
	public void openXmlFilePathDialog() {
		Intent intent = new Intent(this, FileDialog.class);
		String sdcardPath = Environment.getExternalStorageDirectory().getPath();
		intent.putExtra(FileDialog.START_PATH, sdcardPath);
		intent.putExtra(FileDialog.CAN_SELECT_DIR, true);
		intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
		intent.putExtra(FileDialog.FORMAT_FILTER, new String[] { "xml" });
		startActivityForResult(intent, REQUEST_CHOOSE_XML_TEST);
	}
}

