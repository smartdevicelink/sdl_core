/**
 * @file VehicleInfo.qml
 * @brief Vehicle information interface realisation.
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
import "Common.js" as Common

Item {
    function isReady () {
        return {
            available: dataContainer.hmiVehicleInfoAvailable
        }
    }

    function getVehicleType() {
        return {
            "vehicleType": {
                "make": "Ford",
                "model": "Fiesta",
                "modelYear": "2013",
                "trim": "SE"
                }
        }
    }

    function getGpsData(appID) {
        console.debug("enter")
        console.debug("exit")
        return { gps: dataContainer.vehicleInfoModel.gps }
    }

    function getSpeed(appID) {
        console.debug("enter")
        console.debug("exit")
        return { speed: dataContainer.vehicleInfoModel.speed }
    }

    function getRpm(appID) {
        console.debug("enter")
        console.debug("exit")
        return {
            rpm: dataContainer.vehicleInfoModel.rpm,
            __retCode: Common.Result.DATA_NOT_AVAILABLE
        }
    }

    function getFuelLevel(appID) {
        console.debug("enter")
        console.debug("exit")
        return { fuelLevel: dataContainer.vehicleInfoModel.fuelLevel }
    }

    function getFuelLevelState(appID) {
        console.debug("enter")
        console.debug("exit")
        return { fuelLevel_State: dataContainer.vehicleInfoModel.fuelLevel_State }
    }

    function getInstantFuelConsumption(appID) {
        console.debug("enter")
        console.debug("exit")
        return { instantFuelConsumption: dataContainer.vehicleInfoModel.instantFuelConsumption }
    }

    function getExternalTemperature(appID) {
        console.debug("enter")
        console.debug("exit")
        return { externalTemperature: dataContainer.vehicleInfoModel.externalTemperature }
    }

    function getVin(appID) {
        console.debug("enter")
        console.debug("exit")
        return { vin: dataContainer.vehicleInfoModel.vin }
    }

    function getPrndl(appID) {
        console.debug("enter")
        if (dataContainer.vehicleInfoModel.prndl === -1) { // Test value, to check response
            console.debug("exit")
            return {
                prndl: dataContainer.vehicleInfoModel.prndl,
                __retCode: Common.Result.DATA_NOT_AVAILABLE
            }
        }
        console.debug("exit")
        return { prndl: dataContainer.vehicleInfoModel.prndl }
    }

    function getTirePressure(appID) {
        console.debug("enter")
        console.debug("exit")
        return { tirePressure: dataContainer.vehicleInfoModel.tirePressure }
    }

    function getOdometer(appID) {
        console.debug("enter")
        console.debug("exit")
        return { odometer: dataContainer.vehicleInfoModel.odometer }
    }

    function getBeltStatus(appID) {
        console.debug("enter")
        console.debug("exit")
        return { beltStatus: dataContainer.vehicleInfoModel.beltStatus }
    }

    function getBodyInformation(appID) {
        console.debug("enter")
        console.debug("exit")
        return { bodyInformation: dataContainer.vehicleInfoModel.bodyInformation }
    }

    function getDeviceStatus(appID) {
        console.debug("enter")
        console.debug("exit")
        return { deviceStatus: dataContainer.vehicleInfoModel.deviceStatus }
    }

    function getDriverBraking(appID) {
        console.debug("enter")
        console.debug("exit")
        return { driverBraking: dataContainer.vehicleInfoModel.driverBraking }
    }

    function getWiperStatus(appID) {
        console.debug("enter")
        console.debug("exit")
        return { wiperStatus: dataContainer.vehicleInfoModel.wiperStatus }
    }

    function getHeadLampStatus(appID) {
        console.debug("enter")
        console.debug("exit")
        return { headLampStatus: dataContainer.vehicleInfoModel.headLampStatus }
    }

    function getEngineTorque(appID) {
        console.debug("enter")
        console.debug("exit")
        return { engineTorque: dataContainer.vehicleInfoModel.engineTorque }
    }

    function getAccPedalPosition(appID) {
        console.debug("enter")
        console.debug("exit")
        return { accPedalPosition: dataContainer.vehicleInfoModel.accPedalPosition }
    }

    function getSteeringWheelAngle(appID) {
        console.debug("enter")
        console.debug("exit")
        return { steeringWheelAngle: dataContainer.vehicleInfoModel.steeringWheelAngle }
    }

    function getECallInfo(appID) {
        console.debug("enter")
        console.debug("exit")
        return { eCallInfo: dataContainer.vehicleInfoModel.eCallInfo }
    }

    function getAirbagStatus(appID) {
        console.debug("enter")
        console.debug("exit")
        return { airbagStatus: dataContainer.vehicleInfoModel.airbagStatus }
    }

    function getEmergencyEvent(appID) {
        console.debug("enter")
        console.debug("exit")
        return { emergencyEvent: dataContainer.vehicleInfoModel.emergencyEvent }
    }

    function getClusterModeStatus(appID) {
        console.debug("enter")
        console.debug("exit")
        return { clusterModeStatus: dataContainer.vehicleInfoModel.clusterModeStatus }
    }

    function getMyKey(appID) {
        console.debug("enter")
        console.debug("exit")
        return { myKey: dataContainer.vehicleInfoModel.myKey }
    }

    function getDTCs(ecuName, dtcMask, appID) {
        var ecuHeader = 2
        var dtc = []

        for (var i = 0; i < 3; i++) {
            dtc.push("line" + i)
        }
        return {ecuHeader: ecuHeader, dtc: dtc}
    }

    function readDID(ecuName, didLocation, appID) {
        console.debug("Enter")
        //TODO{ALeshin}: refactoring of this function, when we'll have Vehicle Info module
        var didResult = []

        for (var i = 0; i < didLocation.length; i++) {
            if (i < 10) {
                didResult[i] = {}
                didResult[i].resultCode = Common.VehicleDataResultCode.VDRC_SUCCESS
                didResult[i].didLocation = didLocation[i]
                didResult[i].data = '0'
            }
            else {
                didResult[i] = {}
                didResult[i].resultCode = Common.VehicleDataResultCode.VDRC_DATA_NOT_AVAILABLE
                didResult[i].didLocation = didLocation[i]
                didResult[i].data = '0'
            }
        }
        console.debug("Exit")
        return {didResult: didResult}
    }

    function subscribeGps(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onGpsChanged.disconnect(model.sendGpsChange)
        model.onGpsChanged.connect(model.sendGpsChange)
        console.debug("exit")
        return {gps:
            { dataType: Common.VehicleDataType.VEHICLEDATA_GPS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS} }
    }
    function unsubscribeGps(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onGpsChanged.disconnect(model.sendGpsChange)
        console.debug("exit")
        return {gps:
            { dataType: Common.VehicleDataType.VEHICLEDATA_GPS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS} }

    }

    function subscribeSpeed(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSpeedChanged.disconnect(model.sendSpeedChange)
        model.onSpeedChanged.connect(model.sendSpeedChange)
        console.debug("exit")
        return { speed:
            { dataType: Common.VehicleDataType.VEHICLEDATA_SPEED,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeSpeed(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSpeedChanged.disconnect(model.sendSpeedChange)
        console.debug("exit")
        return { speed:
            { dataType: Common.VehicleDataType.VEHICLEDATA_SPEED,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeRpm(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onRpmChanged.disconnect(model.sendRpmChange)
        model.onRpmChanged.connect(model.sendRpmChange)
        console.debug("exit")
        return { rpm:
            { dataType: Common.VehicleDataType.VEHICLEDATA_RPM,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeRpm(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onRpmChanged.disconnect(model.sendRpmChange)
        console.debug("exit")
        return { rpm:
            { dataType: Common.VehicleDataType.VEHICLEDATA_RPM,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeFuelLevel(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevelChanged.disconnect(model.sendFuelLevelChange)
        model.onFuelLevelChanged.connect(model.sendFuelLevelChange)
        console.debug("exit")
        return { fuelLevel:
            { dataType: Common.VehicleDataType.VEHICLEDATA_FUELLEVEL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeFuelLevel(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevelChanged.disconnect(model.sendFuelLevelChange)
        console.debug("exit")
        return { fuelLevel:
            { dataType: Common.VehicleDataType.VEHICLEDATA_FUELLEVEL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeFuelLevel_State(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevel_StateChanged.disconnect(model.sendFuelLevel_StateChange)
        model.onFuelLevel_StateChanged.connect(model.sendFuelLevel_StateChange)
        console.debug("exit")
        return { fuelLevel_State:
            { dataType: Common.VehicleDataType.VEHICLEDATA_FUELLEVEL_STATE,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeFuelLevel_State(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevel_StateChanged.disconnect(model.sendFuelLevel_StateChange)
        console.debug("exit")
        return { fuelLevel_State:
            { dataType: Common.VehicleDataType.VEHICLEDATA_FUELLEVEL_STATE,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeInstantFuelConsumption(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onInstantFuelConsumptionChanged.disconnect(model.sendInstantFuelConsumptionChange)
        model.onInstantFuelConsumptionChanged.connect(model.sendInstantFuelConsumptionChange)
        console.debug("exit")
        return { instantFuelConsumption:
            { dataType: Common.VehicleDataType.VEHICLEDATA_FUELCONSUMPTION,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeInstantFuelConsumption(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onInstantFuelConsumptionChanged.disconnect(model.sendInstantFuelConsumptionChange)
        console.debug("exit")
        return { instantFuelConsumption:
            { dataType: Common.VehicleDataType.VEHICLEDATA_FUELCONSUMPTION,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeExternalTemperature(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onExternalTemperatureChanged.disconnect(model.sendExternalTemperatureChange)
        model.onExternalTemperatureChanged.connect(model.sendExternalTemperatureChange)
        console.debug("exit")
        return { externalTemperature:
            { dataType: Common.VehicleDataType.VEHICLEDATA_EXTERNTEMP,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeExternalTemperature(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onExternalTemperatureChanged.disconnect(model.sendExternalTemperatureChange)
        console.debug("exit")
        return { externalTemperature:
            { dataType: Common.VehicleDataType.VEHICLEDATA_EXTERNTEMP,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribePrndl(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onPrndlChanged.disconnect(model.sendPrndlChange)
        model.onPrndlChanged.connect(model.sendPrndlChange)
        console.debug("exit")
        return { prndl:
            { dataType: Common.VehicleDataType.VEHICLEDATA_PRNDL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribePrndl(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onPrndlChanged.disconnect(model.sendPrndlChange)
        console.debug("exit")
        return { prndl:
            { dataType: Common.VehicleDataType.VEHICLEDATA_PRNDL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeVin(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onVinChanged.disconnect(model.sendVinChange)
        model.onVinChanged.connect(model.sendVinChange)
        console.debug("exit")
        return { vin:
            { dataType: Common.VehicleDataType.VEHICLEDATA_VIN,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeVin(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onVinChanged.disconnect(model.sendVinChange)
        console.debug("exit")
        return { vin:
            { dataType: Common.VehicleDataType.VEHICLEDATA_VIN,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeTirePressure(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onTirePressureChanged.disconnect(model.sendTirePressureChange)
        model.onTirePressureChanged.connect(model.sendTirePressureChange)
        console.debug("exit")
        return { tirePressure:
            { dataType: Common.VehicleDataType.VEHICLEDATA_TIREPRESSURE,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeTirePressure(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onTirePressureChanged.disconnect(model.sendTirePressureChange)
        console.debug("exit")
        return { tirePressure:
            { dataType: Common.VehicleDataType.VEHICLEDATA_TIREPRESSURE,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeOdometer(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onOdometerChanged.disconnect(model.sendOdometerChange)
        model.onOdometerChanged.connect(model.sendOdometerChange)
        console.debug("exit")
        return { odometer:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ODOMETER,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeOdometer(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onOdometerChanged.disconnect(model.sendOdometerChange)
        console.debug("exit")
        return { odometer:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ODOMETER,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeBeltStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBeltStatusChanged.disconnect(model.sendBeltStatusChange)
        model.onBeltStatusChanged.connect(model.sendBeltStatusChange)
        console.debug("exit")
        return { beltStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_BELTSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeBeltStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBeltStatusChanged.disconnect(model.sendBeltStatusChange)
        console.debug("exit")
        return { beltStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_BELTSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeBodyInformation(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBodyInformationChanged.disconnect(model.sendBodyInformationChange)
        model.onBodyInformationChanged.connect(model.sendBodyInformationChange)
        console.debug("exit")
        return { bodyInformation:
            { dataType: Common.VehicleDataType.VEHICLEDATA_BODYINFO,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeBodyInformation(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBodyInformationChanged.disconnect(model.sendBodyInformationChange)
        console.debug("exit")
        return { bodyInformation:
            { dataType: Common.VehicleDataType.VEHICLEDATA_BODYINFO,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeDeviceStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDeviceStatusChanged.disconnect(model.sendDeviceStatusChange)
        model.onDeviceStatusChanged.connect(model.sendDeviceStatusChange)
        console.debug("exit")
        return { deviceStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_DEVICESTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeDeviceStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDeviceStatusChanged.disconnect(model.sendDeviceStatusChange)
        console.debug("exit")
        return { deviceStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_DEVICESTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeDriverBraking(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDriverBrakingChanged.disconnect(model.sendDriverBrakingChange)
        model.onDriverBrakingChanged.connect(model.sendDriverBrakingChange)
        console.debug("exit")
        return { driverBraking:
            { dataType: Common.VehicleDataType.VEHICLEDATA_BRAKING,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeDriverBraking(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDriverBrakingChanged.disconnect(model.sendDriverBrakingChange)
        console.debug("exit")
        return { driverBraking:
            { dataType: Common.VehicleDataType.VEHICLEDATA_BRAKING,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeWiperStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onWiperStatusChanged.disconnect(model.sendWiperStatusChange)
        model.onWiperStatusChanged.connect(model.sendWiperStatusChange)
        console.debug("exit")
        return { wiperStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_WIPERSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeWiperStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onWiperStatusChanged.disconnect(model.sendWiperStatusChange)
        console.debug("exit")
        return { wiperStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_WIPERSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeHeadLampStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onHeadLampStatusChanged.disconnect(model.sendHeadLampStatusChange)
        model.onHeadLampStatusChanged.connect(model.sendHeadLampStatusChange)
        console.debug("exit")
        return { headLampStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_HEADLAMPSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeHeadLampStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onHeadLampStatusChanged.disconnect(model.sendHeadLampStatusChange)
        console.debug("exit")
        return { headLampStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_HEADLAMPSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeEngineTorque(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onEngineTorqueChanged.disconnect(model.sendEngineTorqueChange)
        model.onEngineTorqueChanged.connect(model.sendEngineTorqueChange)
        console.debug("exit")
        return { engineTorque:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ENGINETORQUE,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeEngineTorque(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onEngineTorqueChanged.disconnect(model.sendEngineTorqueChange)
        console.debug("exit")
        return { engineTorque:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ENGINETORQUE,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeAccPedalPosition(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAccPedalPositionChanged.disconnect(model.sendAccPedalPositionChange)
        model.onAccPedalPositionChanged.connect(model.sendAccPedalPositionChange)
        console.debug("exit")
        return { accPedalPosition:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ACCPEDAL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeAccPedalPosition(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAccPedalPositionChanged.disconnect(model.sendAccPedalPositionChange)
        console.debug("exit")
        return { accPedalPosition:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ACCPEDAL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeSteeringWheelAngle(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSteeringWheelAngleChanged.disconnect(model.sendSteeringWheelAngleChange)
        model.onSteeringWheelAngleChanged.connect(model.sendSteeringWheelAngleChange)
        console.debug("exit")
        return { steeringWheelAngle:
            { dataType: Common.VehicleDataType.VEHICLEDATA_STEERINGWHEEL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeSteeringWheelAngle(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSteeringWheelAngleChanged.disconnect(model.sendSteeringWheelAngleChange)
        console.debug("exit")
        return { steeringWheelAngle:
            { dataType: Common.VehicleDataType.VEHICLEDATA_STEERINGWHEEL,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeECallInfo(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onECallInfoChanged.disconnect(model.sendECallInfoChange)
        model.onECallInfoChanged.connect(model.sendECallInfoChange)
        console.debug("exit")
        return { eCallInfo:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ECALLINFO,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeECallInfo(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onECallInfoChanged.disconnect(model.sendECallInfoChange)
        console.debug("exit")
        return { eCallInfo:
            { dataType: Common.VehicleDataType.VEHICLEDATA_ECALLINFO,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeAirbagStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAirbagStatusChanged.disconnect(model.sendAirbagStatusChange)
        model.onAirbagStatusChanged.connect(model.sendAirbagStatusChange)
        console.debug("exit")
        return { airbagStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_AIRBAGSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeAirbagStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAirbagStatusChanged.disconnect(model.sendAirbagStatusChange)
        console.debug("exit")
        return { airbagStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_AIRBAGSTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeEmergencyEvent(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onEmergencyEventChanged.disconnect(model.sendEmergencyEventChange)
        model.onEmergencyEventChanged.connect(model.sendEmergencyEventChange)
        console.debug("exit")
        return { emergencyEvent:
            { dataType: Common.VehicleDataType.VEHICLEDATA_EMERGENCYEVENT,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeEmergencyEvent(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onEmergencyEventChanged.disconnect(model.sendEmergencyEventChange)
        console.debug("exit")
        return { emergencyEvent:
            { dataType: Common.VehicleDataType.VEHICLEDATA_EMERGENCYEVENT,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeClusterModeStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onClusterModeStatusChanged.disconnect(model.sendClusterModeStatusChange)
        model.onClusterModeStatusChanged.connect(model.sendClusterModeStatusChange)
        console.debug("exit")
        return { clusterModeStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_CLUSTERMODESTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeClusterModeStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onClusterModeStatusChanged.disconnect(model.sendClusterModeStatusChange)
        console.debug("exit")
        return { clusterModeStatus:
            { dataType: Common.VehicleDataType.VEHICLEDATA_CLUSTERMODESTATUS,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }

    function subscribeMyKey(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onMyKeyChanged.disconnect(model.sendMyKeyChange)
        model.onMyKeyChanged.connect(model.sendMyKeyChange)
        console.debug("exit")
        return { myKey:
            { dataType: Common.VehicleDataType.VEHICLEDATA_MYKEY,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
    function unsubscribeMyKey(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onMyKeyChanged.disconnect(model.sendMyKeyChange)
        console.debug("exit")
        return { myKey:
            { dataType: Common.VehicleDataType.VEHICLEDATA_MYKEY,
              resultCode: Common.VehicleDataResultCode.VDRC_SUCCESS } }
    }
}
