package com.batutin.android.androidvideostreaming.media;

import android.hardware.Camera;
import android.media.CamcorderProfile;

import com.batutin.android.androidvideostreaming.activity.ALog;

import java.util.HashMap;
import java.util.Map;

public class CamcorderProfileUtils {

    public static CamcorderProfile getFirstCameraCamcorderProfile(int quality) throws IllegalArgumentException {
        ALog.d("start getting CamcorderProfile");
        int nCamera = Camera.getNumberOfCameras();
        CamcorderProfile profile = null;
        for (int cameraId = 0; cameraId < nCamera; cameraId++) {
            profile = getCamcorderProfile(cameraId, quality);
            break;
        }
        if (profile == null) {
            ALog.e("failed to get CamcorderProfile");
            throw new IllegalArgumentException("CamcorderProfile is null");
        }
        ALog.d("CamcorderProfile " + profile.toString() + " found");
        return profile;
    }

    public static Map<Integer, CamcorderProfile> getSupportedCamcorderProfileMapForCamera(int cameraId) {
        ALog.d("start getting CamcorderProfile map");
        Map<Integer, CamcorderProfile> profileMap = new HashMap<Integer, CamcorderProfile>();
        for (int quality = CamcorderProfile.QUALITY_LOW; quality <= CamcorderProfile.QUALITY_QVGA; quality++) {
            CamcorderProfile profile = getCamcorderProfile(cameraId, quality);
            if (profile != null) {
                profileMap.put(quality, profile);
            }
        }
        ALog.d("CamcorderProfile " + profileMap.toString() + " found");
        return profileMap;
    }

    private static CamcorderProfile getCamcorderProfile(int cameraId, int quality) throws IllegalArgumentException {
        if (CamcorderProfile.hasProfile(cameraId, quality) == false) {
            return null;
        }
        CamcorderProfile profile = CamcorderProfile.get(cameraId, quality);
        return profile;
    }
}