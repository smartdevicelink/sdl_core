/**
 * @file VehicleInfo.qml
 * @brief Container for information about vehicle.
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

import "../hmi_api/Common.js" as Common

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

    function getVehicleData(gps, speed, rpm, fuelLevel, fuelLevel_State, instantFuelConsumption, externalTemperature,
                            vin, prndl, tirePressure, odometer, beltStatus, bodyInformation, deviceStatus, driverBraking,
                            wiperStatus, headLampStatus, engineTorque, accPedalPosition, steeringWheelAngle, myKey, appID) {
        //TODO {ALeshin}: Refactoring of this function
        throw Common.Result.UNSUPPORTED_REQUEST

        console.debug("enter")

        console.debug("exit");
        return {
                gps: gps ? dataContainer.vehicleInfoModel.gps : undefined,
                speed: speed ? dataContainer.vehicleInfoModel.speed : undefined,
                rpm: rpm ? dataContainer.vehicleInfoModel.rpm : undefined,
                fuelLevel: fuelLevel ? dataContainer.vehicleInfoModel.fuelLevel : undefined,
                fuelLevel_State: fuelLevel_State ? dataContainer.vehicleInfoModel.fuelLevel_State : undefined,
                instantFuelConsumption: instantFuelConsumption ? dataContainer.vehicleInfoModel.instantFuelConsumption : undefined,
                externalTemperature: externalTemperature ? dataContainer.vehicleInfoModel.externalTemperature : undefined,
                vin: vin ? dataContainer.vehicleInfoModel.vin : undefined,
                prndl: prndl ? dataContainer.vehicleInfoModel.prndl : undefined,
                tirePressure: tirePressure ? dataContainer.vehicleInfoModel.tirePressure : undefined,
                odometer: odometer ? dataContainer.vehicleInfoModel.odometer : undefined,
                beltStatus: beltStatus ? dataContainer.vehicleInfoModel.beltStatus : undefined,
                bodyInformation: bodyInformation ? dataContainer.vehicleInfoModel.bodyInformation : undefined,
                deviceStatus: deviceStatus ? dataContainer.vehicleInfoModel.deviceStatus : undefined,
                driverBraking: driverBraking ? dataContainer.vehicleInfoModel.driverBraking : undefined,
                wiperStatus: wiperStatus ? dataContainer.vehicleInfoModel.wiperStatus : undefined,
                headLampStatus: headLampStatus ? dataContainer.vehicleInfoModel.headLampStatus : undefined,
                engineTorque: engineTorque ? dataContainer.vehicleInfoModel.engineTorque : undefined,
                accPedalPosition: accPedalPosition ? dataContainer.vehicleInfoModel.accPedalPosition : undefined,
                steeringWheelAngle: steeringWheelAngle ? dataContainer.vehicleInfoModel.steeringWheelAngle : undefined,
                myKey: myKey ? dataContainer.vehicleInfoModel.myKey : undefined
        }
    }
}
