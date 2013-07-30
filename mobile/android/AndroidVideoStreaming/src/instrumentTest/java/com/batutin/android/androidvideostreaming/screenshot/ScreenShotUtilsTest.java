package com.batutin.android.androidvideostreaming.screenshot;

import android.graphics.Bitmap;
import android.test.ActivityInstrumentationTestCase2;
import android.test.UiThreadTest;
import android.widget.ImageView;

import com.batutin.android.androidvideostreaming.R;
import com.batutin.android.androidvideostreaming.activity.MainActivity;

/**
 * Created by Andrew Batutin on 7/30/13.
 */
public class ScreenShotUtilsTest extends ActivityInstrumentationTestCase2<MainActivity> {

    private ScreenShotUtils sut;
    private ImageView view;

    public ScreenShotUtilsTest() {
        super(MainActivity.class);
    }

    @Override
    public void setUp() throws Exception {
        sut = new ScreenShotUtils();
        view = (ImageView) getActivity().findViewById(R.id.test_view);
    }

    @UiThreadTest
    public void testTakeScreenshot() throws Exception {
        Bitmap bitmap = sut.takeScreenshot(view);
        assertNotNull("bitmap should not be null", bitmap);
    }
}
