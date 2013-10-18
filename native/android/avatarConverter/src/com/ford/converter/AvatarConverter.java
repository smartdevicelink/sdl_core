/**
 * 
 */
package com.ford.converter;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.lang.String;

/**
 * @author kbotnar
 * 
 */
public class AvatarConverter {
	private static final String FRENCH = "fr";
	private static final String SPANISH = "spa";

	private static final String LANGUAGE = SPANISH;

	private static final String OLD_PACK_NAME = "com.ford.avatar";
	private static final String NEW_PACK_NAME = "com.ford.avatar" + LANGUAGE;

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		// copy MB src
		String rootPath = "..\\..\\..\\..\\";
		String src = rootPath + "ava\\common";
		String dest = rootPath + "ava_" + LANGUAGE + "\\common";
		try {
			emptyFolder(dest);
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (!copyFiles(src, dest))
			return;

		// copy projet
		src = rootPath + "ava\\native\\android\\src";
		dest = rootPath + "ava_" + LANGUAGE + "\\native\\android\\src";
		try {
			emptyFolder(dest);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		if (!copyFiles(src, dest))
			return;

		// delete unnecessary files
		String libPath = dest + "\\libs";
		String objPath = dest + "\\obj";
		String binPath = dest + "\\bin";
		String genPath = dest + "\\gen";
		try {
			emptyFolder(libPath);
			emptyFolder(objPath);
			emptyFolder(binPath);
			emptyFolder(genPath);
		} catch (IOException e1) {
			e1.printStackTrace();
		}

		changePackage(dest);
		changeManifest(dest);
		replaceSplashScreen(dest);
		changeServerPort(dest);
		changeJSFlag(dest);
		changeJniFiles(dest);
		changeSettings(dest);
		changeDownloader(dest);
		replaceMaps(rootPath, dest);
		renameProject(dest);
		System.out.println("-= Convertion is done =-");

	}

	private static void changeManifest(String root) {
		// change package
		changeTextInFile(root + "\\AndroidManifest.xml", OLD_PACK_NAME,
				NEW_PACK_NAME);
		// change app name
		String oldName = "@string/app_name";
		String newName = oldName + "_" + LANGUAGE;
		changeTextInFile(root + "\\AndroidManifest.xml", oldName, newName);
		// change settings name
		oldName = "@string/str_menu_settings";
		newName = oldName + "_" + LANGUAGE;
		changeTextInFile(root + "\\AndroidManifest.xml", oldName, newName);

	}

	private static boolean copyFiles(String source, String destination) {
		// check if source directory exists
		File srcFolder = new File(source);
		File destFolder = new File(destination);
		if (!srcFolder.exists()) {
			System.out.println("Directory does not exist.");
			return false;
		}
		try {
			copyFolder(srcFolder, destFolder);
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	public static boolean copyFolder(File src, File dest) throws IOException {

		if (src.isDirectory()) {

			// if directory not exists, create it
			if (!dest.exists()) {
				if (!dest.mkdirs()) {
					System.out.println("Directory " + dest
							+ " was not created.");
					return false;
				}
			}

			// list all the directory contents
			if (src.list() != null) {
				String files[] = src.list();

				for (String file : files) {
					// construct the src and dest file structure
					File srcFile = new File(src, file);
					File destFile = new File(dest, file);
					// recursive copy
					copyFolder(srcFile, destFile);
				}
			}
		} else {
			// if file, then copy it
			// Use bytes stream to support all file types
			InputStream in = new FileInputStream(src);
			OutputStream out = new FileOutputStream(dest);

			byte[] buffer = new byte[1024];

			int length;
			// copy the file content in bytes
			while ((length = in.read(buffer)) > 0) {
				out.write(buffer, 0, length);
			}

			in.close();
			out.close();
			dest.setReadable(true);
			dest.setWritable(true);
			System.out.println("File copied from " + src + " to " + dest);
		}
		return true;
	}

	public static void emptyFolder(String path) throws IOException {
		File dest = new File(path);
		if (dest.isDirectory()) {

			// if directory exists, delete the files it contains
			if (dest.exists()) {

				// list all the directory contents
				String files[] = dest.list();

				for (String file : files) {
					// construct the dest file structure
					path = dest.getAbsolutePath() + "\\" + file;
					// recursive delete
					emptyFolder(path);
				}

				if (dest.delete()) {
					System.out
							.println("Delete file: " + dest.getAbsolutePath());
				}
			}

		} else {
			// if file, then delete it
			try {
				if (dest.delete()) {
					System.out
							.println("Delete file: " + dest.getAbsolutePath());
				}
			} catch (SecurityException e) {
				e.printStackTrace();
			}
		}

	}

	private static void changePackage(String root) {
		// change package folder name
		String packPath = root + "\\src\\com\\ford\\avatar";
		File packDir = new File(packPath);
		File newPackDir = new File(packPath + LANGUAGE);
		if (packDir.renameTo(newPackDir)) {
			System.out.println("Rename package folder to: "
					+ newPackDir.getAbsolutePath());
		}
		// go trough all files and change package name
		refactorSources(newPackDir, OLD_PACK_NAME, NEW_PACK_NAME);

	}

	private static void refactorSources(File fileRoot, String oldText,
			String newText) {
		if (fileRoot.isDirectory()) {
			// if directory exists, refactor the files it contains
			if (fileRoot.exists()) {
				// list all the directory contents
				String files[] = fileRoot.list();
				for (String file : files) {
					// construct the root file structure
					String path = fileRoot.getAbsolutePath() + "\\" + file;
					// recursive refactor
					File inFile = new File(path);
					refactorSources(inFile, oldText, newText);
				}
			}

		} else {
			// if file, then refactor it
			changeTextInFile(fileRoot.getAbsolutePath(), oldText, newText);
			// System.out.println("Refactor file: " + dest.getAbsolutePath());

		}

	}

	private static boolean changeTextInFile(String fileName, String oldText,
			String newText) {
		File file = new File(fileName);
		if (!file.exists()) {
			return false;
		}
		BufferedReader br;
		try {
			br = new BufferedReader(new FileReader(file));
		} catch (FileNotFoundException e) {
			e.printStackTrace();
			return false;
		} catch (SecurityException se) {
			se.printStackTrace();
			return false;
		}
		// read string and search for old text
		StringBuffer text = new StringBuffer();
		try {
			String data = null;
			while ((data = br.readLine()) != null) {
				text.append(data);
				text.append("\n");
			}
			br.close();
		} catch (IOException e) {
			e.printStackTrace();
			return false;
		}

		// replace all text occurrences
		int startIndex = 0;
		int index = -1;
		while ((index = text.indexOf(oldText, startIndex)) >= 0) {
			text.replace(index, index + oldText.length(), newText);
			startIndex = index + newText.length();
		}

		// replase old file with new one
		if (file.delete()) {
			try {
				if (file.createNewFile()) {
					BufferedWriter bwr;
					bwr = new BufferedWriter(new FileWriter(file));
					bwr.write(text.toString());
					bwr.flush();
					bwr.close();
					System.out.println("File " + file + " was refactored");
					return true;
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		return false;
	}

	private static void replaceSplashScreen(String dest) {
		String logoSrc = "res\\logo" + LANGUAGE + ".png";
		String logoDest = dest + "\\res\\drawable\\logo.png";
		// delete old logo
		File oldLogo = new File(logoDest);
		if (oldLogo.exists() && oldLogo.delete()) {
			// copy file and rename it to "logo"
			// Use bytes stream to support all file types
			InputStream in;
			OutputStream out;
			try {
				in = new FileInputStream(logoSrc);
				out = new FileOutputStream(logoDest);

				byte[] buffer = new byte[1024];

				int length;
				// copy the file content in bytes
				while ((length = in.read(buffer)) > 0) {
					out.write(buffer, 0, length);
				}

				in.close();
				out.close();
			} catch (FileNotFoundException e) {
				e.printStackTrace();
			} catch (IOException e) {
				e.printStackTrace();
			}

			System.out.println("File copied from " + logoSrc + " to " + logoDest);
		}

	}

	private static void changeServerPort(String root) {
		String filePath = root + "\\src\\com\\ford\\avatar" + LANGUAGE
				+ "\\util\\RPCConst.java";
		// change server port
		String strOldPort = "TCP_SERVER_PORT = 21111";
		String strConst = strOldPort.substring(0, strOldPort.indexOf('=') + 1);
		String strPort = strOldPort.substring(strOldPort.indexOf('=') + 1);
		int oldPort = Integer.parseInt(strPort.trim());
		String strNewPort = strConst + String.valueOf(oldPort);
		if(LANGUAGE.compareTo(FRENCH)==0){
			strNewPort = strConst + String.valueOf(oldPort+1);
		} else {
			strNewPort = strConst + String.valueOf(oldPort+2);
		}
		if(!changeTextInFile(filePath, strOldPort, strNewPort)){
			System.out.println("!!!ERROR " + filePath + " was not refactored");
		}
	}

	private static void changeJSFlag(String root) {
		String filePath = root + "\\assets\\app\\Flags.js";
		String oldValue = "'eng',";
		String newValue = "'" + LANGUAGE + "',";
		changeTextInFile(filePath, oldValue, newValue);
	}

	private static void changeJniFiles(String root) {
		String oldValue = "com_ford_avatar";
		String newValue = oldValue + LANGUAGE;
		// rename cpp files
		String oldFilePath = root
				+ "\\jni\\com_ford_avatar_jsonserver_MBWrapper.cpp";
		String newFilePath = root + "\\jni\\com_ford_avatar" + LANGUAGE
				+ "_jsonserver_MBWrapper.cpp";
		File jniFile = new File(oldFilePath);
		if (jniFile.exists()) {
			File newFile = new File(newFilePath);
			if (jniFile.renameTo(newFile)) {
				changeTextInFile(newFile.getAbsolutePath(), oldValue, newValue);
			}
		}
		// rename .h files
		oldFilePath = root + "\\jni\\com_ford_avatar_jsonserver_MBWrapper.h";
		newFilePath = root + "\\jni\\com_ford_avatar" + LANGUAGE
				+ "_jsonserver_MBWrapper.h";
		jniFile = new File(oldFilePath);
		if (jniFile.exists()) {
			File newFile = new File(newFilePath);
			if (jniFile.renameTo(newFile)) {
				changeTextInFile(newFile.getAbsolutePath(), oldValue, newValue);
			}
		}
		// refactor .mk files
		oldFilePath = root + "\\jni\\Android.mk";
		jniFile = new File(oldFilePath);
		if (jniFile.exists()) {
			changeTextInFile(jniFile.getAbsolutePath(), oldValue, newValue);
		}
	}

	private static void changeSettings(String root) {
		String filePath = root + "\\res\\xml\\settings.xml";
		File settingsFile = new File(filePath);
		if (settingsFile.exists()) {
//			String oldValue = "str_preference_general";
//			String newValue = oldValue + "_" + LANGUAGE;
//			changeTextInFile(settingsFile.getAbsolutePath(), oldValue, newValue);
//			oldValue = "str_preference_summary_fullscreen";
//			newValue = oldValue + "_" + LANGUAGE;
//			changeTextInFile(settingsFile.getAbsolutePath(), oldValue, newValue);
//			oldValue = "str_preference_title_fullscreen";
//			newValue = oldValue + "_" + LANGUAGE;
//			changeTextInFile(settingsFile.getAbsolutePath(), oldValue, newValue);
//			oldValue = "str_preference_summary_maps";
//			newValue = oldValue + "_" + LANGUAGE;
//			changeTextInFile(settingsFile.getAbsolutePath(), oldValue, newValue);
//			oldValue = "str_preference_title_maps";
//			newValue = oldValue + "_" + LANGUAGE;
//			changeTextInFile(settingsFile.getAbsolutePath(), oldValue, newValue);
//			oldValue = "str_preference_summary_vehicles";
//			newValue = oldValue + "_" + LANGUAGE;
//			changeTextInFile(settingsFile.getAbsolutePath(), oldValue, newValue);
//			oldValue = "str_preference_title_vehicles";
//			newValue = oldValue + "_" + LANGUAGE;
//			changeTextInFile(settingsFile.getAbsolutePath(), oldValue, newValue);
		}
		filePath = root + "\\res\\menu\\optionsmenu.xml";
		File menuFile = new File(filePath);
		if (menuFile.exists()) {
			String oldValue = "str_menu_settings";
			String newValue = oldValue + "_" + LANGUAGE;
			changeTextInFile(menuFile.getAbsolutePath(), oldValue, newValue);
			oldValue = "str_menu_quit";
			newValue = oldValue + "_" + LANGUAGE;
			changeTextInFile(menuFile.getAbsolutePath(), oldValue, newValue);
			oldValue = "str_menu_rate";
			newValue = oldValue + "_" + LANGUAGE;
			changeTextInFile(menuFile.getAbsolutePath(), oldValue, newValue);
		}

	}

	private static void changeDownloader(String root) {
		String filePath = root + "\\src\\com\\ford\\avatar" + LANGUAGE
				+ "\\downloader\\DownloaderClient.java";
		String oldValue = "toast_download_complete";
		String newValue = oldValue + "_" + LANGUAGE;
		changeTextInFile(filePath, oldValue, newValue);
	}

	private static void replaceMaps(String root, String dest) {
		String src = root + "ava\\web\\src\\" + LANGUAGE + "\\map";
		String mapDest = dest + "\\assets\\map";
		try {
			emptyFolder(mapDest);
		} catch (IOException e) {
			e.printStackTrace();
		}
		if (!copyFiles(src, mapDest))
			return;
	}

	private static void renameProject(String dest) {
		String filePath = dest + "\\.project";
		File projectFile = new File(filePath);
		if (projectFile.exists()) {
			String oldValue = "<name>avatar";
			String newValue = oldValue + LANGUAGE;
			changeTextInFile(projectFile.getAbsolutePath(), oldValue, newValue);
			oldValue = "workspace_loc:/avatar";
			newValue = oldValue + LANGUAGE;
			changeTextInFile(projectFile.getAbsolutePath(), oldValue, newValue);
		}

		filePath = dest + "\\.cproject";
		projectFile = new File(filePath);
		if (projectFile.exists()) {
			String oldValue = "workspace_loc:/avatar";
			String newValue = oldValue + LANGUAGE;
			changeTextInFile(projectFile.getAbsolutePath(), oldValue, newValue);
			oldValue = "workspacePath=\"/avatar";
			newValue = oldValue + LANGUAGE;
			changeTextInFile(projectFile.getAbsolutePath(), oldValue, newValue);
		}
	}

}
