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
/*
 * Reference implementation of VehicleInfo component.
 * 
 * Interface to get or set some essential information sent from SDLCore.
 * VehicleInfo is responsible for sending a data about the condition of the
 * vehicle between SDLCore and CAN network. Instead CAN network used
 * VehicleInfoModel.
 * 
 */

FFW.VehicleInfo = FFW.RPCObserver.create( {

    /**
     * If true then VehicleInfo is present and ready to communicate with SDL.
     *
     * @type {Boolean}
     */
    isReady: false,

    /**
     * access to basic RPC functionality
     */
    client: FFW.RPCClient.create( {
        componentName: "VehicleInfo"
    } ),

    /**
     * connect to RPC bus
     */
    connect: function() {
        this.client.connect( this, 600 );
    },

    /**
     * disconnect from RPC bus
     */
    disconnect: function() {
        this.client.disconnect();
    },

    /**
     * Client is registered - we can send request starting from this point of
     * time
     */
    onRPCRegistered: function() {
        Em.Logger.log( "FFW.VehicleInfo.onRPCRegistered" );
        this._super();
    },

    /**
     * Client is unregistered - no more requests
     */
    onRPCUnregistered: function() {
        Em.Logger.log( "FFW.VehicleInfo.onRPCUnregistered" );
        this._super();
    },

    /**
     * Client disconnected.
     */
    onRPCDisconnected: function() {

    },

    /**
     * when result is received from RPC component this function is called It is
     * the propriate place to check results of reuqest execution Please use
     * previously store reuqestID to determine to which request repsonse belongs
     * to
     */
    onRPCResult: function( response ) {
        Em.Logger.log( "FFW.VehicleInfo.onRPCResult" );
        this._super();
    },

    /**
     * handle RPC erros here
     */
    onRPCError: function( error ) {
        Em.Logger.log( "FFW.VehicleInfo.onRPCError" );
        this._super();
    },

    /**
     * handle RPC notifications here
     */
    onRPCNotification: function( notification ) {
        Em.Logger.log( "FFW.VehicleInfo.onRPCNotification" );
        this._super();
    },

    /**
     * handle RPC requests here
     * 
     * @type {Object} request
     */
    onRPCRequest: function( request ) {
        Em.Logger.log( "FFW.VehicleInfo.onRPCRequest" );
        this._super();
        
        switch( request.method ){
            case "VehicleInfo.GetVehicleData": {
                
                var vehicleData = SDL.SDLVehicleInfoModel.getVehicleData( request.params ), resultCode;

                if( vehicleData ){
                    resultCode = "SUCCESS";
                }else{
                    resultCode = "GENERIC_ERROR";
                }
    
                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "resultCode": resultCode, // type (enum) from SDL protocol
                        "method": "VehicleInfo.GetVehicleDataResponse"
                    // request.params.dataType: vehicleData
                    }
                };
                JSONMessage.result[SDL.SDLVehicleInfoModel.vehicleData[request.params.dataType].type] = vehicleData;
                this.client.send( JSONMessage );
                
                break;
            }
            
            case "VehicleInfo.ReadDID": {
                
                SDL.SDLVehicleInfoModel.vehicleInfoReadDID( request.params, request.id );
                
                break;
            }
            
            case "VehicleInfo.GetDTCs": {
                
                SDL.SDLVehicleInfoModel.vehicleInfoGetDTCs( request.params, request.id );
                
                break;
            }
            
            case "VehicleInfo.GetVehicleType": {
                
                SDL.SDLVehicleInfoModel.getVehicleType( request.id );
                
                break;
            }
            
            case "VehicleInfo.IsReady": {
                
                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "method" : "VehicleInfo.IsReady",
                        "available": this.get('isReady')
                    }
                };
                
                this.client.send( JSONMessage );
                
                break;
            }
            
            default: {
                // statements_def
                break;
            }
        }
    },

    /**
     * Notifies if data was changed
     * 
     * @type {Object} params
     */
    OnVehicleData: function( params ) {
        Em.Logger.log( "FFW.VehicleInfo.OnVehicleData" );

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VehicleInfo.OnVehicleData",
            "params": params
        };
        this.client.send( JSONMessage );
    },

    /**
     * GetVehicleType Response
     * 
     * @type {string} vehicleType
     * @type {int} id
     */
    GetVehicleTypeResponse: function( vehicleType, id ) {
        Em.Logger.log( "FFW.VehicleInfo.GetVehicleType" );

        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": id,
            "result": {
                "resultCode": "SUCCESS", // type (enum) from SDL protocol
                "method": "VehicleInfo.GetVehicleTypeResponse",
                "vehicleType": vehicleType
            }
        };

        this.client.send( JSONMessage );
    },

    /**
     * ReadDID Response
     * 
     * @type {array} dataResult
     * @type {array} data
     * @type {string} info
     * @type {string} result
     * @type {int} id
     */
    vehicleInfoReadDIDResponse: function( dataResult, data, info, result, id ) {
        Em.Logger.log( "FFW.VehicleInfo.ReadDID" );

        var JSONMessage;
        // send repsonse
        if( result != 'ENCRYPTED' ){
            JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "result": {
                    "resultCode": result, // type (enum) from SDL protocol
                    "method": "VehicleInfo.ReadDIDResponse",
                    "info": info,
                    "dataResult": dataResult,
                    "data": data
                }
            };
        }else{
            JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "result": {
                    "resultCode": result, // type (enum) from SDL protocol
                    "method": "VehicleInfo.ReadDIDResponse",
                    "info": info
                }
            };
        }
        this.client.send( JSONMessage );
    },

    /**
     * GetDTCs Response
     * 
     * @type {array} data
     * @type {string} info
     * @type {string} result
     * @type {int} id
     */
    vehicleInfoGetDTCsResponse: function( data, info, result, id ) {
        Em.Logger.log( "FFW.VehicleInfo.GetDTCs" );

        var JSONMessage;
        // send repsonse
        if( result != 'ENCRYPTED' ){
            JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "result": {
                    "resultCode": result, // type (enum) from SDL protocol
                    "method": "VehicleInfo.GetDTCsResponse",
                    "info": info,
                    "dtcList": data
                }
            };
        }else{
            JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "result": {
                    "resultCode": result, // type (enum) from SDL protocol
                    "method": "VehicleInfo.GetDTCsResponse",
                    "info": info
                }
            };
        }
        this.client.send( JSONMessage );
    }
} )