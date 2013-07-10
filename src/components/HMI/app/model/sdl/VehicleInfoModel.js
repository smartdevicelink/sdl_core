/*
 * Copyright (c) 2013, Ford Motor Company All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  · Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *  · Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *  · Neither the name of the Ford Motor Company nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
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

SDL.SDLVehicleInfoModel = Em.Object.create( {

    /**
     * Stored VehicleInfo transmission state Data
     * 
     * @type {Array}
     */
    vehicleInfoPRNDL:
        [
            "PARK",
            "REVERSE",
            "NEUTRAL",
            "FORWARD_DRIVE_2",
            "LOWGEAR"
        ],

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
    ecuDIDData:
        [
            {
                'data': "ECU 1 Test Data"
            },
            {
                'data': "ECU 2 Test Data"
            }
        ],

    /**
     * Type of current vehicle: make of the vehicle, model of the vehicle, model
     * Year of the vehicle, trim of the vehicle.
     * 
     * @type {Object}
     */
    vehicleType: {
        make: "Ford",
        model: "Fiesta",
        modelYear: "2013",
        trim: "SE"
    },

    /**
     * Stored VehicleInfo Data
     * 
     * @type {Object}
     */
    vehicleData: {
        'speed': 80.0,
        'rpm': 5000,
        'fuelLevel': 0.2,
        'fuelLevel_State': "UNKNOWN",
        'instantFuelConsumption': 2.2,
        'tirePressure': "UNKNOWN",
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
        'bodyInformation':{
            'parkBrakeActive': false,
            'ignitionStableStatus': "MISSING_FROM_TRANSMITTER",
            'ignitionStatus': "UNKNOWN"
        },
        'deviceStatus':{
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
            'eCallEventActive': false,
        },
        'driverBraking': "NOT_SUPPORTED",
        'wiperStatus': "NO_DATA_EXISTS",
        'headLampStatus':{
            "lowBeamsOn": false,
            "highBeamsOn": false
        },
        'engineTorque': 2.5,
        'accPedalPosition': 0.5,
        'steeringWheelAngle': 1.2,
        'myKey':{
            "e911Override": "NO_DATA_EXISTS"
        },
        'avgFuelEconomy': 0.1,
        'batteryVoltage': 12.5,
        'externalTemperature': 40.0,
        'vin': '52-452-52-752',
        'prndl': 'PARK',
        'batteryPackVoltage': 12.5,
        'batteryPackCurrent': 7.0,
        'batteryPackTemperature': 30,
        'engineTorque': 650,
        'odometer': 0,
        'tripOdometer': 0,
        'genericbinary': '165165650',
        'satRadioESN': "165165650",
        'rainSensor': 165165650,
        'gps': {
            'longitudeDegrees': 423293,
            'latitudeDegrees': -830464,
            'utcYear': 2013,
            'utcMonth': 2,
            'utcDay': 14,
            'utcHours': 13,
            'utcMinutes': 16,
            'utcSeconds': 54,
            'compassDirection': 'SOUTHWEST',
            'pdop': 15,
            'hdop': 5,
            'vdop': 30,
            'actual': false,
            'satellites': 8,
            'dimension': '2D',
            'altitude': 7,
            'heading': 173,
            'speed': 2
        }
    },

    /**
     * Method to set selected state of vehicle transmission to vehicleData
     */
    onPRNDLSelected: function() {
        if( this.prndlSelectState ) {
            this.set( 'vehicleData.VEHICLEDATA_PRNDLSTATUS.data', this.prndlSelectState );
        }
    }.observes( 'this.prndlSelectState' ),

    /**
     * Method calls GetVehicleType response
     * 
     * @type {Number}
     */
    getVehicleType: function( id ) {
        FFW.VehicleInfo.GetVehicleTypeResponse( this.vehicleType, id );
    },

    /**
     * SDL VehicleInfo.GetDTCs handler fill data for response about vehicle
     * errors
     * 
     * @type {Object} params
     * @type {Number} id
     */
    vehicleInfoGetDTCs: function( params, id ) {
        var dtc = [],
            result = "",
            ecuHeader = 2;

        for( var i = 0; i < 3; i++ ){
            dtc.push( "line " + i );
        }

        result = "SUCCESS";

        FFW.VehicleInfo.vehicleInfoGetDTCsResponse( ecuHeader, dtc, result, id );
 
    },

    /**
     * SDL VehicleInfo.ReadDID handler send response about vehicle conditions
     * 
     * @type {Object} params
     * @type {Number} id
     */
    vehicleInfoReadDID: function( params, id ) {
        var didResult = [], resultCode = "";
        // magic number used because there is no huge database on HMI of vehicle
        // data
        if( this.ecuDIDData[1].data ){
            resultCode = "SUCCESS";
        }else{
            resultCode = "INVALID_DATA";
        }

        for( var i = 0; i < params.didLocation.length; i++ ){
            if( i < 10 ){
                didResult[i].resultCode = 'SUCCESS';
                didResult[i].didLocation = params.didLocation[i];
                didResult[i].data = '0';
            }else{
                didResult[i].resultCode = "VEHICLE_DATA_NOT_AVAILABLE";
                didResult[i].didLocation = params.didLocation[i];
                didResult[i].data = '0';
            }
        }

        FFW.VehicleInfo.vehicleInfoReadDIDResponse( didResult, resultCode, id );
    },

    /**
     * Function returns response message to VehicleInfoRPC
     * 
     * @type {Object} message
     */
    getVehicleData: function( message ) {
        var data = {};
        
        for (var key in message) {
            data[key] = this.vehicleData[key];
        }
        
        return data;
    },

    /**
     * Function send all vehicle conditions on FFW.VehicleInfo.OnVehicleData fo
     * notification when data changes
     */
    onVehicleDataChanged: function() {

        var jsonData = {};
        for( var i in this.vehicleData ){
            jsonData[this.vehicleData[i].type] = this.vehicleData[i].data;
        }
        FFW.VehicleInfo.OnVehicleData( jsonData );

    }.observes( 'this.vehicleData.VEHICLEDATA_PRNDLSTATUS.data' )
} );
