/**
 * @file VehicleInfoModel.qml
 * @brief Data model for vehicle info.
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

import QtQuick 1.1
import "../hmi_api/Common.js" as Common

QtObject {
    id: vehicleInfo
    property real speed: 80.1
    property int rpm: 5000
    property real fuelLevel: 0.2
    property int fuelLevel_State: Common.ComponentVolumeStatus.CVS_LOW
    property real instantFuelConsumption: 2.2
    property int driverBraking: Common.VehicleDataEventStatus.VDES_NO_EVENT
    property int wiperStatus: Common.WiperStatus.OFF
    property real engineTorque: 2.5
    property real accPedalPosition: 0.5
    property real steeringWheelAngle: 1.2
    property real externalTemperature: 40.0
    property string vin: '52-452-52-752'
    property int odometer: 2
    property int prndl: Common.PRNDL.FIFTH

    property variant headLampStatus: {
        "lowBeamsOn": false,
        "highBeamsOn": false,
        "ambientLightSensorStatus": Common.AmbientLightStatus.TWILIGHT_1
    }
    property variant deviceStatus: {
        'voiceRecOn': false,
        'btIconOn': false,
        'callActive': false,
        'phoneRoaming': false,
        'textMsgAvailable': false,
        'battLevelStatus': Common.DeviceLevelStatus.ZERO_LEVEL_BARS,
        'stereoAudioOutputMuted': false,
        'monoAudioOutputMuted': false,
        'signalLevelStatus': Common.DeviceLevelStatus.ZERO_LEVEL_BARS,
        'primaryAudioSource': Common.PrimaryAudioSource.NO_SOURCE_SELECTED,
        'eCallEventActive': false
    }
    property variant bodyInformation: {
        'parkBrakeActive': false,
        'ignitionStableStatus': Common.IgnitionStableStatus.IGNITION_SWITCH_NOT_STABLE,
        'ignitionStatus': Common.IgnitionStatus.IS_UNKNOWN
    }
    property variant beltStatus: {
        'driverBeltDeployed': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'passengerBeltDeployed': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'passengerBuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'driverBuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'leftRow2BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'passengerChildDetected': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'rightRow2BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow2BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow3BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'leftRow3BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'rightRow3BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'leftRearInflatableBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'rightRearInflatableBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow1BeltDeployed': Common.VehicleDataEventStatus.VDES_NO_EVENT,
        'middleRow1BuckleBelted': Common.VehicleDataEventStatus.VDES_NO_EVENT,
    }
    property variant tirePressure: {
        'pressureTelltale': Common.WarningLightStatus.WLS_ON,
        'leftFront': { status: Common.ComponentVolumeStatus.CVS_NORMAL},
        'rightFront': { status:  Common.ComponentVolumeStatus.CVS_UNKNOWN },
        'leftRear': { status: Common.ComponentVolumeStatus.CVS_NORMAL },
        'rightRear': { status: Common.ComponentVolumeStatus.CVS_UNKNOWN },
        'innerLeftRear': { status: Common.ComponentVolumeStatus.CVS_UNKNOWN },
        'innerRightRear': { status: Common.ComponentVolumeStatus.CVS_UNKNOWN }
    }
    property variant gps: {
        'longitudeDegrees': 42.3293,
        'latitudeDegrees': -83.0464,
        'utcYear': 2013,
        'utcMonth': 2,
        'utcDay': 14,
        'utcHours': 13,
        'utcMinutes': 16,
        'utcSeconds': 54,
        'compassDirection': Common.CompassDirection.SOUTHWEST,
        'pdop': 4,
        'hdop': 5,
        'vdop': 6,
        'actual': false,
        'satellites': 8,
        'dimension': Common.Dimension.Dimension_2D,
        'altitude': 7,
        'heading': 173,
        'speed': 2
    }
    property variant ecuDIDData: {
        'data1': 'ECU 1 Test Data',
        'data2': 'ECU 2 Test Data'
    }
    property variant eCallInfo: {
        "eCallNotificationStatus" : Common.VehicleDataNotificationStatus.VDNS_NORMAL,
        "auxECallNotificationStatus" : Common.VehicleDataNotificationStatus.VDNS_ACTIVE,
        "eCallConfirmationStatus" : Common.ECallConfirmationStatus.ECCS_NORMAL
    }
    property variant airbagStatus: {
        "driverAirbagDeployed" : Common.VehicleDataEventStatus.VDES_NO_EVENT,
        "driverSideAirbagDeployed" : Common.VehicleDataEventStatus.VDES_NO,
        "driverCurtainAirbagDeployed" : Common.VehicleDataEventStatus.VDES_YES,
        "passengerAirbagDeployed" : Common.VehicleDataEventStatus.VDES_NOT_SUPPORTED,
        "passengerCurtainAirbagDeployed" : Common.VehicleDataEventStatus.VDES_FAULT,
        "driverKneeAirbagDeployed" : Common.VehicleDataEventStatus.VDES_NO,
        "passengerSideAirbagDeployed" : Common.VehicleDataEventStatus.VDES_YES,
        "passengerKneeAirbagDeployed" : Common.VehicleDataEventStatus.VDES_NOT_SUPPORTED
    }
    property variant emergencyEvent: {
        "emergencyEventType" : Common.EmergencyEventType.EET_NO_EVENT,
        "fuelCutoffStatus" : Common.EmergencyEventType.EET_FRONTAL,
        "rolloverEvent" : Common.EmergencyEventType.EET_SIDE,
        "maximumChangeVelocity" : Common.EmergencyEventType.EET_REAR,
        "multipleEvents" : Common.EmergencyEventType.EET_ROLLOVER
    }
    property variant clusterModeStatus: {
        "powerModeActive" : true,
        "powerModeQualificationStatus" : Common.PowerModeQualificationStatus.POWER_MODE_UNDEFINED,
        "carModeStatus" : Common.CarModeStatus.CMS_NORMAL,
        "powerModeStatus" : Common.PowerModeStatus.KEY_OUT
    }
    property variant myKey: {
        "e911Override": Common.VehicleDataStatus.VDS_NO_DATA_EXISTS
    }


    function sendGpsChange(){
        sdlVehicleInfo.onGps(vehicleInfo.gps)
    }
    function sendSpeedChange(){
        sdlVehicleInfo.onSpeed(vehicleInfo.speed)
    }
    function sendRpmChange(){
        sdlVehicleInfo.onRpm(vehicleInfo.rpm)
    }
    function sendFuelLevelChange(){
        sdlVehicleInfo.onFuelLevel(vehicleInfo.fuelLevel)
    }
    function sendFuelLevel_StateChange(){
        sdlVehicleInfo.onFuelLevel_State(vehicleInfo.fuelLevel_State)
    }
    function sendInstantFuelConsumptionChange(){
        sdlVehicleInfo.onInstantFuelConsumption(vehicleInfo.instantFuelConsumption)
    }
    function sendExternalTemperatureChange(){
        sdlVehicleInfo.onExternalTemperature(vehicleInfo.externalTemperature)
    }
    function sendPrndlChange() {
        if (dataContainer.vehicleInfoModel.prndl !== -1)
        {sdlVehicleInfo.onPrndl(vehicleInfo.prndl)}
    }
    function sendTirePressureChange(){
        sdlVehicleInfo.onTirePressure(vehicleInfo.tirePressure)
    }
    function sendOdometerChange(){
        sdlVehicleInfo.onOdometer(vehicleInfo.odometer)
    }
    function sendBeltStatusChange(){
        sdlVehicleInfo.onBeltStatus(vehicleInfo.beltStatus)
    }
    function sendBodyInformationChange(){
        sdlVehicleInfo.onBodyInformation(vehicleInfo.bodyInformation)
    }
    function sendDeviceStatusChange(){
        sdlVehicleInfo.onDeviceStatus(vehicleInfo.deviceStatus)
    }
    function sendDriverBrakingChange(){
        sdlVehicleInfo.onDriverBraking(vehicleInfo.driverBraking)
    }
    function sendWiperStatusChange(){
        sdlVehicleInfo.onWiperStatus(vehicleInfo.wiperStatus)
    }
    function sendHeadLampStatusChange(){
        sdlVehicleInfo.onHeadLampStatus(vehicleInfo.headLampStatus)
    }
    function sendEngineTorqueChange(){
        sdlVehicleInfo.onEngineTorque(vehicleInfo.engineTorque)
    }
    function sendAccPedalPositionChange(){
        sdlVehicleInfo.onAccPedalPosition(vehicleInfo.accPedalPosition)
    }
    function sendSteeringWheelAngleChange(){
        sdlVehicleInfo.onSteeringWheelAngle(vehicleInfo.steeringWheelAngle)
    }
    function sendECallInfoChange(){
        sdlVehicleInfo.onECallInfo(vehicleInfo.eCallInfo)
    }
    function sendAirbagStatusChange(){
        sdlVehicleInfo.onAirbagStatus(vehicleInfo.airbagStatus)
    }
    function sendEmergencyEventChange(){
        sdlVehicleInfo.onEmergencyEvent(vehicleInfo.emergencyEvent)
    }
    function sendClusterModeStatusChange(){
        sdlVehicleInfo.onClusterModeStatus(vehicleInfo.clusterModeStatus)
    }
    function sendMyKeyChange(){
        sdlVehicleInfo.onMyKey(vehicleInfo.myKey)
    }
}
