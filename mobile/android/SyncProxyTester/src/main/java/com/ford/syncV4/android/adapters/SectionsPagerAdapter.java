package com.ford.syncV4.android.adapters;

import android.content.res.Resources;
import android.support.v4.app.FragmentManager;
import android.util.Log;

import com.ford.syncV4.android.activity.PlaceholderFragment;

import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * Created with Android Studio.
 * User: Yuriy Chernyshov
 * Date: 5/8/14
 * Time: 8:22 PM
 */
public class SectionsPagerAdapter extends FragmentPagerAdapterCustom {

    private static final String LOG_TAG = SectionsPagerAdapter.class.getSimpleName();

    private Resources mResources;
    private int mCounter = 0;
    private final List<PlaceholderFragment> fragments = new ArrayList<PlaceholderFragment>();

    public SectionsPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    public void setResources(Resources mResources) {
        this.mResources = mResources;
    }

    @Override
    public PlaceholderFragment getItem(int position) {
        // getItem is called to instantiate the fragment for the given page.
        // Return a PlaceholderFragment (defined as a static inner class below).

        PlaceholderFragment fragment;
        if (fragments.size() - 1 < position) {
            fragment = PlaceholderFragment.newInstance(position + 1);
            fragments.add(fragment);
        } else {
            fragment = fragments.get(position);
        }

        Log.d(LOG_TAG, "Add Fragment hash:" + fragment.hashCode() + " to List");

        return fragment;
    }

    @Override
    public int getCount() {
        return mCounter;
    }

    @Override
    public CharSequence getPageTitle(int position) {
        Locale locale = Locale.getDefault();
        return "Tab:" + mCounter;
    }

    public void incrementCounter() {
        mCounter++;
        notifyDataSetChanged();
    }

    public void decrementCounter() {
        mCounter--;
        notifyDataSetChanged();
    }

    public void removeFragmentFromList(int position) {

        //PlaceholderFragment fragment = fragments.get(position);
        //removeFragment(fragment.getAppId());

        //fragments.remove(position);

        //fragment.onDestroy();
        //fragments.add(position, null);
        //destroyItem(null, position, fragment);

        //Log.d(LOG_TAG, "Fragment is " + fragment.hashCode() + " removed, pos:" + position);
    }
}