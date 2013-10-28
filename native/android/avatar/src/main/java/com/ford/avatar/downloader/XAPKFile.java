package com.ford.avatar.downloader;

public class XAPKFile {

	public final boolean mIsMain;
	public final int mFileVersion;
	public final long mFileSize;

	public XAPKFile(boolean isMain, int fileVersion, long fileSize) {
		mIsMain = isMain;
		mFileVersion = fileVersion;
		mFileSize = fileSize;
	}
}
