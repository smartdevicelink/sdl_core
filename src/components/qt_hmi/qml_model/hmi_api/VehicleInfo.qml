import QtQuick 2.0

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
        console.debug("enter getVehicleData()", gps, speed, rpm, fuelLevel, fuelLevel_State, instantFuelConsumption, externalTemperature,
                      vin, prndl, tirePressure, odometer, beltStatus, bodyInformation, deviceStatus, driverBraking,
                      wiperStatus, headLampStatus, engineTorque, accPedalPosition, steeringWheelAngle, myKey, appID)

        var dataToSend = {};
        var functionArgs = [ "gps", "speed", "rpm", "fuelLevel", "fuelLevel_State", "instantFuelConsumption", "externalTemperature",
                        "vin", "prndl", "tirePressure", "odometer", "beltStatus", "bodyInformation", "deviceStatus", "driverBraking",
                        "wiperStatus", "headLampStatus", "engineTorque", "accPedalPosition", "steeringWheelAngle", "myKey" ]

        // no appID argument in response
        for (var i = 0; i < arguments.length - 1; i++) {
            if (arguments[i] === true) {
                dataToSend[functionArgs[i]] = dataContainer.vehicleInfoModel[functionArgs[i]];
            }
        }

        console.debug("exit getVehicleData()", dataToSend);
        return dataToSend
    }
}
