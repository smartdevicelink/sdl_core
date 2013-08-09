package com.batutin.android.androidvideostreaming.media;

import android.hardware.Camera;
import android.media.CamcorderProfile;

import java.util.HashMap;
import java.util.Map;

public class CamcorderProfileUtils {

    public static CamcorderProfile getFirstCameraCamcorderProfile(int quality) throws IllegalArgumentException {
        int nCamera = Camera.getNumberOfCameras();
        CamcorderProfile profile = null;
        for (int cameraId = 0; cameraId < nCamera; cameraId++) {
            profile = getCamcorderProfile(cameraId, quality);
            break;
        }
        if (profile == null) {
            throw new IllegalArgumentException("CamcorderProfile is null");
        }
        return profile;
    }

    public static Map<Integer, CamcorderProfile> getSupportedCamcorderProfileMapForCamera(int cameraId) {
        Map<Integer, CamcorderProfile> profileList = new HashMap<Integer, CamcorderProfile>();
        for (int quality = CamcorderProfile.QUALITY_LOW; quality <= CamcorderProfile.QUALITY_QVGA; quality++) {
            CamcorderProfile profile = getCamcorderProfile(cameraId, quality);
            if (profile != null) {
                profileList.put(quality, profile);
            }
        }
        return profileList;
    }

    private static CamcorderProfile getCamcorderProfile(int cameraId, int quality) throws IllegalArgumentException {
        if (CamcorderProfile.hasProfile(cameraId, quality) == false) {
            return null;
        }
        CamcorderProfile profile = CamcorderProfile.get(cameraId, quality);
        return profile;
    }
}