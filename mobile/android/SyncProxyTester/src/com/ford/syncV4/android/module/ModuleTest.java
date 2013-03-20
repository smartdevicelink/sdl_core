package com.ford.syncV4.android.module;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.Vector;

import org.xmlpull.v1.XmlPullParser;

import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.Dialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.net.Uri;
import android.os.Environment;
import android.util.Log;
import android.util.Pair;
import android.util.Xml;

import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.adapters.logAdapter;
import com.ford.syncV4.android.constants.AcceptedRPC;
import com.ford.syncV4.android.service.ProxyService;
import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.RPCRequest;
import com.ford.syncV4.proxy.RPCStruct;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.AddCommand;
import com.ford.syncV4.proxy.rpc.AddSubMenu;
import com.ford.syncV4.proxy.rpc.Alert;
import com.ford.syncV4.proxy.rpc.AlertManeuver;
import com.ford.syncV4.proxy.rpc.ChangeRegistration;
import com.ford.syncV4.proxy.rpc.CreateInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteCommand;
import com.ford.syncV4.proxy.rpc.DeleteFile;
import com.ford.syncV4.proxy.rpc.DeleteInteractionChoiceSet;
import com.ford.syncV4.proxy.rpc.DeleteSubMenu;
import com.ford.syncV4.proxy.rpc.DialNumber;
import com.ford.syncV4.proxy.rpc.EncodedSyncPData;
import com.ford.syncV4.proxy.rpc.EndAudioPassThru;
import com.ford.syncV4.proxy.rpc.GetDTCs;
import com.ford.syncV4.proxy.rpc.GetVehicleData;
import com.ford.syncV4.proxy.rpc.ListFiles;
import com.ford.syncV4.proxy.rpc.PerformAudioPassThru;
import com.ford.syncV4.proxy.rpc.PerformInteraction;
import com.ford.syncV4.proxy.rpc.PutFile;
import com.ford.syncV4.proxy.rpc.ReadDID;
import com.ford.syncV4.proxy.rpc.RegisterAppInterface;
import com.ford.syncV4.proxy.rpc.ResetGlobalProperties;
import com.ford.syncV4.proxy.rpc.ScrollableMessage;
import com.ford.syncV4.proxy.rpc.SetAppIcon;
import com.ford.syncV4.proxy.rpc.SetDisplayLayout;
import com.ford.syncV4.proxy.rpc.SetGlobalProperties;
import com.ford.syncV4.proxy.rpc.SetMediaClockTimer;
import com.ford.syncV4.proxy.rpc.Show;
import com.ford.syncV4.proxy.rpc.ShowConstantTBT;
import com.ford.syncV4.proxy.rpc.Slider;
import com.ford.syncV4.proxy.rpc.Speak;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.SubscribeButton;
import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UnregisterAppInterface;
import com.ford.syncV4.proxy.rpc.UnsubscribeButton;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UpdateTurnList;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.proxy.rpc.enums.UpdateMode;

public class ModuleTest {
	/** Log tag for the class. */
	private static final String TAG = ModuleTest.class.getSimpleName();
	/** Specifies whether to display debug info from the XML parser. */
	private static final boolean DISPLAY_PARSER_DEBUG_INFO = false;
	
	/**
	 * The tag name used to specify where to get binary data from (e.g., for
	 * PutFile message).
	 */
	private final static String BINARY_TAG_NAME = "Binary";
	/**
	 * Attribute name for binary data, because it requires special handling when
	 * creating certain messages (e.g. calling PutFile's
	 * {@link RPCStruct#setBulkData(byte[])} method).
	 */
	private final static String BULK_DATA_ATTR = "bulkData";
	
	private static ModuleTest _instance;
	private SyncProxyTester _mainInstance;
	private logAdapter _msgAdapter;
	private static Runnable threadContext;
	private static ModuleTest DialogThreadContext;
	private Thread mainThread;
	
	private boolean pass;
	private boolean integration;
	private String userPrompt;
	
	private int numIterations;
	
	private ArrayList<Pair<Integer, Result>> expecting = new ArrayList<Pair<Integer, Result>>();
	private ArrayList<Pair<String, ArrayList<RPCRequest>>> testList = new ArrayList<Pair<String, ArrayList<RPCRequest>>> ();
	
	public static ArrayList<Pair<Integer, Result>> responses = new ArrayList<Pair<Integer, Result>>();
	
	public ModuleTest() {
		this._mainInstance = SyncProxyTester.getInstance();
		this._msgAdapter = SyncProxyTester.getMessageAdapter();
		
		// Set this's instance
		_instance = this;
		_mainInstance.setTesterMain(_instance);
		
		mainThread = makeThread();
	}
	
	public void runTests() {
		mainThread.start();
	}

	public void restart() {
		mainThread.interrupt();
		mainThread = null;
		mainThread = makeThread();
		mainThread.start();
	}
	
	private String[] mFileList;
	//private File mPath = new File(Environment.getExternalStorageDirectory() + "");//"//yourdir//");
	//private File mPath = new File("/sdcard/");
	private File mPath = new File(Environment.getExternalStorageDirectory() + "");
	private String mChosenFile;
	private static final String FTYPE = ".xml";    
	private static final int DIALOG_LOAD_FILE = 1000;
	
	private void loadFileList(){
		try{
			mPath.mkdirs();
		} catch(SecurityException e) {
			Log.e(TAG, "unable to write on the sd card " + e.toString());
		}
		if (mPath.exists()) {
			FilenameFilter filter = new FilenameFilter() {
				public boolean accept(File dir, String filename) {
					File sel = new File(dir, filename);
					return filename.contains(FTYPE);// || sel.isDirectory();
				}
			};
			mFileList = mPath.list(filter);
		} else {
			mFileList= new String[0];
		}
	}
	
	Dialog dialog;
	
	protected Dialog onCreateDialog(final int id) {
		DialogThreadContext = this;
		//Dialog dialog = null;
		_mainInstance.runOnUiThread(new Runnable() {
			public void run() {
				dialog = null;
				AlertDialog.Builder builder = new Builder(_mainInstance);
			
				switch(id) {
					case DIALOG_LOAD_FILE:
						builder.setTitle("Choose your file");
						if (mFileList == null) {
							Log.e(TAG, "Showing file picker before loading the file list");
							dialog = builder.create();
							//return dialog;
							synchronized (DialogThreadContext) { DialogThreadContext.notify();}
							Thread.currentThread().interrupt();
						}
						builder.setItems(mFileList, new DialogInterface.OnClickListener() {
							public void onClick(DialogInterface dialog, int which) {
								mChosenFile = mFileList[which];
								//you can do stuff with the file here too
								synchronized (DialogThreadContext) { DialogThreadContext.notify();}
								Thread.currentThread().interrupt();
							}
						});
						break;
				}
				dialog = builder.show();
			}
		});

		try {
			synchronized (this) { this.wait();}
		} catch (InterruptedException e) {
			_msgAdapter.logMessage("InterruptedException", true);
		}
		return dialog;
		
		/*
		Dialog dialog = null;
		AlertDialog.Builder builder = new Builder(_mainInstance);
	
		switch(id) {
			case DIALOG_LOAD_FILE:
				builder.setTitle("Choose your file");
				if (mFileList == null) {
					Log.e(TAG, "Showing file picker before loading the file list");
					dialog = builder.create();
					return dialog;
				}
				builder.setItems(mFileList, new DialogInterface.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						mChosenFile = mFileList[which];
						//you can do stuff with the file here too
					}
				});
				break;
		}
		dialog = builder.show();
		return dialog;
		*/
	}
	
	public Thread makeThread () {
		return new Thread (new Runnable () {
			public void run () {
				mChosenFile = null;
				loadFileList();
				onCreateDialog(DIALOG_LOAD_FILE);
				if (mChosenFile != null) {
					AcceptedRPC acceptedRPC = new AcceptedRPC();
					XmlPullParser parser = Xml.newPullParser();
					RPCRequest rpc;
					try {
						//FileInputStream fin = new FileInputStream("/sdcard/test.xml");
						FileInputStream fin = new FileInputStream("/sdcard/" + mChosenFile);
						InputStreamReader isr = new InputStreamReader(fin);
						
						String outFile = "/sdcard/" + mChosenFile.substring(0, mChosenFile.length() - 4) + ".csv";
						File out = new File(outFile);
						FileWriter writer = new FileWriter(out);
						writer.flush();
						
						parser.setInput(isr);
						int eventType = parser.getEventType();
						String name;
						boolean done = false;
						while (eventType != XmlPullParser.END_DOCUMENT && !done) {
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
								if (name.equalsIgnoreCase("test")) {
									_msgAdapter.logMessage("test " + parser.getAttributeValue(0), true);
									testList.add(new Pair<String, ArrayList<RPCRequest>> (parser.getAttributeValue(0), new ArrayList<RPCRequest> ()));
									expecting.clear();
									responses.clear();
									try {
										if (parser.getAttributeName(1) != null) {
											if (parser.getAttributeName(1).equalsIgnoreCase("iterations")) {
												try {numIterations = Integer.parseInt(parser.getAttributeValue(1));} 
												catch (Exception e) {Log.e(TAG, "Unable to parse number of iterations");}
											} else numIterations = 1;
										} else numIterations = 1;
									} catch (Exception e) {
										numIterations = 1;
									}
								} else if (name.equalsIgnoreCase("type")) {
									if (parser.getAttributeValue(0).equalsIgnoreCase("integration")) integration = true;
									else if (parser.getAttributeValue(0).equalsIgnoreCase("unit")) integration = false;
								} else if (acceptedRPC.isAcceptedRPC(name)) {
									//Create correct object
									if (name.equalsIgnoreCase(Names.RegisterAppInterface)) {
										rpc = new RegisterAppInterface();
									} else if (name.equalsIgnoreCase(Names.UnregisterAppInterface)) {
										rpc = new UnregisterAppInterface();
									} else if (name.equalsIgnoreCase(Names.SetGlobalProperties)) {
										rpc = new SetGlobalProperties();
									} else if (name.equalsIgnoreCase(Names.ResetGlobalProperties)) {
										rpc = new ResetGlobalProperties();
									} else if (name.equalsIgnoreCase(Names.AddCommand)) {
										rpc = new AddCommand();
									} else if (name.equalsIgnoreCase(Names.DeleteCommand)) {
										rpc = new DeleteCommand();
									} else if (name.equalsIgnoreCase(Names.AddSubMenu)) {
										rpc = new AddSubMenu();
									} else if (name.equalsIgnoreCase(Names.DeleteSubMenu)) {
										rpc = new DeleteSubMenu();
									} else if (name.equalsIgnoreCase(Names.CreateInteractionChoiceSet)) {
										rpc = new CreateInteractionChoiceSet();
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
									} else if (name.equalsIgnoreCase(Names.DialNumber)) {
										rpc = new DialNumber();
									} else if (name.equalsIgnoreCase(Names.PerformAudioPassThru)) {
										rpc = new PerformAudioPassThru();
									} else if (name.equalsIgnoreCase(Names.EndAudioPassThru)) {
										rpc = new EndAudioPassThru();
									} else if (name.equalsIgnoreCase(Names.SubscribeButton)) {
										rpc = new SubscribeButton();
									} else if (name.equalsIgnoreCase(Names.UnsubscribeButton)) {
										rpc = new UnsubscribeButton();
									} else if (name.equalsIgnoreCase(Names.SubscribeVehicleData)) {
										rpc = new SubscribeVehicleData();
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
										rpc = new PutFile();
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
									} else {
										rpc = new SetGlobalProperties();
									}

									if (parser.getAttributeName(0) != null && 
											parser.getAttributeName(0).equalsIgnoreCase("correlationID")) {
										try {rpc.setCorrelationID(Integer.parseInt(parser.getAttributeValue(0)));} 
										catch (Exception e) {Log.e(TAG, "Unable to parse Integer");}
									}
									
									//TODO: Set rpc parameters
									Hashtable hash = setParams(name, parser);
									logParserDebugInfo("" + hash);
									//TODO: Iterate through hash table and add it to parameters
									for (Object key : hash.keySet()) {
										if (((String)key).equals(BULK_DATA_ATTR)) {
											rpc.setBulkData((byte[]) hash.get(key));
										} else {
											rpc.setParameters((String) key, hash.get(key));
										}
									}
									
								    Iterator it = hash.entrySet().iterator();
								    while (it.hasNext()) {
								    	Hashtable.Entry pairs = (Hashtable.Entry)it.next();
								        System.out.println(pairs.getKey() + " = " + pairs.getValue());
								    }
									
									Pair<String, ArrayList<RPCRequest>> temp = testList.get(testList.size()-1);
									temp.second.add(rpc);
									testList.set(testList.size()-1, temp);
								} else if (name.equalsIgnoreCase("result")) {
									expecting.add(new Pair<Integer, Result>(Integer.parseInt(parser.getAttributeValue(0)), (Result.valueForString(parser.getAttributeValue(1)))));
								} else if (name.equalsIgnoreCase("userPrompt") && integration) {
									userPrompt = parser.getAttributeValue(0);
								}
								break;
							case XmlPullParser.END_TAG:
								name = parser.getName();
								if (name.equalsIgnoreCase("test")) {
									try {
										boolean localPass = true;
										int i = numIterations;
										int numPass = 0;
										while (i > 0) {
											xmlTest();
											if (pass) numPass++;
											else localPass = false;
											i--;
										}
										if (localPass) writer.write("" + testList.get(testList.size()-1).first + ", Pass, " + numPass + ", " + numIterations + "\n");
										if (!localPass) writer.write("" + testList.get(testList.size()-1).first + ", Fail, " + numPass + ", " + numIterations + "\n");
										Log.i(TAG, "" + testList.get(testList.size()-1).first + ", " + localPass + ", " + numPass + ", " + numIterations);
										_msgAdapter.logMessage("" + testList.get(testList.size()-1).first + ", " + localPass + ", " + numPass + ", " + numIterations, true);
									} catch (Exception e) {
										_msgAdapter.logMessage("Test " + testList.get(testList.size()-1).first + " Failed! ", Log.ERROR, e);
									}
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

						Intent email = new Intent(Intent.ACTION_SEND);
						email.setType("plain/text");
						email.putExtra(Intent.EXTRA_EMAIL, new String[]{"youremail@ford.com"});		  
						email.putExtra(Intent.EXTRA_SUBJECT, "Lua Unit Test Export");
						email.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(out));
						
						_mainInstance.startActivity(Intent.createChooser(email, "Choose an Email client :"));
						
					} catch (Exception e) {
						_msgAdapter.logMessage("Parser Failed!!", Log.ERROR, e);
					}
				}
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
									Log.e("TESTING", "In Vector Loop, nestedWhileDone == true, END_TAG, name: " + name);
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
										catch (Exception e) {Log.e(TAG, "Unable to parse Integer");}
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
							catch (Exception e) {Log.e(TAG, "Unable to parse Integer");}
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
						String filename = parser.getAttributeValue(0);
						byte[] data = SyncProxyTester.contentsOfFile(filename);
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
			_msgAdapter.logMessage("Parser Failed!!", Log.ERROR, e);
		}
		
		logParserDebugInfo("Returning at end of setParams function");
		return hash;
	}
	
	private Boolean xmlTest() {
		pass = false;
		
		Thread newThread = new Thread(new Runnable() {
			public void run () {
				threadContext = this;
				
				int numResponses = expecting.size();
				if (numResponses > 0) ProxyService.waiting(true);
				
				for (RPCRequest rpc : testList.get(testList.size()-1).second) {
					_msgAdapter.logMessage(rpc, true);
					try {
						ProxyService.getProxyInstance().sendRPCRequest(rpc);
					} catch (SyncException e) {
						_msgAdapter.logMessage("Error sending RPC", Log.ERROR, e, true);
					}
					
					try {
						synchronized (this) {
							this.wait(1000);
						}
					} catch (InterruptedException e) {
						_msgAdapter.logMessage("InterruptedException", true);
					}
				}
				
				try {
					for (int i = 0; i < numResponses; i++) synchronized (this) { this.wait(10000);}
				} catch (InterruptedException e) {
					_msgAdapter.logMessage("InterruptedException", true);
				}
				
				ProxyService.waiting(false);
				
				try {
					synchronized (this) { this.wait(5000);}
				} catch (InterruptedException e) {
					_msgAdapter.logMessage("InterruptedException", true);
				}
				
				if (expecting.equals(responses)) {
					pass = true;
					if (integration) {
						_mainInstance.runOnUiThread(new Runnable() {
							public void run() {
								AlertDialog.Builder alert = new AlertDialog.Builder(_mainInstance);
								alert.setMessage(userPrompt);
								alert.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
									@Override
									public void onClick(DialogInterface dialog, int which) {
										pass = true;
										synchronized (threadContext) { threadContext.notify();}
									}
								});
								alert.setNegativeButton("No", new DialogInterface.OnClickListener() {
									@Override
									public void onClick(DialogInterface dialog, int which) {
										pass = false;
										synchronized (threadContext) { threadContext.notify();}
									}
								});
								alert.show();
							}
						});
	
						try {
							synchronized (this) { this.wait();}
						} catch (InterruptedException e) {
							_msgAdapter.logMessage("InterruptedException", true);
						}
					}
				}
				
				synchronized (_instance) { _instance.notify();}
				
				Thread.currentThread().interrupt();
			}
		});
		newThread.start();
		
		try {
			synchronized (this) { this.wait();}
		} catch (InterruptedException e) {
			_msgAdapter.logMessage("InterruptedException", true);
		}
		
		newThread.interrupt();
		newThread = null;
		return pass;
	}
	
	public static ModuleTest getModuleTestInstance() {
		return _instance;
	}
	
	public Runnable getThreadContext() {
		return threadContext;
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
		
		Log.d(TAG, s);
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
