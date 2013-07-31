package com.batutin.android.androidvideostreaming.screenshot;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.test.AndroidTestCase;

import com.batutin.android.androidvideostreaming.R;

/**
 * Created by Andrew Batutin on 7/31/13.
 */
public class ColorSpaceUtilsTest extends AndroidTestCase{

    ColorSpaceUtils sut;
    private static final int WIDTH = 256;
    private static final int HEIGHT = 128;
    private Bitmap mTestBitmaps;

    public ColorSpaceUtilsTest() {
    }

    @Override
    public void setUp() throws Exception {
        super.setUp();
        mTestBitmaps = generateTestBitmaps(420, 320);
        sut = new ColorSpaceUtils();


    }

    private Bitmap generateTestBitmaps(int width, int height) {
        Bitmap dst = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
        Canvas c = new Canvas(dst);
        Resources res = this.getContext().getResources();
        Bitmap src = BitmapFactory.decodeResource(res, R.drawable.ic_launcher);
        c.drawBitmap(src, null, new Rect(0, 0, WIDTH, HEIGHT), null);
        return dst;
    }

    public void testGenerateYUVFromBitmap() throws Exception {
        YuvImage image = sut.generateYuvImage(ImageFormat.NV21, mTestBitmaps, 0);
        assertNotNull(image);
    }


}
