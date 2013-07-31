package com.batutin.android.androidvideostreaming.screenshot;

import android.graphics.Bitmap;
import android.graphics.Color;
import android.graphics.ColorMatrix;
import android.graphics.ImageFormat;
import android.graphics.YuvImage;

/**
 * Created by Andrew Batutin on 7/31/13.
 */
public class ColorSpaceUtils {

    // Coefficients are taken from jcolor.c in libjpeg.
    private static final int CSHIFT = 16;
    private static final int CYR = 19595;
    private static final int CYG = 38470;
    private static final int CYB = 7471;
    private static final int CUR = -11059;
    private static final int CUG = -21709;
    private static final int CUB = 32768;
    private static final int CVR = 32768;
    private static final int CVG = -27439;
    private static final int CVB = -5329;

    // from http://d.hatena.ne.jp/Superdry/20110130/1296405341
    private int[] convertRGB2YUV(int color) {
        ColorMatrix cm = new ColorMatrix();
        cm.setRGB2YUV();
        final float[] yuvArray = cm.getArray();

        int r = Color.red(color);
        int g = Color.green(color);
        int b = Color.blue(color);
        int[] result = new int[3];

        // Adding a 127 U and V.
        result[0] = floatToByte(yuvArray[0] * r + yuvArray[1] * g + yuvArray[2] * b);
        result[1] = floatToByte(yuvArray[5] * r + yuvArray[6] * g + yuvArray[7] * b) + 127;
        result[2] = floatToByte(yuvArray[10] * r + yuvArray[11] * g + yuvArray[12] * b) + 127;
        return result;
    }

    private int floatToByte(float x) {
        int n = java.lang.Math.round(x);
        return n;
    }

    // Generate YuvImage based on the content in bitmap. If paddings > 0, the
    // strides of YuvImage are calculated by adding paddings to bitmap.getWidth().
    private YuvImage generateYuvImage(int format, Bitmap bitmap, int paddings) {
        int width = bitmap.getWidth();
        int height = bitmap.getHeight();

        int stride = width + paddings;

        YuvImage image = null;
        int[] argb = new int [stride * height];
        bitmap.getPixels(argb, 0, stride, 0, 0, width, height);
        byte[] yuv = convertArgbsToYuvs(argb, stride, height, format);

        int[] strides = null;
        if (format == ImageFormat.NV21) {
            strides = new int[] {stride, stride};
        } else if (format == ImageFormat.YUY2) {
            strides = new int[] {stride * 2};
        }
        image = new YuvImage(yuv, format, width, height, strides);

        return image;
    }

    private byte[] convertArgbsToYuvs(int[] argb, int width, int height,
                                      int format) {
        byte[] yuv = new byte[width * height *
                ImageFormat.getBitsPerPixel(format)];
        if (format == ImageFormat.NV21) {
            int vuStart = width * height;
            byte[] yuvColor = new byte[3];
            for (int row = 0; row < height; ++row) {
                for (int col = 0; col < width; ++col) {
                    int idx = row * width + col;
                    argb2yuv(argb[idx], yuvColor);
                    yuv[idx] = yuvColor[0];
                    if ((row & 1) == 0 && (col & 1) == 0) {
                        int offset = row / 2 * width + col / 2 * 2;
                        yuv[vuStart + offset] = yuvColor[2];
                        yuv[vuStart + offset + 1] = yuvColor[1];
                    }
                }
            }
        } else if (format == ImageFormat.YUY2) {
            byte[] yuvColor0 = new byte[3];
            byte[] yuvColor1 = new byte[3];
            for (int row = 0; row < height; ++row) {
                for (int col = 0; col < width; col += 2) {
                    int idx = row * width + col;
                    argb2yuv(argb[idx], yuvColor0);
                    argb2yuv(argb[idx + 1], yuvColor1);
                    int offset = idx / 2 * 4;
                    yuv[offset] = yuvColor0[0];
                    yuv[offset + 1] = yuvColor0[1];
                    yuv[offset + 2] = yuvColor1[0];
                    yuv[offset + 3] = yuvColor0[2];
                }
            }
        }

        return yuv;
    }

    private void argb2yuv(int argb, byte[] yuv) {
        int r = Color.red(argb);
        int g = Color.green(argb);
        int b = Color.blue(argb);
        yuv[0] = (byte) ((CYR * r + CYG * g + CYB * b) >> CSHIFT);
        yuv[1] = (byte) (((CUR * r + CUG * g + CUB * b) >> CSHIFT) + 128);
        yuv[2] = (byte) (((CVR * r + CVG * g + CVB * b) >> CSHIFT) + 128);
    }
}
