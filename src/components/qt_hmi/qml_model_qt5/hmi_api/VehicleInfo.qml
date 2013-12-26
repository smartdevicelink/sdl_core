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

import QtQuick 2.0
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
    }
    function unsubscribeGps(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onGpsChanged.disconnect(model.sendGpsChange)
        console.debug("exit")
    }

    function subscribeSpeed(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSpeedChanged.disconnect(model.sendSpeedChange)
        model.onSpeedChanged.connect(model.sendSpeedChange)
        console.debug("exit")
    }
    function unsubscribeSpeed(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSpeedChanged.disconnect(model.sendSpeedChange)
        console.debug("exit")
    }

    function subscribeRpm(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onRpmChanged.disconnect(model.sendRpmChange)
        model.onRpmChanged.connect(model.sendRpmChange)
        console.debug("exit")
    }
    function unsubscribeRpm(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onRpmChanged.disconnect(model.sendRpmChange)
        console.debug("exit")
    }

    function subscribeFuelLevel(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevelChanged.disconnect(model.sendFuelLevelChange)
        model.onFuelLevelChanged.connect(model.sendFuelLevelChange)
        console.debug("exit")
    }
    function unsubscribeFuelLevel(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevelChanged.disconnect(model.sendFuelLevelChange)
        console.debug("exit")
    }

    function subscribeFuelLevel_State(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevel_StateChanged.disconnect(model.sendFuelLevel_StateChange)
        model.onFuelLevel_StateChanged.connect(model.sendFuelLevel_StateChange)
        console.debug("exit")
    }
    function unsubscribeFuelLevel_State(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onFuelLevel_StateChanged.disconnect(model.sendFuelLevel_StateChange)
        console.debug("exit")
    }

    function subscribeInstantFuelConsumption(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onInstantFuelConsumptionChanged.disconnect(model.sendInstantFuelConsumptionChange)
        model.onInstantFuelConsumptionChanged.connect(model.sendInstantFuelConsumptionChange)
        console.debug("exit")
    }
    function unsubscribeInstantFuelConsumption(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onInstantFuelConsumptionChanged.disconnect(model.sendInstantFuelConsumptionChange)
        console.debug("exit")
    }

    function subscribeExternalTemperature(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onExternalTemperatureChanged.disconnect(model.sendExternalTemperatureChange)
        model.onExternalTemperatureChanged.connect(model.sendExternalTemperatureChange)
        console.debug("exit")
    }
    function unsubscribeExternalTemperature(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onExternalTemperatureChanged.disconnect(model.sendExternalTemperatureChange)
        console.debug("exit")
    }

    function subscribePrndl(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onPrndlChanged.disconnect(model.sendPrndlChange)
        model.onPrndlChanged.connect(model.sendPrndlChange)
        console.debug("exit")
    }
    function unsubscribePrndl(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onPrndlChanged.disconnect(model.sendPrndlChange)
        console.debug("exit")
    }

    function subscribeVin(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onVinChanged.disconnect(model.sendVinChange)
        model.onVinChanged.connect(model.sendVinChange)
        console.debug("exit")
    }
    function unsubscribeVin(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onVinChanged.disconnect(model.sendVinChange)
        console.debug("exit")
    }

    function subscribeTirePressure(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onTirePressureChanged.disconnect(model.sendTirePressureChange)
        model.onTirePressureChanged.connect(model.sendTirePressureChange)
        console.debug("exit")
    }
    function unsubscribeTirePressure(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onTirePressureChanged.disconnect(model.sendTirePressureChange)
        console.debug("exit")
    }

    function subscribeOdometer(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onOdometerChanged.disconnect(model.sendOdometerChange)
        model.onOdometerChanged.connect(model.sendOdometerChange)
        console.debug("exit")
    }
    function unsubscribeOdometer(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onOdometerChanged.disconnect(model.sendOdometerChange)
        console.debug("exit")
    }

    function subscribeBeltStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBeltStatusChanged.disconnect(model.sendBeltStatusChange)
        model.onBeltStatusChanged.connect(model.sendBeltStatusChange)
        console.debug("exit")
    }
    function unsubscribeBeltStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBeltStatusChanged.disconnect(model.sendBeltStatusChange)
        console.debug("exit")
    }

    function subscribeBodyInformation(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBodyInformationChanged.disconnect(model.sendBodyInformationChange)
        model.onBodyInformationChanged.connect(model.sendBodyInformationChange)
        console.debug("exit")
    }
    function unsubscribeBodyInformation(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onBodyInformationChanged.disconnect(model.sendBodyInformationChange)
        console.debug("exit")
    }

    function subscribeDeviceStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDeviceStatusChanged.disconnect(model.sendDeviceStatusChange)
        model.onDeviceStatusChanged.connect(model.sendDeviceStatusChange)
        console.debug("exit")
    }
    function unsubscribeDeviceStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDeviceStatusChanged.disconnect(model.sendDeviceStatusChange)
        console.debug("exit")
    }

    function subscribeDriverBraking(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDriverBrakingChanged.disconnect(model.sendDriverBrakingChange)
        model.onDriverBrakingChanged.connect(model.sendDriverBrakingChange)
        console.debug("exit")
    }
    function unsubscribeDriverBraking(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onDriverBrakingChanged.disconnect(model.sendDriverBrakingChange)
        console.debug("exit")
    }

    function subscribeWiperStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onWiperStatusChanged.disconnect(model.sendWiperStatusChange)
        model.onWiperStatusChanged.connect(model.sendWiperStatusChange)
        console.debug("exit")
    }
    function unsubscribeWiperStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onWiperStatusChanged.disconnect(model.sendWiperStatusChange)
        console.debug("exit")
    }

    function subscribeHeadLampStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onHeadLampStatusChanged.disconnect(model.sendHeadLampStatusChange)
        model.onHeadLampStatusChanged.connect(model.sendHeadLampStatusChange)
        console.debug("exit")
    }
    function unsubscribeHeadLampStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onHeadLampStatusChanged.disconnect(model.sendHeadLampStatusChange)
        console.debug("exit")
    }

    function subscribeEngineTorque(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onEngineTorqueChanged.disconnect(model.sendEngineTorqueChange)
        model.onEngineTorqueChanged.connect(model.sendEngineTorqueChange)
        console.debug("exit")
    }
    function unsubscribeEngineTorque(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onEngineTorqueChanged.disconnect(model.sendEngineTorqueChange)
        console.debug("exit")
    }

    function subscribeAccPedalPosition(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAccPedalPositionChanged.disconnect(model.sendAccPedalPositionChange)
        model.onAccPedalPositionChanged.connect(model.sendAccPedalPositionChange)
        console.debug("exit")
    }
    function unsubscribeAccPedalPosition(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAccPedalPositionChanged.disconnect(model.sendAccPedalPositionChange)
        console.debug("exit")
    }

    function subscribeSteeringWheelAngle(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSteeringWheelAngleChanged.disconnect(model.sendSteeringWheelAngleChange)
        model.onSteeringWheelAngleChanged.connect(model.sendSteeringWheelAngleChange)
        console.debug("exit")
    }
    function unsubscribeSteeringWheelAngle(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onSteeringWheelAngleChanged.disconnect(model.sendSteeringWheelAngleChange)
        console.debug("exit")
    }

    function subscribeECallInfo(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onECallInfoChanged.disconnect(model.sendECallInfoChange)
        model.onECallInfoChanged.connect(model.sendECallInfoChange)
        console.debug("exit")
    }
    function unsubscribeECallInfo(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onECallInfoChanged.disconnect(model.sendECallInfoChange)
        console.debug("exit")
    }

    function subscribeAirbagStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAirbagStatusChanged.disconnect(model.sendAirbagStatusChange)
        model.onAirbagStatusChanged.connect(model.sendAirbagStatusChange)
        console.debug("exit")
    }
    function unsubscribeAirbagStatus(appID) {
        console.debug("enter")
        var model = dataContainer.vehicleInfoModel;
        model.onAirbagStatusChanged.disconnect(model.sendAirbagStatusChange)
        console.debug("exit")
    }
}
