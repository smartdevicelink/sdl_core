package com.batutin.android.androidvideostreaming.screenshot;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.media.MediaCodecInfo;
import android.media.MediaCodecList;
import android.view.View;

/**
 * Created by Andrew Batutin on 7/30/13.
 */
public class ScreenShotUtils {

    public Bitmap takeScreenshot(View view) {
        Bitmap result =getBitmapFromView(view);
        return result;
    }

    private static Bitmap getBitmapFromView(View view) {
        Bitmap returnedBitmap = Bitmap.createBitmap(view.getWidth(), view.getHeight(),Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(returnedBitmap);
        Drawable bgDrawable =view.getBackground();
        if (bgDrawable!=null)
            bgDrawable.draw(canvas);
        else
            canvas.drawColor(Color.WHITE);
        view.draw(canvas);
        return returnedBitmap;
    }


    public byte[] createInputFrame(int width, int height, int colorFormat, int stride, int sliceHeight, int chromaStride, int frameSize) {
        byte[] inputFrame = new byte[frameSize];
        if (colorFormat == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar ||
                colorFormat == MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar) {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int Y = (x + y) & 255;
                    int Cb = 255*x/width;
                    int Cr = 255*y/height;
                    inputFrame[y*stride + x] = (byte) Y;
                    inputFrame[stride*sliceHeight + (y/2)*chromaStride + (x/2)] = (byte) Cb;
                    inputFrame[stride*sliceHeight + chromaStride*(sliceHeight/2) + (y/2)*chromaStride + (x/2)] = (byte) Cr;
                }
            }
        } else {
            for (int y = 0; y < height; y++) {
                for (int x = 0; x < width; x++) {
                    int Y = (x + y) & 255;
                    int Cb = 255*x/width;
                    int Cr = 255*y/height;
                    inputFrame[y*stride + x] = (byte) Y;
                    inputFrame[stride*sliceHeight + 2*(y/2)*chromaStride + 2*(x/2)] = (byte) Cb;
                    inputFrame[stride*sliceHeight + 2*(y/2)*chromaStride + 2*(x/2) + 1] = (byte) Cr;
                }
            }
        }
        return inputFrame;
    }

    public int calcSliceHeightForHeight(int height, MediaCodecInfo codecInfo) {
        int sliceHeight = height;
        if (codecInfo.getName().startsWith("OMX.Nvidia.")) {
            sliceHeight = (sliceHeight + 15)/16*16;
        }
        return sliceHeight;
    }

    public int calcStrideForWidth(int width, MediaCodecInfo codecInfo) {
        int stride = width;
        if (codecInfo.getName().startsWith("OMX.Nvidia.")) {
            stride = (stride + 15)/16*16;
        }
        return stride;
    }

    public int calcWidth(MediaCodecInfo codecInfo, int width) {
        if (codecInfo.getName().equals("OMX.TI.DUCATI1.VIDEO.H264E")) {
            // This codec doesn't support a width not a multiple of 16,
            // so round down.
            width &= ~15;
        }
        return width;
    }

    public int getCapabilities(MediaCodecInfo.CodecCapabilities capabilities) {
        int colorFormat = 0;
        for (int i = 0; i < capabilities.colorFormats.length && colorFormat == 0; i++) {
            int format = capabilities.colorFormats[i];
            switch (format) {
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar:
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedPlanar:
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420SemiPlanar:
                case MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420PackedSemiPlanar:
                case MediaCodecInfo.CodecCapabilities.COLOR_TI_FormatYUV420PackedSemiPlanar:
                    colorFormat = format;
                    break;
                default:

                    break;
            }
        }
        return colorFormat;
    }

    public MediaCodecInfo getMediaCodecInfo(String mimeType, int numCodecs) {
        MediaCodecInfo codecInfo = null;
        for (int i = 0; i < numCodecs && codecInfo == null; i++) {
            MediaCodecInfo info = MediaCodecList.getCodecInfoAt(i);
            if (!info.isEncoder()) {
                continue;
            }
            String[] types = info.getSupportedTypes();
            boolean found = false;
            for (int j = 0; j < types.length && !found; j++) {
                if (types[j].equals(mimeType))
                    found = true;
            }
            if (!found)
                continue;
            codecInfo = info;
        }
        return codecInfo;
    }

    public int calcFrameStride(int stride, int sliceHeight, int chromaStride) {
        return stride*sliceHeight + 2*chromaStride*sliceHeight/2;
    }

    public int calcChromaStride(int stride) {
        return stride/2;
    }
    
}
