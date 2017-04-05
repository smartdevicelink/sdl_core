/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: ·
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. · Redistributions in binary
 * form must reproduce the above copyright notice, this list of conditions and
 * the following disclaimer in the documentation and/or other materials provided
 * with the distribution. · Neither the name of the Ford Motor Company nor the
 * names of its contributors may be used to endorse or promote products derived
 * from this software without specific prior written permission.
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
/**
 * @name SDL.SDLVehicleInfoModel
 * @desc SDL model with vehicle information used instead of CAN network.
 *       VehicleInfoModel is simulation of real CAN network.
 * @category Model
 * @filesource app/model/sdl/SDLVehicleInfoModel.js
 * @version 1.0
 */

SDL.SDLVehicleInfoModel = Em.Object
    .create( {

        /**
         * Stored VehicleInfo transmission state Data
         * 
         * @type {Array}
         */
        vehicleInfoPRNDL: [
            "PARK",
            "REVERSE",
            "NEUTRAL",
            "DRIVE",
            "SPORT",
            "LOWGEAR",
            "FIRST",
            "SECOND",
            "THIRD",
            "FOURTH",
            "FIFTH",
            "SIXTH"
        ],

        /**
         * Data changed in Odometr Input in VehicleInfo popUp
         *
         * @type {String}
         */
        odometrInputBinding: 'this.vehicleData.odometer',

        /**
         * PRNDL state value
         * 
         * @type {String}
         */
        prndlSelectState: 'PARK',

        /**
         * Stored VehicleInfo Data
         * 
         * @type {Array}
         */
        ecuDIDData: [
            {
                'data': "ECU 1 Test Data"
            }, {
                'data': "ECU 2 Test Data"
            }
        ],

        /**
         * Type of current vehicle: make of the vehicle, model of the vehicle,
         * model Year of the vehicle, trim of the vehicle.
         * 
         * @type {Object}
         */
        vehicleType: {
            make: "Ford",
            model: "Fiesta",
            modelYear: "2013",
            trim: "SE"
        },

        eVehicleDataType:{
            "gps" :"VEHICLEDATA_GPS",
            "speed": "VEHICLEDATA_SPEED",
            "rpm": "VEHICLEDATA_RPM",
            "fuelLevel": "VEHICLEDATA_FUELLEVEL",
            "fuelLevel_State": "VEHICLEDATA_FUELLEVEL_STATE",
            "instantFuelConsumption": "VEHICLEDATA_FUELCONSUMPTION",
            "prndl": "VEHICLEDATA_PRNDL",
            "externalTemperature": "VEHICLEDATA_EXTERNTEMP",
            "tirePressure": "VEHICLEDATA_TIREPRESSURE",
            "odometer": "VEHICLEDATA_ODOMETER",
            "beltStatus": "VEHICLEDATA_BELTSTATUS",
            "bodyInformation": "VEHICLEDATA_BODYINFO",
            "deviceStatus": "VEHICLEDATA_DEVICESTATUS",
            "eCallInfo": "VEHICLEDATA_ECALLINFO",
            "airbagStatus": "VEHICLEDATA_AIRBAGSTATUS",
            "emergencyEvent": "VEHICLEDATA_EMERGENCYEVENT",
            "clusterModes": "VEHICLEDATA_CLUSTERMODESTATUS",
            "myKey": "VEHICLEDATA_MYKEY",
            "driverBraking": "VEHICLEDATA_BRAKING",
            "wiperStatus": "VEHICLEDATA_WIPERSTATUS",
            "headLampStatus": "VEHICLEDATA_HEADLAMPSTATUS",
            "engineTorque":"VEHICLEDATA_ENGINETORQUE",
            "accPedalPosition": "VEHICLEDATA_ACCPEDAL",
            "steeringWheelAngle": "VEHICLEDATA_STEERINGWHEEL"
        },

        /**
         * Stored VehicleInfo Data
         * 
         * @type {Object}
         */
        vehicleData: {
            'displayResolution':{
                'width': 800,
                'height': 480
            },
            'speed': 80.08E0,
            'fuelLevel': 0.2E0,
            'fuelLevel_State': "UNKNOWN",
            'instantFuelConsumption': 2.2E0,
            'tirePressure': {
                'pressureTelltale': 'OFF',
                'leftFront': {
                    'status': 'UNKNOWN'
                },
                'rightFront': {
                    'status': 'UNKNOWN'
                },
                'leftRear': {
                    'status': 'UNKNOWN'
                },
                'rightRear': {
                    'status': 'UNKNOWN'
                },
                'innerLeftRear': {
                    'status': 'UNKNOWN'
                },
                'innerRightRear': {
                    'status': 'UNKNOWN'
                }
            },
            'beltStatus': {
                'driverBeltDeployed': "NOT_SUPPORTED",
                'passengerBeltDeployed': "NOT_SUPPORTED",
                'passengerBuckleBelted': "NOT_SUPPORTED",
                'driverBuckleBelted': "NOT_SUPPORTED",
                'leftRow2BuckleBelted': "NOT_SUPPORTED",
                'passengerChildDetected': "NOT_SUPPORTED",
                'rightRow2BuckleBelted': "NOT_SUPPORTED",
                'middleRow2BuckleBelted': "NOT_SUPPORTED",
                'middleRow3BuckleBelted': "NOT_SUPPORTED",
                'leftRow3BuckleBelted': "NOT_SUPPORTED",
                'rightRow3BuckleBelted': "NOT_SUPPORTED",
                'leftRearInflatableBelted': "NOT_SUPPORTED",
                'rightRearInflatableBelted': "NOT_SUPPORTED",
                'middleRow1BeltDeployed': "NOT_SUPPORTED",
                'middleRow1BuckleBelted': "NOT_SUPPORTED"
            },
            'bodyInformation': {
                'parkBrakeActive': false,
                'ignitionStableStatus': "MISSING_FROM_TRANSMITTER",
                'ignitionStatus': "UNKNOWN",
                'driverDoorAjar': true,
                'passengerDoorAjar': true,
                'rearLeftDoorAjar': true,
                'rearRightDoorAjar': true
            },
            'deviceStatus': {
                'voiceRecOn': false,
                'btIconOn': false,
                'callActive': false,
                'phoneRoaming': false,
                'textMsgAvailable': false,
                'battLevelStatus': "NOT_PROVIDED",
                'stereoAudioOutputMuted': false,
                'monoAudioOutputMuted': false,
                'signalLevelStatus': "NOT_PROVIDED",
                'primaryAudioSource': "NO_SOURCE_SELECTED",
                'eCallEventActive': false
            },
            'driverBraking': "NOT_SUPPORTED",
            'wiperStatus': "NO_DATA_EXISTS",
            'headLampStatus': {
                "lowBeamsOn": false,
                "highBeamsOn": false,
                "ambientLightSensorStatus": "NIGHT"
            },
            'engineTorque': 2.5E0,
            'accPedalPosition': 10.5E0,
            'steeringWheelAngle': 1.2E0,
            'myKey': {
                "e911Override": "NO_DATA_EXISTS"
            },
            'avgFuelEconomy': 0.1,
            'batteryVoltage': 12.5,
            'vin': '52-452-52-752',
            'prndl': 'PARK',
            'odometer': 23,
            'batteryPackVoltage': 12.5,
            'batteryPackCurrent': 7.0,
            'batteryPackTemperature': 30,
            'tripOdometer': 0,
            'genericbinary': '165165650',
            'satRadioESN': "165165650",
            'rainSensor': 165165650,
            'gps': {
                'longitudeDegrees': 42.5E0,
                'latitudeDegrees': -83.3E0,
                'utcYear': 2013,
                'utcMonth': 2,
                'utcDay': 14,
                'utcHours': 13,
                'utcMinutes': 16,
                'utcSeconds': 54,
                'compassDirection': 'SOUTHWEST',
                'pdop': 8.4E0,
                'hdop': 5.9E0,
                'vdop': 3.2E0,
                'actual': false,
                'satellites': 8,
                'dimension': '2D',
                'altitude': 7.7E0,
                'heading': 173.99E0,
                'speed': 2.78E0
            },
            'eCallInfo': {
                'eCallNotificationStatus': 'NORMAL',
                'auxECallNotificationStatus': 'NORMAL',
                'eCallConfirmationStatus': 'NORMAL'
            },
            'airbagStatus': {
                'driverAirbagDeployed': 'NO_EVENT',
                'driverSideAirbagDeployed': 'NO_EVENT',
                'driverCurtainAirbagDeployed': 'NO_EVENT',
                'passengerAirbagDeployed': 'NO_EVENT',
                'passengerCurtainAirbagDeployed': 'NO_EVENT',
                'driverKneeAirbagDeployed': 'NO_EVENT',
                'passengerSideAirbagDeployed': 'NO_EVENT',
                'passengerKneeAirbagDeployed': 'NO_EVENT'
            },
            'emergencyEvent': {
                'emergencyEventType': 'NO_EVENT',
                'fuelCutoffStatus': 'NORMAL_OPERATION',
                'rolloverEvent': 'NO_EVENT',
                'maximumChangeVelocity': 'NO_EVENT',
                'multipleEvents': 'NO_EVENT'
            },
            'clusterModeStatus': {
                'powerModeActive': false,
                'powerModeQualificationStatus': 'POWER_MODE_EVALUATION_IN_PROGRESS',
                'carModeStatus': 'FACTORY',
                'powerModeStatus': 'KEY_OUT'
            }

        },

        /**
         * Method to set selected state of vehicle transmission to vehicleData
         */
        onPRNDLSelected: function() {

            if (this.prndlSelectState) {
                this.set('vehicleData.prndl', this.prndlSelectState);
            }
        }.observes('this.prndlSelectState'),

        /**
         * Method calls GetVehicleType response
         * 
         * @type {Number}
         */
        getVehicleType: function(id) {

            FFW.VehicleInfo.GetVehicleTypeResponse(this.vehicleType, id);
        },

        /**
         * SDL VehicleInfo.GetDTCs handler fill data for response about vehicle
         * errors
         * 
         * @type {Object} params
         * @type {Number} id
         */
        vehicleInfoGetDTCs: function(params, id) {

            var dtc = [], result = "", ecuHeader = 2;

            for ( var i = 0; i < 3; i++) {
                dtc.push("line " + i);
            }

            result = "SUCCESS";

            FFW.VehicleInfo.vehicleInfoGetDTCsResponse(ecuHeader,
                dtc,
                result,
                id);

        },

        /**
         * SDL VehicleInfo.ReadDID handler send response about vehicle
         * conditions
         * 
         * @type {Object} params
         * @type {Number} id
         */
        vehicleInfoReadDID: function(params, id) {

            var didResult = [], resultCode = "";
            // magic number used because there is no huge database on HMI of
            // vehicle
            // data
            if (this.ecuDIDData[1].data) {
                resultCode = "SUCCESS";
            } else {
                resultCode = "INVALID_DATA";
            }

            for ( var i = 0; i < params.didLocation.length; i++) {
                if (i < 10) {
                    didResult[i] = {};
                    didResult[i].resultCode = 'SUCCESS';
                    didResult[i].didLocation = params.didLocation[i];
                    didResult[i].data = '0';
                } else {
                    didResult[i] = {};
                    didResult[i].resultCode = "VEHICLE_DATA_NOT_AVAILABLE";
                    didResult[i].didLocation = params.didLocation[i];
                    didResult[i].data = '0';
                }
            }

            FFW.VehicleInfo.vehicleInfoReadDIDResponse(didResult,
                resultCode,
                id);
        },

        /**
         * Function returns response message to VehicleInfoRPC
         *
         * @type {Object} message
         */
        SubscribeVehicleData: function(message) {
            if (SDL.SDLController.getApplicationModel(message.params.appID)) {

                var subscribeVIData = {};
                for (var key in message.params) {

                    if (key === "clusterModeStatus") {
                        key = "clusterModes";
                    }

                    if (key != 'appID' && SDL.SDLController.getApplicationModel(message.params.appID).subscribedData[key] === true) {
                        subscribeVIData[key] = {
                            dataType: this.eVehicleDataType[key],
                            resultCode: "DATA_ALREADY_SUBSCRIBED"
                        };
                    } else if (key != 'appID' && key in SDL.SDLController.getApplicationModel(message.params.appID).subscribedData) {
                        SDL.SDLController.getApplicationModel(message.params.appID).subscribedData[key] = true;
                        subscribeVIData[key] = {
                            dataType: this.eVehicleDataType[key],
                            resultCode: "SUCCESS"
                        };
                    } else if (key === "externalTemperature") {
                        subscribeVIData[key] = {
                            dataType: this.eVehicleDataType[key],
                            resultCode: "VEHICLE_DATA_NOT_AVAILABLE"
                        };
                    }
                }
            }
            FFW.VehicleInfo.sendVISubscribeVehicleDataResult(SDL.SDLModel.resultCode["SUCCESS"], message.id, message.method, subscribeVIData);
        },

        /**
         * Function returns response message to VehicleInfoRPC
         *
         * @type {Object} message
         */
        UnsubscribeVehicleData: function(message) {
            if (SDL.SDLController.getApplicationModel(message.params.appID)) {

                var subscribeVIData = {};
                for (var key in message.params) {

                    if (key === "clusterModeStatus") {
                        key = "clusterModes";
                    }

                    if (key != 'appID' && SDL.SDLController.getApplicationModel(message.params.appID).subscribedData[key] === false) {
                        SDL.SDLController.getApplicationModel(message.params.appID).subscribedData[key] = false;
                        subscribeVIData[key] = {
                            dataType: this.eVehicleDataType[key],
                            resultCode: "DATA_NOT_SUBSCRIBED"
                        };
                    } else if (key != 'appID' && key in SDL.SDLController.getApplicationModel(message.params.appID).subscribedData) {
                        SDL.SDLController.getApplicationModel(message.params.appID).subscribedData[key] = false;
                        subscribeVIData[key] = {
                            dataType: this.eVehicleDataType[key],
                            resultCode: "SUCCESS"
                        };
                    } else if (key === "externalTemperature") {
                        subscribeVIData[key] = {
                            dataType: this.eVehicleDataType[key],
                            resultCode: "VEHICLE_DATA_NOT_AVAILABLE"
                        };
                    }
                }
            }
            FFW.VehicleInfo.sendVISubscribeVehicleDataResult(SDL.SDLModel.resultCode["SUCCESS"], message.id, message.method, subscribeVIData);
        },

        /**
         * Function returns response message to VehicleInfoRPC
         * 
         * @type {Object} message
         */
        getVehicleData: function(message) {

            var data = {}
            text = "Params ", result = true;

            for ( var key in message.params) {
                if (key != "appID") {
                    if (this.vehicleData[key]) {
                        data[key] = this.vehicleData[key];
                    } else {
                        if (!result) {

                            text += ", " + key;
                        } else {

                            text += key;
                            result = false;
                        }
                    }
                }
            }

            text += " are not available";

            if (result) {
                FFW.VehicleInfo.sendGetVehicleDataResut(SDL.SDLModel.resultCode["SUCCESS"], message.id, message.method, data);
            } else {
                FFW.VehicleInfo.sendGetVehicleDataError(SDL.SDLModel.resultCode["DATA_NOT_AVAILABLE"], message.id, message.method, text, data);
            }
        },

        /**
         * Function send all vehicle conditions on FFW.VehicleInfo.OnVehicleData
         * for notification when data changes
         */
        onVehicleDataChanged: function() {

            var appID = null;

            for (var i = 0; i < SDL.SDLModel.registeredApps.length; i++) {
                appID = SDL.SDLModel.registeredApps[i].appID;
                if (SDL.SDLController.getApplicationModel(appID).subscribedData["prndl"]) {

                    var jsonData = {};
                    jsonData["prndl"] = this.vehicleData["prndl"];
                    FFW.VehicleInfo.OnVehicleData(jsonData);
                    return;
                }
            }

        }.observes('this.vehicleData.prndl'),

        /**
         * Function send all vehicle conditions on FFW.VehicleInfo.OnVehicleData
         * for notification when data changes
         */
        onOdometerDataChanged: function() {

            var jsonData = {};

            jsonData["odometer"] = parseInt(this.odometrInput);
            FFW.VehicleInfo.OnVehicleData(jsonData);

        }.observes('this.odometrInput')
    });
