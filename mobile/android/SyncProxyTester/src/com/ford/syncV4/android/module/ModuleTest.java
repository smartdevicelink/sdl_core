package com.ford.syncV4.android.module;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Vector;

import org.xmlpull.v1.XmlPullParser;

import com.ford.syncV4.exception.SyncException;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.Choice;
import com.ford.syncV4.proxy.rpc.StartTime;
import com.ford.syncV4.proxy.rpc.enums.Result;
import com.ford.syncV4.android.activity.SyncProxyTester;
import com.ford.syncV4.android.adapters.logAdapter;
import com.ford.syncV4.android.constants.AcceptedRPC;
import com.ford.syncV4.android.constants.TestRPC;
import com.ford.syncV4.android.service.ProxyService;

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

public class ModuleTest {
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
	private ArrayList<Pair<String, ArrayList<TestRPC>>> testList = new ArrayList<Pair<String, ArrayList<TestRPC>>> ();
	
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
			Log.e("ModuleTest", "unable to write on the sd card " + e.toString());
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
							Log.e("ModuleTest", "Showing file picker before loading the file list");
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
					Log.e("ModuleTest", "Showing file picker before loading the file list");
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
						boolean done = false;
						while (eventType != XmlPullParser.END_DOCUMENT && !done) {
							String name = null;
							switch (eventType) {
								case XmlPullParser.START_DOCUMENT:
									break;
								case XmlPullParser.START_TAG:
									name = parser.getName();
									if (name.equalsIgnoreCase("test")) {
										_msgAdapter.logMessage("test " + parser.getAttributeValue(0), true);
										testList.add(new Pair<String, ArrayList<TestRPC>> (parser.getAttributeValue(0), new ArrayList<TestRPC> ()));
										expecting.clear();
										responses.clear();
										try {
											if (parser.getAttributeName(1) != null) {
												if (parser.getAttributeName(1).equalsIgnoreCase("iterations")) {
													try {numIterations = Integer.parseInt(parser.getAttributeValue(1));} 
													catch (Exception e) {Log.e("parser", "Unable to parse number of iterations");}
												} else numIterations = 1;
											} else numIterations = 1;
										} catch (Exception e) {
											numIterations = 1;
										}
									} else if (name.equalsIgnoreCase("type")) {
										if (parser.getAttributeValue(0).equalsIgnoreCase("integration")) integration = true;
										else if (parser.getAttributeValue(0).equalsIgnoreCase("unit")) integration = false;
									//} else if (name.equalsIgnoreCase("alert")) {
									} else if (acceptedRPC.isAcceptedRPC(name)) {
										//TestObj testObj = new TestObj();
										//testObj.RPC = name;
										TestRPC testrpc = new TestRPC(name);
										int i = 0;
										try {
											while (parser.getAttributeName(i) != null) {
												if (parser.getAttributeName(i).equalsIgnoreCase(Names.choiceSet)) {
													Choice choice = new Choice();
													choice.setChoiceID(Integer.parseInt(parser.getAttributeValue(i)));
													if (parser.getAttributeName(i + 1).equalsIgnoreCase("choiceSetName")) {
														i++;
														choice.setMenuName(parser.getAttributeValue(i));
													}
													if (parser.getAttributeName(i + 1).equalsIgnoreCase("choiceSetVrCommands")) {
														i++;
														Vector<String> choiceSetVrCommands = new Vector<String>();
														choiceSetVrCommands.add(parser.getAttributeValue(i));
														choice.setVrCommands(choiceSetVrCommands);
													}
													Vector<Choice> choiceSet = new Vector<Choice>();
													choiceSet.add(choice);
													testrpc.setChoiceSet(choiceSet);
												} else if (parser.getAttributeName(i).equalsIgnoreCase(Names.hours)) {
													StartTime startTime = new StartTime();
													startTime.setHours(Integer.parseInt(parser.getAttributeValue(i)));
													if (parser.getAttributeName(i + 1).equalsIgnoreCase(Names.minutes)) {
														i++;
														startTime.setMinutes(Integer.parseInt(parser.getAttributeValue(i)));
													}
													if (parser.getAttributeName(i + 1).equalsIgnoreCase(Names.seconds)) {
														i++;
														startTime.setSeconds(Integer.parseInt(parser.getAttributeValue(i)));
													}
													testrpc.setStartTime(startTime);
												} else {
													testrpc.setField(parser.getAttributeName(i), parser.getAttributeValue(i));
												}
												i++;
											}
										} catch (Exception e) {}
										Pair<String, ArrayList<TestRPC>> temp = testList.get(testList.size()-1);
										temp.second.add(testrpc);
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
											Log.i("Test App Result", "" + testList.get(testList.size()-1).first + ", " + localPass + ", " + numPass + ", " + numIterations);
											_msgAdapter.logMessage("" + testList.get(testList.size()-1).first + ", " + localPass + ", " + numPass + ", " + numIterations, true);
										} catch (Exception e) {
											_msgAdapter.logMessage("Test " + testList.get(testList.size()-1).first + " Failed! ", Log.ERROR, e);
										}
									}
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
	
	private Boolean xmlTest() {
		pass = false;
		
		Thread newThread = new Thread(new Runnable() {
			public void run () {
				threadContext = this;
				
				int numResponses = expecting.size();
				if (numResponses > 0) ProxyService.waiting(true);
				
				for (TestRPC testrpc : testList.get(testList.size()-1).second) {
					_msgAdapter.logMessage(testrpc.getRPC(), true);
					try {
						ProxyService.getProxyInstance().sendRPCRequest(testrpc.getRPC());
					} catch (SyncException e) {
						_msgAdapter.logMessage("Error sending RPC", Log.ERROR, e, true);
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