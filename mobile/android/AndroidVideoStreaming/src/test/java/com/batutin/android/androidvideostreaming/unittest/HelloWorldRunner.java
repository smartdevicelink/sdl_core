package com.batutin.android.androidvideostreaming.unittest;

import com.xtremelabs.robolectric.RobolectricTestRunner;
import org.junit.runners.model.InitializationError;

import java.io.File;

public class HelloWorldRunner extends RobolectricTestRunner {
	public HelloWorldRunner(Class testClass) throws InitializationError {
		super(testClass, new File("src/main"));
	}
}
