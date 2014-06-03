package com.ford.syncV4.android.module;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.PowerManager;
import android.os.PowerManager.WakeLock;
import android.util.Log;
import android.util.Pair;
import android.util.Xml;

import com.ford.syncV4.android.MainApp;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.adapters.LogAdapter;
import com.ford.syncV4.android.constants.AcceptedRPC;
import com.ford.syncV4.android.manager.AppPreferencesManager;
import com.ford.syncV4.android.marshaller.CustomJsonRPCMarshaller;
import com.ford.syncV4.android.marshaller.InvalidJsonRPCMarshaller;
import com.ford.syncV4.android.module.reader.BinaryDataReader;
import com.ford.syncV4.android.module.reader.BinaryDataReaderFactory;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.marshal.IJsonRPCMarshaller;
import com.ford.syncV4.marshal.JsonRPCMarshaller;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuver;
import com.ford.syncV4.proxy.rpc.ChangeRegistration;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.EndAudioPassThru;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.ListFiles;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetDisplayLayout;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.SyncPData;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;
import com.ford.syncV4.util.StringUtils;
import com.ford.syncV4.util.logger.Logger;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Hashtable;
import java.util.List;
import java.util.Vector;
import java.util.concurrent.CountDownLatch;

public class ModuleTest {

	/** Log tag for the class. */
	private static final String TAG = ModuleTest.class.getSimpleName();
	/** Specifies whether to display debug info from the XML parser. */
	private static final boolean DISPLAY_PARSER_DEBUG_INFO = false;
	/** Extension of supported test files. */
	private static final String TEST_FILEEXT = ".xml";

    /**
     * Element name defines that this is a test
     */
    private static final String TEST_TAG_NAME_TEST = "test";
    /**
     * Element name defines that this is a type of the test
     */
    private static final String TEST_TAG_NAME_TYPE = "type";
    /**
     * Element name defines that this is a result of the test
     */
    private static final String TEST_TAG_NAME_RESULT = "result";
    /**
     * Element name defines that this is a possibility to show user prompt
     */
    private static final String TEST_TAG_NAME_USER_PROMPT = "userPrompt";
    /**
     * Element name defines possibility to implement some additional actions
     */
    private static final String TEST_TAG_NAME_ACTION = "action";

	/**
	 * The tag name used to specify where to get binary data from (e.g., for
	 * PutFile message).
	 */
	private final static String BINARY_TAG_NAME = "Binary";
    /** Tag name for float numbers. */
    private static final String TAG_FLOAT = "Float";
	/**
	 * Attribute name for binary data, because it requires special handling when
	 * creating certain messages (e.g. calling PutFile's
	 * {@link RPCStruct#setBulkData(byte[])} method).
	 */
	private final static String BULK_DATA_ATTR = "bulkData";
	/** Attribute name that defines the test's name in a &lt;test&gt; tag. */
	private final static String TEST_NAME_ATTR = "testName";
	/** Attribute name that defines the timeout of a &lt;test&gt; or a request. */
	private final static String PAUSE_ATTR = "pause";
    /**
     * Attribute name that defines a type of the test
     */
	private final static String INTEGRATION_ATTR = "integration";
    /**
     * Attribute name that defines a type of the test
     */
    private final static String UNIT_ATTR = "unit";
    /**
     * Attribute name that defines a type of the action to be performed
     */
    private final static String ACTION_NAME_ATTR = "actionName";
	/**
	 * Attribute name that defines if the generated request's JSON should be
	 * invalid.
	 */
	private final static String INVALID_JSON_ATTR = "invalidJSON";
	/** Attribute name that defines custom JSON of the request. */
	private final static String CUSTOM_JSON_ATTR = "customJSON";
	/** Attribute name of request's correlation ID. */
	private final static String CORRELATION_ID_ATTR = "correlationID";

	private static ModuleTest sInstance;
	private SyncProxyTester mActivityInstance;
	private LogAdapter mLogAdapter;
    /**
     * Context of the XML Test thread
     */
	private static Runnable sXMLTestThreadContext;
    /**
     * Context of the XML Test Action thread
     */
	private static Runnable sTestActionThreadContext;
	private Thread mainThread;
	
	private boolean integration;
	private String userPrompt;
	
	private int numIterations;
	
	private ArrayList<Pair<Integer, Result>> expecting = new ArrayList<Pair<Integer, Result>>();
	private Test currentTest = null;
	
	public static ArrayList<Pair<Integer, Result>> sResponses = new ArrayList<Pair<Integer, Result>>();
	
	/** Factory that is used to return a reader for the binary data in tests. */
	private BinaryDataReaderFactory binaryDataReaderFactory = new BinaryDataReaderFactory();
	
	/** WakeLock to keep screen on while testing. */
	private WakeLock wakeLock = null;

	/** Full path to XML test file or directory with test files. */
	private String mFilePath;

    private ProxyService mProxyService;

    private String mAppId;

	public ModuleTest(String appId, ProxyService proxyService, LogAdapter logAdapter) {
        mProxyService = proxyService;
		mActivityInstance = SyncProxyTester.getInstance();
		mLogAdapter = logAdapter;
		
		// Set this's instance
		sInstance = this;
        mAppId = appId;

		mainThread = makeThread(appId);
	}
	
	/**
	 * Starts the thread for xml thread.
	 * 
	 * @param filePath
	 *            path to XML test file or directory with XML test files to use.
	 */
	public void runTests(String filePath) {
		this.mFilePath = filePath;
		mainThread.start();
	}

	/**
	 * Recreates and starts a new thread for xml testing.
	 * 
	 * @param filePath
	 *            path to XML test file or directory with XML test files to use.
	 *            Pass null to use the previously set value.
	 */
	public void restart(String appId, String filePath) {
		mainThread.interrupt();
		mainThread = null;
		if (filePath != null) {
			this.mFilePath = filePath;
		}
		mainThread = makeThread(appId);
		mainThread.start();
	}
	
	public Thread makeThread (final String appId) {
		return new Thread(new Runnable() {
			public void run() {
				if (mFilePath != null) {
					// if the file path is a directory, find all xmls in it
					File file = new File(mFilePath);
					if (file.isDirectory()) {
						String[] filenames = file.list(new FilenameFilter() {
							@Override
							public boolean accept(File dir, String filename) {
								return filename.endsWith(TEST_FILEEXT);
							}
						});
						if (filenames != null) {
							Arrays.sort(filenames);

							if (filenames.length > 0) {
								List<String> paths = new ArrayList<String>();
								for (String filename : filenames) {
									mLogAdapter.logMessage("Processing "
                                            + filename, Log.INFO, true);
									try {
										String fullPath = mFilePath + File.separator + filename;
										processTestFile(fullPath);
										paths.add(getTestResultsFilename(fullPath));
										String testErrorsFilename = getTestErrorsFilename(fullPath);
										if (!fileIsEmpty(testErrorsFilename)) {
											paths.add(testErrorsFilename);
										}
									} catch (Exception e) {
										mLogAdapter
												.logMessage("Parser Failed!!",
                                                        Log.ERROR, e);
									}
								}
								mLogAdapter.logMessage("All tests finished",
                                        Log.INFO, true);

								if (paths.size() > 0) {
									sendReportEmail(paths);
								}
							} else {
								mLogAdapter.logMessage(
                                        "No test files found in " + mFilePath,
                                        Log.INFO, true);
							}
						} else {
							mLogAdapter.logMessage(
                                    "Couldn't list files in directory",
                                    Log.ERROR, true);
						}
					} else {
						try {
							processTestFile(mFilePath);

							List<String> paths = new ArrayList<String>();
							paths.add(getTestResultsFilename(mFilePath));
							String testErrorsFilename = getTestErrorsFilename(mFilePath);
							if (!fileIsEmpty(testErrorsFilename)) {
								paths.add(testErrorsFilename);
							}
							sendReportEmail(paths);
						} catch (Exception e) {
							mLogAdapter.logMessage("Parser Failed!!",
                                    Log.ERROR, e);
						}
					}
				} else {
					Logger.w(TAG + " No file chosen");
				}
			}

			private void sendReportEmail(List<String> filePaths) {
				// http://stackoverflow.com/questions/2264622/android-multiple-email-attachments-using-intent/3300495#3300495
				if (filePaths != null) {
					final int numFilePaths = filePaths.size();

					Intent email = new Intent(
							numFilePaths > 1 ? Intent.ACTION_SEND_MULTIPLE : Intent.ACTION_SEND);
					email.setType("text/plain");
					email.putExtra(Intent.EXTRA_EMAIL, new String[] { "youremail@ford.com" });
					email.putExtra(Intent.EXTRA_SUBJECT, "Lua Unit Test Export");

					switch (numFilePaths) {
					case 0:
						// no attachments
						break;

					case 1:
						email.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(new File(filePaths.get(0))));
						break;

					default:
						// assuming there can't be negative array size
						ArrayList<Uri> uris = new ArrayList<Uri>(numFilePaths);
						for (String filePath : filePaths) {
							uris.add(Uri.fromFile(new File(filePath)));
						}
						email.putParcelableArrayListExtra(Intent.EXTRA_STREAM, uris);
						break;
					}

					mActivityInstance.startActivity(Intent.createChooser(email,
                            "Choose an Email client :"));
				}
			}

            private void processTestFile(String filename) throws IOException,
                    XmlPullParserException {
                AcceptedRPC acceptedRPC = new AcceptedRPC();
                XmlPullParser parser = Xml.newPullParser();
                RPCRequest rpc;
                TestActionItem testActionItem = null;
                try {
                    if (AppPreferencesManager.getDisableLockFlag()) {
                        acquireWakeLock();
                    }

                    FileInputStream fin = new FileInputStream(filename);
                    InputStreamReader isr = new InputStreamReader(fin);

                    // file writer for test results
                    String outFile = getTestResultsFilename(filename);
                    File out = new File(outFile);
                    FileWriter writer = new FileWriter(out);
                    writer.flush();

                    // file writer for test errors
                    String outErrorFile = getTestErrorsFilename(filename);
                    File outError = new File(outErrorFile);
                    FileWriter errorWriter = new FileWriter(outError);
                    errorWriter.flush();

                    parser.setInput(isr);
                    int eventType = parser.getEventType();
                    String name;
                    boolean isParsingDone = false;
                    while (eventType != XmlPullParser.END_DOCUMENT && !isParsingDone) {
                        name = parser.getName();

                        switch (eventType) {
                            case XmlPullParser.START_DOCUMENT:
                                logParserDebugInfo("START_DOCUMENT, name: " + name);
                                break;
                            case XmlPullParser.END_DOCUMENT:
                                logParserDebugInfo("END_DOCUMENT, name: " + name);
                                break;
                            case XmlPullParser.START_TAG:
                                name = parser.getName();
                                Logger.d(TAG + " Start tag:" + name);
                                if (name.equalsIgnoreCase(TEST_TAG_NAME_TEST)) {
                                    mLogAdapter.logMessage("test " + parser.getAttributeValue(0), true);

                                    long pause = 0;
                                    String pauseString = parser.getAttributeValue(null, PAUSE_ATTR);
                                    if (pauseString != null) {
                                        try {
                                            pause = Long.parseLong(pauseString);
                                        } catch (NumberFormatException e) {
                                            Logger.e(TAG + " Couldn't parse pause number: " + pauseString);
                                        }
                                    }
                                    currentTest = new Test(parser.getAttributeValue(null, TEST_NAME_ATTR),
                                            pause, null);
                                    expecting.clear();
                                    sResponses.clear();
                                    numIterations = 1;
                                    try {
                                        if (parser.getAttributeName(1) != null) {
                                            if (parser.getAttributeName(1).equalsIgnoreCase("iterations")) {
                                                try {
                                                    numIterations = Integer.parseInt(parser.getAttributeValue(1));
                                                } catch (Exception e) {
                                                    Logger.w(TAG + " Unable to parse number of iterations");
                                                }
                                            }
                                        }
                                    } catch (Exception e) {
                                        Logger.w(TAG + " Unable to parse number of iterations");
                                    }
                                } else if (name.equalsIgnoreCase(TEST_TAG_NAME_TYPE)) {
                                    if (parser.getAttributeValue(0).equalsIgnoreCase(INTEGRATION_ATTR))
                                        integration = true;
                                    else if (parser.getAttributeValue(0).equalsIgnoreCase(UNIT_ATTR))
                                        integration = false;
                                } else if (acceptedRPC.isAcceptedRPC(name)) {
                                    //Create correct object
                                    if (name.equalsIgnoreCase(Names.RegisterAppInterface)) {
                                        rpc = new RegisterAppInterface();
                                    } else if (name.equalsIgnoreCase(Names.UnregisterAppInterface)) {
                                        rpc = RPCRequestFactory.buildUnregisterAppInterface();
                                    } else if (name.equalsIgnoreCase(Names.SetGlobalProperties)) {
                                        rpc = RPCRequestFactory.buildSetGlobalProperties();
                                    } else if (name.equalsIgnoreCase(Names.ResetGlobalProperties)) {
                                        rpc = new ResetGlobalProperties();
                                    } else if (name.equalsIgnoreCase(Names.AddCommand)) {
                                        rpc = RPCRequestFactory.buildAddCommand();
                                    } else if (name.equalsIgnoreCase(Names.DeleteCommand)) {
                                        rpc = new DeleteCommand();
                                    } else if (name.equalsIgnoreCase(Names.AddSubMenu)) {
                                        rpc = RPCRequestFactory.buildAddSubMenu();
                                    } else if (name.equalsIgnoreCase(Names.DeleteSubMenu)) {
                                        rpc = new DeleteSubMenu();
                                    } else if (name.equalsIgnoreCase(Names.CreateInteractionChoiceSet)) {
                                        rpc = RPCRequestFactory.buildCreateInteractionChoiceSet();
                                    } else if (name.equalsIgnoreCase(Names.PerformInteraction)) {
                                        rpc = new PerformInteraction();
                                    } else if (name.equalsIgnoreCase(Names.DeleteInteractionChoiceSet)) {
                                        rpc = new DeleteInteractionChoiceSet();
                                    } else if (name.equalsIgnoreCase(Names.Alert)) {
                                        rpc = new Alert();
                                    } else if (name.equalsIgnoreCase(Names.Show)) {
                                        rpc = new Show();
                                    } else if (name.equalsIgnoreCase(Names.Speak)) {
                                        rpc = new Speak();
                                    } else if (name.equalsIgnoreCase(Names.SetMediaClockTimer)) {
                                        rpc = new SetMediaClockTimer();
                                    } else if (name.equalsIgnoreCase(Names.EncodedSyncPData)) {
                                        rpc = new EncodedSyncPData();
                                    } else if (name.equalsIgnoreCase(Names.SyncPData)) {
                                        rpc = new SyncPData();
                                    } else if (name.equalsIgnoreCase(Names.PerformAudioPassThru)) {
                                        rpc = new PerformAudioPassThru();
                                    } else if (name.equalsIgnoreCase(Names.EndAudioPassThru)) {
                                        rpc = new EndAudioPassThru();
                                    } else if (name.equalsIgnoreCase(Names.SubscribeButton)) {
                                        rpc = RPCRequestFactory.buildSubscribeButton();
                                    } else if (name.equalsIgnoreCase(Names.UnsubscribeButton)) {
                                        rpc = new UnsubscribeButton();
                                    } else if (name.equalsIgnoreCase(Names.SubscribeVehicleData)) {
                                        rpc = RPCRequestFactory.buildSubscribeVehicleData();
                                    } else if (name.equalsIgnoreCase(Names.UnsubscribeVehicleData)) {
                                        rpc = new UnsubscribeVehicleData();
                                    } else if (name.equalsIgnoreCase(Names.GetVehicleData)) {
                                        rpc = new GetVehicleData();
                                    } else if (name.equalsIgnoreCase(Names.ReadDID)) {
                                        rpc = new ReadDID();
                                    } else if (name.equalsIgnoreCase(Names.GetDTCs)) {
                                        rpc = new GetDTCs();
                                    } else if (name.equalsIgnoreCase(Names.ScrollableMessage)) {
                                        rpc = new ScrollableMessage();
                                    } else if (name.equalsIgnoreCase(Names.Slider)) {
                                        rpc = new Slider();
                                    } else if (name.equalsIgnoreCase(Names.ShowConstantTBT)) {
                                        rpc = new ShowConstantTBT();
                                    } else if (name.equalsIgnoreCase(Names.AlertManeuver)) {
                                        rpc = new AlertManeuver();
                                    } else if (name.equalsIgnoreCase(Names.UpdateTurnList)) {
                                        rpc = new UpdateTurnList();
                                    } else if (name.equalsIgnoreCase(Names.ChangeRegistration)) {
                                        rpc = new ChangeRegistration();
                                    } else if (name.equalsIgnoreCase(Names.PutFile)) {
                                        rpc = RPCRequestFactory.buildPutFile();
                                    } else if (name.equalsIgnoreCase(Names.DeleteFile)) {
                                        rpc = new DeleteFile();
                                    } else if (name.equalsIgnoreCase(Names.ListFiles)) {
                                        rpc = new ListFiles();
                                    } else if (name.equalsIgnoreCase(Names.SetAppIcon)) {
                                        rpc = new SetAppIcon();
                                    } else if (name.equalsIgnoreCase(Names.SetDisplayLayout)) {
                                        rpc = new SetDisplayLayout();
                                    } else if (name.equalsIgnoreCase("ClearMediaClockTimer")) {
                                        rpc = new Show();
                                        ((Show) rpc).setMainField1(null);
                                        ((Show) rpc).setMainField2(null);
                                        ((Show) rpc).setStatusBar(null);
                                        ((Show) rpc).setMediaClock("     ");
                                        ((Show) rpc).setMediaTrack(null);
                                        ((Show) rpc).setAlignment(null);
                                    } else if (name.equalsIgnoreCase("PauseMediaClockTimer")) {
                                        rpc = new SetMediaClockTimer();
                                        StartTime startTime = new StartTime();
                                        startTime.setHours(0);
                                        startTime.setMinutes(0);
                                        startTime.setSeconds(0);
                                        ((SetMediaClockTimer) rpc).setStartTime(startTime);
                                        ((SetMediaClockTimer) rpc).setUpdateMode(UpdateMode.PAUSE);
                                    } else if (name.equalsIgnoreCase("ResumeMediaClockTimer")) {
                                        rpc = new SetMediaClockTimer();
                                        StartTime startTime = new StartTime();
                                        startTime.setHours(0);
                                        startTime.setMinutes(0);
                                        startTime.setSeconds(0);
                                        ((SetMediaClockTimer) rpc).setStartTime(startTime);
                                        ((SetMediaClockTimer) rpc).setUpdateMode(UpdateMode.RESUME);
                                    } else if (name.equalsIgnoreCase(GenericRequest.NAME)) {
                                        rpc = new GenericRequest();
                                    } else {
                                        rpc = RPCRequestFactory.buildSetGlobalProperties();
                                    }

                                    try {
                                        rpc.setCorrelationID(Integer.parseInt(parser.getAttributeValue(null, CORRELATION_ID_ATTR)));
                                    } catch (NumberFormatException e) {
                                        Logger.e(TAG + " Unable to parse correlation ID");
                                    }

                                    long pause = 0;
                                    String pauseString = parser.getAttributeValue(null, PAUSE_ATTR);
                                    if (pauseString != null) {
                                        try {
                                            pause = Long.parseLong(pauseString);
                                        } catch (NumberFormatException e) {
                                            Logger.e(TAG + " Couldn't parse pause number: " + pauseString);
                                        }
                                    }

                                    boolean generateInvalidJSON = (parser.getAttributeValue(null,
                                            INVALID_JSON_ATTR) != null);
                                    Logger.d("Generate Invalid JSON:" + generateInvalidJSON);

                                    String customJSON = parser.getAttributeValue(null, CUSTOM_JSON_ATTR);

                                    //TODO: Set rpc parameters
                                    Hashtable hash = setParams(name, parser);
                                    logParserDebugInfo("" + hash);
                                    //TODO: Iterate through hash table and add it to parameters
                                    for (Object key : hash.keySet()) {
                                        final Object value = hash.get(key);
                                        if (key.equals(BULK_DATA_ATTR)) {
                                            if (value instanceof byte[]) {
                                                rpc.setBulkData((byte[]) value);
                                            } else {
                                                rpc.setBulkData(new byte[]{});
                                            }
                                        } else {
                                            Logger.d(TAG + " RPC name:" + rpc.getFunctionName() + ", key:" + key + ", val:" + value);
                                            rpc.setParameters((String) key, value);
                                        }
                                    }

                                    for (Object o : hash.entrySet()) {
                                        Hashtable.Entry pairs = (Hashtable.Entry) o;
                                        logParserDebugInfo(pairs.getKey() + " = " + pairs.getValue());
                                    }

                                    if (currentTest != null) {
                                        RPCRequestWrapper wrapper = new RPCRequestWrapper(rpc, pause,
                                                generateInvalidJSON, customJSON);
                                        currentTest.addRequest(wrapper);
                                    }
                                } else if (name.equalsIgnoreCase(TEST_TAG_NAME_RESULT)) {
                                    expecting.add(new Pair<Integer, Result>(Integer.parseInt(parser.getAttributeValue(0)), (Result.valueForString(parser.getAttributeValue(1)))));
                                } else if (name.equalsIgnoreCase(TEST_TAG_NAME_USER_PROMPT) && integration) {
                                    userPrompt = parser.getAttributeValue(0);
                                } else if (name.equalsIgnoreCase(TEST_TAG_NAME_ACTION)) {
                                    String attributeName = parser.getAttributeValue(null, ACTION_NAME_ATTR);
                                    //Logger.d(TAG + " ActionName:" + attributeName);
                                    if (StringUtils.isNotEmpty(attributeName) &&
                                            attributeName.equals(TestActionItem.START_RPC_SERVICE)) {
                                        testActionItem = new TestActionItem(attributeName);
                                        long pause = 0;
                                        String pauseString = parser.getAttributeValue(null, PAUSE_ATTR);
                                        //Logger.d(TAG + " ActionPause:" + pauseString);
                                        if (pauseString != null) {
                                            try {
                                                pause = Long.parseLong(pauseString);
                                            } catch (NumberFormatException e) {
                                                Logger.e(TAG + " Couldn't parse pause number: " + pauseString);
                                            }
                                        }
                                        testActionItem.setDelay(pause);
                                    }
                                }
                                break;
                            case XmlPullParser.END_TAG:
                                name = parser.getName();
                                Logger.d(TAG + " End tag:" + name);
                                if (name.equalsIgnoreCase(TEST_TAG_NAME_TEST)) {
                                    try {
                                        final String FIELD_SEPARATOR = ", ";
                                        final String EOL = "\n";

                                        boolean localPass = true;
                                        int i = numIterations;
                                        int numPass = 0;
                                        boolean doInterrupt = false;
                                        while (i > 0) {
                                            TestResult testResult = xmlTest(appId);
                                            if (testResult.isTestComplete()) {
                                                numPass++;
                                            } else {
                                                localPass = false;
                                                StringBuilder errorBuilder = new StringBuilder(currentTest.getName());
                                                errorBuilder.append(" Expected");
                                                for (Pair p : expecting) {
                                                    errorBuilder.append(FIELD_SEPARATOR);
                                                    errorBuilder.append(p.first);
                                                    errorBuilder.append(" ");
                                                    errorBuilder.append(p.second);
                                                }
                                                errorBuilder.append(EOL);

                                                errorBuilder.append(currentTest.getName());
                                                errorBuilder.append(" Actual");
                                                for (Pair p : sResponses) {
                                                    errorBuilder.append(FIELD_SEPARATOR);
                                                    errorBuilder.append(p.first);
                                                    errorBuilder.append(" ");
                                                    errorBuilder.append(p.second);
                                                }
                                                errorBuilder.append(EOL);

                                                String errorLine = errorBuilder.toString();
                                                errorWriter.write(errorLine);
                                            }
                                            i--;

                                            if (testResult.isTestFlowInterrupted()) {
                                                doInterrupt = true;
                                                break;
                                            }

                                            doInterrupt = false;
                                        }

                                        if (doInterrupt) {
                                            isParsingDone = true;
                                        }

                                        if (currentTest != null) {
                                            StringBuilder result = new StringBuilder(currentTest.getName());
                                            String[] fields = {(localPass ? "Pass" : "Fail"), String.valueOf(numPass), String.valueOf(numIterations)};
                                            for (String field : fields) {
                                                result.append(FIELD_SEPARATOR);
                                                result.append(field);
                                            }
                                            String resultLine = result.toString();

                                            writer.write(resultLine + EOL);
                                            Logger.i(TAG + " " + resultLine);
                                            mLogAdapter.logMessage(resultLine, true);
                                        }
                                    } catch (Exception e) {
                                        if (currentTest != null) {
                                            mLogAdapter.logMessage("Test " + currentTest.getName() + " Failed! ", Log.ERROR, e);
                                        }
                                    }
                                } else if (name.equalsIgnoreCase(TEST_TAG_NAME_ACTION)) {
                                    // Deprecated
                                    /*if (testActionItem != null) {
                                        testAction(testActionItem);
                                    }*/
                                }
                                break;
                            case XmlPullParser.TEXT:
                                //logParserDebugInfo("TEXT, name: " + name);
                                break;
                            case XmlPullParser.CDSECT:
                                logParserDebugInfo("CDSECT, name: " + name);
                                break;
                            case XmlPullParser.ENTITY_REF:
                                logParserDebugInfo("ENTITY_REF, name: " + name);
                                break;
                            case XmlPullParser.IGNORABLE_WHITESPACE:
                                logParserDebugInfo("IGNORABLE_WHITESPACE, name: " + name);
                                break;
                            case XmlPullParser.PROCESSING_INSTRUCTION:
                                logParserDebugInfo("PROCESSING_INSTRUCTION, name: " + name);
                                break;
                            case XmlPullParser.COMMENT:
                                logParserDebugInfo("COMMENT, name: " + name);
                                break;
                            case XmlPullParser.DOCDECL:
                                logParserDebugInfo("DOCDECL, name: " + name);
                                break;
                            default:
                                break;
                        }
                        eventType = parser.next();
                    }
                    writer.close();
                    errorWriter.close();
                    Logger.d(TAG + " Tests finished");

                    currentTest = null;
                } finally {
                    releaseWakeLock();
                }
            }

			private String getTestResultsFilename(String filename) {
				return filename.substring(0,
						filename.length() - TEST_FILEEXT.length())
						+ ".csv";
			}

			private String getTestErrorsFilename(String filename) {
				return filename.substring(0,
						filename.length() - TEST_FILEEXT.length())
						+ "Errors.csv";
			}

			private boolean fileIsEmpty(String filename) {
				return new File(filename).length() <= 0;
			}

		});
	}
	
	private Hashtable setParams(String name, XmlPullParser parser) {

		logParserDebugInfo("setParams start name: " + name);
		
		Hashtable hash = new Hashtable();
		
		int eventType = 0;
		Boolean done = false;
		String tempName = null;
		String vectorName = null;

		try {
			while (eventType != XmlPullParser.END_DOCUMENT && !done) {
				tempName = parser.getName();
				
				switch (eventType) {
				case XmlPullParser.START_DOCUMENT:
					logParserDebugInfo("START_DOCUMENT, tempName: " + tempName);
					break;
				case XmlPullParser.END_DOCUMENT:
					logParserDebugInfo("END_DOCUMENT, tempName: " + tempName);
					break;
				case XmlPullParser.START_TAG:
					if (tempName.equalsIgnoreCase("Vector")) {
						logParserDebugInfo("In Vector");
						Vector<Object> vector = new Vector<Object>();
						
						if (parser.getAttributeName(0) != null) vectorName = parser.getAttributeValue(0);

						Boolean nestedWhileDone = false;
						
						eventType = parser.next();
						while (eventType != XmlPullParser.START_TAG && !nestedWhileDone) {
							if (eventType == XmlPullParser.END_TAG) {
								if (parser.getName().equalsIgnoreCase("Vector")) {
									Logger.e("TESTING", "In Vector Loop, nestedWhileDone == true, END_TAG, name: " + name);
									nestedWhileDone = true;
								}
							} else eventType = parser.next();
						}
						
						while (eventType != XmlPullParser.END_DOCUMENT && !nestedWhileDone) {
							tempName = parser.getName();
							logParserDebugInfo("In Vector Loop, tempName: " + tempName);

							switch (eventType) {
							case XmlPullParser.START_DOCUMENT:
								logParserDebugInfo("In Vector Loop, START_DOCUMENT, name: " + name);
								break;
							case XmlPullParser.END_DOCUMENT:
								logParserDebugInfo("In Vector Loop, END_DOCUMENT, name: " + name);
								break;
							case XmlPullParser.START_TAG:
								if (tempName.equalsIgnoreCase("Integer")) {
									logParserDebugInfo("In Nested Vector Integer");
									if (parser.getAttributeName(0) != null) {
										//try {vector.add(Integer.parseInt(parser.getAttributeValue(0)));} 
										try {vector.add(Double.parseDouble(parser.getAttributeValue(0)));} 
										catch (Exception e) {Logger.e(TAG + " Unable to parse Integer");}
									}
								} else if (tempName.equalsIgnoreCase("String")) {
									logParserDebugInfo("In Nested Vector String");
									if (parser.getAttributeName(0) != null) {
										vector.add(parser.getAttributeValue(0));
									}
								} else {
									vector.add(setParams(tempName, parser));
								}
								break;
							case XmlPullParser.END_TAG:
								logParserDebugInfo("In Vector Loop, END_TAG, name: " + name);
								if (tempName.equalsIgnoreCase("Vector")) {
									logParserDebugInfo("In Vector Loop, nestedWhileDone == true, END_TAG, name: " + name);
									nestedWhileDone = true;
								}
								break;
							case XmlPullParser.TEXT:
								//logParserDebugInfo("TEXT, name: " + name);
								break;
							case XmlPullParser.CDSECT:
								logParserDebugInfo("In Vector Loop, CDSECT, name: " + name);
								break;
							case XmlPullParser.ENTITY_REF:
								logParserDebugInfo("In Vector Loop, ENTITY_REF, name: " + name);
								break;
							case XmlPullParser.IGNORABLE_WHITESPACE:
								logParserDebugInfo("In Vector Loop, IGNORABLE_WHITESPACE, name: " + name);
								break;
							case XmlPullParser.PROCESSING_INSTRUCTION:
								logParserDebugInfo("In Vector Loop, PROCESSING_INSTRUCTION, name: " + name);
								break;
							case XmlPullParser.COMMENT:
								logParserDebugInfo("In Vector Loop, COMMENT, name: " + name);
								break;
							case XmlPullParser.DOCDECL:
								logParserDebugInfo("In Vector Loop, DOCDECL, name: " + name);
								break;
							default:
								break;
							}
							eventType = parser.next();
						}
						logParserDebugInfo("out of Vector loop");
						hash.put(vectorName, vector);
					} else if (tempName.equalsIgnoreCase("Integer")) {
						logParserDebugInfo("In Integer");
						if (parser.getAttributeName(0) != null) {
							//try {hash.put(parser.getAttributeName(0), Integer.parseInt(parser.getAttributeValue(0)));} 
							try {hash.put(parser.getAttributeName(0), Double.parseDouble(parser.getAttributeValue(0)));} 
							catch (Exception e) {Logger.e(TAG + " Unable to parse Integer");}
						}
                    } else if (tempName.equalsIgnoreCase(TAG_FLOAT)) {
                        logParserDebugInfo("In " + TAG_FLOAT);
                        if (parser.getAttributeName(0) != null) {
                            try {hash.put(parser.getAttributeName(0), Double.parseDouble(parser.getAttributeValue(0)));}
                            catch (Exception e) {Logger.e(TAG + " Unable to parse " + TAG_FLOAT);}
                        }
					} else if (tempName.equalsIgnoreCase("Boolean")) {
						logParserDebugInfo("In Boolean");
						if (parser.getAttributeName(0) != null) {
							if (parser.getAttributeValue(0).equalsIgnoreCase("true")) hash.put(parser.getAttributeName(0), true);
							else if (parser.getAttributeValue(0).equalsIgnoreCase("false")) hash.put(parser.getAttributeName(0), false);
						}
					} else if (tempName.equalsIgnoreCase("String")) {
						logParserDebugInfo("In String");
						if (parser.getAttributeName(0) != null) {
							hash.put(parser.getAttributeName(0), parser.getAttributeValue(0));
						}
					} else if (tempName.equalsIgnoreCase(BINARY_TAG_NAME)) {
						logParserDebugInfo("In " + BINARY_TAG_NAME);
						String srcData = parser.getAttributeValue(0);
                        final BinaryDataReader reader = binaryDataReaderFactory
                                .getReaderForString(srcData);
                        byte[] data;
                        if (reader != null) {
                            data = reader.read(srcData);
                        } else {
                            // if reader can't be found, set empty data
                            data = new byte[]{ };
                        }
						if (data != null) {
							hash.put(BULK_DATA_ATTR, data);
						}
					} else {
						logParserDebugInfo("Returning in else statement");
						//return setParams(tempName, parser);
						hash.put(tempName, setParams(tempName, parser));
					}
					break;
				case XmlPullParser.END_TAG:
					if (tempName.equalsIgnoreCase(name)) {
						done = true;
					}
					break;
				case XmlPullParser.TEXT:
					//logParserDebugInfo("TEXT, tempName: " + tempName);
					break;
				case XmlPullParser.CDSECT:
					logParserDebugInfo("CDSECT, tempName: " + tempName);
					break;
				case XmlPullParser.ENTITY_REF:
					logParserDebugInfo("ENTITY_REF, tempName: " + tempName);
					break;
				case XmlPullParser.IGNORABLE_WHITESPACE:
					logParserDebugInfo("IGNORABLE_WHITESPACE, tempName: " + tempName);
					break;
				case XmlPullParser.PROCESSING_INSTRUCTION:
					logParserDebugInfo("PROCESSING_INSTRUCTION, tempName: " + tempName);
					break;
				case XmlPullParser.COMMENT:
					logParserDebugInfo("COMMENT, tempName: " + tempName);
					break;
				case XmlPullParser.DOCDECL:
					logParserDebugInfo("DOCDECL, tempName: " + tempName);
					break;
				default:
					break;
				}
				eventType = parser.next();
			}
		} catch (Exception e) {
			mLogAdapter.logMessage("Parser Failed!!", Log.ERROR, e);
		}
		
		logParserDebugInfo("Returning at end of setParams function");
		return hash;
	}

    /**
     * Performs additional actions within Test Cases flow, such as : restore RPC session
     * @param testActionItem test action item which describes an action to be performed
     */
    private void testAction(final TestActionItem testActionItem) {

        Thread newThread = new Thread(new Runnable() {
            @Override
            public void run() {
                sTestActionThreadContext = this;

                if (mProxyService == null && testActionItem == null) {
                    Logger.e(TAG + " Test Action is null");
                    interruptThread();
                    return;
                }

                if (!mProxyService.isSyncProxyConnected()) {
                    Logger.e(TAG + " Test Action - connection - is null");
                    interruptThread();
                    return;
                }

                mProxyService.waiting(true);

                mProxyService.testInitializeSessionRPCOnly(mAppId);

                long pause = testActionItem.getDelay();
                if (pause > 0) {
                    Logger.d(TAG + " Pause for " + pause + " ms for " + testActionItem.getActionName());
                    try {
                        // delay after the test
                        synchronized (this) {
                            ((Object) this).wait(pause);
                        }
                    } catch (InterruptedException e) {
                        mLogAdapter.logMessage("InterruptedException", true);
                    }
                } else {
                    Logger.i(TAG + " No pause for " + pause + " ms for " + testActionItem.getActionName());
                }

                // wait for incoming messages
                try {
                    synchronized (this) {
                        ((Object) this).wait(100);
                    }
                } catch (InterruptedException e) {
                    mLogAdapter.logMessage("InterruptedException", true);
                }

                mProxyService.waiting(false);

                interruptThread();
            }

            private void interruptThread() {
                synchronized (sInstance) {
                    sInstance.notify();
                }
                Thread.currentThread().interrupt();
            }
        });

        newThread.start();

        try {
            synchronized (this) {
                this.wait();
            }
        } catch (InterruptedException e) {
            mLogAdapter.logMessage("InterruptedException", true);
        }

        newThread.interrupt();
    }
	
	private TestResult xmlTest(final String appId) {

        final TestResult testResult = new TestResult();

		Thread newThread = new Thread(new Runnable() {

			public void run () {

                sXMLTestThreadContext = this;

                if (mProxyService == null && currentTest == null) {
                    Logger.e(TAG + " Current test is null");
                    interruptThread();
                    return;
                }

                if (currentTest.getRequests() == null) {
                    Logger.e(TAG + " Current test request is null");
                    interruptThread();
                    return;
                }

                /*if (!mProxyService.isSyncProxyConnected()) {
                    Logger.e(TAG + " Current test - connection - is null");

                    showInterruptDialog("Warning",
                            "Connection is lost, would You like to continue run tests?", true);

                    if (testResult.isTestFlowInterrupted()) {
                        interruptThread();
                        return;
                    }
                }*/

                // Temporary commit this check point

                /*if (!currentTest.getName().toLowerCase()
                        .startsWith(Names.UnregisterAppInterface.toLowerCase()) &&
                        mProxyService.getSessionId() == 0) {
                    Log.e(CLASS_NAME, "Current test session id 0");

                    showInterruptDialog("Warning",
                            "Current Session Id is 0, would You like to continue run tests?", true);

                    if (testResult.isTestFlowInterrupted()) {
                        interruptThread();
                        return;
                    }
                }*/

                int numResponses = expecting.size();
                if (numResponses > 0) {
                    mProxyService.waiting(true);
                }

                IJsonRPCMarshaller defaultMarshaller = new JsonRPCMarshaller();
                CustomJsonRPCMarshaller customMarshaller = null;

                for (RPCRequestWrapper wrapper : currentTest.getRequests()) {
                    RPCRequest rpc = wrapper.getRequest();
                    boolean generateInvalidJSON = wrapper.isGenerateInvalidJSON();
                    String customJSON = wrapper.getCustomJSON();
                    if (customJSON != null) {
                        customMarshaller = new CustomJsonRPCMarshaller(null);
                        customMarshaller.setStubbedValue(customJSON);
                    }

                    IJsonRPCMarshaller currentMarshaller;
                    if (customJSON != null) {
                        currentMarshaller = customMarshaller;
                    } else if (generateInvalidJSON) {
                        currentMarshaller = new InvalidJsonRPCMarshaller();
                    } else {
                        currentMarshaller = defaultMarshaller;
                    }
                    //currentMarshaller = (customJSON != null) ? customMarshaller :
                    //                (generateInvalidJSON ? invalidMarshaller : defaultMarshaller);
                    Logger.d("Current Marshaller:" + currentMarshaller);

                    Logger.d(TAG + " Send RPC:" + rpc + " appId:" + appId);
                    mProxyService.sendRPCRequestWithPreprocess(appId, rpc, currentMarshaller, true);

                    long pause = wrapper.getPause();
                    if (pause > 0) {
                        Logger.d(TAG + " Pause for " + pause + " ms. after " +
                                currentTest.getName() + "." + rpc.getFunctionName());
                        try {
                            // delay between requests of one test
                            synchronized (this) {
                                ((Object) this).wait(pause);
                            }
                        } catch (InterruptedException e) {
                            mLogAdapter.logMessage("InterruptedException", true);
                        }
                    } else {
                        Logger.i(TAG + " No pause after " + currentTest.getName() +
                                        "." + rpc.getFunctionName());
                    }

                    // restore the default marshaller
                    /*if (currentMarshaller instanceof InvalidJsonRPCMarshaller) {
                        try {
                            Thread.sleep(200);
                        } catch (InterruptedException e) {
                            e.printStackTrace();
                        }
                    }*/
                    //mProxyService.syncProxySetJsonRPCMarshaller(defaultMarshaller);
                }

                long pause = currentTest.getPause();
                if (pause > 0) {
                    Logger.d(TAG + " Pause for " + pause + " ms. after " + currentTest.getName());
                    try {
                        // delay after the test
                        synchronized (this) {
                            ((Object) this).wait(pause);
                        }
                    } catch (InterruptedException e) {
                        mLogAdapter.logMessage("InterruptedException", true);
                    }
                } else {
                    Logger.i(TAG + " No pause after " + currentTest.getName());
                }

                // wait for incoming messages
                try {
                    synchronized (this) {
                        ((Object) this).wait(100);
                    }
                } catch (InterruptedException e) {
                    mLogAdapter.logMessage("InterruptedException", true);
                }

                mProxyService.waiting(false);

                if (!expecting.equals(sResponses)) {
                    restoreMarshaller(defaultMarshaller);
                    return;
                }

                // restore the default marshaller
                mProxyService.syncProxySetJsonRPCMarshaller(defaultMarshaller);
                testResult.setTestComplete(true);

                if (!integration) {
                    restoreMarshaller(defaultMarshaller);
                    return;
                }

                MainApp.getInstance().runInUIThread(new Runnable() {
                    public void run() {
                        AlertDialog.Builder alert = new AlertDialog.Builder(mActivityInstance);
                        alert.setMessage(userPrompt);
                        alert.setPositiveButton("Yes",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        testResult.setTestComplete(true);
                                        synchronized (sXMLTestThreadContext) {
                                            sXMLTestThreadContext.notify();
                                        }
                                    }
                                }
                        );
                        alert.setNegativeButton("No",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        testResult.setTestComplete(false);
                                        synchronized (sXMLTestThreadContext) {
                                            sXMLTestThreadContext.notify();
                                        }
                                    }
                                }
                        );
                        alert.show();
                    }
                });

                try {
                    synchronized (this) {
                        ((Object) this).wait();
                    }
                } catch (InterruptedException e) {
                    mLogAdapter.logMessage("InterruptedException", true);
                }

                restoreMarshaller(defaultMarshaller);
			}

            private void showInterruptDialog(final String title, final String message,
                                             final boolean doInterruptFlow) {
                final CountDownLatch countDownLatch = new CountDownLatch(1);

                MainApp.getInstance().runInUIThread(new Runnable() {
                    @Override
                    public void run() {
                        AlertDialog.Builder alert = new AlertDialog.Builder(mActivityInstance);
                        alert.setTitle(title);
                        alert.setMessage(message);
                        alert.setPositiveButton("Yes",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        testResult.setTestComplete(true);
                                        countDownLatch.countDown();
                                    }
                                }
                        );
                        alert.setNegativeButton("No",
                                new DialogInterface.OnClickListener() {
                                    @Override
                                    public void onClick(DialogInterface dialog, int which) {
                                        testResult.setTestComplete(false);
                                        if (doInterruptFlow) {
                                            testResult.setTestFlowInterrupted(true);
                                        }
                                        countDownLatch.countDown();
                                    }
                                }
                        );
                        alert.show();
                    }
                });

                try {
                    countDownLatch.await();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }

            private void restoreMarshaller(IJsonRPCMarshaller defaultMarshaller) {
                mProxyService.syncProxySetJsonRPCMarshaller(defaultMarshaller);
                interruptThread();
            }

            private void interruptThread() {
                synchronized (sInstance) {
                    sInstance.notify();
                }
                Thread.currentThread().interrupt();
            }
		});
		newThread.start();
		
		try {
			synchronized (this) {
                this.wait();
            }
		} catch (InterruptedException e) {
			mLogAdapter.logMessage("InterruptedException", true);
		}
		
		newThread.interrupt();
		return testResult;
	}
	
	public static ModuleTest getModuleTestInstance() {
		return sInstance;
	}

    /**
     * @return context of the XML Test thread
     */
	public Runnable getXMLTestThreadContext() {
		return sXMLTestThreadContext;
	}

    /**
     * @return context of the XML Test Action thread
     */
    public Runnable getTestActionThreadContext() {
        return sTestActionThreadContext;
    }
	
	/**
	 * Logs debug information during the XML parsing process. Can be turned
	 * on/off with the DISPLAY_PARSER_DEBUG_INFO constant.
	 * 
	 * @param s
	 *            string to log
	 */
	private void logParserDebugInfo(String s) {
		if (!DISPLAY_PARSER_DEBUG_INFO) {
			return;
		}
		
		Logger.d(TAG + " " + s);
	}
	
	private void acquireWakeLock() {
		if (wakeLock != null) {
			wakeLock.release();
			wakeLock = null;
		}
		
		try {
			PowerManager pm = (PowerManager) mActivityInstance
					.getSystemService(Context.POWER_SERVICE);
			wakeLock = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK
					| PowerManager.ACQUIRE_CAUSES_WAKEUP
					| PowerManager.ON_AFTER_RELEASE, TAG);
			wakeLock.setReferenceCounted(false);
			wakeLock.acquire();
		} catch (NullPointerException e) {
			Logger.w(TAG + " Can't acquire wakelock", e);
			wakeLock = null;
		}
	}

	private void releaseWakeLock() {
		if (wakeLock != null) {
			wakeLock.release();
			wakeLock = null;
		} else {
			Logger.d(TAG + " Can't release wakeLock, it's null");
		}
	}
}

/*
	public void setParameters(String functionName, Object value) {
		if (value != null) {
			parameters.put(functionName, value);
		} else {
			parameters.remove(functionName);
		}
	}
	
	public Object getParameters(String functionName) {
		return parameters.get(functionName);
	}
*/
