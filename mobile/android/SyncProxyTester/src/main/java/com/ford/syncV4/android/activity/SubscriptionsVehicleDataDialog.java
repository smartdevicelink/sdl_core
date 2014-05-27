package com.ford.syncV4.android.activity;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.content.Context;
import android.content.DialogInterface;
import android.os.Bundle;

import com.ford.syncV4.proxy.rpc.SubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.UnsubscribeVehicleData;
import com.ford.syncV4.proxy.rpc.enums.VehicleDataType;

import java.util.HashMap;
import java.util.Map;
import java.util.Vector;

/**
 * Created with Android Studio.
 * Author: Chernyshov Yuriy - Mobile Development
 * Date: 2/24/14
 * Time: 2:19 PM
 */
public class SubscriptionsVehicleDataDialog extends DialogFragment {

    private static final String LOG_TAG = "SubscriptionsVehicleDataDialog";

    private static final Map<VehicleDataType, String> METHOD_NAMES_MAP =
            new HashMap<VehicleDataType, String>() {{
                put(VehicleDataType.VEHICLEDATA_GPS, "Gps");
                put(VehicleDataType.VEHICLEDATA_SPEED, "Speed");
                put(VehicleDataType.VEHICLEDATA_RPM, "Rpm");
                put(VehicleDataType.VEHICLEDATA_FUELLEVEL, "FuelLevel");
                put(VehicleDataType.VEHICLEDATA_FUELLEVEL_STATE, "FuelLevel_State");
                put(VehicleDataType.VEHICLEDATA_FUELCONSUMPTION, "InstantFuelConsumption");
                put(VehicleDataType.VEHICLEDATA_EXTERNTEMP, "ExternalTemperature");
                // put(VehicleDataType.VEHICLEDATA_VIN, "VIN");
                put(VehicleDataType.VEHICLEDATA_PRNDL, "Prndl");
                put(VehicleDataType.VEHICLEDATA_TIREPRESSURE, "TirePressure");
                put(VehicleDataType.VEHICLEDATA_ODOMETER, "Odometer");
                put(VehicleDataType.VEHICLEDATA_BELTSTATUS, "BeltStatus");
                put(VehicleDataType.VEHICLEDATA_BODYINFO, "BodyInformation");
                put(VehicleDataType.VEHICLEDATA_DEVICESTATUS, "DeviceStatus");
                put(VehicleDataType.VEHICLEDATA_BRAKING, "DriverBraking");
                put(VehicleDataType.VEHICLEDATA_WIPERSTATUS, "WiperStatus");
                put(VehicleDataType.VEHICLEDATA_HEADLAMPSTATUS, "HeadLampStatus");
                put(VehicleDataType.VEHICLEDATA_BATTVOLTAGE, "BatteryVoltage");
                put(VehicleDataType.VEHICLEDATA_ENGINETORQUE, "EngineTorque");
                put(VehicleDataType.VEHICLEDATA_ACCPEDAL, "AccPedalPosition");
                put(VehicleDataType.VEHICLEDATA_STEERINGWHEEL, "SteeringWheelAngle");
                put(VehicleDataType.VEHICLEDATA_ECALLINFO, "ECallInfo");
                put(VehicleDataType.VEHICLEDATA_AIRBAGSTATUS, "AirbagStatus");
                put(VehicleDataType.VEHICLEDATA_EMERGENCYEVENT, "EmergencyEvent");
                put(VehicleDataType.VEHICLEDATA_CLUSTERMODESTATUS, "ClusterModeStatus");
                put(VehicleDataType.VEHICLEDATA_MYKEY, "MyKey");
            }};

    public static SubscriptionsVehicleDataDialog newInstance() {
        SubscriptionsVehicleDataDialog subscriptionsVehicleDataDialog = new SubscriptionsVehicleDataDialog();
        return subscriptionsVehicleDataDialog;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        final Context mContext = getActivity();

        // the local copy of isVehicleDataSubscribed
        final boolean[] checkedVehicleDataTypes = ((SyncProxyTester) getActivity())
                .cloneIsVehicleDataSubscribed();

        return new AlertDialog.Builder(mContext)
                .setMultiChoiceItems(vehicleDataTypeNames(), checkedVehicleDataTypes,
                        new DialogInterface.OnMultiChoiceClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which, boolean isChecked) {
                        /**
                         * NB! This method is intentionally left empty. If the 3rd
                         * parameter to setMultiChoiceItems() is null, the user's
                         * changes to checked items don't save.
                         **/
                    }
                })
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        Vector<VehicleDataType> subscribeVehicleData = new Vector<VehicleDataType>();
                        Vector<VehicleDataType> unsubscribeVehicleData = new Vector<VehicleDataType>();
                        VehicleDataType[] dataTypes = VehicleDataType.values();

                        // subscribe and unsubscribe to new checked/unchecked items only
                        for (int i = 0; i < checkedVehicleDataTypes.length; i++) {
                            boolean checked = checkedVehicleDataTypes[i];
                            boolean wasChecked = ((SyncProxyTester) getActivity())
                                    .getIsVehicleDataSubscribedAt(i);
                            if (checked && !wasChecked) {
                                subscribeVehicleData.add(dataTypes[i]);
                            } else if (!checked && wasChecked) {
                                unsubscribeVehicleData.add(dataTypes[i]);
                            }
                        }

                        if (!subscribeVehicleData.isEmpty()) {
                            SubscribeVehicleData msg = new SubscribeVehicleData();
                            for (VehicleDataType vdt : subscribeVehicleData) {
                                ((SyncProxyTester) getActivity())
                                        .setVehicleDataParam(msg, SubscribeVehicleData.class,
                                        "set" + METHOD_NAMES_MAP.get(vdt));
                            }
                            msg.setCorrelationID(((SyncProxyTester) getActivity()).getNextCorrelationIdForCurrentFragment());

                            ((SyncProxyTester) getActivity()).onSubscribeVehicleDialogResult(msg);
                        }

                        if (!unsubscribeVehicleData.isEmpty()) {
                            UnsubscribeVehicleData msg = new UnsubscribeVehicleData();
                            for (VehicleDataType vdt : unsubscribeVehicleData) {
                                ((SyncProxyTester) getActivity()).
                                        setVehicleDataParam(msg, UnsubscribeVehicleData.class,
                                        "set" + METHOD_NAMES_MAP.get(vdt));
                            }
                            msg.setCorrelationID(((SyncProxyTester) getActivity()).getNextCorrelationIdForCurrentFragment());

                            ((SyncProxyTester) getActivity()).onUnsubscribeVehicleDialogResult(msg);
                        }
                        ((SyncProxyTester) getActivity())
                                .setIsVehicleDataSubscribed(checkedVehicleDataTypes.clone());

                        if (subscribeVehicleData.isEmpty() && unsubscribeVehicleData.isEmpty()) {
                            SafeToast.showToastAnyThread("Nothing new here");
                        }
                    }
                })
                .setNegativeButton("Cancel", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                })
                .show();
    }

    private String[] vehicleDataTypeNames() {
        return new String[]{ "GPS", "Speed", "RPM",
                "Fuel Level", "Fuel Level State",
                "Fuel Consumption", "External Temp",
                "VIN", "PRNDL", "Tire Pressure",
                "Odometer", "Belt Status",
                "Body Info", "Device Status",
                "Braking", "Wiper Status",
                "Head Lamp Status", "Batt Voltage",
                "Engine Torque", "Acc Pedal",
                "Steering Wheel", "ECall Info",
                "Airbag Status", "Emergency Event",
                "Cluster Mode Status", "MyKey" };
    }
}