/**
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
