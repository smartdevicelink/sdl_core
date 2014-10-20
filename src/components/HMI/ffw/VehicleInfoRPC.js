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
    isReady: true,

    /**
     * Contains response codes for request that should be processed but there were some kind of errors
     * Error codes will be injected into response.
     */
    errorResponsePull: {},

    /**
     * access to basic RPC functionality
     */
    client: FFW.RPCClient.create( {
        componentName: "VehicleInfo"
    }),

    /**
     * connect to RPC bus
     */
    connect: function() {

        this.client.connect(this, 700); // Magic number is unique identifier for
        // component
    },

    /**
     * disconnect from RPC bus
     */
    disconnect: function() {

        this.onRPCUnregistered();
        this.client.disconnect();
    },

    /**
     * Client is registered - we can send request starting from this point of
     * time
     */
    onRPCRegistered: function() {

        Em.Logger.log("FFW.VehicleInfo.onRPCRegistered");
        this._super();
    },

    /**
     * Client is unregistered - no more requests
     */
    onRPCUnregistered: function() {

        Em.Logger.log("FFW.VehicleInfo.onRPCUnregistered");
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
    onRPCResult: function(response) {

        Em.Logger.log("FFW.VehicleInfo.onRPCResult");
        this._super();
    },

    /**
     * handle RPC erros here
     */
    onRPCError: function(error) {

        Em.Logger.log("FFW.VehicleInfo.onRPCError");
        this._super();
    },

    /**
     * handle RPC notifications here
     */
    onRPCNotification: function(notification) {

        Em.Logger.log("FFW.VehicleInfo.onRPCNotification");
        this._super();
    },

    /**
     * handle RPC requests here
     * 
     * @type {Object} request
     */
    onRPCRequest: function(request) {

        Em.Logger.log("FFW.VehicleInfo.onRPCRequest");
        if (this.validationCheck(request)) {

            switch (request.method) {
            case "VehicleInfo.GetVehicleData": {

                SDL.SDLVehicleInfoModel.getVehicleData(request);

                break;
            }

            case "VehicleInfo.ReadDID": {

                SDL.SDLVehicleInfoModel.vehicleInfoReadDID(request.params,
                    request.id);

                break;
            }

            case "VehicleInfo.GetDTCs": {

                SDL.SDLVehicleInfoModel.vehicleInfoGetDTCs(request.params,
                    request.id);

                break;
            }

            case "VehicleInfo.DiagnosticMessage": {

                Em.Logger.log("FFW." + request.method + "Response");

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "messageDataResult": [200],
                        "code": SDL.SDLModel.resultCode["SUCCESS"],
                        "method": "VehicleInfo.DiagnosticMessage"
                    }
                };

                this.client.send(JSONMessage);

                break;

                break;
            }

            case "VehicleInfo.SubscribeVehicleData": {

                SDL.SDLVehicleInfoModel.SubscribeVehicleData(request);

                break;
            }

            case "VehicleInfo.UnsubscribeVehicleData": {

                SDL.SDLVehicleInfoModel.UnsubscribeVehicleData(request);

                break;
            }

            case "VehicleInfo.GetVehicleType": {

                SDL.SDLVehicleInfoModel.getVehicleType(request.id);

                break;
            }

            case "VehicleInfo.IsReady": {

                Em.Logger.log("FFW." + request.method + "Response");

                // send response
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "available": this.get('isReady'),
                        "code": SDL.SDLModel.resultCode["SUCCESS"],
                        "method": "VehicleInfo.IsReady"
                    }
                };

                this.client.send(JSONMessage);

                break;
            }

            default: {
                // statements_def
                break;
            }
            }
        }
    },

    /**
     * Send error response from onRPCRequest
     *
     * @param {Number}
     *            resultCode
     * @param {Number}
     *            id
     * @param {String}
     *            method
     */
    sendError: function(resultCode, id, method, message) {

        Em.Logger.log("FFW." + method + "Response");

        if (resultCode != SDL.SDLModel.resultCode["SUCCESS"]) {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "error": {
                    "code": resultCode, // type (enum) from SDL protocol
                    "message": message,
                    "data": {
                        "method": method
                    }
                }
            };
            this.client.send(JSONMessage);
        }
    },

    /**
     * Send response from onRPCRequest
     *
     * @param {Number}
     *            resultCode
     * @param {Number}
     *            id
     * @param {String}
     *            method
     */
    sendVIResult: function(resultCode, id, method) {

        Em.Logger.log("FFW." + method + "Response");

        if (resultCode === SDL.SDLModel.resultCode["SUCCESS"]) {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "result": {
                    "code": resultCode,
                    "method": method
                }
            };
            this.client.send(JSONMessage);
        }
    },

    /**
     * Send response from onRPCRequest
     *
     * @param {Number} resultCode
     * @param {Number} id
     * @param {String} method
     * @param {Object} data
     */
    sendVISubscribeVehicleDataResult: function(resultCode, id, method, data) {

        Em.Logger.log("FFW." + method + "Response");

        if (resultCode === SDL.SDLModel.resultCode["SUCCESS"]) {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "result": data
            };

            JSONMessage.result.code = resultCode;
            JSONMessage.result.method = method;

            this.client.send(JSONMessage);
        }
    },

    /**
     * Send error response from onRPCRequest
     * 
     * @param {Number}
     *            resultCode
     * @param {Number}
     *            id
     * @param {String}
     *            method
     */
    sendGetVehicleDataError: function(resultCode, id, method, message, data) {

        Em.Logger.log("FFW." + method + "Response");

        if (resultCode != SDL.SDLModel.resultCode["SUCCESS"]) {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "error": {
                    "code": resultCode, // type (enum) from SDL protocol
                    "message": message,
                    "data": data
                }
            };

            JSONMessage.error.data.method = method;
            this.client.send(JSONMessage);
        }
    },

    /**
     * Send error response from onRPCRequest
     * 
     * @param {Number}
     *            resultCode
     * @param {Number}
     *            id
     * @param {String}
     *            method
     */
    sendGetVehicleDataResut: function(resultCode, id, method, data) {

        Em.Logger.log("FFW." + method + "Response");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": id,
            "result": data
        };
        JSONMessage.result.code = resultCode;
        JSONMessage.result.method = method;
        this.client.send(JSONMessage);
    },

    /**
     * Notifies if data was changed
     * 
     * @type {Object} params
     */
    OnVehicleData: function(params) {

        Em.Logger.log("FFW.VehicleInfo.OnVehicleData");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VehicleInfo.OnVehicleData",
            "params": params
        };
        this.client.send(JSONMessage);
    },

    /**
     * GetVehicleType Response
     * 
     * @type {string} vehicleType
     * @type {int} id
     */
    GetVehicleTypeResponse: function(vehicleType, id) {

        Em.Logger.log("FFW.VehicleInfo.GetVehicleTypeResponse");

        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": id,
            "result": {
                "code": SDL.SDLModel.resultCode["SUCCESS"],
                "method": "VehicleInfo.GetVehicleType",
                "vehicleType": vehicleType
            }
        };

        this.client.send(JSONMessage);
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
    vehicleInfoReadDIDResponse: function(didResult, resultCode, id) {

        Em.Logger.log("FFW.VehicleInfo.ReadDIDResponse");

        var JSONMessage;
        // send repsonse
        JSONMessage = {
            "jsonrpc": "2.0",
            "id": id,
            "result": {
                "code": SDL.SDLModel.resultCode[resultCode], // type (enum)
                // from SDL
                // protocol
                "method": "VehicleInfo.ReadDID",
                "didResult": didResult
            }
        };
        this.client.send(JSONMessage);
    },

    /**
     * GetDTCs Response
     * 
     * @type {Number} ecuHeader
     * @type {Array} dtc
     * @type {String} result
     * @type {Number} id
     */
    vehicleInfoGetDTCsResponse: function(ecuHeader, dtc, result, id) {

        Em.Logger.log("FFW.VehicleInfo.GetDTCsResponse");

        var JSONMessage;
        // send repsonse
        JSONMessage = {
            "jsonrpc": "2.0",
            "id": id,
            "result": {
                "code": SDL.SDLModel.resultCode[result], // type (enum) from
                // SDL protocol
                "method": "VehicleInfo.GetDTCs",
                "ecuHeader": ecuHeader,
                "dtc": dtc
            }
        };

        this.client.send(JSONMessage);
    }
})