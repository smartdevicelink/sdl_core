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

    property variant myKey: {
        "e911Override": Common.VehicleDataStatus.VDS_NO_DATA_EXISTS
    }
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
        'longitudeDegrees': 423293,
        'latitudeDegrees': -830464,
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

    onSpeedChanged: { sdlVehicleInfo.onSpeed(vehicleInfo.speed) }
    onRpmChanged: { sdlVehicleInfo.onRpm(vehicleInfo.rpm) }
    onFuelLevelChanged: { sdlVehicleInfo.onFuelLevel(vehicleInfo.fuelLevel) }
    onFuelLevel_StateChanged:  { sdlVehicleInfo.onFuelLevelState(vehicleInfo.fuelLevel_State) }
    onInstantFuelConsumptionChanged: { sdlVehicleInfo.onInstantFuelConsumption(vehicleInfo.instantFuelConsumption) }
    onDriverBrakingChanged: { sdlVehicleInfo.onDriverBraking(vehicleInfo.driverBraking) }
    onWiperStatusChanged: { sdlVehicleInfo.onWiperStatus(vehicleInfo.wiperStatus) }
    onEngineTorqueChanged: { sdlVehicleInfo.onEngineTorque(vehicleInfo.engineTorque) }
    onAccPedalPositionChanged: { sdlVehicleInfo.onAccPedalPosition(vehicleInfo.accPedalPosition) }
    onSteeringWheelAngleChanged: { sdlVehicleInfo.onSteeringWheelAngle(vehicleInfo.steeringWheelAngle) }
    onExternalTemperatureChanged: { sdlVehicleInfo.onExternalTemperature(vehicleInfo.externalTemperature) }
    onVinChanged: { sdlVehicleInfo.onVin(vehicleInfo.vin) }
    onOdometerChanged: { sdlVehicleInfo.onOdometer(vehicleInfo.odometer) }
    onPrndlChanged: { if (dataContainer.vehicleInfoModel.prndl !== -1) {sdlVehicleInfo.onPrndl(vehicleInfo.prndl)} }
    onMyKeyChanged: { sdlVehicleInfo.onMyKey(vehicleInfo.myKey) }
    onHeadLampStatusChanged: { sdlVehicleInfo.onHeadLampStatus(vehicleInfo.headLampStatus) }
    onDeviceStatusChanged: { sdlVehicleInfo.onDeviceStatus(vehicleInfo.deviceStatus) }
    onBodyInformationChanged: { sdlVehicleInfo.onBodyInformation(vehicleInfo.bodyInformation) }
    onBeltStatusChanged: { sdlVehicleInfo.onBeltStatus(vehicleInfo.beltStatus) }
    onTirePressureChanged: { sdlVehicleInfo.onTirePressure(vehicleInfo.tirePressure) }
    onGpsChanged: { sdlVehicleInfo.onGpsData(vehicleInfo.gps) }
}
