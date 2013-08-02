package com.ford.syncV4.protocol.enums;

import com.ford.syncV4.proxy.constants.Names;

import java.util.HashMap;
import java.util.Map;

public class FunctionID {
    private static Map<String, Integer> functionIDMap = null;

    public FunctionID() {
    }

    static public String getFunctionName(int i) {
        if (null == functionIDMap) {
            initFunctionIds();
        }

        for (Map.Entry<String, Integer> entry : functionIDMap.entrySet()) {
            if (i == entry.getValue()) {
                return entry.getKey();
            }
        }

        return null;
    }

    static public int getFunctionID(String functionName) {
        if (null == functionIDMap) {
            initFunctionIds();
        }

        final Integer functionID = functionIDMap.get(functionName);
        return (functionID != null) ? functionID : -1;
    }

    static public void initFunctionIds() {

        functionIDMap = new HashMap<String, Integer>(60) {{
            /*
                Base Request / Response RPCs
                Range = 0x 0000 0001 - 0x 0000 7FFF
             */
            put(Names.RegisterAppInterface, 1);
            put(Names.UnregisterAppInterface, 2);
            put(Names.SetGlobalProperties, 3);
            put(Names.ResetGlobalProperties, 4);
            put(Names.AddCommand, 5);
            put(Names.DeleteCommand, 6);
            put(Names.AddSubMenu, 7);
            put(Names.DeleteSubMenu, 8);
            put(Names.CreateInteractionChoiceSet, 9);
            put(Names.PerformInteraction, 10);
            put(Names.DeleteInteractionChoiceSet, 11);
            put(Names.Alert, 12);
            put(Names.Show, 13);
            put(Names.Speak, 14);
            put(Names.SetMediaClockTimer, 15);
            put(Names.PerformAudioPassThru, 16);
            put(Names.EndAudioPassThru, 17);
            put(Names.SubscribeButton, 18);
            put(Names.UnsubscribeButton, 19);
            put(Names.SubscribeVehicleData, 20);
            put(Names.UnsubscribeVehicleData, 21);
            put(Names.GetVehicleData, 22);
            put(Names.ReadDID, 23);
            put(Names.GetDTCs, 24);
            put(Names.ScrollableMessage, 25);
            put(Names.Slider, 26);
            put(Names.ShowConstantTBT, 27);
            put(Names.AlertManeuver, 28);
            put(Names.UpdateTurnList, 29);
            put(Names.ChangeRegistration, 30);
            put(Names.GenericResponse, 31);
            put(Names.PutFile, 32);
            put(Names.DeleteFile, 33);
            put(Names.ListFiles, 34);
            put(Names.SetAppIcon, 35);
            put(Names.SetDisplayLayout, 36);

            /*
                Base Notifications
                Range = 0x 0000 8000 - 0x 0000 FFFF
             */
            put(Names.OnHMIStatus, 32768);
            put(Names.OnAppInterfaceUnregistered, 32769);
            put(Names.OnButtonEvent, 32770);
            put(Names.OnButtonPress, 32771);
            put(Names.OnVehicleData, 32772);
            put(Names.OnCommand, 32773);
            put(Names.OnTBTClientState, 32774);
            put(Names.OnDriverDistraction, 32775);
            put(Names.OnPermissionsChange, 32776);
            put(Names.OnAudioPassThru, 32777);
            put(Names.OnLanguageChange, 32778);

            /*
                Ford Specific Request / Response RPCs
                Range = 0x 0001 0000 - 0x 0001 7FFF
             */
            put(Names.EncodedSyncPData, 65536);
            put(Names.SyncPData, 65537);

            /*
                Ford Specific Notifications
                Range = 0x 0001 8000 - 0x 0001 FFFF
             */
            put(Names.OnEncodedSyncPData, 98304);
            put(Names.OnSyncPData, 98305);
        }};
    }
}