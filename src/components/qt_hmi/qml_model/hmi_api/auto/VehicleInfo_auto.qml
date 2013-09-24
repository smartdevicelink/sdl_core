import QtQuick 2.0
import ".."
Item {
  VehicleInfo {
    id: sdlVehicleInfo
  }
  function isReady(params) {
     return sdlVehicleInfo.isReady()
  }

  function getVehicleType(params) {
     return sdlVehicleInfo.getVehicleType()
  }

  function readDID(params) {
     return sdlVehicleInfo.readDID(params.ecuName, params.didLocation, params.appID)
  }

  function getDTCs(params) {
     return sdlVehicleInfo.getDTCs(params.ecuName, params.dtcMask, params.appID)
  }

  function getVehicleData(params) {
     return sdlVehicleInfo.getVehicleData(params.gps, params.speed, params.rpm, params.fuelLevel, params.fuelLevel_State, params.instantFuelConsumption, params.externalTemperature, params.vin, params.prndl, params.tirePressure, params.odometer, params.beltStatus, params.bodyInformation, params.deviceStatus, params.driverBraking, params.wiperStatus, params.headLampStatus, params.engineTorque, params.accPedalPosition, params.steeringWheelAngle, params.myKey, params.appID)
  }

  signal onVehicleData(var gps, var speed, var rpm, var fuelLevel, var fuelLevel_State, var instantFuelConsumption, var externalTemperature, var vin, var prndl, var tirePressure, var odometer, var beltStatus, var bodyInformation, var deviceStatus, var driverBraking, var wiperStatus, var headLampStatus, var engineTorque, var accPedalPosition, var steeringWheelAngle, var myKey)
}
