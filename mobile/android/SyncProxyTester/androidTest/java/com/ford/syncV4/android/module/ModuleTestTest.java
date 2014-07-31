package com.ford.syncV4.android.module;

import android.util.Pair;

import com.ford.syncV4.proxy.rpc.enums.Result;

import junit.framework.TestCase;

import java.util.ArrayList;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 31.07.14
 * Time: 15:14
 */
public class ModuleTestTest extends TestCase {

    public void testDoesEqualsArraysReturnTrueWithSameOrder() {
        final ArrayList<Pair<Integer, Result>> expectingResults = new ArrayList<Pair<Integer, Result>>();
        expectingResults.add(new Pair<Integer, Result>(123, Result.ABORTED));
        expectingResults.add(new Pair<Integer, Result>(456, Result.APPLICATION_NOT_REGISTERED));
        expectingResults.add(new Pair<Integer, Result>(789, Result.APPLICATION_REGISTERED_ALREADY));

        final ArrayList<Pair<Integer, Result>> actualResults = new ArrayList<Pair<Integer, Result>>();
        actualResults.add(new Pair<Integer, Result>(123, Result.ABORTED));
        actualResults.add(new Pair<Integer, Result>(456, Result.APPLICATION_NOT_REGISTERED));
        actualResults.add(new Pair<Integer, Result>(789, Result.APPLICATION_REGISTERED_ALREADY));

        assertTrue(ModuleTest.doesArraysEquals(expectingResults, actualResults));
    }

    public void testDoesEqualsArraysReturnTrueWithDifferentOrder() {
        final ArrayList<Pair<Integer, Result>> expectingResults = new ArrayList<Pair<Integer, Result>>();
        expectingResults.add(new Pair<Integer, Result>(123, Result.ABORTED));
        expectingResults.add(new Pair<Integer, Result>(456, Result.APPLICATION_NOT_REGISTERED));
        expectingResults.add(new Pair<Integer, Result>(789, Result.APPLICATION_REGISTERED_ALREADY));

        final ArrayList<Pair<Integer, Result>> actualResults = new ArrayList<Pair<Integer, Result>>();
        actualResults.add(new Pair<Integer, Result>(789, Result.APPLICATION_REGISTERED_ALREADY));
        actualResults.add(new Pair<Integer, Result>(123, Result.ABORTED));
        actualResults.add(new Pair<Integer, Result>(456, Result.APPLICATION_NOT_REGISTERED));

        assertTrue(ModuleTest.doesArraysEquals(expectingResults, actualResults));
    }

    public void testDoesEqualsArraysReturnTrueWithBothNull() {
        assertTrue(ModuleTest.doesArraysEquals(null, null));
    }

    public void testDoesEqualsArraysReturnTrueWithBothEmpty() {
        assertTrue(ModuleTest.doesArraysEquals(new ArrayList<Pair<Integer, Result>>(),
                new ArrayList<Pair<Integer, Result>>()));
    }

    public void testDoesEqualsArraysReturnFalse() {
        final ArrayList<Pair<Integer, Result>> expectingResults = new ArrayList<Pair<Integer, Result>>();
        expectingResults.add(new Pair<Integer, Result>(123, Result.ABORTED));
        expectingResults.add(new Pair<Integer, Result>(456, Result.APPLICATION_NOT_REGISTERED));
        expectingResults.add(new Pair<Integer, Result>(789, Result.APPLICATION_REGISTERED_ALREADY));

        final ArrayList<Pair<Integer, Result>> actualResults = new ArrayList<Pair<Integer, Result>>();
        actualResults.add(new Pair<Integer, Result>(789, Result.GENERIC_ERROR));
        actualResults.add(new Pair<Integer, Result>(123, Result.DUPLICATE_NAME));
        actualResults.add(new Pair<Integer, Result>(456, Result.SUCCESS));

        assertFalse(ModuleTest.doesArraysEquals(expectingResults, actualResults));
    }

    public void testDoesEqualsArraysReturnFalseWithOneOfThemNull() {
        assertFalse(ModuleTest.doesArraysEquals(new ArrayList<Pair<Integer, Result>>(), null));
        assertFalse(ModuleTest.doesArraysEquals(null, new ArrayList<Pair<Integer, Result>>()));
    }
}