/**
 * @name MFT.ApplinkVehicleInfoModel
 * 
 * @desc Applink model with vehicle information used instead of CAN network. VehicleInfoModel is simulation of real CAN network. 
 * 
 * @category    Model
 * @filesource  app/model/applink/ApplinkVehicleInfoModel.js
 * @version     1.0
 *
 * @author      Andriy Melnik
 */
 
MFT.ApplinkVehicleInfoModel = Em.Object.create({

    /**
     * Stored VehicleInfo transmission state Data
     */
    vehicleInfoPRNDL: [
        {
            name:   "PARK",
            id:     0
        },
        {
            name:   "REVERSE",
            id:     1
        },
        {
            name:   "NEUTRAL",
            id:     2
        },
        {
            name:   "FORWARD_DRIVE_2",
            id:     3
        },
        {
            name:   "LOWGEAR",
            id:     4
        }
    ],

    /**
     * Stored VehicleInfo Data
     */
    ecuDIDData:[
        {
            'data':     "ECU 1 Test Data"
        },
        {
            'data':     "ECU 2 Test Data"
        }
    ],

    /**
     * Stored VehicleInfo Data
     */
    vehicleData: {
        'speed':                    null,
        'rpm':                      null,
        'fuelLevel':                null,
        'avgFuelEconomy':           null,
        'batteryVoltage':           null,
        'externalTemperature':      null,
        'vin':                      null,
        'prndl':                    null,
        'tirePressure':             null,
        'batteryPackVoltage':       null,
        'batteryPackCurrent':       null,
        'batteryPackTemperature':   null,
        'engineTorque':             null,
        'odometer':                 null,
        'tripOdometer':             null,
        'genericbinary':            null
    },

    /**
     * Applink VehicleInfo.GetDTCs handler
     * fill data for response about vehicle errors
     */
    vehicleInfoGetDTCs: function( id ){
        var data = {},
            i = 0,
            info = "Inormation about reported DTC's",
            result = "";

        for(i = 0; i < 3; i++) {
            data[i] = {};
            data[i].identifier =  "0";
            data[i].statusByte =  "0";
        }

        result = "SUCCESS";

        FFW.VehicleInfo.vehicleInfoGetDTCsResponse( data, info, result, id );
    },

    /**
     * Applink VehicleInfo.ReadDID handler
     * send response about vehicle conditions
     */
    vehicleInfoReadDID: function(params, id){
        var data = [],
            i = 0,
            info = '',
            dataResult = [],
            resultCode = "";
        if(this.ecuDIDData[params.ecuName].data){
            info = this.ecuDIDData[params.ecuName].data;
            result = "SUCCESS";
        }else{
            info = "";
            result = "INVALID_DATA";
        }

        
        for(i = 0; i < params.didLocation.length; i++) {
            if(i < 10){
                dataResult[i] = 'SUCCESS';
                data[i] =       0;
            }else{
                dataResult[i] = "INVALID_DATA";
                data[i] =       0;
            }
        }

        FFW.VehicleInfo.vehicleInfoReadDIDResponse( dataResult, data, info, result, id );
    },

    /** 
     * Function returns response message to VehicleInfoRPC
     */
    getVehicleData: function( message ){

        return this.vehicleData[message.dataType];

    },

    /** 
     * Function send all vehicle conditions on FFW.VehicleInfo.OnVehicleData
     * fo notification when data changes
     */
    onVehicleDataChanged: function(){

        var jsonData = {};
        for(var i  in this.vehicleData) {
          jsonData[i] = this.vehicleData[i];
        }
        FFW.VehicleInfo.OnVehicleData(jsonData);

    }.observes('this.vehicleData.speed',
        'this.vehicleData.rpm',
        'this.vehicleData.fuelLevel',
        'this.vehicleData.avgFuelEconomy',
        'this.vehicleData.batteryVoltage',
        'this.vehicleData.externalTemperature',
        'this.vehicleData.vin',
        'this.vehicleData.prndl',
        'this.vehicleData.tirePressure', 
        'this.vehicleData.batteryPackVoltage',
        'this.vehicleData.batteryPackCurrent',
        'this.vehicleData.batteryPackTemperature',
        'this.vehicleData.engineTorque',
        'this.vehicleData.odometer',
        'this.vehicleData.tripOdometer',
        'this.vehicleData.genericbinary' )
});
 