package com.ford.syncV4.proxy.rpc.enums;

import junit.framework.TestCase;

import org.hamcrest.collection.IsArrayContainingInAnyOrder;

import static org.hamcrest.CoreMatchers.is;
import static org.hamcrest.MatcherAssert.assertThat;

/**
 * Tests for VehicleDataType enum.
 *
 * Created by enikolsky on 2014-02-11.
 */
public class VehicleDataTypeTest extends TestCase {
    public void testAllValuesFromSpecificationShouldBeDefined() {
        assertThat(VehicleDataType.values(),
                IsArrayContainingInAnyOrder.arrayContainingInAnyOrder(
                        VehicleDataType.VEHICLEDATA_GPS,
                        VehicleDataType.VEHICLEDATA_SPEED,
                        VehicleDataType.VEHICLEDATA_RPM,
                        VehicleDataType.VEHICLEDATA_FUELLEVEL,
                        VehicleDataType.VEHICLEDATA_FUELLEVEL_STATE,
                        VehicleDataType.VEHICLEDATA_FUELCONSUMPTION,
                        VehicleDataType.VEHICLEDATA_EXTERNTEMP,
                        VehicleDataType.VEHICLEDATA_VIN,
                        VehicleDataType.VEHICLEDATA_PRNDL,
                        VehicleDataType.VEHICLEDATA_TIREPRESSURE,
                        VehicleDataType.VEHICLEDATA_ODOMETER,
                        VehicleDataType.VEHICLEDATA_BELTSTATUS,
                        VehicleDataType.VEHICLEDATA_BODYINFO,
                        VehicleDataType.VEHICLEDATA_DEVICESTATUS,
                        VehicleDataType.VEHICLEDATA_ECALLINFO,
                        VehicleDataType.VEHICLEDATA_AIRBAGSTATUS,
                        VehicleDataType.VEHICLEDATA_EMERGENCYEVENT,
                        VehicleDataType.VEHICLEDATA_CLUSTERMODESTATUS,
                        VehicleDataType.VEHICLEDATA_MYKEY,
                        VehicleDataType.VEHICLEDATA_BRAKING,
                        VehicleDataType.VEHICLEDATA_WIPERSTATUS,
                        VehicleDataType.VEHICLEDATA_HEADLAMPSTATUS,
                        VehicleDataType.VEHICLEDATA_BATTVOLTAGE,
                        VehicleDataType.VEHICLEDATA_ENGINETORQUE,
                        VehicleDataType.VEHICLEDATA_ACCPEDAL,
                        VehicleDataType.VEHICLEDATA_STEERINGWHEEL));
    }


    public void testValueForStringVEHICLEDATA_GPSShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_GPS"),
                is(VehicleDataType.VEHICLEDATA_GPS));
    }

    public void testValueForStringVEHICLEDATA_SPEEDShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_SPEED"),
                is(VehicleDataType.VEHICLEDATA_SPEED));
    }

    public void testValueForStringVEHICLEDATA_RPMShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_RPM"),
                is(VehicleDataType.VEHICLEDATA_RPM));
    }

    public void testValueForStringVEHICLEDATA_FUELLEVELShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_FUELLEVEL"),
                is(VehicleDataType.VEHICLEDATA_FUELLEVEL));
    }

    public void testValueForStringVEHICLEDATA_FUELLEVEL_STATEShouldReturnCorrectValue() {
        assertThat(
                VehicleDataType.valueForString("VEHICLEDATA_FUELLEVEL_STATE"),
                is(VehicleDataType.VEHICLEDATA_FUELLEVEL_STATE));
    }

    public void testValueForStringVEHICLEDATA_FUELCONSUMPTIONShouldReturnCorrectValue() {
        assertThat(
                VehicleDataType.valueForString("VEHICLEDATA_FUELCONSUMPTION"),
                is(VehicleDataType.VEHICLEDATA_FUELCONSUMPTION));
    }

    public void testValueForStringVEHICLEDATA_EXTERNTEMPShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_EXTERNTEMP"),
                is(VehicleDataType.VEHICLEDATA_EXTERNTEMP));
    }

    public void testValueForStringVEHICLEDATA_VINShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_VIN"),
                is(VehicleDataType.VEHICLEDATA_VIN));
    }

    public void testValueForStringVEHICLEDATA_PRNDLShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_PRNDL"),
                is(VehicleDataType.VEHICLEDATA_PRNDL));
    }

    public void testValueForStringVEHICLEDATA_TIREPRESSUREShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_TIREPRESSURE"),
                is(VehicleDataType.VEHICLEDATA_TIREPRESSURE));
    }

    public void testValueForStringVEHICLEDATA_ODOMETERShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_ODOMETER"),
                is(VehicleDataType.VEHICLEDATA_ODOMETER));
    }

    public void testValueForStringVEHICLEDATA_BELTSTATUSShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_BELTSTATUS"),
                is(VehicleDataType.VEHICLEDATA_BELTSTATUS));
    }

    public void testValueForStringVEHICLEDATA_BODYINFOShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_BODYINFO"),
                is(VehicleDataType.VEHICLEDATA_BODYINFO));
    }

    public void testValueForStringVEHICLEDATA_DEVICESTATUSShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_DEVICESTATUS"),
                is(VehicleDataType.VEHICLEDATA_DEVICESTATUS));
    }

    public void testValueForStringVEHICLEDATA_ECALLINFOShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_ECALLINFO"),
                is(VehicleDataType.VEHICLEDATA_ECALLINFO));
    }

    public void testValueForStringVEHICLEDATA_AIRBAGSTATUSShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_AIRBAGSTATUS"),
                is(VehicleDataType.VEHICLEDATA_AIRBAGSTATUS));
    }

    public void testValueForStringVEHICLEDATA_EMERGENCYEVENTShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_EMERGENCYEVENT"),
                is(VehicleDataType.VEHICLEDATA_EMERGENCYEVENT));
    }

    public void testValueForStringVEHICLEDATA_CLUSTERMODESTATUSShouldReturnCorrectValue() {
        assertThat(
                VehicleDataType.valueForString("VEHICLEDATA_CLUSTERMODESTATUS"),
                is(VehicleDataType.VEHICLEDATA_CLUSTERMODESTATUS));
    }

    public void testValueForStringVEHICLEDATA_MYKEYShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_MYKEY"),
                is(VehicleDataType.VEHICLEDATA_MYKEY));
    }

    public void testValueForStringVEHICLEDATA_BRAKINGShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_BRAKING"),
                is(VehicleDataType.VEHICLEDATA_BRAKING));
    }

    public void testValueForStringVEHICLEDATA_WIPERSTATUSShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_WIPERSTATUS"),
                is(VehicleDataType.VEHICLEDATA_WIPERSTATUS));
    }

    public void testValueForStringVEHICLEDATA_HEADLAMPSTATUSShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_HEADLAMPSTATUS"),
                is(VehicleDataType.VEHICLEDATA_HEADLAMPSTATUS));
    }

    public void testValueForStringVEHICLEDATA_BATTVOLTAGEShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_BATTVOLTAGE"),
                is(VehicleDataType.VEHICLEDATA_BATTVOLTAGE));
    }

    public void testValueForStringVEHICLEDATA_ENGINETORQUEShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_ENGINETORQUE"),
                is(VehicleDataType.VEHICLEDATA_ENGINETORQUE));
    }

    public void testValueForStringVEHICLEDATA_ACCPEDALShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_ACCPEDAL"),
                is(VehicleDataType.VEHICLEDATA_ACCPEDAL));
    }

    public void testValueForStringVEHICLEDATA_STEERINGWHEELShouldReturnCorrectValue() {
        assertThat(VehicleDataType.valueForString("VEHICLEDATA_STEERINGWHEEL"),
                is(VehicleDataType.VEHICLEDATA_STEERINGWHEEL));
    }


    public void testVEHICLEDATA_GPSToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_GPS.toString(),
                is("VEHICLEDATA_GPS"));
    }

    public void testVEHICLEDATA_SPEEDToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_SPEED.toString(),
                is("VEHICLEDATA_SPEED"));
    }

    public void testVEHICLEDATA_RPMToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_RPM.toString(),
                is("VEHICLEDATA_RPM"));
    }

    public void testVEHICLEDATA_FUELLEVELToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_FUELLEVEL.toString(),
                is("VEHICLEDATA_FUELLEVEL"));
    }

    public void testVEHICLEDATA_FUELLEVEL_STATEToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_FUELLEVEL_STATE.toString(),
                is("VEHICLEDATA_FUELLEVEL_STATE"));
    }

    public void testVEHICLEDATA_FUELCONSUMPTIONToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_FUELCONSUMPTION.toString(),
                is("VEHICLEDATA_FUELCONSUMPTION"));
    }

    public void testVEHICLEDATA_EXTERNTEMPToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_EXTERNTEMP.toString(),
                is("VEHICLEDATA_EXTERNTEMP"));
    }

    public void testVEHICLEDATA_VINToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_VIN.toString(),
                is("VEHICLEDATA_VIN"));
    }

    public void testVEHICLEDATA_PRNDLToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_PRNDL.toString(),
                is("VEHICLEDATA_PRNDL"));
    }

    public void testVEHICLEDATA_TIREPRESSUREToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_TIREPRESSURE.toString(),
                is("VEHICLEDATA_TIREPRESSURE"));
    }

    public void testVEHICLEDATA_ODOMETERToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_ODOMETER.toString(),
                is("VEHICLEDATA_ODOMETER"));
    }

    public void testVEHICLEDATA_BELTSTATUSToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_BELTSTATUS.toString(),
                is("VEHICLEDATA_BELTSTATUS"));
    }

    public void testVEHICLEDATA_BODYINFOToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_BODYINFO.toString(),
                is("VEHICLEDATA_BODYINFO"));
    }

    public void testVEHICLEDATA_DEVICESTATUSToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_DEVICESTATUS.toString(),
                is("VEHICLEDATA_DEVICESTATUS"));
    }

    public void testVEHICLEDATA_ECALLINFOToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_ECALLINFO.toString(),
                is("VEHICLEDATA_ECALLINFO"));
    }

    public void testVEHICLEDATA_AIRBAGSTATUSToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_AIRBAGSTATUS.toString(),
                is("VEHICLEDATA_AIRBAGSTATUS"));
    }

    public void testVEHICLEDATA_EMERGENCYEVENTToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_EMERGENCYEVENT.toString(),
                is("VEHICLEDATA_EMERGENCYEVENT"));
    }

    public void testVEHICLEDATA_CLUSTERMODESTATUSToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_CLUSTERMODESTATUS.toString(),
                is("VEHICLEDATA_CLUSTERMODESTATUS"));
    }

    public void testVEHICLEDATA_MYKEYToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_MYKEY.toString(),
                is("VEHICLEDATA_MYKEY"));
    }

    public void testVEHICLEDATA_BRAKINGToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_BRAKING.toString(),
                is("VEHICLEDATA_BRAKING"));
    }

    public void testVEHICLEDATA_WIPERSTATUSToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_WIPERSTATUS.toString(),
                is("VEHICLEDATA_WIPERSTATUS"));
    }

    public void testVEHICLEDATA_HEADLAMPSTATUSToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_HEADLAMPSTATUS.toString(),
                is("VEHICLEDATA_HEADLAMPSTATUS"));
    }

    public void testVEHICLEDATA_BATTVOLTAGEToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_BATTVOLTAGE.toString(),
                is("VEHICLEDATA_BATTVOLTAGE"));
    }

    public void testVEHICLEDATA_ENGINETORQUEToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_ENGINETORQUE.toString(),
                is("VEHICLEDATA_ENGINETORQUE"));
    }

    public void testVEHICLEDATA_ACCPEDALToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_ACCPEDAL.toString(),
                is("VEHICLEDATA_ACCPEDAL"));
    }

    public void testVEHICLEDATA_STEERINGWHEELToStringShouldReturnCorrectValue() {
        assertThat(VehicleDataType.VEHICLEDATA_STEERINGWHEEL.toString(),
                is("VEHICLEDATA_STEERINGWHEEL"));
    }

    public void testValueForStringUnknownShouldThrowIllegalArgumentException() {
        try {
            VehicleDataType.valueForString("Unknown");
            fail("Should have thrown IllegalArgumentException");
        } catch (IllegalArgumentException e) {
            // success
        }
    }
}
