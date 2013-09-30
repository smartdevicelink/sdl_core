package com.ford.syncV4.streaming;

import java.util.Arrays;

/**
 * Created by Andrew Batutin on 9/30/13.
 */
public class MobileNaviDataFrame {

    public final static int MOBILE_NAVI_DATA_SIZE = 1000;
    private byte[] data;

    public byte[] getData() {
        return data;
    }

    public MobileNaviDataFrame(byte[] data) {
        checkPreconditions(data);
        this.data = data;
    }

    private void checkPreconditions(byte[] data) {
        if (data == null) {
            throw new IllegalArgumentException("data length must be !=  null");
        }
        if (data.length > MOBILE_NAVI_DATA_SIZE) {
            throw new IllegalArgumentException("data length must be > MOBILE_NAVI_DATA_SIZE");
        }
    }

    public static MobileNaviDataFrame createEndOfSessionFrame() {
        MobileNaviDataFrame frame = new MobileNaviDataFrame(new byte[]{-1});
        return frame;
    }

    @Override
    public String toString() {
        String result = super.toString() + "; data = " + Arrays.toString(data);
        return result;
    }
}
