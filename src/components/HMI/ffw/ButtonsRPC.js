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
 * Reference implementation of Buttons component.
 * 
 * Buttons sends to SDLCore press and hold events of soft buttons, presets and
 * some hard keys.
 */

FFW.Buttons = FFW.RPCObserver.create( {

    /*
     * access to basic RPC functionality
     */
    client: FFW.RPCClient.create( {
        componentName: "Buttons"
    }),

    /**
     * Contains response codes for request that should be processed but there were some kind of errors
     * Error codes will be injected into response.
     */
    errorResponsePull: {},

    /*
     * connect to RPC bus
     */
    connect: function() {

        this.client.connect(this, 200); // Magic number is unique identifier for
        // component
    },

    /*
     * disconnect from RPC bus
     */
    disconnect: function() {

        this.onRPCUnregistered();
        this.client.disconnect();
    },

    /*
     * Client is registered - we can send request starting from this point of
     * time
     */
    onRPCRegistered: function() {

        Em.Logger.log("FFW.Buttons.onRPCRegistered");
        this._super();
    },

    /*
     * Client is unregistered - no more requests
     */
    onRPCUnregistered: function() {

        Em.Logger.log("FFW.Buttons.onRPCUnregistered");
        this._super();
    },

    /*
     * Client disconnected.
     */
    onRPCDisconnected: function() {

    },

    /*
     * when result is received from RPC component this function is called It is
     * the propriate place to check results of reuqest execution Please use
     * previously store reuqestID to determine to which request repsonse belongs
     * to
     */
    onRPCResult: function(response) {

        Em.Logger.log("FFW.Buttons.onRPCResult");
        this._super();
    },

    /*
     * handle RPC erros here
     */
    onRPCError: function(error) {

        Em.Logger.log("FFW.Buttons.onRPCError");
        this._super();
    },

    /*
     * handle RPC notifications here
     */
    onRPCNotification: function(notification) {

        Em.Logger.log("FFW.Buttons.onRPCNotification");
        this._super();
    },

    /*
     * handle RPC requests here
     */
    onRPCRequest: function(request) {

        Em.Logger.log("FFW.Buttons.onRPCRequest");
        this._super();

        if (request.method == "Buttons.GetCapabilities") {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": request.id,
                "result": {
                    "capabilities": [
                        {
                            "name": "PRESET_0",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_1",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_2",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_3",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_4",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_5",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_6",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_7",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_8",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "PRESET_9",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "OK",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "SEEKLEFT",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "SEEKRIGHT",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "TUNEUP",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }, {
                            "name": "TUNEDOWN",
                            "shortPressAvailable": true,
                            "longPressAvailable": true,
                            "upDownAvailable": true
                        }
                    ],
                    "presetBankCapabilities": {
                        "onScreenPresetsAvailable": true
                    },
                    "code": 0,
                    "method": "Buttons.GetCapabilities"
                }
            };
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
    sendError: function(resultCode, id, method, message) {

        Em.Logger.log("FFW." + method + "Response");

        if (resultCode) {

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

    /*
     * Notifies the ButtonsRPC that the web is all set. Should be called twice:
     * when the RPC link is up or failed to connect and all the views are
     * rendered.
     */
    buttonPressed: function(id, type) {

        Em.Logger.log("FFW.Buttons.buttonPressed " + type);

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "Buttons.OnButtonPress",
            "params": {
                "name": id,
                "mode": type
            }
        };

        this.client.send(JSONMessage);
    },

    /*
     * Notifies the ButtonsRPC that the web is all set. Should be called twice:
     * when the RPC link is up or failed to connect and all the views are
     * rendered.
     */
    buttonEvent: function(id, type) {

        Em.Logger.log("FFW.Buttons.OnButtonEvent " + type);

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "Buttons.OnButtonEvent",
            "params": {
                "name": id,
                "mode": type
            }
        };

        this.client.send(JSONMessage);

    },

    /*
     * Notifies the ButtonsRPC that the web is all set. Should be called twice:
     * when the RPC link is up or failed to connect and all the views are
     * rendered.
     */
    buttonPressedCustom: function(name, type, softButtonID, appID) {

        Em.Logger.log("FFW.Buttons.OnButtonPress " + type);

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "Buttons.OnButtonPress",
            "params": {
                "name": name,
                "mode": type,
                "customButtonID": softButtonID,
                "appID": appID
            }
        };

        this.client.send(JSONMessage);
    },

    /*
     * Notifies the ButtonsRPC that the web is all set. Should be called twice:
     * when the RPC link is up or failed to connect and all the views are
     * rendered.
     */
    buttonEventCustom: function(name, type, softButtonID, appID) {

        Em.Logger.log("FFW.Buttons.OnButtonEvent " + type);

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "Buttons.OnButtonEvent",
            "params": {
                "name": name,
                "mode": type,
                "customButtonID": softButtonID,
                "appID": appID
            }
        };

        this.client.send(JSONMessage);

    }
});
