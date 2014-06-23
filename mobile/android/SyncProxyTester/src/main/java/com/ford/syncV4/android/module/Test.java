package com.ford.syncV4.android.module;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 18.03.14
 * Time: 12:35
 */

import java.util.ArrayList;
import java.util.List;

/** Represents a test as read from the test file. */
class Test {

    private String name = null;
    private long pause = 0;
    private List<RPCRequestWrapper> requests = null;

    public Test() {
        super();
    }

    public Test(String name, long pause, List<RPCRequestWrapper> requests) {
        this.name = name;
        this.pause = pause;
        this.requests = requests;
    }

    /**
     * Adds the specified request to the list. If the list is null, it's
     * created first.
     *
     * @param request request to add
     */
    public void addRequest(RPCRequestWrapper request) {
        if (requests == null) {
            requests = new ArrayList<RPCRequestWrapper>();
        }

        requests.add(request);
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public long getPause() {
        return pause;
    }

    public void setPause(long pause) {
        this.pause = pause;
    }

    public int getRequestsSize() {
        if (requests == null) {
            return 0;
        }
        return requests.size();
    }

    public List<RPCRequestWrapper> getRequests() {
        return requests;
    }

    public void setRequests(List<RPCRequestWrapper> requests) {
        this.requests = requests;
    }
}