package com.ford.sdlreverse.activity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceManager;
import android.view.Window;

import com.ford.avatar.R;
import com.ford.sdlreverse.util.ActivityUtils;

/**
 * @author vsaenko
 * 
 * @version $Revision: 1.0 $
 */
public class AcSettings extends PreferenceActivity {

	private String mCurrVehicle;
	private String mPreviousVehicle;

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.preference.PreferenceActivity#onCreate(android.os.Bundle)
	 */
	/**
	 * Method onCreate.
	 * 
	 * @param savedInstanceState
	 *            Bundle
	 */
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		super.onCreate(savedInstanceState);

		ActivityUtils.setActivityProperties(this);
		addPreferencesFromResource(R.xml.settings);
		// FULLSCREEN
		final Preference preferenceFullscreen = findPreference("FULLSCREEN");
		preferenceFullscreen
				.setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {

					@Override
					public boolean onPreferenceClick(Preference preference) {
						AvatarActivity.fullscreenPreferenceChanged = !AvatarActivity.fullscreenPreferenceChanged;
						return true;
					}
				});

		// MAPS
		final Preference preferenceMaps = findPreference("MAPS");
		preferenceMaps
				.setOnPreferenceClickListener(new Preference.OnPreferenceClickListener() {

					@Override
					public boolean onPreferenceClick(Preference preference) {
						AvatarActivity.navigationPreferenceChanged = !AvatarActivity.navigationPreferenceChanged;
						return true;
					}
				});
		// CARS
		final Preference preferenceCars = findPreference("VEHICLES");
		preferenceCars
				.setOnPreferenceChangeListener(new Preference.OnPreferenceChangeListener() {

					@Override
					public boolean onPreferenceChange(Preference preference,
							Object newValue) {
						// newvalue is a selected value
						mCurrVehicle = newValue.toString();
						return true;
					}
				});
		
	}

	protected void onResume() {
		super.onResume();
		ActivityUtils.setAppIsForeground(true);
		SharedPreferences shPreferences = PreferenceManager
				.getDefaultSharedPreferences(getApplicationContext());
		mPreviousVehicle = shPreferences.getString("VEHICLES", "Ford");
		mCurrVehicle = mPreviousVehicle;

	}

	protected void onPause() {
		super.onPause();
		ActivityUtils.setAppIsForeground(false);
		if (mPreviousVehicle.compareTo(mCurrVehicle) != 0)
			AvatarActivity.vehiclePreferenceChanged = true;
	}

}