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

    function readDID(ecuName, didLocation, appID) {
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
                didResult[i].resultCode = Common.VehicleDataResultCode.VDRC_SUCCESS
                didResult[i].didLocation = didLocation[i]
                didResult[i].data = '0'
            }
            return {didResult: didResult}
        }
    }
}
