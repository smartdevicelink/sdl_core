package com.ford.syncV4.android.activity;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.DialogFragment;
import android.bluetooth.BluetoothAdapter;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Environment;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentTransaction;
import android.util.Pair;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.CheckedTextView;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.Toast;

import com.ford.syncV4.android.R;
import com.ford.syncV4.android.activity.mobilenav.AudioServicePreviewFragment;
import com.ford.syncV4.android.activity.mobilenav.MobileNavPreviewFragment;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.constants.Const;
import com.ford.syncV4.android.constants.MenuConstants;
import com.ford.syncV4.android.constants.SyncSubMenu;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.module.GenericRequest;
import com.ford.syncV4.android.policies.PoliciesTesterActivity;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.android.utils.AppUtils;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCMessage;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.RPCResponse;
import com.ford.syncV4.proxy.TTSChunkFactory;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuver;
import com.ford.syncV4.proxy.rpc.ChangeRegistration;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.DiagnosticMessage;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.EndAudioPassThru;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.Image;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetDisplayLayout;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.SoftButton;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.proxy.rpc.TTSChunk;
import com.ford.syncV4.proxy.rpc.Turn;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.VrHelpItem;
import com.ford.syncV4.proxy.rpc.enums.ButtonName;
import com.ford.syncV4.proxy.rpc.enums.GlobalProperty;
import com.ford.syncV4.proxy.rpc.enums.ImageType;
import com.ford.syncV4.proxy.rpc.enums.InteractionMode;
import com.ford.syncV4.proxy.rpc.enums.Language;
import com.ford.syncV4.proxy.rpc.enums.LayoutMode;
import com.ford.syncV4.proxy.rpc.enums.SoftButtonType;
import com.ford.syncV4.proxy.rpc.enums.SpeechCapabilities;
import com.ford.syncV4.proxy.rpc.enums.SystemAction;
import com.ford.syncV4.proxy.rpc.enums.TextAlignment;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;
import com.ford.syncV4.transport.TransportType;
import com.ford.syncV4.util.Base64;
import com.ford.syncV4.util.logger.Logger;
import com.lamerman.FileDialog;
import com.lamerman.SelectionMode;

import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Comparator;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Vector;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 08.05.14
 * Time: 15:45
 */
public class PlaceholderFragment extends Fragment {

    // NO STATIC fields must be declared here except the ones described values
    // The reason is that this class represent each instance of the SyncProxyTester application

    /**
     * Placeholder for the Application Id field
     */
    public static final String EMPTY_APP_ID = "";

    private static final String LOG_TAG = PlaceholderFragment.class.getSimpleName();
    private static final String MOBILE_NAV_FRAGMENT_TAG = "MOBILE_NAV_FRAGMENT_TAG";
    private static final String AUDIO_FRAGMENT_TAG = "AUDIO_FRAGMENT_TAG";
    private final static String APP_SETUP_DIALOG_TAG = "AppSetupDialogTag";
    private final static String POLICY_FILES_SETUP_DIALOG_TAG = "PolicyFilesSetupDialogTag";
    private final static String PUT_FILE_DIALOG_TAG = "PutFileDialogTag";
    private final static String ADD_COMMAND_DIALOG_TAG = "AddCommandDialogTag";
    private final static String SEND_SINGLE_RPC_COMMAND_DIALOG_TAG = "SendSingleRPCCommandDialogTag";
    private final static String PERFORM_AUDIO_PASS_THRU_DIALOG_TAG = "PerformAudioPassThruDialogTag";
    private final static String SYSTEM_REQST_DIALOG_TAG = "SystemRequestDialogTag";
    private final static String ADD_SUB_MENU_DIALOG_TAG = "AddSubMenuDialogTag";
    private final static String SET_GLOBAL_PROPERTIES_DIALOG_TAG = "SetGlobalPropertiesDialogTag";
    private final static String SUBSCRIPTION_VEHICLE_DATA_DIALOG_TAG = "SubscriptionVehicleDataDialogTag";
    private final static String REGISTER_APP_INTERFACE_DIALOG_TAG = "RegisterAppInterfaceDialogTag";
    private final static String HASH_ID_SET_UP_DIALOG_TAG = "HashIdSetUpDialogTag";
    private final static String FEEDBACK_DIALOG_TAG = "FeedbackDialogTag";
    private static final String MSC_PREFIX = "msc_";

    private static final int UPDATETURNLIST_MAXSOFTBUTTONS = 1;
    private static final int ALERT_MAXSOFTBUTTONS = 4;
    private static final int SCROLLABLEMESSAGE_MAXSOFTBUTTONS = 8;
    private static final int ALERTMANEUVER_MAXSOFTBUTTONS = 3;
    private static final int SHOWCONSTANTTBT_MAXSOFTBUTTONS = 3;
    private static final int CREATECHOICESET_MAXCHOICES = 100;
    private static final int SHOW_MAXSOFTBUTTONS = 8;
    private static final int CHOICESETID_UNSET = -1;

    // Request id for SoftButtonsListActivity
    private static final int REQUEST_LIST_SOFTBUTTONS = 43;
    // Request id for ChoiceListActivity
    public static final int REQUEST_LIST_CHOICES = 45;

    /**
     * Autoincrementing id for new choices.
     */
    private static int autoIncChoiceId = 9000;

    private boolean isFirstActivityRun = true;

    private ArrayAdapter<VehicleDataType> mVehicleDataType = null;
    private LogAdapter mLogAdapter;
    private ArrayAdapter<ButtonName> mButtonAdapter = null;
    private ArrayAdapter<SyncSubMenu> mSubmenuAdapter = null;
    private ArrayAdapter<Integer> mCommandAdapter = null;
    private ArrayAdapter<Integer> mChoiceSetAdapter = null;
    private ArrayAdapter<String> mPutFileAdapter = null;

    private final Map<Integer, Integer> mCommandIdToParentSubmenuMap =
            new ConcurrentHashMap<Integer, Integer>();

    private MobileNavPreviewFragment mMobileNavPreviewFragment;
    private AudioServicePreviewFragment mAudioServicePreviewFragment;

    private boolean[] isButtonSubscribed = null;
    private boolean[] isVehicleDataSubscribed = null;
    private int mAutoIncCorrId = 0;
    private int autoIncChoiceSetId = 1;
    private int mTabId = 0;
    /**
     * Application Id associated with the Fragment
     */
    private String mAppId = "";
    private Integer mLatestDeleteCommandCmdID = null;
    /**
     * Reference to PutFile dialog's local filename text field, so that the
     * filename is set after choosing.
     */
    private EditText txtLocalFileName;

    /**
     * Latest SyncSubMenu, required to delete the submenu from the adapter
     * when a successful DeleteSubMenuResponse comes.
     */
    private SyncSubMenu _latestDeleteSubmenu = null;
    /**
     * Stores the number of selections of each message to sort them by
     * most-popular usage.
     *
     * Make it temporary public to access from the Fragment
     */
    public Map<String, Integer> messageSelectCount;
    /**
     * Latest choiceSetId, required to add it to the adapter when a successful
     * CreateInteractionChoiceSetResponse comes.
     */
    private int mLatestCreateChoiceSetId = CHOICESETID_UNSET;
    /**
     * Latest choiceSetId, required to delete it from the adapter when a
     * successful DeleteInteractionChoiceSetResponse comes.
     */
    private int _latestDeleteChoiceSetId = CHOICESETID_UNSET;
    /**
     * The Include Soft Buttons checkbox in the current dialog. Kept here to
     * check it when the user has explicitly set the soft buttons.
     */
    private CheckBox chkIncludeSoftButtons;
    /**
     * List of soft buttons for current function. Passed between
     * {@link SoftButtonsListActivity} and this activity.
     */
    private Vector<SoftButton> currentSoftButtons;
    /**
     * Latest SyncSubMenu, required to add the submenu from the adapter when a
     * successful AddSubMenuResponse comes.
     */
    private SyncSubMenu mLatestAddSubmenu = null;
    private Pair<Integer, Integer> mLatestAddCommand = null;

    /**
     * Returns a new instance of this fragment for the given section number.
     */
    public static PlaceholderFragment newInstance(int sectionNumber) {
        PlaceholderFragment fragment = new PlaceholderFragment();
        Bundle args = new Bundle();
        args.putInt(Const.ARG_APP_ID, sectionNumber);
        fragment.setArguments(args);
        return fragment;
    }

    public PlaceholderFragment() {
        mMobileNavPreviewFragment = new MobileNavPreviewFragment();
        mAudioServicePreviewFragment = new AudioServicePreviewFragment();

        Logger.d(LOG_TAG + " Constructor");
    }

    public static int getNewChoiceId() {
        return autoIncChoiceId++;
    }

    /**
     * Return Application Id associated with the specific Fragment
     * @return Application Id
     */
    public String getAppId() {
        return mAppId;
    }

    /**
     * Set Application Id for the specific Fragment
     * @param value Application Id
     */
    public void setAppId(String value) {
        mAppId = value;
        mLogAdapter.setAppId(mAppId);
        mMobileNavPreviewFragment.setAppId(mAppId);
        mAudioServicePreviewFragment.setAppId(mAppId);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        mTabId = getArguments().getInt(Const.ARG_APP_ID);
        String autoIncCorrId = mTabId + "000";
        mAutoIncCorrId = Integer.valueOf(autoIncCorrId);

        Logger.d(LOG_TAG + " On Create, init corId:" + mAutoIncCorrId + ", AppId:" + mAppId);

        mVehicleDataType = new ArrayAdapter<VehicleDataType>(getActivity(),
                android.R.layout.simple_spinner_item, VehicleDataType.values());
        mVehicleDataType.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mLogAdapter = new LogAdapter("SyncProxyTester", false, R.layout.row, new ArrayList<Object>());

        if (isFirstActivityRun) {
            int fragmentsCounter = ((SyncProxyTester) getActivity()).getFragmentsCount();
            BaseDialogFragment appSetupDialogFragment =
                    BaseDialogFragment.newInstance(AppSetUpDialog.class.getName(), getAppId(),
                                                   fragmentsCounter <= 1);
            appSetupDialogFragment.show(getActivity().getFragmentManager(), APP_SETUP_DIALOG_TAG);
            //appSetupDialogFragment.setCancelable(false);
        } else {
            ((SyncProxyTester) getActivity()).onSetUpDialogResult(getAppId());
        }
        isFirstActivityRun = false;
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        Logger.d(LOG_TAG + " On Create View");

        setHasOptionsMenu(true);

        View rootView = inflater.inflate(R.layout.fragment_main, container, false);

        rootView.findViewById(R.id.btnSendMessage).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                sendMessageListener();
            }
        });
        rootView.findViewById(R.id.btnPlayPause).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                audioButtonListener();
            }
        });

        ListView listView = (ListView) rootView.findViewById(R.id.messageList);
        listView.setClickable(true);
        listView.setAdapter(mLogAdapter);
        listView.setTranscriptMode(ListView.TRANSCRIPT_MODE_NORMAL);
        listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                listClickHandler(parent.getItemAtPosition(position));
            }
        });

        resetAdapters();

        /*FragmentTransaction transaction = getChildFragmentManager().beginTransaction();
        transaction.replace(R.id.video_fragment_holder, mMobileNavPreviewFragment,
                MOBILE_NAV_FRAGMENT_TAG);
        transaction.replace(R.id.audio_fragment_holder, mAudioServicePreviewFragment,
                AUDIO_FRAGMENT_TAG);
        transaction.commit();*/

        return rootView;
    }

    @Override
    public void onActivityCreated(Bundle savedInstanceState) {
        super.onActivityCreated(savedInstanceState);

        Logger.d(LOG_TAG + " On Activity Created " + mAppId);
    }

    @Override
    public void onResume() {
        super.onResume();

        Logger.d(LOG_TAG + " On Resume " + mAppId);

        FragmentTransaction transaction = getChildFragmentManager().beginTransaction();
        transaction.replace(R.id.video_fragment_holder, mMobileNavPreviewFragment,
                MOBILE_NAV_FRAGMENT_TAG);
        transaction.replace(R.id.audio_fragment_holder, mAudioServicePreviewFragment,
                AUDIO_FRAGMENT_TAG);
        transaction.commit();
    }

    @Override
    public void onPause() {
        super.onPause();

        Logger.d(LOG_TAG + " On Pause " + mAppId);

        saveMessageSelectCount();

        FragmentTransaction transaction = getChildFragmentManager().beginTransaction();
        transaction.remove(mMobileNavPreviewFragment);
        transaction.remove(mAudioServicePreviewFragment);
        transaction.commit();
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();

        Logger.d(LOG_TAG + " On Destroy View " + mAppId);
    }

    @Override
    public void onDetach() {
        super.onDetach();

        Logger.d(LOG_TAG + " On Detach " + mAppId);
    }

    @Override
    public void onCreateOptionsMenu(Menu menu, MenuInflater inflater) {
        super.onCreateOptionsMenu(menu, inflater);

        int menuSize = menu.size();
        int tabsCount = ((SyncProxyTester) getActivity()).getFragmentsCount();
        int nextMenuItemOrder = menu.getItem(menuSize - 1).getOrder() + 1;
        menu.add(0, MenuConstants.MENU_PROXY_START, nextMenuItemOrder++, "Proxy Start");
        menu.add(0, MenuConstants.MENU_TOGGLE_CONSOLE, nextMenuItemOrder++, "Toggle Console");
        menu.add(0, MenuConstants.MENU_CLEAR, nextMenuItemOrder++, "Clear Messages");
        //if (tabsCount == 1) {
            menu.add(0, MenuConstants.MENU_EXIT, nextMenuItemOrder++, "Exit");
        //}
        menu.add(0, MenuConstants.MENU_CLOSE_SESSION, nextMenuItemOrder++, "Close Session");
        //menu.add(0, MENU_TOGGLE_MEDIA, nextMenuItemOrder++, "Toggle Media");
        menu.add(0, MenuConstants.MENU_HASH_ID_SETUP, nextMenuItemOrder++, "HashId setup");
        menu.add(0, MenuConstants.MENU_CLEAR_FUNCTIONS_USAGE, nextMenuItemOrder++, "Reset functions usage");
        if (tabsCount == 1) {
            menu.add(0, MenuConstants.MENU_XML_TEST, nextMenuItemOrder++, "XML Test");
        }
        menu.add(0, MenuConstants.MENU_POLICIES_TEST, nextMenuItemOrder++, "Policies Test");
        menu.add(0, MenuConstants.MENU_SET_UP_POLICY_FILES, nextMenuItemOrder++, "Set Up Policy files");
        MenuItem menuitem = menu.add(0, MenuConstants.MENU_WAKE_LOCK, nextMenuItemOrder++, "Lock screen while testing");
        menuitem.setCheckable(true);
        menuitem.setChecked(!AppPreferencesManager.getDisableLockFlag());
        menu.add(0, MenuConstants.MENU_FEEDBACK, nextMenuItemOrder, "Feedback");
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {

        switch (item.getItemId()) {
            case MenuConstants.MENU_PROXY_START:
                if (AppPreferencesManager.getTransportType() == TransportType.BLUETOOTH) {
                    BluetoothAdapter mBtAdapter = BluetoothAdapter.getDefaultAdapter();
                    if (!mBtAdapter.isEnabled()) {
                        mBtAdapter.enable();
                    }

                    if (!mBtAdapter.isDiscovering()) {
                        Intent discoverableIntent = new Intent(
                                BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
                        discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION,
                                300);
                        startActivity(discoverableIntent);
                    }
                }

                // TODO : To be reconsider
                //if (mBoundProxyService == null) {
                //    bindProxyService(this, mProxyServiceConnectionProxy);
                //} else {
                //    mBoundProxyService.addLogAdapter(mLogAdapter);
                //}

                ExecutorService executorService = Executors.newSingleThreadExecutor();
                executorService.submit(new Runnable() {
                    @Override
                    public void run() {
                        /*if (mBoundProxyService != null) {

                            // We need to set listener to null and then re-init it, unless there will
                            // be another way to check it at 'reset()' method

                            if (!mBoundProxyService.isSyncProxyConnected()) {
                                mBoundProxyService.getTestConfig().setDoCallRegisterAppInterface(false);
                            }
                            mBoundProxyService.setProxyServiceEvent(null);

                            mBoundProxyService.reset();

                            // Re-init listener

                            mBoundProxyService.setProxyServiceEvent(SyncProxyTester.this);
                        }*/
                    }
                });
                return true;
            case MenuConstants.MENU_XML_TEST:
                openXmlFilePathDialog();
                break;
            case MenuConstants.MENU_POLICIES_TEST:
                if (PoliciesTesterActivity.getInstance() == null) {
                    Intent intent = new Intent(getActivity(), PoliciesTesterActivity.class);
                    intent.addFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                    intent.addFlags(Intent.FLAG_ACTIVITY_NO_USER_ACTION);
                    intent.putExtra(BaseDialogFragment.APP_ID_KEY, getAppId());
                    startActivity(intent);
                    if (SyncProxyTester.getESN() != null) {
                        PoliciesTesterActivity.setESN(SyncProxyTester.getESN());
                        PoliciesTesterActivity.setAppId(getAppId());
                    }
                }
                //PoliciesTest.runPoliciesTest();
                break;
            case MenuConstants.MENU_SET_UP_POLICY_FILES:
                BaseDialogFragment mPolicyFilesSetUpDialog = BaseDialogFragment.newInstance(
                        PolicyFilesSetUpDialog.class.getName(), getAppId());
                mPolicyFilesSetUpDialog.show(getActivity().getFragmentManager(),
                        POLICY_FILES_SETUP_DIALOG_TAG);
                break;
            case MenuConstants.MENU_EXIT:
                ((SyncProxyTester) getActivity()).stopProxyServiceOnExit();
                break;
            case MenuConstants.MENU_TOGGLE_CONSOLE:
                ListView listView = (ListView) getView().findViewById(R.id.messageList);
                if (listView.getVisibility() == ScrollView.GONE) {
                    listView.setVisibility(ListView.VISIBLE);
                } else {
                    listView.setVisibility(ListView.GONE);
                }
                return true;
            case MenuConstants.MENU_CLEAR:
                mLogAdapter.clear();
                return true;
            case MenuConstants.MENU_TOGGLE_MEDIA:
                SharedPreferences settings = getActivity().getSharedPreferences(Const.PREFS_NAME, 0);
                boolean isMediaApp = settings.getBoolean(Const.PREFS_KEY_ISMEDIAAPP,
                        Const.PREFS_DEFAULT_ISMEDIAAPP);
                SharedPreferences.Editor editor = settings.edit();
                editor.putBoolean(Const.PREFS_KEY_ISMEDIAAPP, !isMediaApp);
                editor.commit();
                return true;
            case MenuConstants.MENU_CLOSE_SESSION:
                try {
                    ((SyncProxyTester) getActivity()).closeSession(mAppId, mTabId - 1);
                } catch (SyncException e) {
                    Logger.e("Close Session error:" + e.getMessage());
                }
                return true;
            case MenuConstants.MENU_CLEAR_FUNCTIONS_USAGE:
                clearMessageSelectCount();
                break;
            case MenuConstants.MENU_WAKE_LOCK:
                AppPreferencesManager.toggleDisableLock();
                break;
            case MenuConstants.MENU_HASH_ID_SETUP:
                BaseDialogFragment hashIdSetUpDialog = BaseDialogFragment.newInstance(
                        HashIdSetUpDialog.class.getName(), getAppId());
                hashIdSetUpDialog.show(getActivity().getFragmentManager(), HASH_ID_SET_UP_DIALOG_TAG);
                break;
            case MenuConstants.MENU_FEEDBACK:
                DialogFragment feedbackDialog = FeedbackDialog.newInstance();
                feedbackDialog.show(getActivity().getFragmentManager(), FEEDBACK_DIALOG_TAG);
                break;
        }

        return false;
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        Logger.i(LOG_TAG + " OnActivityResult, request:" + requestCode + ", result:" + resultCode +
                ", data:" + data);
        switch (requestCode) {
            case REQUEST_LIST_SOFTBUTTONS:
                if (resultCode == Activity.RESULT_OK) {
                    currentSoftButtons = (Vector<SoftButton>) IntentHelper.
                            getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);

                    if (chkIncludeSoftButtons != null) {
                        chkIncludeSoftButtons.setChecked(true);
                    }
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;
            case REQUEST_LIST_CHOICES:
                if (resultCode == Activity.RESULT_OK) {
                    Vector<Choice> choices = (Vector<Choice>) IntentHelper.
                            getObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                    sendCreateInteractionChoiceSet(choices);
                }
                IntentHelper.removeObjectForKey(Const.INTENTHELPER_KEY_OBJECTSLIST);
                break;
            case Const.REQUEST_FILE_OPEN:
                if (resultCode == Activity.RESULT_OK) {
                    String fileName = data.getStringExtra(FileDialog.RESULT_PATH);
                    if (txtLocalFileName != null) {
                        txtLocalFileName.setText(fileName);
                    }
                }
                break;
            case Const.REQUEST_CHOOSE_XML_TEST:
                if (resultCode == Activity.RESULT_OK) {
                    String filePath = data.getStringExtra(FileDialog.RESULT_PATH);
                    Logger.d("XML test path:" + filePath);
                    if (filePath != null) {
                        SyncProxyTester syncProxyTester = (SyncProxyTester) getActivity();
                        if (syncProxyTester == null) {
                            return;
                        }
                        syncProxyTester.xmlTestContinue(getAppId(), filePath);
                    }
                }
                break;
            default:
                Logger.w(LOG_TAG + " Unknown request code: " + requestCode);
                break;
        }
    }

    /**
     * Called when the app is activated from HMI for the first time. ProxyService
     * automatically subscribes to buttons, so we reflect that in the subscription list.
     */
    public void buttonsSubscribed(Vector<ButtonName> buttons) {
        List<ButtonName> buttonNames = Arrays.asList(ButtonName.values());
        for (ButtonName buttonName : buttons) {
            isButtonSubscribed[buttonNames.indexOf(buttonName)] = true;
        }
    }

    /**
     * Return a clone of the {@code isVehicleDataSubscribed}
     *
     * @return a clone of the {@code isVehicleDataSubscribed}
     */
    public boolean[] cloneIsVehicleDataSubscribed() {
        return isVehicleDataSubscribed.clone();
    }

    /**
     * @param position position in the array
     * @return
     */
    public boolean getIsVehicleDataSubscribedAt(int position) {
        return isVehicleDataSubscribed[position];
    }

    /**
     * Set a value of the {@code isVehicleDataSubscribed} array
     *
     * @param value
     */
    public void setIsVehicleDataSubscribed(boolean[] value) {
        isVehicleDataSubscribed = value;
    }

    public ArrayAdapter<SyncSubMenu> getSubMenuAdapter() {
        return mSubmenuAdapter;
    }

    public ArrayAdapter<String> getPutFileAdapter() {
        return mPutFileAdapter;
    }

    public LogAdapter getLogAdapter() {
        return mLogAdapter;
    }

    /**
     * Return the next correlation id
     *
     * @return int
     */
    public int getCorrelationId() {
        return mAutoIncCorrId++;
    }

    public void setAudioServiceStateOn(OutputStream outputStream) {
        if (mAudioServicePreviewFragment != null) {
            mAudioServicePreviewFragment.setAudioServiceStateOn(outputStream);
        }
    }

    public void setMobileNaviStateOn(OutputStream outputStream) {
        if (mMobileNavPreviewFragment != null) {
            mMobileNavPreviewFragment.setMobileNaviStateOn(outputStream);
        }
    }

    public void setAudioServiceStateOff() {
        if (mAudioServicePreviewFragment != null) {
            mAudioServicePreviewFragment.setStateOff();
        }
    }

    public void setMobileNaviStateOff() {
        if (mMobileNavPreviewFragment != null) {
            mMobileNavPreviewFragment.setStateOff();
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.AddCommandDialog}
     *
     * @param addCommand {@link com.ford.syncV4.proxy.rpc.AddCommand}
     */
    public void onAddCommandDialogResult(AddCommand addCommand) {
        if (mLatestAddCommand != null) {
            Logger.w("Latest addCommand should be null, but it is " + mLatestAddCommand.first +
                    " / " + mLatestAddCommand.second);
        }
        Integer parentID = null;
        if (addCommand.getMenuParams() != null) {
            parentID = addCommand.getMenuParams().getParentID();
        }
        mLatestAddCommand = new Pair<Integer, Integer>(addCommand.getCmdID(), parentID);
    }

    /**
     * Called when a AddCommandResponse comes. If successful, add it to the
     * adapter.
     */
    public void onAddCommandResponse(boolean success) {
        if (mLatestAddCommand != null) {
            if (success) {
                mCommandAdapter.add(mLatestAddCommand.first);
                if (null != mLatestAddCommand.second) {
                    mCommandIdToParentSubmenuMap.put(mLatestAddCommand.first,
                            mLatestAddCommand.second);
                }
            }
            mLatestAddCommand = null;
        } else {
            Logger.w("Latest addCommand is null");
        }
    }

    /**
     * Called when a DeleteCommandResponse comes. If successful, remove it from
     * the adapter.
     */
    public void onDeleteCommandResponse(boolean success) {
        if (mLatestDeleteCommandCmdID != null) {
            if (success) {
                mCommandAdapter.remove(mLatestDeleteCommandCmdID);
                mCommandIdToParentSubmenuMap.remove(mLatestDeleteCommandCmdID);
            }
            mLatestDeleteCommandCmdID = null;
        } else {
            Logger.w("Latest deleteCommand is unset");
        }
    }

    private void listClickHandler(Object listObj) {
        if (listObj instanceof RPCMessage) {
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
            String rawJSON;

            Integer corrId = -1;
            if (listObj instanceof RPCRequest) {
                corrId = ((RPCRequest) listObj).getCorrelationID();
            } else if (listObj instanceof RPCResponse) {
                corrId = ((RPCResponse) listObj).getCorrelationID();
            }

            try {
                byte protocolVersion =
                        ((SyncProxyTester) getActivity()).mBoundProxyService
                                .syncProxyGetWiProVersion();
                rawJSON = ((RPCMessage) listObj).serializeJSON(protocolVersion).toString(2);
                builder.setTitle("Raw JSON" + (corrId != -1 ?
                        " (Corr ID " + corrId + ")" : ""));
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
            AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
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

    /**
     * Initializes/resets the adapters keeping created submenus, interaction
     * choice set ids, etc.
     */
    public void resetAdapters() {
        // set up storage for subscription records

        isButtonSubscribed = new boolean[ButtonName.values().length];
        mButtonAdapter = new ArrayAdapter<ButtonName>(getActivity(),
                android.R.layout.select_dialog_multichoice, ButtonName.values()) {
            public View getView(int position, View convertView, ViewGroup parent) {
                CheckedTextView ret = (CheckedTextView) super.getView(position, convertView, parent);
                ret.setChecked(isButtonSubscribed[position]);
                return ret;
            }
        };

        isVehicleDataSubscribed = new boolean[VehicleDataType.values().length];

        mSubmenuAdapter = new ArrayAdapter<SyncSubMenu>(getActivity(),
                android.R.layout.select_dialog_item);
        mSubmenuAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mCommandAdapter = new ArrayAdapter<Integer>(getActivity(),
                android.R.layout.select_dialog_item);
        mCommandAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mCommandIdToParentSubmenuMap.clear();

        mChoiceSetAdapter = new ArrayAdapter<Integer>(getActivity(),
                android.R.layout.simple_spinner_item);
        mChoiceSetAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        mPutFileAdapter = new ArrayAdapter<String>(getActivity(),
                android.R.layout.select_dialog_item);
        mPutFileAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
    }

    /**
     * Called when a CreateChoiceSetResponse comes. If successful, add it to the
     * adapter. In any case, remove the key from the map.
     */
    public void onCreateChoiceSetResponse(boolean success) {
        if (mLatestCreateChoiceSetId != CHOICESETID_UNSET) {
            if (success) {
                mChoiceSetAdapter.add(mLatestCreateChoiceSetId);
            }
            mLatestCreateChoiceSetId = CHOICESETID_UNSET;
        } else {
            Logger.w("Latest createChoiceSetId is unset");
        }
    }

    /**
     * Called when a DeleteChoiceSetResponse comes. If successful, remove it
     * from the adapter.
     */
    public void onDeleteChoiceSetResponse(boolean success) {
        if (_latestDeleteChoiceSetId != CHOICESETID_UNSET) {
            if (success) {
                mChoiceSetAdapter.remove(_latestDeleteChoiceSetId);
            }
            _latestDeleteChoiceSetId = CHOICESETID_UNSET;
        } else {
            Logger.w("Latest deleteChoiceSetId is unset");
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
                mSubmenuAdapter.remove(_latestDeleteSubmenu);

                for (Iterator<Map.Entry<Integer, Integer>> it = mCommandIdToParentSubmenuMap
                        .entrySet().iterator(); it.hasNext(); ) {
                    Map.Entry<Integer, Integer> entry = it.next();
                    if (entry.getValue() == _latestDeleteSubmenu.getSubMenuId()) {
                        mCommandAdapter.remove(entry.getKey());
                        it.remove();
                    }
                }
            }
            _latestDeleteSubmenu = null;
        } else {
            Logger.w("Latest deleteSubMenu is unset");
        }
    }

    /**
     * This is a callback function for the result of the
     * {@link com.ford.syncV4.android.activity.AddSubMenuDialog}
     *
     * @param addSubMenu  {@link com.ford.syncV4.android.activity.AddSubMenuDialog} request
     * @param syncSubMenu SubMenu structure
     */
    public void onAddSubMenuDialogResult(AddSubMenu addSubMenu, SyncSubMenu syncSubMenu) {
        if (mLatestAddSubmenu != null) {
            Logger.w("Latest AddSubMenu should be null, but equals to " + mLatestAddSubmenu);
        }
        mLatestAddSubmenu = syncSubMenu;
    }

    /**
     * Called when a AddSubMenuResponse comes. If successful, add it to the
     * adapter.
     */
    public void onAddSubMenuResponse(boolean success) {
        if (mLatestAddSubmenu != null) {
            if (success) {
                mSubmenuAdapter.add(mLatestAddSubmenu);
            }
            mLatestAddSubmenu = null;
        } else {
            Logger.w("Latest addSubMenu is unset");
        }
    }

    private void sendCreateInteractionChoiceSet(Vector<Choice> choices) {
        ProxyService boundProxyService = ((SyncProxyTester) getActivity()).mBoundProxyService;
        if (boundProxyService != null) {
            int choiceSetID = autoIncChoiceSetId++;
            boundProxyService.commandCreateInteractionChoiceSetResumable(getAppId(), choices,
                    choiceSetID, getCorrelationId());

            if (mLatestCreateChoiceSetId != CHOICESETID_UNSET) {
                Logger.w("Latest createChoiceSetId should be unset, but equals to " +
                        mLatestCreateChoiceSetId);
            }
            mLatestCreateChoiceSetId = choiceSetID;
        }
    }

    private void audioButtonListener() {
        ((SyncProxyTester) getActivity()).mBoundProxyService.playPauseAnnoyingRepetitiveAudio();
    }

    private void sendMessageListener() {
        final ArrayAdapter<String> adapter = ((SyncProxyTester) getActivity()).getFunctionsAdapter();

        loadMessageSelectCount();

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

        final SyncProxyTester syncProxyTester = (SyncProxyTester) getActivity();
        if (syncProxyTester == null) {
            return;
        }
        final ProxyService boundProxyService = syncProxyTester.mBoundProxyService;
        if (boundProxyService == null) {
            return;
        }
        final ArrayAdapter<ImageType> imageTypeAdapter =
                ((SyncProxyTester) getActivity()).getImageTypeAdapter();

        new AlertDialog.Builder(getActivity())
                .setTitle("Pick a Function")
                .setAdapter(adapter, new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        if (adapter.getItem(which).equals(Names.Alert)) {
                            sendAlert();
                        } else if (adapter.getItem(which).equals(Names.Speak)) {
                            //something
                            AlertDialog.Builder builder;
                            AlertDialog dlg;

                            Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
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
                                    msg.setCorrelationID(getCorrelationId());
                                    String speak1 = txtSpeakText1.getText().toString();
                                    String speak2 = txtSpeakText2.getText().toString();
                                    String speak3 = txtSpeakText3.getText().toString();
                                    String speak4 = txtSpeakText4.getText().toString();
                                    Vector<TTSChunk> chunks = new Vector<TTSChunk>();

                                    if (speak1.length() > 0) {
                                        chunks.add(TTSChunkFactory
                                                .createChunk((SpeechCapabilities) spnSpeakType1
                                                        .getSelectedItem(), speak1));
                                    }
                                    if (speak2.length() > 0) {
                                        chunks.add(TTSChunkFactory
                                                .createChunk((SpeechCapabilities) spnSpeakType2
                                                        .getSelectedItem(), speak2));
                                    }
                                    if (speak3.length() > 0) {
                                        chunks.add(TTSChunkFactory
                                                .createChunk((SpeechCapabilities) spnSpeakType3
                                                        .getSelectedItem(), speak3));
                                    }
                                    if (speak4.length() > 0) {
                                        chunks.add(TTSChunkFactory
                                                .createChunk(SpeechCapabilities.SAPI_PHONEMES,
                                                        speak4));
                                    }
                                    msg.setTtsChunks(chunks);
                                    sendRPCRequestToProxy(msg);
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
                        } else if (adapter.getItem(which).equals(Names.Show)) {
                            sendShow();
                        } else if (adapter.getItem(which).equals(SyncProxyTester.ButtonSubscriptions)) {
                            //something
                            AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(mButtonAdapter, new DialogInterface.OnClickListener() {

                                public void onClick(DialogInterface dialog, int which) {
                                    boolean needToSubscribe = !isButtonSubscribed[which];
                                    ButtonName buttonName = ButtonName.values()[which];
                                    int corrId = getCorrelationId();
                                    if (needToSubscribe) {
                                        boundProxyService.commandSubscribeButtonResumable(getAppId(),
                                                buttonName, corrId);
                                    } else {
                                        UnsubscribeButton msg = new UnsubscribeButton();
                                        msg.setCorrelationID(corrId);
                                        msg.setButtonName(buttonName);
                                        mLogAdapter.logMessage(msg, true);
                                        sendRPCRequestToProxy(msg);
                                    }
                                    isButtonSubscribed[which] = !isButtonSubscribed[which];
                                }
                            });
                            AlertDialog dlg = builder.create();
                            dlg.show();
                        } else if (adapter.getItem(which).equals(Names.AddCommand)) {
                            sendAddCommand();
                        } else if (adapter.getItem(which).equals(Names.SystemRequest)) {
                            sendSystemRequest();
                        } else if (adapter.getItem(which).equals(Names.DeleteCommand)) {
                            sendDeleteCommand();
                        } else if (adapter.getItem(which).equals(Names.AddSubMenu)) {
                            sendAddSubmenu();
                        } else if (adapter.getItem(which).equals(Names.DeleteSubMenu)) {
                            sendDeleteSubMenu();
                        } else if (adapter.getItem(which).equals(Names.SetGlobalProperties)) {
                            sendSetGlobalProperties();
                        } else if (adapter.getItem(which).equals(Names.ResetGlobalProperties)) {
                            sendResetGlobalProperties();
                        } else if (adapter.getItem(which).equals(Names.SetMediaClockTimer)) {
                            sendSetMediaClockTimer();
                        } else if (adapter.getItem(which).equals(Names.CreateInteractionChoiceSet)) {
                            sendCreateInteractionChoiceSet();
                        } else if (adapter.getItem(which).equals(Names.DeleteInteractionChoiceSet)) {
                            //something
                            AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(mChoiceSetAdapter, new DialogInterface.OnClickListener() {

                                public void onClick(DialogInterface dialog, int which) {
                                    DeleteInteractionChoiceSet msg = new DeleteInteractionChoiceSet();
                                    msg.setCorrelationID(getCorrelationId());
                                    int commandSetID = mChoiceSetAdapter.getItem(which);
                                    msg.setInteractionChoiceSetID(commandSetID);
                                    sendRPCRequestToProxy(msg);
                                    if (_latestDeleteChoiceSetId != CHOICESETID_UNSET) {
                                        Logger.w("Latest deleteChoiceSetId should be unset, but " +
                                                "equals to " + _latestDeleteChoiceSetId);
                                    }
                                    _latestDeleteChoiceSetId = commandSetID;
                                }
                            });
                            AlertDialog dlg = builder.create();
                            dlg.show();
                        } else if (adapter.getItem(which).equals(Names.PerformInteraction)) {
                            sendPerformInteraction();
                        } else if (adapter.getItem(which).equals(Names.EncodedSyncPData)) {
                            sendSyncPData(true);
                        } else if (adapter.getItem(which).equals(Names.SyncPData)) {
                            sendSyncPData(false);
                        } else if (adapter.getItem(which).equals(Names.Slider)) {
                            sendSlider();
                        } else if (adapter.getItem(which).equals(Names.ScrollableMessage)) {
                            //something
                            AlertDialog.Builder builder;
                            AlertDialog dlg;

                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
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
                            btnSoftButtons.setOnClickListener(new View.OnClickListener() {
                                @Override
                                public void onClick(View v) {
                                    IntentHelper.addObjectForKey(currentSoftButtons,
                                            Const.INTENTHELPER_KEY_OBJECTSLIST);
                                    Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                    intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                            SCROLLABLEMESSAGE_MAXSOFTBUTTONS);
                                    startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                                }
                            });

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    ScrollableMessage msg = new ScrollableMessage();
                                    msg.setCorrelationID(getCorrelationId());
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
                                    sendRPCRequestToProxy(msg);
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
                        } else if (adapter.getItem(which).equals(Names.ChangeRegistration)) {
                            //ChangeRegistration
                            AlertDialog.Builder builder;
                            AlertDialog dlg;

                            Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
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
                                    ChangeRegistration msg = new ChangeRegistration();
                                    msg.setLanguage((Language) spnLanguage.getSelectedItem());
                                    msg.setHmiDisplayLanguage((Language) spnHmiDisplayLanguage.getSelectedItem());
                                    msg.setCorrelationID(getCorrelationId());
                                    sendRPCRequestToProxy(msg);
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
                        } else if (adapter.getItem(which).equals(Names.PutFile)) {
                            BaseDialogFragment putFileDialogFragment = BaseDialogFragment.newInstance(
                                    PutFileDialog.class.getName(), getAppId());
                            putFileDialogFragment.show(getActivity().getFragmentManager(), PUT_FILE_DIALOG_TAG);
                        } else if (adapter.getItem(which).equals(Names.DeleteFile)) {
                            //DeleteFile
                            AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                            builder.setAdapter(mPutFileAdapter, new DialogInterface.OnClickListener() {

                                public void onClick(DialogInterface dialog, int which) {
                                    String syncFileName = mPutFileAdapter.getItem(which);
                                    DeleteFile msg = new DeleteFile();
                                    msg.setSyncFileName(syncFileName);
                                    msg.setCorrelationID(getCorrelationId());
                                    sendRPCRequestToProxy(msg);
                                    mPutFileAdapter.remove(syncFileName);
                                }
                            });
                            AlertDialog dlg = builder.create();
                            dlg.show();
                        } else if (adapter.getItem(which).equals(Names.ListFiles)) {
                            //ListFiles
                            boundProxyService.commandListFiles(getAppId());
                        } else if (adapter.getItem(which).equals(Names.SetAppIcon)) {
                            //SetAppIcon
                            AlertDialog.Builder builder;
                            AlertDialog dlg;

                            Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.setappicon, null);

                            final EditText txtSyncFileName = (EditText) layout.findViewById(R.id.syncFileNameIcon);

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    String syncFileName = txtSyncFileName.getText().toString();
                                    SetAppIcon msg = new SetAppIcon();
                                    msg.setSyncFileName(syncFileName);
                                    msg.setCorrelationID(getCorrelationId());
                                    sendRPCRequestToProxy(msg);
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
                        } else if (adapter.getItem(which).equals(Names.PerformAudioPassThru)) {
                            BaseDialogFragment performAudioPassThruDialog =
                                    BaseDialogFragment.newInstance(
                                            PerformAudioPassThruDialog.class.getName(), getAppId());
                            performAudioPassThruDialog.show(getActivity().getFragmentManager(),
                                    PERFORM_AUDIO_PASS_THRU_DIALOG_TAG);
                        } else if (adapter.getItem(which).equals(Names.EndAudioPassThru)) {
                            //EndAudioPassThru
                            EndAudioPassThru msg = new EndAudioPassThru();
                            msg.setCorrelationID(getCorrelationId());
                            sendRPCRequestToProxy(msg);
                        } else if (adapter.getItem(which).equals(SyncProxyTester.VehicleDataSubscriptions)) {
                            sendVehicleDataSubscriptions();
                        } else if (adapter.getItem(which).equals(Names.GetVehicleData)) {
                            sendGetVehicleData();
                        } else if (adapter.getItem(which).equals(Names.ReadDID)) {
                            //ReadDID
                            AlertDialog.Builder builder;
                            AlertDialog dlg;

                            final Context mContext = adapter.getContext();
                            LayoutInflater inflater = (LayoutInflater) mContext
                                    .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                            View layout = inflater.inflate(R.layout.readdid, null);

                            final EditText txtECUNameDID = (EditText) layout.findViewById(R.id.txtECUNameDID);
                            final EditText txtDIDLocation = (EditText) layout.findViewById(R.id.txtDIDLocation);

                            builder = new AlertDialog.Builder(mContext);
                            builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int id) {
                                    try {
                                        Vector<Integer> didlocations = new Vector<Integer>();
                                        didlocations.add(Integer.parseInt(txtDIDLocation.getText().toString()));
                                        ReadDID msg = new ReadDID();
                                        msg.setEcuName(Integer.parseInt(txtECUNameDID.getText().toString()));
                                        msg.setDidLocation(didlocations);
                                        msg.setCorrelationID(getCorrelationId());
                                        sendRPCRequestToProxy(msg);
                                    } catch (NumberFormatException e) {
                                        SafeToast.showToastAnyThread("Couldn't parse number");
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
                        } else if (adapter.getItem(which).equals(Names.GetDTCs)) {
                            sendGetDTCs();
                        } else if (adapter.getItem(which).equals(Names.ShowConstantTBT)) {
                            sendShowConstantTBT();
                        } else if (adapter.getItem(which).equals(Names.AlertManeuver)) {
                            sendAlertManeuver();
                        } else if (adapter.getItem(which).equals(Names.UpdateTurnList)) {
                            sendUpdateTurnList();
                        } else if (adapter.getItem(which).equals(Names.SetDisplayLayout)) {
                            sendSetDisplayLayout();
                        } else if (adapter.getItem(which).equals(Names.UnregisterAppInterface)) {
                            sendUnregisterAppInterface();
                        } else if (adapter.getItem(which).equals(Names.RegisterAppInterface)) {
                            sendRegisterAppInterface();
                        } else if (adapter.getItem(which).equals(Names.DiagnosticMessage)) {
                            sendDiagnosticMessage();
                        } else if (adapter.getItem(which).equals(GenericRequest.NAME)) {
                            sendGenericRequest();
                        }

                        String function = adapter.getItem(which);
                        Integer curCount = messageSelectCount.get(function);
                        if (curCount == null) {
                            curCount = 0;
                        }
                        messageSelectCount.put(function, curCount + 1);

                        saveMessageSelectCount();
                    }

                    private void sendAlertManeuver() {
                        final Context mContext = adapter.getContext();
                        LayoutInflater inflater = (LayoutInflater) mContext
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.alertmaneuver, null);

                        final EditText txtTtsChunks = (EditText) layout.findViewById(R.id.txtTtsChunks);
                        final CheckBox useSoftButtons = (CheckBox) layout.findViewById(R.id.alertManeuver_chkIncludeSBs);

                        SoftButton sb1 = new SoftButton();
                        sb1.setSoftButtonID(
                                SyncProxyTester.getNewSoftButtonId());
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
                        btnSoftButtons.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                IntentHelper.addObjectForKey(
                                        currentSoftButtons,
                                        Const.INTENTHELPER_KEY_OBJECTSLIST);
                                Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                        ALERTMANEUVER_MAXSOFTBUTTONS);
                                startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                            }
                        });

                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                Vector<TTSChunk> ttsChunks = new Vector<TTSChunk>();
                                String ttsChunksString = txtTtsChunks.getText().toString();
                                for (String ttsChunk : ttsChunksString.split(SyncProxyTester.JOIN_STRING)) {
                                    TTSChunk chunk = TTSChunkFactory.createChunk(
                                            SpeechCapabilities.TEXT,
                                            ttsChunk);
                                    ttsChunks.add(chunk);
                                }

                                if (!ttsChunks.isEmpty()) {
                                    AlertManeuver msg = new AlertManeuver();
                                    msg.setTtsChunks(ttsChunks);
                                    msg.setCorrelationID(getCorrelationId());
                                    if (useSoftButtons.isChecked()) {
                                        if (currentSoftButtons != null) {
                                            msg.setSoftButtons(currentSoftButtons);
                                        } else {
                                            msg.setSoftButtons(new Vector<SoftButton>());
                                        }
                                    }
                                    currentSoftButtons = null;
                                    sendRPCRequestToProxy(msg);
                                } else {
                                    SafeToast.showToastAnyThread("No TTS Chunks entered");
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

                    /**
                     * Opens the UI for DeleteCommand and sends it.
                     */
                    private void sendDeleteCommand() {
                        AlertDialog.Builder builder = new AlertDialog.Builder(adapter.getContext());
                        builder.setAdapter(mCommandAdapter, new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                DeleteCommand msg = new DeleteCommand();
                                msg.setCorrelationID(getCorrelationId());
                                int cmdID = mCommandAdapter.getItem(which);
                                msg.setCmdID(cmdID);
                                sendRPCRequestToProxy(msg);
                                if (mLatestDeleteCommandCmdID != null) {
                                    Logger.w("Latest deleteCommand should be null, but it is " +
                                            mLatestDeleteCommandCmdID);
                                }
                                mLatestDeleteCommandCmdID = cmdID;
                            }
                        });
                        builder.show();
                    }

                    /**
                     * Sends Alert message.
                     */
                    private void sendAlert() {
                        final Context mContext = adapter.getContext();
                        LayoutInflater inflater = (LayoutInflater) mContext
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.alert, null);
                        final EditText txtSpeak = (EditText) layout.findViewById(R.id.txtSpeak);
                        final EditText txtAlertField1 = (EditText) layout.findViewById(R.id.txtAlertField1);
                        final EditText txtAlertField2 = (EditText) layout.findViewById(R.id.txtAlertField2);
                        final EditText txtAlertField3 = (EditText) layout.findViewById(R.id.txtAlertField3);
                        final EditText txtDuration = (EditText) layout.findViewById(R.id.txtDuration);
                        final CheckBox chkPlayTone = (CheckBox) layout.findViewById(R.id.chkPlayTone);
                        final CheckBox useProgressIndicator = (CheckBox) layout.findViewById(R.id.alert_useProgressIndicator);
                        final CheckBox useDuration = (CheckBox) layout.findViewById(R.id.alert_useDuration);

                        chkIncludeSoftButtons = (CheckBox) layout.findViewById(R.id.chkIncludeSBs);

                        SoftButton sb1 = new SoftButton();
                        sb1.setSoftButtonID(
                                SyncProxyTester.getNewSoftButtonId());
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
                        btnSoftButtons.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                IntentHelper
                                        .addObjectForKey(currentSoftButtons,
                                                Const.INTENTHELPER_KEY_OBJECTSLIST);
                                Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER, ALERT_MAXSOFTBUTTONS);
                                startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                            }
                        });

                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                try {
                                    Alert msg = new Alert();
                                    msg.setCorrelationID(getCorrelationId());
                                    msg.setAlertText1(txtAlertField1.getText().toString());
                                    msg.setAlertText2(txtAlertField2.getText().toString());
                                    msg.setAlertText3(txtAlertField3.getText().toString());
                                    if (useDuration.isChecked()) {
                                        msg.setDuration(Integer.parseInt(txtDuration.getText().toString()));
                                    }
                                    msg.setPlayTone(chkPlayTone.isChecked());
                                    msg.setProgressIndicator(useProgressIndicator.isChecked());

                                    String toSpeak = txtSpeak.getText().toString();
                                    if (toSpeak.length() > 0) {
                                        Vector<TTSChunk> ttsChunks = TTSChunkFactory
                                                .createSimpleTTSChunks(toSpeak);
                                        msg.setTtsChunks(ttsChunks);
                                    }
                                    if (chkIncludeSoftButtons.isChecked() &&
                                            (currentSoftButtons != null) &&
                                            (currentSoftButtons.size() > 0)) {
                                        msg.setSoftButtons(currentSoftButtons);
                                    }
                                    sendRPCRequestToProxy(msg);
                                } catch (NumberFormatException e) {
                                    SafeToast.showToastAnyThread("Couldn't parse number");
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
                        builder.show();
                    }

                    /**
                     * Sends UnregisterAppInterface message.
                     */
                    private void sendUnregisterAppInterface() {
                        UnregisterAppInterface unregisterAppInterface = RPCRequestFactory.buildUnregisterAppInterface();
                        unregisterAppInterface.setCorrelationID(getCorrelationId());

                        SendSingleRPCRequestDialog sendSingleRPCRequestDialog = SendSingleRPCRequestDialog.newInstance();
                        sendSingleRPCRequestDialog.setCommand(unregisterAppInterface);
                        sendSingleRPCRequestDialog.show(getActivity().getFragmentManager(), SEND_SINGLE_RPC_COMMAND_DIALOG_TAG);
                    }

                    /**
                     * Opens the dialog for DeleteSubMenu message and sends it.
                     */
                    private void sendDeleteSubMenu() {
                        AlertDialog.Builder builder =
                                new AlertDialog.Builder(adapter.getContext());
                        builder.setAdapter(mSubmenuAdapter,
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog,
                                                        int which) {
                                        SyncSubMenu menu =
                                                mSubmenuAdapter.getItem(which);
                                        DeleteSubMenu msg = new DeleteSubMenu();
                                        msg.setCorrelationID(getCorrelationId());
                                        msg.setMenuID(menu.getSubMenuId());
                                        sendRPCRequestToProxy(msg);
                                        if (_latestDeleteSubmenu != null) {
                                            Logger.w("Latest deleteSubmenu should be null, but equals to " + _latestDeleteSubmenu);
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
                        builder.setAdapter(mVehicleDataType, new DialogInterface.OnClickListener() {

                            public void onClick(DialogInterface dialog, int which) {
                                GetVehicleData msg = new GetVehicleData();

                                final String[] methodNames =
                                        {"Gps", "Speed", "Rpm",
                                                "FuelLevel",
                                                "FuelLevel_State",
                                                "InstantFuelConsumption",
                                                "ExternalTemperature",
                                                "Vin", "Prndl",
                                                "TirePressure",
                                                "Odometer",
                                                "BeltStatus",
                                                "BodyInformation",
                                                "DeviceStatus",
                                                "DriverBraking",
                                                "WiperStatus",
                                                "HeadLampStatus",
                                                "BatteryVoltage",
                                                "EngineTorque",
                                                "AccPedalPosition",
                                                "SteeringWheelAngle",
                                                "ECallInfo",
                                                "AirbagStatus",
                                                "EmergencyEvent",
                                                "ClusterModeStatus",
                                                "MyKey"};
                                final String setterName = "set" + methodNames[which];
                                ((SyncProxyTester) getActivity()).setVehicleDataParam(msg,
                                        GetVehicleData.class, setterName);

                                msg.setCorrelationID(getCorrelationId());
                                sendRPCRequestToProxy(msg);
                            }
                        });
                        builder.show();
                    }

                    /**
                     * Opens the dialog for GetDTCs message and sends it.
                     */
                    private void sendGetDTCs() {
                        final Context mContext = adapter.getContext();
                        LayoutInflater inflater = (LayoutInflater) mContext
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
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
                                    msg.setCorrelationID(getCorrelationId());
                                    sendRPCRequestToProxy(msg);
                                } catch (NumberFormatException e) {
                                    SafeToast.showToastAnyThread("Couldn't parse number");
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
                        BaseDialogFragment addSubMenuDialogFragment =
                                BaseDialogFragment.newInstance(AddSubMenuDialog.class.getName(),
                                        getAppId());
                        addSubMenuDialogFragment.show(getActivity().getFragmentManager(),
                                ADD_SUB_MENU_DIALOG_TAG);
                    }

                    private void sendSystemRequest(){
                        BaseDialogFragment fragment = BaseDialogFragment.newInstance(
                                SystemRequestDialog.class.getName(), getAppId());
                        fragment.show(getActivity().getFragmentManager(), SYSTEM_REQST_DIALOG_TAG);
                    }

                    /**
                     * Opens the dialog for AddCommand message and sends it.
                     */
                    private void sendAddCommand() {
                        BaseDialogFragment addCommandDialogFragment =
                                BaseDialogFragment.newInstance(AddCommandDialog.class.getName(),
                                        getAppId());
                        addCommandDialogFragment.show(getActivity().getFragmentManager(),
                                ADD_COMMAND_DIALOG_TAG);
                    }

                    /**
                     * Opens the dialog for SyncPData or EncodedSyncPData message and sends it.
                     * @param sendEncoded true to send EncodedSyncPData message; SyncPData otherwise
                     */
                    private void sendSyncPData(final Boolean sendEncoded) {
                        final Context mContext = adapter.getContext();
                        LayoutInflater inflater = (LayoutInflater) mContext
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.encodedsyncpdata, null);

                        txtLocalFileName = (EditText) layout.findViewById(R.id.encodedsyncpdata_localFileName);
                        final Button btnSelectLocalFile = (Button) layout.findViewById(R.id.encodedsyncpdata_selectFileButton);
                        btnSelectLocalFile.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                // show Choose File dialog
                                Intent intent = new Intent(mContext, FileDialog.class);
                                intent.putExtra(FileDialog.START_PATH, "/sdcard");
                                intent.putExtra(FileDialog.CAN_SELECT_DIR, false);
                                intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
                                startActivityForResult(intent, Const.REQUEST_FILE_OPEN);
                            }
                        });

                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int id) {
                                String filename = txtLocalFileName.getText().toString();
                                byte[] data = AppUtils.contentsOfResource(filename);
                                if (data != null) {
                                    RPCRequest request = null;
                                    if (sendEncoded) {
                                        String base64Data = Base64.encodeBytes(data);
                                        EncodedSyncPData msg = new EncodedSyncPData();
                                        Vector<String> syncPData = new Vector<String>();
                                        syncPData.add(base64Data);
                                        msg.setData(syncPData);
                                        msg.setCorrelationID(getCorrelationId());
                                        request = msg;
                                    } else {
                                        SyncPData msg = new SyncPData();
                                        msg.setCorrelationID(getCorrelationId());
                                        msg.setBulkData(data);
                                        request = msg;
                                    }
                                    sendRPCRequestToProxy(request);
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
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.showconstanttbt,
                                (ViewGroup) getActivity().findViewById(R.id.showconstanttbt_Root));

                        final CheckBox useNavigationText1 = (CheckBox) layout.findViewById(R.id.showconstanttbt_useNavigationText1);
                        final EditText txtNavigationText1 = (EditText) layout.findViewById(R.id.showconstanttbt_txtNavigationText1);
                        final CheckBox useNavigationText2 = (CheckBox) layout.findViewById(R.id.showconstanttbt_useNavigationText2);
                        final EditText txtNavigationText2 = (EditText) layout.findViewById(R.id.showconstanttbt_txtNavigationText2);
                        final CheckBox useETA = (CheckBox) layout.findViewById(R.id.showconstanttbt_useETA);
                        final EditText txtEta = (EditText) layout.findViewById(R.id.showconstanttbt_txtEta);
                        final CheckBox useTimeToDestination = (CheckBox) layout.findViewById(R.id.showconstanttbt_useTimeToDestination);
                        final EditText txtTimeToDestination = (EditText) layout.findViewById(R.id.showconstanttbt_txtTimeToDestination);
                        final CheckBox useTotalDistance = (CheckBox) layout.findViewById(R.id.showconstanttbt_useTotalDistance);
                        final EditText txtTotalDistance = (EditText) layout.findViewById(R.id.showconstanttbt_txtTotalDistance);
                        final CheckBox chkUseTurnIcon = (CheckBox) layout.findViewById(R.id.showconstanttbt_turnIconCheck);
                        final Spinner spnTurnIconType = (Spinner) layout.findViewById(R.id.showconstanttbt_turnIconType);
                        final EditText txtTurnIconValue = (EditText) layout.findViewById(R.id.showconstanttbt_turnIconValue);
                        final CheckBox chkUseNextTurnIcon = (CheckBox) layout.findViewById(R.id.showconstanttbt_nextTurnIconCheck);
                        final Spinner spnNextTurnIconType = (Spinner) layout.findViewById(R.id.showconstanttbt_nextTurnIconType);
                        final EditText txtNextTurnIconValue = (EditText) layout.findViewById(R.id.showconstanttbt_nextTurnIconValue);
                        final CheckBox useDistanceToManeuver = (CheckBox) layout.findViewById(R.id.showconstanttbt_useDistanceToManeuver);
                        final EditText txtDistanceToManeuver = (EditText) layout.findViewById(R.id.showconstanttbt_txtDistanceToManeuver);
                        final CheckBox useDistanceToManeuverScale = (CheckBox) layout.findViewById(R.id.showconstanttbt_useDistanceToManeuverScale);
                        final EditText txtDistanceToManeuverScale = (EditText) layout.findViewById(R.id.showconstanttbt_txtDistanceToManeuverScale);
                        final CheckBox useManeuverComplete = (CheckBox) layout.findViewById(R.id.showconstanttbt_useManeuverComplete);
                        final CheckBox chkManeuverComplete = (CheckBox) layout.findViewById(R.id.showconstanttbt_chkManeuverComplete);
                        final CheckBox useSoftButtons = (CheckBox) layout.findViewById(R.id.showconstanttbt_useSoftButtons);

                        spnTurnIconType.setAdapter(imageTypeAdapter);
                        spnTurnIconType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

                        spnNextTurnIconType.setAdapter(imageTypeAdapter);
                        spnNextTurnIconType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

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
                        btnSoftButtons.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                IntentHelper.addObjectForKey(currentSoftButtons,
                                        Const.INTENTHELPER_KEY_OBJECTSLIST);
                                Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                        SHOWCONSTANTTBT_MAXSOFTBUTTONS);
                                startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                            }
                        });

                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                try {
                                    ShowConstantTBT msg = new ShowConstantTBT();

                                    if (useNavigationText1.isChecked()) {
                                        msg.setNavigationText1(txtNavigationText1.getText().toString());
                                    }
                                    if (useNavigationText2.isChecked()) {
                                        msg.setNavigationText2(txtNavigationText2.getText().toString());
                                    }
                                    if (useETA.isChecked()) {
                                        msg.setEta(txtEta.getText().toString());
                                    }
                                    if (useTimeToDestination.isChecked()) {
                                        msg.setTimeToDestination(txtTimeToDestination.getText().toString());
                                    }
                                    if (useTotalDistance.isChecked()) {
                                        msg.setTotalDistance(txtTotalDistance.getText().toString());
                                    }

                                    if (chkUseTurnIcon.isChecked()) {
                                        Image image = new Image();
                                        image.setImageType(imageTypeAdapter.getItem(
                                                spnTurnIconType.getSelectedItemPosition()));
                                        image.setValue(txtTurnIconValue.getText().toString());
                                        msg.setTurnIcon(image);
                                    }

                                    if (chkUseNextTurnIcon.isChecked()) {
                                        Image image = new Image();
                                        image.setImageType(imageTypeAdapter.getItem(
                                                spnNextTurnIconType.getSelectedItemPosition()));
                                        image.setValue(txtNextTurnIconValue.getText().toString());
                                        msg.setNextTurnIcon(image);
                                    }

                                    if (useDistanceToManeuver.isChecked()) {
                                        msg.setDistanceToManeuver((float) Integer.parseInt(txtDistanceToManeuver.getText().toString()));
                                    }
                                    if (useDistanceToManeuverScale.isChecked()) {
                                        msg.setDistanceToManeuverScale((float) Integer.parseInt(txtDistanceToManeuverScale.getText().toString()));
                                    }
                                    if (useManeuverComplete.isChecked()) {
                                        msg.setManeuverComplete(chkManeuverComplete.isChecked());
                                    }
                                    msg.setCorrelationID(getCorrelationId());
                                    if (useSoftButtons.isChecked()) {
                                        if (currentSoftButtons != null) {
                                            msg.setSoftButtons(currentSoftButtons);
                                        } else {
                                            msg.setSoftButtons(new Vector<SoftButton>());
                                        }
                                    }
                                    sendRPCRequestToProxy(msg);
                                } catch (NumberFormatException e) {
                                    SafeToast.showToastAnyThread("Couldn't parse number");
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
                        Choice choice1 = new Choice();
                        choice1.setChoiceID(getNewChoiceId());
                        choice1.setMenuName("super");
                        Vector<String> vrCommands = new Vector<String>();
                        vrCommands.add("super");
                        vrCommands.add("best");
                        choice1.setVrCommands(vrCommands);
                        Image image = new Image();
                        image.setImageType(ImageType.DYNAMIC);
                        image.setValue("turn_left.png");
                        choice1.setImage(image);
                        choice1.setSecondaryText("42");
                        choice1.setTertiaryText("The Cat");

                        Choice choice2 = new Choice();
                        choice2.setChoiceID(getNewChoiceId());
                        choice2.setMenuName("awesome");
                        vrCommands = new Vector<String>();
                        vrCommands.add("magnificent");
                        vrCommands.add("incredible");
                        choice2.setVrCommands(vrCommands);
                        image = new Image();
                        image.setImageType(ImageType.DYNAMIC);
                        image.setValue("action.png");
                        choice2.setImage(image);
                        choice2.setTertiaryText("Schrödinger's cat");

                        Vector<Choice> choices = new Vector<Choice>();
                        choices.add(choice1);
                        choices.add(choice2);

                        IntentHelper.addObjectForKey(choices, Const.INTENTHELPER_KEY_OBJECTSLIST);
                        Intent intent = new Intent(adapter.getContext(), ChoiceListActivity.class);
                        intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                CREATECHOICESET_MAXCHOICES);
                        startActivityForResult(intent, REQUEST_LIST_CHOICES);
                    }

                    /**
                     * Opens the dialog for Show message and sends it.
                     */
                    private void sendShow() {
                        final Context mContext = adapter.getContext();
                        LayoutInflater inflater = (LayoutInflater) mContext
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
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
                        final CheckBox secondaryGraphicCheck = (CheckBox) layout.findViewById(R.id.show_secondaryGraphicCheck);
                        final Spinner secondaryGraphicType = (Spinner) layout.findViewById(R.id.show_secondaryGraphicType);
                        final EditText secondaryGraphic = (EditText) layout.findViewById(R.id.show_secondaryGraphic);
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
                            int visibility = View.GONE;
                            mediaClock.setVisibility(visibility);
                            mediaTrack.setVisibility(visibility);
                            mediaTrackCheck.setVisibility(visibility);
                            mediaClockCheck.setVisibility(visibility);
                        }

                        graphicType.setAdapter(imageTypeAdapter);
                        graphicType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));
                        secondaryGraphicType.setAdapter(imageTypeAdapter);
                        secondaryGraphicType.setSelection(imageTypeAdapter.getPosition(ImageType.DYNAMIC));

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
                        btnSoftButtons.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                IntentHelper
                                        .addObjectForKey(currentSoftButtons, Const.INTENTHELPER_KEY_OBJECTSLIST);
                                Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER, SHOW_MAXSOFTBUTTONS);
                                startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
                            }
                        });

                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                Show msg = new Show();
                                msg.setCorrelationID(getCorrelationId());

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
                                if (secondaryGraphicCheck.isChecked()) {
                                    Image image = new Image();
                                    image.setImageType((ImageType) secondaryGraphicType.getSelectedItem());
                                    image.setValue(secondaryGraphic.getText().toString());
                                    msg.setSecondaryGraphic(image);
                                }
                                if (chkIncludeSoftButtons.isChecked() &&
                                        (currentSoftButtons != null)) {
                                    msg.setSoftButtons(currentSoftButtons);
                                }
                                currentSoftButtons = null;
                                chkIncludeSoftButtons = null;
                                if (customPresetsCheck.isChecked()) {
                                    String[] customPresetsList = customPresets.getText().
                                            toString().split(SyncProxyTester.JOIN_STRING);
                                    msg.setCustomPresets(new Vector<String>(Arrays.
                                            asList(customPresetsList)));
                                }
                                sendRPCRequestToProxy(msg);
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
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);

                        View layout = inflater.inflate(R.layout.performinteraction,
                                (ViewGroup) getActivity().findViewById(R.id.performinteraction_Root));

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
                        final CheckBox interactionLayoutCheck =
                                (CheckBox) layout.findViewById(
                                        R.id.performinteraction_interactionLayoutCheck);
                        final Spinner interactionLayoutSpinner =
                                (Spinner) layout.findViewById(
                                        R.id.performinteraction_interactionLayoutSpinner);

                        final ArrayAdapter<InteractionMode> interactionModeAdapter = new ArrayAdapter<InteractionMode>(
                                mContext, android.R.layout.simple_spinner_item, InteractionMode.values());
                        interactionModeAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
                        interactionModeSpinner.setAdapter(interactionModeAdapter);
                        interactionModeSpinner.setSelection(interactionModeAdapter.getPosition(InteractionMode.BOTH));

                        final ArrayAdapter<LayoutMode>
                                interactionLayoutAdapter =
                                new ArrayAdapter<LayoutMode>(mContext,
                                        android.R.layout.simple_spinner_item,
                                        LayoutMode.values());
                        interactionLayoutAdapter.setDropDownViewResource(
                                android.R.layout.simple_spinner_dropdown_item);
                        interactionLayoutSpinner
                                .setAdapter(interactionLayoutAdapter);

                        final String[] choiceSetIDStrings = new String[mChoiceSetAdapter.getCount()];
                        final boolean[] choiceSetIDSelections = new boolean[choiceSetIDStrings.length];

                        for (int i = 0; i < mChoiceSetAdapter.getCount(); ++i) {
                            choiceSetIDStrings[i] = mChoiceSetAdapter.getItem(i).toString();
                        }

                        choiceSetIDs.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                new AlertDialog.Builder(mContext).
                                        setMultiChoiceItems(choiceSetIDStrings, choiceSetIDSelections, new DialogInterface.OnMultiChoiceClickListener() {
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
                                        choiceSetIDs.add(mChoiceSetAdapter.getItem(i));
                                    }
                                }
                                sendPerformInteractionRequest(choiceSetIDs);
                            }

                            private void sendPerformInteractionRequest(Vector<Integer> choiceSetIDs) {

                                PerformInteraction msg = new PerformInteraction();
                                msg.setCorrelationID(getCorrelationId());
                                msg.setInitialText(initialText.getText().toString());
                                msg.setInitialPrompt(((SyncProxyTester) getActivity()).ttsChunksFromString(initialPrompt.getText().toString()));
                                msg.setInteractionMode(
                                        interactionModeAdapter.getItem(
                                                interactionModeSpinner.getSelectedItemPosition()));
                                msg.setInteractionChoiceSetIDList(choiceSetIDs);

                                if (helpPromptCheck.isChecked()) {
                                    msg.setHelpPrompt(((SyncProxyTester) getActivity())
                                            .ttsChunksFromString(helpPrompt.getText().toString()));
                                }

                                if (timeoutPromptCheck.isChecked()) {
                                    msg.setTimeoutPrompt(((SyncProxyTester) getActivity())
                                            .ttsChunksFromString(timeoutPrompt.getText().toString()));
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

                                    String[] itemTextArray = vrHelpItemText.getText().toString().split(SyncProxyTester.JOIN_STRING);
                                    String[] itemPosArray = vrHelpItemPos.getText().toString().split(SyncProxyTester.JOIN_STRING);
                                    String[] itemImageArray = vrHelpItemImage.getText().toString()
                                            .split(SyncProxyTester.JOIN_STRING);
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

                                if (interactionLayoutCheck.isChecked()) {
                                    msg.setInteractionLayout(interactionLayoutAdapter
                                            .getItem(interactionLayoutSpinner
                                                    .getSelectedItemPosition()));
                                }
                                sendRPCRequestToProxy(msg);
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
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.updateturnlist, null);
                        final EditText txtTurnList = (EditText) layout.findViewById(R.id.updateturnlist_txtTurnList);
                        final EditText txtIconList = (EditText) layout.findViewById(R.id.updateturnlist_txtIconList);
                        final CheckBox useTurnList = (CheckBox) layout.findViewById(R.id.updateturnlist_useTurnList);
                        final CheckBox useIconList = (CheckBox) layout.findViewById(R.id.updateturnlist_useIconList);
                        final CheckBox useSoftButtons = (CheckBox) layout.findViewById(R.id.updateturnlist_chkIncludeSBs);

                        SoftButton sb1 = new SoftButton();
                        sb1.setSoftButtonID(SyncProxyTester.getNewSoftButtonId());
                        sb1.setText("Close");
                        sb1.setType(SoftButtonType.SBT_TEXT);
                        sb1.setIsHighlighted(false);
                        sb1.setSystemAction(SystemAction.DEFAULT_ACTION);
                        currentSoftButtons = new Vector<SoftButton>();
                        currentSoftButtons.add(sb1);

                        Button btnSoftButtons = (Button) layout.findViewById(R.id.updateturnlist_btnSoftButtons);
                        btnSoftButtons.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                IntentHelper.addObjectForKey(currentSoftButtons,
                                        Const.INTENTHELPER_KEY_OBJECTSLIST);
                                Intent intent = new Intent(mContext, SoftButtonsListActivity.class);
                                intent.putExtra(Const.INTENT_KEY_OBJECTS_MAXNUMBER,
                                        UPDATETURNLIST_MAXSOFTBUTTONS);
                                startActivityForResult(intent, REQUEST_LIST_SOFTBUTTONS);
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
                                boolean turnListEnabled = useTurnList.isChecked();
                                boolean iconListEnabled = useIconList.isChecked();
                                String turnListString = txtTurnList.getText().toString();
                                String iconListString = txtIconList.getText().toString();
                                if ((turnListString.length() > 0) || (iconListString.length() > 0)) {
                                    Vector<Turn> tarray = new Vector<Turn>();

                                    String[] iconNames = iconListString.split(SyncProxyTester.JOIN_STRING);
                                    String[] turnNames = turnListString.split(SyncProxyTester.JOIN_STRING);
                                    int turnCount = Math.max(iconNames.length, turnNames.length);

                                    for (int i = 0; i < turnCount; ++i) {
                                        Turn t = new Turn();
                                        if (turnListEnabled) {
                                            t.setNavigationText((i < turnNames.length) ? turnNames[i] : "");
                                        }

                                        if (iconListEnabled) {
                                            Image ti = new Image();
                                            ti.setValue((i < iconNames.length) ? iconNames[i] : "");
                                            ti.setImageType(ImageType.DYNAMIC);
                                            t.setTurnIcon(ti);
                                        }
                                        tarray.add(t);
                                    }
                                    UpdateTurnList msg = new UpdateTurnList();
                                    msg.setCorrelationID(getCorrelationId());
                                    msg.setTurnList(tarray);
                                    if (useSoftButtons.isChecked()) {
                                        if (currentSoftButtons != null) {
                                            msg.setSoftButtons(
                                                    currentSoftButtons);
                                        } else {
                                            msg.setSoftButtons(
                                                    new Vector<SoftButton>());
                                        }
                                    }
                                    currentSoftButtons = null;
                                    sendRPCRequestToProxy(msg);
                                } else {
                                    SafeToast.showToastAnyThread("Both fields are empty, " +
                                            "nothing to send");
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
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.slider, null);
                        final EditText txtNumTicks = (EditText) layout.findViewById(R.id.txtNumTicks);
                        final EditText txtPosititon = (EditText) layout.findViewById(R.id.txtPosition);
                        final EditText txtSliderHeader = (EditText) layout.findViewById(R.id.txtSliderHeader);
                        final EditText txtSliderFooter = (EditText) layout.findViewById(R.id.txtSliderFooter);
                        final CheckBox useTimeout = (CheckBox) layout.findViewById(R.id.slider_useTimeout);
                        final EditText txtTimeout = (EditText) layout.findViewById(R.id.txtTimeout);

                        final CheckBox chkDynamicFooter = (CheckBox) layout.findViewById(R.id.slider_chkDynamicFooter);
                        chkDynamicFooter.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                            @Override
                            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                                if (!isChecked) {
                                    // set default static text
                                    txtSliderFooter.setText(R.string.slider_footer);
                                } else {
                                    updateDynamicFooter(txtNumTicks, txtSliderFooter, SyncProxyTester.JOIN_STRING);
                                }
                            }
                        });

                        /**
                         * Process a possibility to exclude Footer from Slider request
                         */
                        final boolean[] isUseFooterInSlider = {true};
                        final CheckBox useFooterInSliderCheckBox = (CheckBox) layout.findViewById(R.id.use_footer_in_slider_checkbox);
                        isUseFooterInSlider[0] = useFooterInSliderCheckBox.isChecked();
                        useFooterInSliderCheckBox.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
                            @Override
                            public void onCheckedChanged(CompoundButton compoundButton, boolean b) {
                                isUseFooterInSlider[0] = b;
                                chkDynamicFooter.setEnabled(b);
                                txtSliderFooter.setEnabled(b);
                            }
                        });

                        builder = new AlertDialog.Builder(mContext);
                        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                try {
                                    Slider msg = new Slider();
                                    if (useTimeout.isChecked()) {
                                        msg.setTimeout(Integer.parseInt(txtTimeout.getText().toString()));
                                    }
                                    msg.setNumTicks(Integer.parseInt(txtNumTicks.getText().toString()));
                                    msg.setSliderHeader(txtSliderHeader.getText().toString());

                                    /**
                                     * Do not set Footer
                                     */
                                    if (isUseFooterInSlider[0]) {
                                        Vector<String> footerElements;
                                        String footer = txtSliderFooter.getText().toString();
                                        if (chkDynamicFooter.isChecked()) {
                                            footerElements = new Vector<String>(Arrays.asList(footer.split(SyncProxyTester.JOIN_STRING)));
                                        } else {
                                            footerElements = new Vector<String>();
                                            footerElements.add(footer);
                                        }
                                        msg.setSliderFooter(footerElements);
                                    }

                                    msg.setPosition(Integer.parseInt(txtPosititon.getText().toString()));
                                    msg.setCorrelationID(getCorrelationId());
                                    sendRPCRequestToProxy(msg);
                                } catch (NumberFormatException e) {
                                    SafeToast.showToastAnyThread("Couldn't parse number");
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
                        BaseDialogFragment subscriptionsVehicleDataDialog =
                                BaseDialogFragment.newInstance(
                                        SubscriptionsVehicleDataDialog.class.getName(), getAppId());
                        subscriptionsVehicleDataDialog.show(getActivity().getFragmentManager(),
                                SUBSCRIPTION_VEHICLE_DATA_DIALOG_TAG);
                    }

                    private void sendSetGlobalProperties() {
                        BaseDialogFragment setGlobalPropertiesDialog =
                                BaseDialogFragment.newInstance(
                                        SetGlobalPropertiesDialog.class.getName(), getAppId());
                        setGlobalPropertiesDialog.show(getActivity().getFragmentManager(),
                                SET_GLOBAL_PROPERTIES_DIALOG_TAG);
                    }

                    private void sendResetGlobalProperties() {
                        AlertDialog.Builder builder;

                        Context mContext = adapter.getContext();
                        LayoutInflater inflater = (LayoutInflater) mContext
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.resetglobalproperties,
                                (ViewGroup) getActivity().findViewById(R.id.resetglobalproperties_Root));

                        final CheckBox choiceHelpPrompt = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceHelpPrompt);
                        final CheckBox choiceTimeoutPrompt = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceTimeoutPrompt);
                        final CheckBox choiceVRHelpTitle = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceVRHelpTitle);
                        final CheckBox choiceVRHelpItem = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceVRHelpItems);
                        final CheckBox choiceKeyboardProperties = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceKeyboardProperties);
                        final CheckBox choiceMenuIcon = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceMenuIcon);
                        final CheckBox choiceMenuName = (CheckBox) layout.findViewById(R.id.resetglobalproperties_choiceMenuName);

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

                                if (choiceMenuIcon.isChecked()) {
                                    properties.add(GlobalProperty.MENUICON);
                                }

                                if (choiceMenuName.isChecked()) {
                                    properties.add(GlobalProperty.MENUNAME);
                                }

                                if (choiceKeyboardProperties.isChecked()) {
                                    properties.add(GlobalProperty.KEYBOARDPROPERTIES);
                                }

                                if (!properties.isEmpty()) {
                                    msg.setProperties(properties);
                                    msg.setCorrelationID(getCorrelationId());
                                    sendRPCRequestToProxy(msg);
                                } else {
                                    SafeToast.showToastAnyThread("No items selected");
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
                                .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
                        View layout = inflater.inflate(R.layout.setdisplaylayout,
                                (ViewGroup) getActivity().findViewById(R.id.setdisplaylayout_itemRoot));

                        final EditText editDisplayLayout = (EditText) layout.findViewById(R.id.setdisplaylayout_displayLayout);

                        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
                        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                SetDisplayLayout msg = new SetDisplayLayout();
                                msg.setCorrelationID(getCorrelationId());
                                msg.setDisplayLayout(editDisplayLayout.getText().toString());
                                sendRPCRequestToProxy(msg);
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
                        msg.setCorrelationID(getCorrelationId());
                        sendRPCRequestToProxy(msg);
                    }
                })
                .setNegativeButton("Close", null)
                .show();
    }

    private void sendSetMediaClockTimer() {
        AlertDialog.Builder builder;

        Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.setmediaclock, null);
        final EditText txtStartHours = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_startTimeHours);
        final EditText txtStartMinutes = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_startTimeMinutes);
        final EditText txtStartSeconds = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_startTimeSeconds);
        final EditText txtEndHours = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_endTimeHours);
        final EditText txtEndMinutes = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_endTimeMinutes);
        final EditText txtEndSeconds = (EditText) layout.findViewById(
                R.id.setmediaclocktimer_endTimeSeconds);
        final Spinner spnUpdateMode = (Spinner) layout.findViewById(
                R.id.setmediaclocktimer_spnUpdateMode);

        ArrayAdapter<UpdateMode> spinnerAdapter =
                new ArrayAdapter<UpdateMode>(mContext,
                        android.R.layout.simple_spinner_item,
                        UpdateMode.values());
        spinnerAdapter.setDropDownViewResource(
                android.R.layout.simple_spinner_dropdown_item);
        spnUpdateMode.setAdapter(spinnerAdapter);

        builder = new AlertDialog.Builder(mContext);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                SetMediaClockTimer msg = new SetMediaClockTimer();
                msg.setCorrelationID(getCorrelationId());
                UpdateMode updateMode =
                        (UpdateMode) spnUpdateMode.getSelectedItem();
                msg.setUpdateMode(updateMode);

                try {
                    Integer hours = Integer.parseInt(
                            txtStartHours.getText().toString());
                    Integer minutes = Integer.parseInt(
                            txtStartMinutes.getText().toString());
                    Integer seconds = Integer.parseInt(
                            txtStartSeconds.getText().toString());
                    StartTime startTime = new StartTime();
                    startTime.setHours(hours);
                    startTime.setMinutes(minutes);
                    startTime.setSeconds(seconds);
                    msg.setStartTime(startTime);
                } catch (NumberFormatException e) {
                    // skip setting start time if parsing failed
                }

                try {
                    Integer hours =
                            Integer.parseInt(txtEndHours.getText().toString());
                    Integer minutes = Integer.parseInt(
                            txtEndMinutes.getText().toString());
                    Integer seconds = Integer.parseInt(
                            txtEndSeconds.getText().toString());
                    StartTime endTime = new StartTime();
                    endTime.setHours(hours);
                    endTime.setMinutes(minutes);
                    endTime.setSeconds(seconds);
                    msg.setEndTime(endTime);
                } catch (NumberFormatException e) {
                    // skip setting start time if parsing failed
                }
                sendRPCRequestToProxy(msg);
            }
        });
        builder.setNegativeButton("Cancel",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
        builder.setView(layout);
        builder.show();
    }

    private void sendDiagnosticMessage() {
        final Context mContext = getActivity();
        LayoutInflater inflater = (LayoutInflater) mContext.getSystemService(
                Context.LAYOUT_INFLATER_SERVICE);
        View layout = inflater.inflate(R.layout.diagnosticmessage, null);
        final EditText txtTargetID = (EditText) layout.findViewById(
                R.id.diagnosticmessage_txtTargetID);
        final EditText txtMessageLength = (EditText) layout.findViewById(
                R.id.diagnosticmessage_txtMessageLength);
        final EditText txtMessageData = (EditText) layout.findViewById(
                R.id.diagnosticmessage_txtMessageData);
        final CheckBox useTargetID = (CheckBox) layout.findViewById(
                R.id.diagnosticmessage_useTargetID);
        final CheckBox useMessageLength = (CheckBox) layout.findViewById(
                R.id.diagnosticmessage_useMessageLength);
        final CheckBox useMessageData = (CheckBox) layout.findViewById(
                R.id.diagnosticmessage_useMessageData);

        AlertDialog.Builder builder = new AlertDialog.Builder(mContext);
        builder.setPositiveButton("OK", new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int id) {
                try {
                    DiagnosticMessage msg = new DiagnosticMessage();
                    msg.setCorrelationID(getCorrelationId());

                    if (useTargetID.isChecked()) {
                        msg.setTargetID(Integer.valueOf(
                                txtTargetID.getText().toString()));
                    }

                    if (useMessageLength.isChecked()) {
                        msg.setMessageLength(Integer.valueOf(
                                txtMessageLength.getText().toString()));
                    }

                    if (useMessageData.isChecked()) {
                        final String[] msgData = txtMessageData.getText()
                                .toString()
                                .split(SyncProxyTester.JOIN_STRING);
                        final Vector<Integer> data = new Vector<Integer>();
                        for (String s : msgData) {
                            data.add(Integer.valueOf(s));
                        }
                        msg.setMessageData(data);
                    }
                    if (((SyncProxyTester) getActivity()).mBoundProxyService != null) {
                        ((SyncProxyTester) getActivity()).mBoundProxyService
                                .sendRPCRequestWithPreprocess(getAppId(), msg);
                    }
                } catch (NumberFormatException e) {
                    SafeToast.showToastAnyThread("Couldn't parse number");
                }
            }
        });
        builder.setNegativeButton("Cancel",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
        builder.setView(layout);
        builder.show();
    }

    /**
     * Sends RegisterAppInterface message.
     */
    private void sendRegisterAppInterface() {
        BaseDialogFragment registerAppInterfaceDialog =
                BaseDialogFragment.newInstance(RegisterAppInterfaceDialog.class.getName(),
                        getAppId());
        registerAppInterfaceDialog.show(getActivity().getFragmentManager(),
                REGISTER_APP_INTERFACE_DIALOG_TAG);
    }

    private void loadMessageSelectCount() {
        SharedPreferences prefs = getActivity().getSharedPreferences(Const.PREFS_NAME, 0);
        messageSelectCount = new ConcurrentHashMap<String, Integer>();
        for (Map.Entry<String, ?> entry : prefs.getAll().entrySet()) {
            if (entry.getKey().startsWith(MSC_PREFIX)) {
                messageSelectCount.put(entry.getKey().substring(MSC_PREFIX.length()),
                        (Integer) entry.getValue());
            }
        }
    }

    private void saveMessageSelectCount() {
        if (messageSelectCount == null) {
            return;
        }
        SharedPreferences.Editor editor = getActivity().getSharedPreferences(Const.PREFS_NAME, 0).edit();
        for (Map.Entry<String, Integer> entry : messageSelectCount.entrySet()) {
            editor.putInt(MSC_PREFIX + entry.getKey(), entry.getValue());
        }
        editor.commit();
    }

    private void clearMessageSelectCount() {
        messageSelectCount = new ConcurrentHashMap<String, Integer>();
        SharedPreferences prefs = getActivity().getSharedPreferences(Const.PREFS_NAME, 0);
        SharedPreferences.Editor editor = prefs.edit();
        for (Map.Entry<String, ?> entry : prefs.getAll().entrySet()) {
            if (entry.getKey().startsWith(MSC_PREFIX)) {
                editor.remove(entry.getKey());
            }
        }
        editor.commit();
    }

    private boolean getIsMedia() {
        return getActivity().getSharedPreferences(Const.PREFS_NAME, 0).getBoolean(
                Const.PREFS_KEY_ISMEDIAAPP, Const.PREFS_DEFAULT_ISMEDIAAPP);
    }

    /**
     * Opens a dialog so that the user can select an XML test or a directory
     * with XML tests. The result will come to onActivityResult method.
     */
    public void openXmlFilePathDialog() {
        Intent intent = new Intent(getActivity(), FileDialog.class);
        String sdcardPath = Environment.getExternalStorageDirectory().getPath();
        intent.putExtra(FileDialog.START_PATH, sdcardPath);
        intent.putExtra(FileDialog.CAN_SELECT_DIR, true);
        intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
        intent.putExtra(FileDialog.FORMAT_FILTER, new String[]{"xml"});
        startActivityForResult(intent, Const.REQUEST_CHOOSE_XML_TEST);
    }

    private void sendRPCRequestToProxy(RPCRequest rpcRequest) {
        final SyncProxyTester syncProxyTester = (SyncProxyTester) getActivity();
        if (syncProxyTester == null) {
            return;
        }
        final ProxyService boundProxyService = syncProxyTester.mBoundProxyService;
        if (boundProxyService == null) {
            return;
        }
        boundProxyService.sendRPCRequestWithPreprocess(getAppId(), rpcRequest);
    }
}