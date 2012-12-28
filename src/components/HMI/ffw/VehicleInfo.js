/*
 * Reference implementation of VehicleInfo component.
 * 
 * Interface to get or set some essential information from OS.
 * Since web application is not able to access some OS feature through WebKit
 * it is necessary to use communication with native code to get some or set values 
 * and execute some functions. 
 * 
 *
 * @author Andriy Melnik
 */

FFW.VehicleInfo = FFW.RPCObserver.create({
        
    /*
     *  access to basic RPC functionality
     */     
     client:        FFW.RPCClient.create({ componentName: "VehicleInfo" }),
    
    
    /*
     * connect to RPC bus
     */
    connect: function() {
        // to be removed
        //this.stubGetValues();
        
        this.client.connect(this, 600);
    },

    /*
     * disconnect from RPC bus
     */
    disconnect: function() {
        this.client.disconnect();
    },

    /*
     * Client is registered - we can send request starting from this point of time
     */ 
    onRPCRegistered: function () {
        Em.Logger.log("FFW.VehicleInfo.onRPCRegistered");
        this._super();
    },
    
    /*
     * Client is unregistered - no more requests
     */ 
    onRPCUnregistered: function () {
        Em.Logger.log("FFW.VehicleInfo.onRPCUnregistered");
        this._super();
    },

    /*
     * Client disconnected.
     */
    onRPCDisconnected: function() {

    },

    /*
     * when result is received from RPC component this function is called
     * It is the propriate place to check results of reuqest execution
     * Please use previously store reuqestID to determine to which request repsonse belongs to
     */ 
    onRPCResult: function(response) {
        Em.Logger.log("FFW.VehicleInfo.onRPCResult");
        this._super();
     },
    
    /*
     * handle RPC erros here
     */ 
    onRPCError: function(error) {
        Em.Logger.log("FFW.VehicleInfo.onRPCError");
        this._super();
    },

    /*
     * handle RPC notifications here 
     */ 
    onRPCNotification: function(notification) {
        Em.Logger.log("FFW.VehicleInfo.onRPCNotification");
        this._super();
    },
    
    /*
     * handle RPC requests here
     */ 
    onRPCRequest: function(request) {
        Em.Logger.log("FFW.VehicleInfo.onRPCRequest");
        this._super();

        if (request.method == "VehicleInfo.GetVehicleData") {

            var vehicleData = MFT.ApplinkVehicleInfoModel.onGetVehicleData(request.params),
                resultCode;

            if( vehicleData ){
                resultCode = "SUCCESS";
            }else{
                resultCode = "GENERIC_ERROR";
            }

            // send repsonse
            var JSONMessage = {
                "jsonrpc"   :   "2.0",
                "id"        :   request.id,
                "result":   {
                    "resultCode" :              resultCode, //  type (enum) from AppLink protocol
                    "method"   :                "VehicleInfo.GetVehicleDataResponse",
                    //request.params.dataType:    vehicleData
                }
            };
            this.client.send(JSONMessage);
        }

        if (request.method == "VehicleInfo.ReadDID") {

            MFT.ApplinkVehicleInfoModel.onVehicleInfoReadDID( request.params, request.id );

        }

        if (request.method == "VehicleInfo.GetDTCs") {

            MFT.ApplinkVehicleInfoModel.onVehicleInfoGetDTCs( request.id );

        }
    },

    /*
     * Notifies if data was changed
     */ 
    OnVehicleData: function( params ) {
        Em.Logger.log("FFW.VehicleInfo.OnVehicleData");

        // send repsonse
        var JSONMessage = {
            "jsonrpc":  "2.0",
            "method":   "VehicleInfo.OnVehicleData",
            "params":   params
        };
        this.client.send(JSONMessage);
    },

    /*
     * ReadDID Response
     */ 
    onVehicleInfoReadDIDResponse: function( dataResult, data, info, result, id ) {
        Em.Logger.log("FFW.VehicleInfo.ReadDID");

        // send repsonse
        var JSONMessage = {
            "jsonrpc"   :   "2.0",
            "id"        :   id,
            "result":   {
                "resultCode":       result, //  type (enum) from AppLink protocol
                "method":           "VehicleInfo.ReadDIDResponse",
                "params":{
                    "info":         info,
                    "dataResult":   dataResult,
                    "data":         data
                }
            }
        };
        this.client.send(JSONMessage);
    },

    /*
     * GetDTCs Response
     */ 
    onVehicleInfoGetDTCsResponse: function( data, info, result, id ) {
        Em.Logger.log("FFW.VehicleInfo.GetDTCs");

        // send repsonse
        var JSONMessage = {
            "jsonrpc"   :   "2.0",
            "id"        :   id,
            "result":   {
                "resultCode":       result, //  type (enum) from AppLink protocol
                "method":           "VehicleInfo.GetDTCsResponse",
                "params":{
                    "info":         info,
                    "dtcList":      data
                }
            }
        };
        this.client.send(JSONMessage);
    }
})