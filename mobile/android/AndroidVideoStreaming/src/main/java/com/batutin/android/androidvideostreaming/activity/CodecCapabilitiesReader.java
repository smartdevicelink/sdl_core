package com.batutin.android.androidvideostreaming.activity;

import android.media.MediaCodecInfo;
import android.media.MediaCodecInfo.CodecCapabilities;
import android.media.MediaCodecInfo.CodecProfileLevel;
import android.media.MediaCodecList;

public class CodecCapabilitiesReader {
	private static String colorFormatToString(int colorFormat) {
		String ret = "not found(" + colorFormat + ")";
		switch (colorFormat) {
		case CodecCapabilities.COLOR_Format12bitRGB444:
			ret = "COLOR_Format12bitRGB444";
			break;
		case CodecCapabilities.COLOR_Format16bitARGB1555:
			ret = "COLOR_Format16bitARGB1555";
			break;
		case CodecCapabilities.COLOR_Format16bitARGB4444:
			ret = "COLOR_Format16bitARGB4444";
			break;
		case CodecCapabilities.COLOR_Format16bitBGR565:
			ret = "COLOR_Format16bitBGR565";
			break;
		case CodecCapabilities.COLOR_Format16bitRGB565:
			ret = "COLOR_Format16bitRGB565";
			break;
		case CodecCapabilities.COLOR_Format18BitBGR666:
			ret = "COLOR_Format18BitBGR666";
			break;
		case CodecCapabilities.COLOR_Format18bitARGB1665:
			ret = "COLOR_Format18bitARGB1665";
			break;
		case CodecCapabilities.COLOR_Format18bitRGB666:
			ret = "COLOR_Format18bitRGB666";
			break;
		case CodecCapabilities.COLOR_Format19bitARGB1666:
			ret = "COLOR_Format19bitARGB1666";
			break;
		case CodecCapabilities.COLOR_Format24BitABGR6666:
			ret = "COLOR_Format24BitABGR6666";
			break;
		case CodecCapabilities.COLOR_Format24BitARGB6666:
			ret = "COLOR_Format24BitARGB6666";
			break;
		case CodecCapabilities.COLOR_Format24bitARGB1887:
			ret = "COLOR_Format24bitARGB1887";
			break;
		case CodecCapabilities.COLOR_Format24bitBGR888:
			ret = "COLOR_Format24bitBGR888";
			break;
		case CodecCapabilities.COLOR_Format24bitRGB888:
			ret = "COLOR_Format24bitRGB888";
			break;
		case CodecCapabilities.COLOR_Format25bitARGB1888:
			ret = "COLOR_Format25bitARGB1888";
			break;
		case CodecCapabilities.COLOR_Format32bitARGB8888:
			ret = "COLOR_Format32bitARGB8888";
			break;
		case CodecCapabilities.COLOR_Format32bitBGRA8888:
			ret = "COLOR_Format32bitBGRA8888";
			break;
		case CodecCapabilities.COLOR_Format8bitRGB332:
			ret = "COLOR_Format8bitRGB332";
			break;
		case CodecCapabilities.COLOR_FormatCbYCrY:
			ret = "COLOR_FormatCbYCrY";
			break;
		case CodecCapabilities.COLOR_FormatCrYCbY:
			ret = "COLOR_FormatCrYCbY";
			break;
		case CodecCapabilities.COLOR_FormatL16:
			ret = "COLOR_FormatL16";
			break;
		case CodecCapabilities.COLOR_FormatL2:
			ret = "COLOR_FormatL2";
			break;
		case CodecCapabilities.COLOR_FormatL24:
			ret = "COLOR_FormatL24";
			break;
		case CodecCapabilities.COLOR_FormatL32:
			ret = "COLOR_FormatL32";
			break;
		case CodecCapabilities.COLOR_FormatL4:
			ret = "COLOR_FormatL4";
			break;
		case CodecCapabilities.COLOR_FormatL8:
			ret = "COLOR_FormatL8";
			break;
		case CodecCapabilities.COLOR_FormatMonochrome:
			ret = "COLOR_FormatMonochrome";
			break;
		case CodecCapabilities.COLOR_FormatRawBayer10bit:
			ret = "COLOR_FormatRawBayer10bit";
			break;
		case CodecCapabilities.COLOR_FormatRawBayer8bit:
			ret = "COLOR_FormatRawBayer8bit";
			break;
		case CodecCapabilities.COLOR_FormatRawBayer8bitcompressed:
			ret = "COLOR_FormatRawBayer8bitcompressed";
			break;
		case CodecCapabilities.COLOR_FormatYCbYCr:
			ret = "COLOR_FormatYCbYCr";
			break;
		case CodecCapabilities.COLOR_FormatYCrYCb:
			ret = "COLOR_FormatYCrYCb";
			break;
		case CodecCapabilities.COLOR_FormatYUV411PackedPlanar:
			ret = "COLOR_FormatYUV411PackedPlanar";
			break;
		case CodecCapabilities.COLOR_FormatYUV411Planar:
			ret = "COLOR_FormatYUV411Planar";
			break;
		case CodecCapabilities.COLOR_FormatYUV420PackedPlanar:
			ret = "COLOR_FormatYUV420PackedPlanar";
			break;
		case CodecCapabilities.COLOR_FormatYUV420PackedSemiPlanar:
			ret = "COLOR_FormatYUV420PackedSemiPlanar";
			break;
		case CodecCapabilities.COLOR_FormatYUV420Planar:
			ret = "COLOR_FormatYUV420Planar";
			break;
		case CodecCapabilities.COLOR_FormatYUV420SemiPlanar:
			ret = "COLOR_FormatYUV420SemiPlanar";
			break;
		case CodecCapabilities.COLOR_FormatYUV422PackedPlanar:
			ret = "COLOR_FormatYUV422PackedPlanar";
			break;
		case CodecCapabilities.COLOR_FormatYUV422PackedSemiPlanar:
			ret = "COLOR_FormatYUV422PackedSemiPlanar";
			break;
		case CodecCapabilities.COLOR_FormatYUV422Planar:
			ret = "COLOR_FormatYUV422Planar";
			break;
		case CodecCapabilities.COLOR_FormatYUV422SemiPlanar:
			ret = "COLOR_FormatYUV422SemiPlanar";
			break;
		case CodecCapabilities.COLOR_FormatYUV444Interleaved:
			ret = "COLOR_FormatYUV444Interleaved";
			break;
		case CodecCapabilities.COLOR_QCOM_FormatYUV420SemiPlanar:
			ret = "COLOR_QCOM_FormatYUV420SemiPlanar";
			break;
		case CodecCapabilities.COLOR_TI_FormatYUV420PackedSemiPlanar:
			ret = "COLOR_TI_FormatYUV420PackedSemiPlanar";
			break;

		}

		return ret;
	}

	private static String mpeg4ProfileToString(int profile) {
		String ret = "not found(" + profile + ")";

		switch (profile) {
		case CodecProfileLevel.MPEG4ProfileAdvancedCoding:
			ret = "MPEG4ProfileAdvancedCoding";
			break;
		case CodecProfileLevel.MPEG4ProfileAdvancedCore:
			ret = "MPEG4ProfileAdvancedCore";
			break;
		case CodecProfileLevel.MPEG4ProfileAdvancedRealTime:
			ret = "MPEG4ProfileAdvancedRealTime";
			break;
		case CodecProfileLevel.MPEG4ProfileAdvancedScalable:
			ret = "MPEG4ProfileAdvancedScalable";
			break;
		case CodecProfileLevel.MPEG4ProfileAdvancedSimple:
			ret = "MPEG4ProfileAdvancedSimple";
			break;
		case CodecProfileLevel.MPEG4ProfileBasicAnimated:
			ret = "MPEG4ProfileBasicAnimated";
			break;
		case CodecProfileLevel.MPEG4ProfileCore:
			ret = "MPEG4ProfileCore";
			break;
		case CodecProfileLevel.MPEG4ProfileCoreScalable:
			ret = "MPEG4ProfileCoreScalable";
			break;
		case CodecProfileLevel.MPEG4ProfileHybrid:
			ret = "MPEG4ProfileHybrid";
			break;
		case CodecProfileLevel.MPEG4ProfileMain:
			ret = "MPEG4ProfileMain";
			break;
		case CodecProfileLevel.MPEG4ProfileNbit:
			ret = "MPEG4ProfileNbit";
			break;
		case CodecProfileLevel.MPEG4ProfileScalableTexture:
			ret = "MPEG4ProfileScalableTexture";
			break;
		case CodecProfileLevel.MPEG4ProfileSimple:
			ret = "MPEG4ProfileSimple";
			break;
		case CodecProfileLevel.MPEG4ProfileSimpleFBA:
			ret = "MPEG4ProfileSimpleFBA";
			break;
		case CodecProfileLevel.MPEG4ProfileSimpleFace:
			ret = "MPEG4ProfileSimpleFace";
			break;
		case CodecProfileLevel.MPEG4ProfileSimpleScalable:
			ret = "MPEG4ProfileSimpleScalable";
			break;
		}
		return ret;
	}

	private static String h263ProfileToString(int profile) {
		String ret = "not found(" + profile + ")";

		switch (profile) {
		case CodecProfileLevel.H263ProfileBackwardCompatible:
			ret = "H263ProfileBackwardCompatible";
			break;
		case CodecProfileLevel.H263ProfileBaseline:
			ret = "H263ProfileBaseline";
			break;
		case CodecProfileLevel.H263ProfileH320Coding:
			ret = "H263ProfileH320Coding";
			break;
		case CodecProfileLevel.H263ProfileHighCompression:
			ret = "H263ProfileHighCompression";
			break;
		case CodecProfileLevel.H263ProfileHighLatency:
			ret = "H263ProfileHighLatency";
			break;
		case CodecProfileLevel.H263ProfileISWV2:
			ret = "H263ProfileISWV2";
			break;
		case CodecProfileLevel.H263ProfileISWV3:
			ret = "H263ProfileISWV3";
			break;
		case CodecProfileLevel.H263ProfileInterlace:
			ret = "H263ProfileInterlace";
			break;
		case CodecProfileLevel.H263ProfileInternet:
			ret = "H263ProfileInternet";
			break;
		}
		return ret;
	}

	private static String avcProfileToString(int profile) {
		String ret = "not found(" + profile + ")";

		switch (profile) {
		case CodecProfileLevel.AVCProfileBaseline:
			ret = "AVCProfileBaseline";
			break;
		case CodecProfileLevel.AVCProfileExtended:
			ret = "AVCProfileExtended";
			break;
		case CodecProfileLevel.AVCProfileHigh:
			ret = "AVCProfileHigh";
			break;
		case CodecProfileLevel.AVCProfileHigh10:
			ret = "AVCProfileHigh10";
			break;
		case CodecProfileLevel.AVCProfileHigh422:
			ret = "AVCProfileHigh422";
			break;
		case CodecProfileLevel.AVCProfileHigh444:
			ret = "AVCProfileHigh444";
			break;
		case CodecProfileLevel.AVCProfileMain:
			ret = "AVCProfileMain";
			break;
		}
		return ret;
	}

	private static String aacProfileToString(int profile) {
		String ret = "not found(" + profile + ")";

		switch (profile) {
		case CodecProfileLevel.AACObjectELD:
			ret = "AACObjectELD";
			break;
		case CodecProfileLevel.AACObjectERLC:
			ret = "AACObjectERLC";
			break;
		case CodecProfileLevel.AACObjectHE:
			ret = "AACObjectHE";
			break;
		case CodecProfileLevel.AACObjectHE_PS:
			ret = "AACObjectHE_PS";
			break;
		case CodecProfileLevel.AACObjectLC:
			ret = "AACObjectLC";
			break;
		case CodecProfileLevel.AACObjectLD:
			ret = "AACObjectLD";
			break;
		case CodecProfileLevel.AACObjectLTP:
			ret = "AACObjectLTP";
			break;
		case CodecProfileLevel.AACObjectMain:
			ret = "AACObjectMain";
			break;
		case CodecProfileLevel.AACObjectSSR:
			ret = "AACObjectSSR";
			break;
		case CodecProfileLevel.AACObjectScalable:
			ret = "AACObjectScalable";
			break;
		}
		return ret;
	}

	private static String mpeg4LevelToString(int level) {
		String ret = "not found(" + level + ")";
		switch (level) {
		case CodecProfileLevel.MPEG4Level0:
			ret = "MPEG4Level0";
			break;
		case CodecProfileLevel.MPEG4Level0b:
			ret = "MPEG4Level0b";
			break;
		case CodecProfileLevel.MPEG4Level1:
			ret = "MPEG4Level1";
			break;
		case CodecProfileLevel.MPEG4Level2:
			ret = "MPEG4Level2";
			break;
		case CodecProfileLevel.MPEG4Level3:
			ret = "MPEG4Level3";
			break;
		case CodecProfileLevel.MPEG4Level4:
			ret = "MPEG4Level4";
			break;
		case CodecProfileLevel.MPEG4Level4a:
			ret = "MPEG4Level4a";
			break;
		case CodecProfileLevel.MPEG4Level5:
			ret = "MPEG4Level5";
			break;
		}
		return ret;

	}

	private static String h263LevelToString(int level) {
		String ret = "not found(" + level + ")";
		switch (level) {
		case CodecProfileLevel.H263Level10:
			ret = "H263Level10";
			break;
		case CodecProfileLevel.H263Level20:
			ret = "H263Level20";
			break;
		case CodecProfileLevel.H263Level30:
			ret = "H263Level30";
			break;
		case CodecProfileLevel.H263Level40:
			ret = "H263Level40";
			break;
		case CodecProfileLevel.H263Level45:
			ret = "H263Level45";
			break;
		case CodecProfileLevel.H263Level50:
			ret = "H263Level50";
			break;
		case CodecProfileLevel.H263Level60:
			ret = "H263Level60";
			break;
		case CodecProfileLevel.H263Level70:
			ret = "H263Level70";
			break;
		}
		return ret;
	}

	private static String avcLevelToString(int level) {
		String ret = "not found(" + level + ")";
		switch (level) {
		case CodecProfileLevel.AVCLevel1:
			ret = "AVCLevel1";
			break;
		case CodecProfileLevel.AVCLevel11:
			ret = "AVCLevel11";
			break;
		case CodecProfileLevel.AVCLevel12:
			ret = "AVCLevel12";
			break;
		case CodecProfileLevel.AVCLevel13:
			ret = "AVCLevel13";
			break;
		case CodecProfileLevel.AVCLevel1b:
			ret = "AVCLevel1b";
			break;
		case CodecProfileLevel.AVCLevel2:
			ret = "AVCLevel2";
			break;
		case CodecProfileLevel.AVCLevel21:
			ret = "AVCLevel21";
			break;
		case CodecProfileLevel.AVCLevel22:
			ret = "AVCLevel22";
			break;
		case CodecProfileLevel.AVCLevel3:
			ret = "AVCLevel3";
			break;
		case CodecProfileLevel.AVCLevel31:
			ret = "AVCLevel31";
			break;
		case CodecProfileLevel.AVCLevel32:
			ret = "AVCLevel32";
			break;
		case CodecProfileLevel.AVCLevel4:
			ret = "AVCLevel4";
			break;
		case CodecProfileLevel.AVCLevel41:
			ret = "AVCLevel41";
			break;
		case CodecProfileLevel.AVCLevel42:
			ret = "AVCLevel42";
			break;
		case CodecProfileLevel.AVCLevel5:
			ret = "AVCLevel5";
			break;
		case CodecProfileLevel.AVCLevel51:
			ret = "AVCLevel51";
			break;
		}
		return ret;
	}

	public static String get() {
		String ret = "";
		final int mediaCodecListCount = MediaCodecList.getCodecCount();
		for (int i = 0; i < mediaCodecListCount; i++) {
			final MediaCodecInfo mediaCodecInfo = MediaCodecList
					.getCodecInfoAt(i);
			ret += "name:" + mediaCodecInfo.getName() + "\n";
			ret += "is encoder:" + mediaCodecInfo.isEncoder() + "\n";
			for (final String type : mediaCodecInfo.getSupportedTypes()) {
				ret += "type:" + type + "\n";
				final CodecCapabilities codecCapabilities = mediaCodecInfo
						.getCapabilitiesForType(type);
				for (final int colorFormat : codecCapabilities.colorFormats) {
					ret += "\tColor Format:" + " " + colorFormat + " "
							+ colorFormatToString(colorFormat) + "\n";
				}

				for (final CodecProfileLevel codecProfileLevel : codecCapabilities.profileLevels) {
					String level = "unknown type";
					String profile = "unknown type";
					if (type.contains("3gpp")) {
						level = h263LevelToString(codecProfileLevel.level);
						profile = h263ProfileToString(codecProfileLevel.profile);
					} else if (type.contains("avc")) {
						level = avcLevelToString(codecProfileLevel.level);
						profile = avcProfileToString(codecProfileLevel.profile);
					} else if (type.contains("mp4")) {
						level = mpeg4LevelToString(codecProfileLevel.level);
						profile = mpeg4ProfileToString(codecProfileLevel.profile);
					} else if (type.contains("aac")) {

						profile = aacProfileToString(codecProfileLevel.profile);
					}
					ret += "\tCodec Profile Level:" + level + " profile:"
							+ profile + "\n";
				}
			}
		}
		return ret;
	}

}
