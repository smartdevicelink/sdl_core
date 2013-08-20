package com.ford.syncV4.android.activity;

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
import com.ford.syncV4.proxy.rpc.SetDisplayLayout;
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
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.Turn;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
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
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Timer;
import java.util.TimerTask;
import java.util.Vector;

public class SyncProxyTester extends Activity implements OnClickListener {
    private static final String VERSION = "$Version:$";
    private static final String logTag = "SyncProxyTester";
    private static final String ButtonSubscriptions = "ButtonSubscriptions";
    private static final String VehicleDataSubscriptions = "VehicleDataSubscriptions";
    /**
     * Name of a generic request. See {@link GenericRequest}.
     */
    private static final String GenericRequest = "GenericRequest";
    /**
     * The name of the file where all the data coming with
     * {@link OnAudioPassThru} notifications is saved. The root directory is the
     * external storage.
     */
    private static final String AUDIOPASSTHRU_OUTPUT_FILE = "audiopassthru.wav";
    /**
     * String to join/split help, timeout, VR prompts, etc.
     */
    private static final String JOIN_STRING = ",";
    private static final int ALERT_MAXSOFTBUTTONS = 4;
    private static final int SCROLLABLEMESSAGE_MAXSOFTBUTTONS = 8;
    private static final int SHOW_MAXSOFTBUTTONS = 8;
    private static final int ALERTMANEUVER_MAXSOFTBUTTONS = 3;
    private static final int SHOWCONSTANTTBT_MAXSOFTBUTTONS = 3;
    private static final int UPDATETURNLIST_MAXSOFTBUTTONS = 1;
    private static final int REQUEST_FILE_OPEN = 50;
    private final static int REQUEST_CHOOSE_XML_TEST = 51;
    private static final int PUTFILE_MAXFILESIZE = 4 * 1024 * 1024; // 4MB
    private static final int CHOICESETID_UNSET = -1;
    private static final String MSC_PREFIX = "msc_";
    private static SyncProxyTester _activity;
    private static ArrayList<Object> _logMessages = new ArrayList<Object>();
    private static logAdapter _msgAdapter;
    private static byte[] _ESN;
    /**
     * Autoincrementing id for new softbuttons.
     */
    private static int autoIncSoftButtonId = 5500;
    /**
     * In onCreate() specifies if it is the first time the activity is created
     * during this app launch.
     */
    private static boolean isFirstActivityRun = true;
    private final int PROXY_START = 5;
    private final int XML_TEST = 7;
    private final int POLICIES_TEST = 8;
    private final int MNU_TOGGLE_CONSOLE = 9;
    private final int MNU_CLEAR = 10;
    private final int MNU_EXIT = 11;
    private final int MNU_TOGGLE_MEDIA = 12;
    private final int MNU_UNREGISTER = 14;
    private final int MNU_APP_VERSION = 15;
    private final int MNU_CLEAR_FUNCTIONS_USAGE = 16;
    private final int MNU_WAKELOCK = 17;
    private ModuleTest _testerMain;
    private ScrollView _scroller = null;
    private ListView _listview = null;
    private ArrayAdapter<SyncSubMenu> _submenuAdapter = null;
    private ArrayAdapter<Integer> _commandAdapter = null;
    private Map<Integer, Integer> _commandIdToParentSubmenuMap = null;
    private ArrayAdapter<Integer> _choiceSetAdapter = null;
    private ArrayAdapter<String> _putFileAdapter = null;
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
    /**
     * The output stream to write audioPassThru data.
     */
    private OutputStream audioPassThruOutStream = null;
    /**
     * Media player for the stream of audio pass thru.
     */
    private MediaPlayer audioPassThruMediaPlayer = null;
    /**
     * The most recent sent PerformAudioPassThru message, saved in case we need
     * to retry the request.
     */
    private PerformAudioPassThru latestPerformAudioPassThruMsg = null;
    /**
     * Shared ArrayAdapter containing ImageType values.
     */
    private ArrayAdapter<ImageType> imageTypeAdapter;

    public static SyncProxyTester getInstance() {
        return _activity;
    }

    public static logAdapter getMessageAdapter() {
        return _msgAdapter;
    }

    public static void setESN(byte[] ESN) {
        _ESN = ESN;
    }

    public static int getNewSoftButtonId() {
        return autoIncSoftButtonId++;
    }

    /**
     * Returns the file contents from the specified file.
     *
     * @param filename Name of the file to open.
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

        imageTypeAdapter = new ArrayAdapter<ImageType>(this, android.R.layout.simple_spinner_item, ImageType.values());
        imageTypeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

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
                        try {
                            rawJSON = ((RPCMessage) listObj).getFunctionName() +
                                    " (" + ((RPCMessage) listObj).getMessageType() + ")";
                        } catch (Exception e1) {
                            rawJSON = "Undefined";
                        }
                    }
                    builder.setMessage(rawJSON);
                    builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            dialog.cancel();
                        }
                    });
                    AlertDialog ad = builder.create();
                    ad.show();
                } else if (listObj instanceof String) {
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
     * Shows a dialog where the user can select connection features (media flag, app name, language, HMI language,
     * transport settings, and some extra options). Starts the proxy after selecting.
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

    /**
     * Starts the sync proxy at startup after selecting protocol features.
     */
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
        isButtonSubscribed = new boolean[ButtonName.values().length];
        _buttonAdapter = new ArrayAdapter<ButtonName>(this,
                android.R.layout.select_dialog_multichoice, ButtonName.values()) {
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

    /**
     * Displays the current protocol properties in the activity's title.
     */
    private void showProtocolPropertiesInTitle() {
        final SharedPreferences prefs = getSharedPreferences(Const.PREFS_NAME,
                0);
        int protocolVersion = getCurrentProtocolVersion();
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

    /* Creates the menu items */
    public boolean onCreateOptionsMenu(Menu menu) {
        boolean result = super.onCreateOptionsMenu(menu);
        if (result) {
            menu.add(0, PROXY_START, 0, "Proxy Start");
            menu.add(0, MNU_TOGGLE_CONSOLE, 0, "Toggle Console");
            menu.add(0, MNU_CLEAR, 0, "Clear Messages");
            menu.add(0, MNU_EXIT, 0, "Exit");
/*			menu.add(0, MNU_TOGGLE_MEDIA, 0, "Toggle Media");*/
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
        return Const.PROTOCOL_VERSION_2;
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
                    } catch (SyncException e) {
                    }
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
                if (PoliciesTesterActivity.getInstance() == null) {
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
                    } catch (SyncException e) {
                    }
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

    /**
     * Closes the activity and stops the proxy service.
     */
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
     * Adds the function name to the adapter.
     */
    private void addToFunctionsAdapter(ArrayAdapter<String> adapter, String functionName) {
        adapter.add(functionName);
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

    public void onClick(View v) {
        if (v == findViewById(R.id.btnSendMessage)) {
            final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.select_dialog_item);
            addToFunctionsAdapter(adapter, Names.Alert);
            addToFunctionsAdapter(adapter, Names.Speak);
            addToFunctionsAdapter(adapter, Names.Show);
            addToFunctionsAdapter(adapter, ButtonSubscriptions);
            addToFunctionsAdapter(adapter, Names.AddCommand);
            addToFunctionsAdapter(adapter, Names.DeleteCommand);
            addToFunctionsAdapter(adapter, Names.AddSubMenu);
            addToFunctionsAdapter(adapter, Names.DeleteSubMenu);
            addToFunctionsAdapter(adapter, Names.SetGlobalProperties);
            addToFunctionsAdapter(adapter, Names.ResetGlobalProperties);
            addToFunctionsAdapter(adapter, Names.SetMediaClockTimer);
            addToFunctionsAdapter(adapter, Names.CreateInteractionChoiceSet);
            addToFunctionsAdapter(adapter, Names.DeleteInteractionChoiceSet);
            addToFunctionsAdapter(adapter, Names.PerformInteraction);
            addToFunctionsAdapter(adapter, Names.EncodedSyncPData);
            addToFunctionsAdapter(adapter, Names.SyncPData);
            addToFunctionsAdapter(adapter, Names.Slider);
            addToFunctionsAdapter(adapter, Names.ScrollableMessage);
            addToFunctionsAdapter(adapter, Names.ChangeRegistration);
            addToFunctionsAdapter(adapter, Names.PutFile);
            addToFunctionsAdapter(adapter, Names.DeleteFile);
            addToFunctionsAdapter(adapter, Names.ListFiles);
            addToFunctionsAdapter(adapter, Names.SetAppIcon);
            addToFunctionsAdapter(adapter, Names.PerformAudioPassThru);
            addToFunctionsAdapter(adapter, Names.EndAudioPassThru);
            addToFunctionsAdapter(adapter, VehicleDataSubscriptions);
            addToFunctionsAdapter(adapter, Names.GetVehicleData);
            addToFunctionsAdapter(adapter, Names.ReadDID);
            addToFunctionsAdapter(adapter, Names.GetDTCs);
            addToFunctionsAdapter(adapter, Names.ShowConstantTBT);
            addToFunctionsAdapter(adapter, Names.AlertManeuver);
            addToFunctionsAdapter(adapter, Names.UpdateTurnList);
            addToFunctionsAdapter(adapter, Names.SetDisplayLayout);
            addToFunctionsAdapter(adapter, Names.UnregisterAppInterface);
            addToFunctionsAdapter(adapter, GenericRequest);

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
                    .setTitle("Pick a Function")
                    .setAdapter(adapter, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            if (adapter.getItem(which) == Names.Alert) {
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
                                        intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER, ALERT_MAXSOFTBUTTONS);
                                        startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
                                    }
                                });

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
                                            if (toSpeak.length() > 0) {
                                                Vector<TTSChunk> ttsChunks = TTSChunkFactory.createSimpleTTSChunks(toSpeak);
                                                msg.setTtsChunks(ttsChunks);
                                            }
                                            if (chkIncludeSoftButtons.isChecked() &&
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
                                            chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities) spnSpeakType1.getSelectedItem(), speak1));

                                        }
                                        if (speak2.length() > 0) {
                                            chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities) spnSpeakType2.getSelectedItem(), speak2));

                                        }
                                        if (speak3.length() > 0) {
                                            chunks.add(TTSChunkFactory.createChunk((SpeechCapabilities) spnSpeakType3.getSelectedItem(), speak3));

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
                                sendAddCommand();
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
                                sendAddSubmenu();
                            } else if (adapter.getItem(which) == Names.DeleteSubMenu) {
                                sendDeleteSubMenu();
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

                                ArrayAdapter<UpdateMode> spinnerAdapter = new ArrayAdapter<UpdateMode>(adapter.getContext(),
                                        android.R.layout.simple_spinner_item, UpdateMode.values());
                                spinnerAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                                spnUpdateMode.setAdapter(spinnerAdapter);

                                builder = new AlertDialog.Builder(mContext);
                                builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                    public void onClick(DialogInterface dialog, int id) {
                                        SetMediaClockTimer msg = new SetMediaClockTimer();
                                        msg.setCorrelationID(autoIncCorrId++);
                                        UpdateMode updateMode = (UpdateMode) spnUpdateMode.getSelectedItem();
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
                                sendSyncPData(true);
                            } else if (adapter.getItem(which) == Names.SyncPData) {
                                sendSyncPData(false);
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
                                spinnerAdapterLanguage.setDropDownViewResource(
                                        android.R.layout.simple_spinner_dropdown_item);
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
                                sendPerformAudioPassThru();
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
                                sendGetVehicleData();
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
                                sendGetDTCs();
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
                                        String ttsChunksString = txtTtsChunks.getText().toString();
                                        for (String ttsChunk : ttsChunksString.split(JOIN_STRING)) {
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
                            } else if (adapter.getItem(which) == Names.SetDisplayLayout) {
                                sendSetDisplayLayout();
                            } else if (adapter.getItem(which) ==
                                    Names.UnregisterAppInterface) {
                                sendUnregisterAppInterface();
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
                         * Sends UnregisterAppInterface message.
                         */
                        private void sendUnregisterAppInterface() {
                            UnregisterAppInterface msg =
                                    new UnregisterAppInterface();
                            msg.setCorrelationID(autoIncCorrId++);
                            _msgAdapter.logMessage(msg, true);
                            try {
                                ProxyService.getInstance().getProxyInstance()
                                        .sendRPCRequest(msg);
                            } catch (SyncException e) {
                                _msgAdapter.logMessage("Error sending message: " + e,
                                        Log.ERROR, e);
                            }
                        }

                        /**
                         * Opens the dialog for DeleteSubMenu message and sends it.
                         */
                        private void sendDeleteSubMenu() {
                            AlertDialog.Builder builder =
                                    new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(_submenuAdapter,
                                    new DialogInterface.OnClickListener() {
                                        @Override
                                        public void onClick(DialogInterface dialog,
                                                            int which) {
                                            SyncSubMenu menu =
                                                    _submenuAdapter.getItem(which);
                                            DeleteSubMenu msg = new DeleteSubMenu();
                                            msg.setCorrelationID(autoIncCorrId++);
                                            msg.setMenuID(menu.getSubMenuId());
                                            try {
                                                _msgAdapter.logMessage(msg, true);
                                                ProxyService.getInstance()
                                                        .getProxyInstance()
                                                        .sendRPCRequest(msg);
                                            } catch (SyncException e) {
                                                _msgAdapter.logMessage(
                                                        "Error sending message: " +
                                                                e, Log.ERROR, e);
                                            }

                                            if (_latestDeleteSubmenu != null) {
                                                Log.w(logTag,
                                                        "Latest deleteSubmenu should be null, but equals to " +
                                                                _latestDeleteSubmenu);
                                            }
                                            _latestDeleteSubmenu = menu;
                                        }
                                    });
                            builder.show();
                        }

                        /**
                         * Opens the dialog for GetVehicleData message and sends it.
                         */
                        private void sendGetVehicleData() {
                            AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(_vehicleDataType, new DialogInterface.OnClickListener() {

                                public void onClick(DialogInterface dialog, int which) {
                                    try {
                                        GetVehicleData msg = new GetVehicleData();

                                        final String[] methodNames = {"Gps", "Speed", "Rpm", "FuelLevel",
                                                "FuelLevel_State", "InstantFuelConsumption", "ExternalTemperature",
                                                "Prndl", "TirePressure", "Odometer", "BeltStatus", "BodyInformation",
                                                "DeviceStatus", "DriverBraking", "WiperStatus", "HeadLampStatus",
                                                "EngineTorque", "AccPedalPosition", "SteeringWheelAngle",
                                                "ECallInfo", "AirbagStatus", "EmergencyEvent", "ClusterModeStatus",
                                                "MyKey"};
                                        final String setterName = "set" + methodNames[which];
                                        setVehicleDataParam(msg, GetVehicleData.class, setterName);

                                        msg.setCorrelationID(autoIncCorrId++);
                                        _msgAdapter.logMessage(msg, true);
                                        ProxyService.getInstance().getProxyInstance().sendRPCRequest(msg);
                                    } catch (SyncException e) {
                                        _msgAdapter.logMessage("Error sending message: " + e, Log.ERROR, e);
                                    }
                                }
                            });
                            builder.show();
                        }

                        /**
                         * Calls the setter with setterName on the msg.
                         */
                        private void setVehicleDataParam(RPCRequest msg, Class msgClass, String setterName) {
                            try {
                                Method setter = msgClass.getMethod(setterName, Boolean.class);
                                setter.invoke(msg, true);
                            } catch (NoSuchMethodException e) {
                                Log.e(logTag, "Can't set vehicle data", e);
                            } catch (IllegalAccessException e) {
                                Log.e(logTag, "Can't set vehicle data", e);
                            } catch (InvocationTargetException e) {
                                Log.e(logTag, "Can't set vehicle data", e);
                            }
                        }

                        /**
                         * Opens the dialog for GetDTCs message and sends it.
                         */
                        private void sendGetDTCs() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.getdtcs, null);

                            final EditText txtECUNameDTC = (EditText) layout.findViewById(R.id.getdtcs_txtECUNameDTC);
                            final EditText txtdtcMask = (EditText) layout.findViewById(R.id.getdtcs_dtcMask);

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    try {
                                        GetDTCs msg = new GetDTCs();
                                        msg.setEcuName(Integer.parseInt(txtECUNameDTC.getText().toString()));
                                        msg.setDTCMask(Integer.parseInt(txtdtcMask.getText().toString()));
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

                        /**
                         * Opens the dialog for PerformAudioPassThru message and sends it.
                         */
                        private void sendPerformAudioPassThru() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.performaudiopassthru, null);

                            final EditText txtInitialPrompt = (EditText) layout
                                    .findViewById(R.id.performaudiopassthru_txtInitialPrompt);
                            final EditText txtAudioPassThruDisplayText1 = (EditText) layout
                                    .findViewById(R.id.performaudiopassthru_txtAudioPassThruDisplayText1);
                            final EditText txtAudioPassThruDisplayText2 = (EditText) layout
                                    .findViewById(R.id.performaudiopassthru_txtAudioPassThruDisplayText2);
                            final Spinner spnSamplingRate = (Spinner) layout
                                    .findViewById(R.id.performaudiopassthru_spnSamplingRate);
                            final EditText txtMaxDuration = (EditText) layout
                                    .findViewById(R.id.performaudiopassthru_txtMaxDuration);
                            final Spinner spnBitsPerSample = (Spinner) layout
                                    .findViewById(R.id.performaudiopassthru_spnBitsPerSample);
                            final Spinner spnAudioType = (Spinner) layout
                                    .findViewById(R.id.performaudiopassthru_spnAudioType);
                            final CheckBox chkMuteAudio = (CheckBox) layout
                                    .findViewById(R.id.performaudiopassthru_muteAudio);

                            ArrayAdapter<SamplingRate> spinnerAdapterSamplingRate = new ArrayAdapter<SamplingRate>(
                                    adapter.getContext(),
                                    android.R.layout.simple_spinner_item, SamplingRate.values());
                            spinnerAdapterSamplingRate
                                    .setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                            spnSamplingRate.setAdapter(spinnerAdapterSamplingRate);

                            ArrayAdapter<BitsPerSample> spinnerAdapterBitsPerSample = new ArrayAdapter<BitsPerSample>(
                                    adapter.getContext(),
                                    android.R.layout.simple_spinner_item, BitsPerSample.values());
                            spinnerAdapterBitsPerSample
                                    .setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                            spnBitsPerSample.setAdapter(spinnerAdapterBitsPerSample);

                            ArrayAdapter<AudioType> spinnerAdapterAudioType = new ArrayAdapter<AudioType>(
                                    adapter.getContext(),
                                    android.R.layout.simple_spinner_item, AudioType.values());
                            spinnerAdapterAudioType.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                            spnAudioType.setAdapter(spinnerAdapterAudioType);

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
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
                                        msg.setMuteAudio(chkMuteAudio.isChecked());
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
                            builder.show();
                        }

                        /**
                         * Opens the dialog for AddSubMenu message and sends it.
                         */
                        private void sendAddSubmenu() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.addsubmenu,
                                    (ViewGroup) findViewById(R.id.submenu_Root));

                            final EditText editMenuName = (EditText) layout.findViewById(R.id.addsubmenu_menuName);
                            final EditText editMenuID = (EditText) layout.findViewById(R.id.addsubmenu_menuID);
                            final CheckBox chkUseMenuPos = (CheckBox) layout.findViewById(R.id.addsubmenu_useMenuPos);
                            final EditText editMenuPos = (EditText) layout.findViewById(R.id.addsubmenu_menuPos);

                            // set suggested value
                            editMenuID.setText(String.valueOf(submenucmdID++));

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    String subMenuIDString = editMenuID.getText().toString();
                                    int subMenuID = -1;
                                    try {
                                        subMenuID = Integer.parseInt(subMenuIDString);
                                    } catch (NumberFormatException e) {
                                        Toast.makeText(mContext, "Couldn't parse number " + subMenuIDString,
                                                Toast.LENGTH_LONG).show();
                                        return;
                                    }

                                    int pos = -1;
                                    if (chkUseMenuPos.isChecked()) {
                                        String posString = editMenuPos.getText().toString();
                                        try {
                                            pos = Integer.parseInt(posString);
                                        } catch (NumberFormatException e) {
                                            Toast.makeText(mContext, "Couldn't parse number " + posString,
                                                    Toast.LENGTH_LONG).show();
                                            return;
                                        }
                                    }

                                    AddSubMenu msg = new AddSubMenu();
                                    msg.setCorrelationID(autoIncCorrId++);

                                    SyncSubMenu sm = new SyncSubMenu();
                                    sm.setName(editMenuName.getText().toString());
                                    sm.setSubMenuId(subMenuID);
                                    msg.setMenuID(sm.getSubMenuId());
                                    msg.setMenuName(sm.getName());
                                    if (chkUseMenuPos.isChecked()) {
                                        msg.setPosition(pos);
                                    }

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
                            builder.show();
                        }

                        /**
                         * Opens the dialog for AddCommand message and sends it.
                         */
                        private void sendAddCommand() {
                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.addcommand,
                                    (ViewGroup) findViewById(R.id.itemRoot));

                            final EditText editCmdID = (EditText) layout.findViewById(R.id.addcommand_commandID);
                            final EditText er = (EditText) layout.findViewById(R.id.addcommand_commandName);
                            final CheckBox chkUseVrSynonyms = (CheckBox) layout.findViewById(R.id.addcommand_useVRSynonyms);
                            final EditText editVrSynonyms = (EditText) layout.findViewById(R.id.addcommand_vrSynonym);
                            final CheckBox chkUseParentID = (CheckBox) layout.findViewById(R.id.addcommand_useParentID);
                            final Spinner s = (Spinner) layout.findViewById(R.id.addcommand_availableSubmenus);
                            s.setAdapter(_submenuAdapter);
                            final CheckBox chkUseMenuPos = (CheckBox) layout.findViewById(R.id.addcommand_useMenuPos);
                            final EditText editMenuPos = (EditText) layout.findViewById(R.id.addcommand_menuPos);
                            final CheckBox chkUseIcon = (CheckBox) layout.findViewById(R.id.addcommand_useIcon);
                            final EditText editIconValue = (EditText) layout.findViewById(R.id.addcommand_iconValue);
                            final Spinner spnIconType = (Spinner) layout.findViewById(R.id.addcommand_iconType);

                            // set suggested value
                            editCmdID.setText(String.valueOf(itemcmdID++));

                            spnIconType.setAdapter(imageTypeAdapter);
                            spnIconType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    String cmdIDString = editCmdID.getText().toString();
                                    int cmdID = -1;
                                    try {
                                        cmdID = Integer.parseInt(cmdIDString);
                                    } catch (NumberFormatException e) {
                                        Toast.makeText(mContext, "Couldn't parse number " + cmdIDString, Toast.LENGTH_LONG)
                                                .show();
                                        return;
                                    }

                                    int pos = -1;
                                    if (chkUseMenuPos.isChecked()) {
                                        String posString = editMenuPos.getText().toString();
                                        try {
                                            pos = Integer.parseInt(posString);
                                        } catch (NumberFormatException e) {
                                            Toast.makeText(mContext, "Couldn't parse number " + posString,
                                                    Toast.LENGTH_LONG).show();
                                            return;
                                        }
                                    }

                                    AddCommand msg = new AddCommand();
                                    msg.setCorrelationID(autoIncCorrId++);
                                    msg.setCmdID(cmdID);

                                    String itemText = er.getText().toString();
                                    MenuParams menuParams = new MenuParams();
                                    menuParams.setMenuName(itemText);
                                    if (chkUseMenuPos.isChecked()) {
                                        menuParams.setPosition(pos);
                                    }
                                    if (chkUseParentID.isChecked()) {
                                        SyncSubMenu sm = (SyncSubMenu) s.getSelectedItem();
                                        if (sm != null) {
                                            menuParams.setParentID(sm.getSubMenuId());
                                        }
                                    }
                                    msg.setMenuParams(menuParams);

                                    if (chkUseVrSynonyms.isChecked()) {
                                        msg.setVrCommands(new Vector<String>(Arrays.asList(
                                                editVrSynonyms.getText().toString().split(JOIN_STRING))));
                                    }

                                    if (chkUseIcon.isChecked()) {
                                        Image icon = new Image();
                                        icon.setValue(editIconValue.getText().toString());
                                        icon.setImageType((ImageType) spnIconType.getSelectedItem());
                                        msg.setCmdIcon(icon);
                                    }

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
                            builder.show();
                        }

                        /**
                         * Opens the dialog for SyncPData or EncodedSyncPData message and sends it.
                         * @param sendEncoded true to send EncodedSyncPData message; SyncPData otherwise
                         */
                        private void sendSyncPData(final Boolean sendEncoded) {
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
                                        RPCRequest request = null;
                                        if (sendEncoded) {
                                            String base64Data = Base64.encodeBytes(data);
                                            EncodedSyncPData msg = new EncodedSyncPData();
                                            Vector<String> syncPData = new Vector<String>();
                                            syncPData.add(base64Data);
                                            msg.setData(syncPData);
                                            msg.setCorrelationID(autoIncCorrId++);
                                            request = msg;
                                        } else {
                                            SyncPData msg = new SyncPData();
                                            msg.setCorrelationID(autoIncCorrId++);
                                            msg.setBulkData(data);
                                            request = msg;
                                        }

                                        _msgAdapter.logMessage(request, true);

                                        try {
                                            ProxyService.getInstance().getProxyInstance().sendRPCRequest(request);
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

                            spnTurnIconType.setAdapter(imageTypeAdapter);
                            spnTurnIconType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

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

                                        if (chkUseTurnIcon.isChecked()) {
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

                            Spinner[] spinners = {image1Type, image2Type, image3Type};
                            for (Spinner spinner : spinners) {
                                spinner.setAdapter(imageTypeAdapter);
                                spinner.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));
                            }

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    Vector<Choice> commands = new Vector<Choice>();

                                    if (choice1.isChecked()) {
                                        Choice one = new Choice();
                                        one.setChoiceID(autoIncChoiceSetIdCmdId++);
                                        one.setMenuName(command1.getText().toString());
                                        one.setVrCommands(new Vector<String>(Arrays.asList(
                                                vr1.getText().toString().split(JOIN_STRING))));
                                        if (image1Check.isChecked()) {
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
                                        two.setVrCommands(new Vector<String>(Arrays.asList(
                                                vr2.getText().toString().split(JOIN_STRING))));
                                        if (image2Check.isChecked()) {
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
                                        three.setVrCommands(new Vector<String>(Arrays.asList(
                                                vr3.getText().toString().split(JOIN_STRING))));
                                        if (image3Check.isChecked()) {
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
                            final Spinner graphicType = (Spinner) layout.findViewById(R.id.show_graphicType);
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

                            graphicType.setAdapter(imageTypeAdapter);
                            graphicType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

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
                                    IntentHelper
                                            .addObjectForKey(currentSoftButtons, Const.INTENTHELPER_KEY_SOFTBUTTONSLIST);
                                    Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                    intent.putExtra(Const.INTENT_KEY_SOFTBUTTONS_MAXNUMBER, SHOW_MAXSOFTBUTTONS);
                                    startActivityForResult(intent, Const.REQUEST_LIST_SOFTBUTTONS);
                                }
                            });

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
                                        if (mainField3Check.isChecked()) {
                                            msg.setMainField3(mainField3.getText().toString());
                                        }
                                        if (mainField4Check.isChecked()) {
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
                                        if (graphicCheck.isChecked()) {
                                            Image image = new Image();
                                            image.setImageType((ImageType) graphicType.getSelectedItem());
                                            image.setValue(graphic.getText().toString());
                                            msg.setGraphic(image);
                                        }
                                        if (chkIncludeSoftButtons.isChecked() &&
                                                (currentSoftButtons != null) &&
                                                (currentSoftButtons.size() > 0)) {
                                            msg.setSoftButtons(currentSoftButtons);
                                        }
                                        currentSoftButtons = null;
                                        chkIncludeSoftButtons = null;
                                        if (customPresetsCheck.isChecked()) {
                                            String[] customPresetsList = customPresets.getText().
                                                    toString().split(JOIN_STRING);
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

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
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

                                        if (vrHelpItemCheck.isChecked()) {
                                            Vector<VrHelpItem> vrHelpItems = new Vector<VrHelpItem>();

                                            String[] itemTextArray = vrHelpItemText.getText().toString().split(JOIN_STRING);
                                            String[] itemPosArray = vrHelpItemPos.getText().toString().split(JOIN_STRING);
                                            String[] itemImageArray = vrHelpItemImage.getText().toString()
                                                    .split(JOIN_STRING);
                                            int itemsCount = Math.min(itemTextArray.length,
                                                    Math.min(itemPosArray.length, itemImageArray.length));

                                            for (int i = 0; i < itemsCount; ++i) {
                                                VrHelpItem item = new VrHelpItem();
                                                item.setText(itemTextArray[i]);

                                                try {
                                                    item.setPosition(Integer.parseInt(itemPosArray[i]));
                                                } catch (NumberFormatException e) {
                                                    // set default position
                                                    item.setPosition(1);
                                                }

                                                Image image = new Image();
                                                image.setValue(itemImageArray[i]);
                                                image.setImageType(ImageType.DYNAMIC);
                                                item.setImage(image);

                                                vrHelpItems.add(item);
                                            }

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
                                    for (String stringChunk : string.split(JOIN_STRING)) {
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
                                        Vector<Turn> tarray = new Vector<Turn>();

                                        String[] iconNames = iconListString.split(JOIN_STRING);
                                        String[] turnNames = turnListString.split(JOIN_STRING);
                                        int turnCount = Math.max(iconNames.length, turnNames.length);

                                        for (int i = 0; i < turnCount; ++i) {
                                            Turn t = new Turn();
                                            t.setNavigationText((i < turnNames.length) ? turnNames[i] : "");
                                            Image ti = new Image();
                                            ti.setValue((i < iconNames.length) ? iconNames[i] : "");
                                            ti.setImageType(ImageType.DYNAMIC);
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

                            final CheckBox chkDynamicFooter = (CheckBox) layout.findViewById(R.id.slider_chkDynamicFooter);
                            chkDynamicFooter.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                                @Override
                                public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                                    if (!isChecked) {
                                        // set default static text
                                        txtSliderFooter.setText(R.string.slider_footer);
                                    } else {
                                        updateDynamicFooter(txtNumTicks, txtSliderFooter, JOIN_STRING);
                                    }
                                }
                            });

                            txtNumTicks.setOnFocusChangeListener(new OnFocusChangeListener() {
                                @Override
                                public void onFocusChange(View v, boolean hasFocus) {
                                    if ((!hasFocus) && chkDynamicFooter.isChecked()) {
                                        updateDynamicFooter(txtNumTicks, txtSliderFooter, JOIN_STRING);
                                    }
                                }
                            });

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    try {
                                        if (chkDynamicFooter.isChecked()) {
                                            updateDynamicFooter(txtNumTicks, txtSliderFooter, JOIN_STRING);
                                        }

                                        Slider msg = new Slider();
                                        msg.setTimeout(Integer.parseInt(txtTimeout.getText().toString()));
                                        msg.setNumTicks(Integer.parseInt(txtNumTicks.getText().toString()));
                                        msg.setSliderHeader(txtSliderHeader.getText().toString());

                                        Vector<String> footerelements = null;
                                        String footer = txtSliderFooter.getText().toString();
                                        if (chkDynamicFooter.isChecked()) {
                                            footerelements = new Vector<String>(Arrays.asList(footer.split(JOIN_STRING)));
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

                                    final Map<VehicleDataType, String> methodNamesMap =
                                            new HashMap<VehicleDataType, String>() {{
                                                put(VehicleDataType.VEHICLEDATA_GPS, "Gps");
                                                put(VehicleDataType.VEHICLEDATA_SPEED, "Speed");
                                                put(VehicleDataType.VEHICLEDATA_RPM, "Rpm");
                                                put(VehicleDataType.VEHICLEDATA_FUELLEVEL, "FuelLevel");
                                                put(VehicleDataType.VEHICLEDATA_FUELLEVEL_STATE, "FuelLevel_State");
                                                put(VehicleDataType.VEHICLEDATA_INSTANTFUELCONSUMPTION,
                                                        "InstantFuelConsumption");
                                                put(VehicleDataType.VEHICLEDATA_EXTERNTEMP, "ExternalTemperature");
                                                put(VehicleDataType.VEHICLEDATA_PRNDLSTATUS, "Prndl");
                                                put(VehicleDataType.VEHICLEDATA_TIREPRESSURE, "TirePressure");
                                                put(VehicleDataType.VEHICLEDATA_ODOMETER, "Odometer");
                                                put(VehicleDataType.VEHICLEDATA_BELTSTATUS, "BeltStatus");
                                                put(VehicleDataType.VEHICLEDATA_BODYINFORMATION, "BodyInformation");
                                                put(VehicleDataType.VEHICLEDATA_DEVICESTATUS, "DeviceStatus");
                                                put(VehicleDataType.VEHICLEDATA_DRIVERBRAKING, "DriverBraking");
                                                put(VehicleDataType.VEHICLEDATA_WIPERSTATUS, "WiperStatus");
                                                put(VehicleDataType.VEHICLEDATA_HEADLAMPSTATUS, "HeadLampStatus");
                                                put(VehicleDataType.VEHICLEDATA_ENGINETORQUE, "EngineTorque");
                                                put(VehicleDataType.VEHICLEDATA_ACCPEDALPOSITION, "AccPedalPosition");
                                                put(VehicleDataType.VEHICLEDATA_STEERINGWHEELANGLE, "SteeringWheelAngle");
                                                put(VehicleDataType.VEHICLEDATA_ECALLINFO, "ECallInfo");
                                                put(VehicleDataType.VEHICLEDATA_AIRBAGSTATUS, "AirbagStatus");
                                                put(VehicleDataType.VEHICLEDATA_EMERGENCYEVENT, "EmergencyEvent");
                                                put(VehicleDataType.VEHICLEDATA_CLUSTERMODESTATUS, "ClusterModeStatus");
                                                put(VehicleDataType.VEHICLEDATA_MYKEY, "MyKey");
                                            }};

                                    if (!subscribeVehicleData.isEmpty()) {
                                        try {
                                            SubscribeVehicleData msg = new SubscribeVehicleData();
                                            for (VehicleDataType vdt : subscribeVehicleData) {
                                                setVehicleDataParam(msg, SubscribeVehicleData.class,
                                                        "set" + methodNamesMap.get(vdt));
                                            }
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
                                            for (VehicleDataType vdt : unsubscribeVehicleData) {
                                                setVehicleDataParam(msg, UnsubscribeVehicleData.class,
                                                        "set" + methodNamesMap.get(vdt));
                                            }
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
                            final String[] vehicleDataTypeNames = new String[]{
                                    "GPS", "Speed", "RPM", "Fuel Level", "Fuel Level State",
                                    "Instant Fuel Consumption", "External Temp", "PRNDL Status",
                                    "Tire Pressure", "Odometer", "Belt Status", "Body Information",
                                    "Device Status", "Driver Braking", "Wiper Status", "Head Lamp Status",
                                    "Engine Torque", "Acc Pedal Position", "Steering Wheel Angle",
                                    "ECall Info", "Airbag Status", "Emergency Event", "Cluster Mode Status", "MyKey"
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
                            final EditText vrHelpItemPos = (EditText) layout.findViewById(R.id.setglobalproperties_vrHelpItemPos);
                            final CheckBox choiceHelpPrompt = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceHelpPrompt);
                            final CheckBox choiceTimeoutPrompt = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceTimeoutPrompt);
                            final CheckBox choiceVRHelpTitle = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceVRHelpTitle);
                            final CheckBox choiceVRHelpItem = (CheckBox) layout.findViewById(R.id.setglobalproperties_choiceVRHelpItem);

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    SetGlobalProperties msg = new SetGlobalProperties();
                                    int numberOfChoices = 0;

                                    if (choiceHelpPrompt.isChecked()) {
                                        Vector<TTSChunk> help = new Vector<TTSChunk>();
                                        String helpString = helpPrompt.getText().toString();
                                        for (String ttsChunk : helpString.split(JOIN_STRING)) {
                                            TTSChunk chunk = TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, ttsChunk);
                                            help.add(chunk);
                                        }
                                        msg.setHelpPrompt(help);
                                        ++numberOfChoices;
                                    }

                                    if (choiceTimeoutPrompt.isChecked()) {
                                        Vector<TTSChunk> timeout = new Vector<TTSChunk>();
                                        String timeoutString = timeoutPrompt.getText().toString();
                                        for (String ttsChunk : timeoutString.split(JOIN_STRING)) {
                                            TTSChunk chunk = TTSChunkFactory.createChunk(SpeechCapabilities.TEXT, ttsChunk);
                                            timeout.add(chunk);
                                        }
                                        msg.setTimeoutPrompt(timeout);
                                        ++numberOfChoices;
                                    }

                                    if (choiceVRHelpTitle.isChecked()) {
                                        msg.setVrHelpTitle(vrHelpTitle.getText().toString());
                                        ++numberOfChoices;
                                    }

                                    if (choiceVRHelpItem.isChecked()) {
                                        Vector<VrHelpItem> vrHelpItems = new Vector<VrHelpItem>();

                                        String[] itemTextArray = vrHelpItemText.getText().toString().split(JOIN_STRING);
                                        String[] itemPosArray = vrHelpItemPos.getText().toString().split(JOIN_STRING);
                                        String[] itemImageArray = vrHelpItemImage.getText().toString()
                                                .split(JOIN_STRING);
                                        int itemsCount = Math.min(itemTextArray.length,
                                                Math.min(itemPosArray.length, itemImageArray.length));

                                        for (int i = 0; i < itemsCount; ++i) {
                                            VrHelpItem item = new VrHelpItem();
                                            item.setText(itemTextArray[i]);

                                            try {
                                                item.setPosition(Integer.parseInt(itemPosArray[i]));
                                            } catch (NumberFormatException e) {
                                                // set default position
                                                item.setPosition(1);
                                            }

                                            Image image = new Image();
                                            image.setValue(itemImageArray[i]);
                                            image.setImageType(ImageType.DYNAMIC);
                                            item.setImage(image);

                                            vrHelpItems.add(item);
                                        }

                                        msg.setVrHelp(vrHelpItems);
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

                                    if (choiceVRHelpTitle.isChecked()) {
                                        properties.add(GlobalProperty.VRHELPTITLE);
                                    }

                                    if (choiceVRHelpItem.isChecked()) {
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
                         * Opens the dialog for SetDisplayLayout message and sends it.
                         */
                        private void sendSetDisplayLayout() {
                            Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.setdisplaylayout,
                                    (ViewGroup) findViewById(R.id.setdisplaylayout_itemRoot));

                            final EditText editDisplayLayout = (EditText) layout.findViewById(R.id.setdisplaylayout_displayLayout);

                            AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    SetDisplayLayout msg = new SetDisplayLayout();
                                    msg.setCorrelationID(autoIncCorrId++);
                                    msg.setDisplayLayout(editDisplayLayout.getText().toString());
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
     * @param cmdID     ID of the new command
     * @param submenuID ID of the command's parent submenu
     */
    private void addCommandToList(Integer cmdID, Integer submenuID) {
        _commandAdapter.add(cmdID);
        if (null != submenuID) {
            _commandIdToParentSubmenuMap.put(cmdID, submenuID);
        }
    }

    /**
     * Removes command ID from the adapter.
     *
     * @param cmdID ID of the command
     */
    private void removeCommandFromList(Integer cmdID) {
        _commandAdapter.remove(cmdID);
        _commandIdToParentSubmenuMap.remove(cmdID);
    }

    //upon onDestroy(), dispose current proxy and create a new one to enable auto-start
    //call resetProxy() to do so
    public void endSyncProxyInstance() {
        ProxyService serviceInstance = ProxyService.getInstance();
        if (serviceInstance != null) {
            SyncProxyALM proxyInstance = serviceInstance.getProxyInstance();
            //if proxy exists, reset it
            if (proxyInstance != null) {
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

    public void setTesterMain(ModuleTest _instance) {
        this._testerMain = _instance;
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
                        .entrySet().iterator(); it.hasNext(); ) {
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

    /**
     * Returns whether the external storage is available for writing.
     */
    private boolean isExtStorageWritable() {
        String state = Environment.getExternalStorageState();
        return Environment.MEDIA_MOUNTED.equals(state);
    }

    /**
     * Called when a connection to a SYNC device has been closed.
     */
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
     * Opens a dialog so that the user can select an XML test or a directory
     * with XML tests. The result will come to onActivityResult method.
     */
    public void openXmlFilePathDialog() {
        Intent intent = new Intent(this, FileDialog.class);
        String sdcardPath = Environment.getExternalStorageDirectory().getPath();
        intent.putExtra(FileDialog.START_PATH, sdcardPath);
        intent.putExtra(FileDialog.CAN_SELECT_DIR, true);
        intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
        intent.putExtra(FileDialog.FORMAT_FILTER, new String[]{"xml"});
        startActivityForResult(intent, REQUEST_CHOOSE_XML_TEST);
    }

    public void startVideoSession(View v) {
        toggleStartStopVideoSessionButtons();
    }

    public void stopVideoSession(View v) {
        toggleStartStopVideoSessionButtons();
    }

    private void toggleStartStopVideoSessionButtons() {
        Button startSessionButton = (Button) this.findViewById(R.id.btnStartVideoSession);
        Button stopSessionButton = (Button) this.findViewById(R.id.btnStopVideoSession);
        if (startSessionButton.isEnabled()) {
            startSessionButton.setEnabled(false);
            stopSessionButton.setEnabled(true);
        } else {
            startSessionButton.setEnabled(true);
            stopSessionButton.setEnabled(false);
        }
    }

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
}

