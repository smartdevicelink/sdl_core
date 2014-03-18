package com.ford.syncV4.android.module;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.03.14
 * Time: 12:32
 */
/**
 * This class holds a result of the single test execution.
 */
public class TestResult {

    /**
     * Indicates that single test is completed
     */
    private boolean mTestComplete;

    /**
     * Indicates that a test flow must be terminated
     */
    private boolean mTestFlowInterrupted;

    /**
     * @return a complete result of the single test run
     */
    public boolean isTestComplete() {
        return mTestComplete;
    }

    /**
     * Set a complete result of the single test run
     * @param mTestComplete boolean value
     */
    public void setTestComplete(boolean mTestComplete) {
        this.mTestComplete = mTestComplete;
    }

    /**
     * @return <b>true</b> if the entire test flow process must be terminated, for example if the
     * connection lost, <b>false</b> otherwise (a default value)
     */
    public boolean isTestFlowInterrupted() {
        return mTestFlowInterrupted;
    }

    /**
     * Set <b>true</b> if the entire test flow process must be terminated, for example if the
     * connection lost, <b>false</b> otherwise (a default value)
     * @param mTestFlowInterrupted boolean value
     */
    public void setTestFlowInterrupted(boolean mTestFlowInterrupted) {
        this.mTestFlowInterrupted = mTestFlowInterrupted;
    }
}