package com.batutin.android.androidvideostreaming.media;

import android.media.CamcorderProfile;

import java.util.Map;

/**
 * Created by Andrew Batutin on 8/9/13.
 */
public class CamcorderProfileUtilsTest extends MediaUtilsTest{

    public CamcorderProfileUtilsTest() {
    }

    public void testGetCamcorderProfileLowQualityShouldReturnCamcorderProfile() throws Exception {
        CamcorderProfile profile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile
                .QUALITY_LOW);
        assertNotNull(profile);
    }

    public void testGetCamcorderProfileHighQualityShouldReturnCamcorderProfile() throws Exception {
        CamcorderProfile profile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(CamcorderProfile
                .QUALITY_HIGH);
        assertNotNull(profile);
    }

    public void testSelectGetCamcorderProfileShouldThroughException() throws Exception {
        try {
            CamcorderProfile profile = CamcorderProfileUtils.getFirstCameraCamcorderProfile(-1);
            assertNull("should not get here", profile);
        } catch (IllegalArgumentException e) {
            assertNotNull(e);
        }
    }

    public void testGetSupportedCamcorderProfileListForCameraShouldReturnListOfCamcorderProfile() throws Exception {
        Map<Integer, CamcorderProfile> resultList = CamcorderProfileUtils.getSupportedCamcorderProfileMapForCamera(0);
        assertNotNull(resultList);
        assertTrue(resultList.size() > 0);
        assertNotNull("QUALITY_LOW is mandatory profile", resultList.containsKey(CamcorderProfile.QUALITY_LOW));
        assertNotNull("QUALITY_HIGH is mandatory profile", resultList.containsKey(CamcorderProfile.QUALITY_HIGH));
    }
}
