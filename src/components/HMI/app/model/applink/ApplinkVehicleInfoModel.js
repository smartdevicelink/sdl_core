/**
 * @name MFT.ApplinkVehicleInfoModel
 * 
 * @desc General Applink model
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
            name:   "Parking",
            id:     0
        },
        {
            name:   "Reverse gear",
            id:     1
        },
        {
            name:   "No gear",
            id:     2
        },
        {
            name:   "Forward drive",
            id:     3
        },
        {
            name:   "1st gear hold",
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
    vehicleData: [{
            'type':     "gps",
            'data':     null
        },
        {
            'type':     "speed",
            'data':     null
        },
        {
            'type':     "rpm",
            'data':     null
        },
        {
            'type':     "fuelLevel",
            'data':     null
        },
        {
            'type':     "avgFuelEconomy",
            'data':     null
        },
        {
            'type':     "batteryVoltage",
            'data':     null
        },
        {
            'type':     "externalTemperature",
            'data':     null
        },
        {
            'type':     "vin",
            'data':     null
        },
        {
            'type':     "prndl",
            'data':     null
        },
        {
            'type':     "tirePressure",
            'data':     null
        },
        {
            'type':     "batteryPackVoltage",
            'data':     null
        },
        {
            'type':     "batteryPackCurrent",
            'data':     null
        },
        {
            'type':     "batteryPackTemperature",
            'data':     null
        },
        {
            'type':     "engineTorque",
            'data':     null
        },
        {
            'type':     "odometer",
            'data':     null
        },
        {
            'type':     "tripOdometer",
            'data':     null
        },
        {
            'type':     "genericbinary",
            'data':     null
        }
    ],

    /**
     * Applink VehicleInfo.GetDTCs handler
     */
    onVehicleInfoGetDTCs: function( id ){
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

        FFW.VehicleInfo.onVehicleInfoGetDTCsResponse( data, info, result, id );
    },

    /**
     * Applink VehicleInfo.ReadDID handler
     */
    onVehicleInfoReadDID: function(params, id){
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

        if(params.didLocation.length < 10){
            for(i = 0; i < params.didLocation.length; i++) {
              dataResult[i] =   'SUCCESS';
              data[i] =         0;
            }
        }else{
            result = "INVALID_DATA";
        }

        FFW.VehicleInfo.onVehicleInfoReadDIDResponse( dataResult, data, info, result, id );
    },

    /** 
     * Function returns response message to VehicleInfoRPC
     */
    onGetVehicleData: function( message ){

        return this.vehicleData[message.dataType];

    },

    /** 
     * Function call FFW.VehicleInfo.OnVehicleData when data changes
     */
    onVehicleDataChanged: function(){

        var jsonData = {},
            i = 0;
        for(i = 0; i < this.vehicleData.length; i++) {
          jsonData[this.vehicleData[i].type] = this.vehicleData[i].data;
        }
        FFW.VehicleInfo.OnVehicleData(jsonData);

    }.observes('this.vehicleData.@each.data')
});
 