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

FFW.CAN = FFW.RPCObserver.create( {

    /**
     * If true then CAN is present and ready to communicate with SDL.
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
        componentName: "CAN",
        url: FLAGS.CAN_WEBSOCKET_URL
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

        Em.Logger.log("FFW.CAN.onRPCRegistered");
        this._super();
    },

    /**
     * Client is unregistered - no more requests
     */
    onRPCUnregistered: function() {

        Em.Logger.log("FFW.CAN.onRPCUnregistered");
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

        Em.Logger.log("FFW.CAN.onRPCResult");
        this._super();
    },

    /**
     * handle RPC erros here
     */
    onRPCError: function(error) {

        Em.Logger.log("FFW.CAN.onRPCError");
        this._super();
    },

    /**
     * handle RPC notifications here
     */
    onRPCNotification: function(notification) {

        Em.Logger.log("FFW.CAN.onRPCNotification");

        switch (notification.method) {
            case "CAN.OnRadioDetails": {

                if ("radioStation" in notification.params) {
                    SDL.RadioModel.updateRadioFrequency(notification.params.radioStation);
                }
                SDL.RadioModel.updateSongInfo(notification.params.songInfo);


                break;
            }
            case "CAN.StartScan": {

                SDL.RadioModel.toggleProperty('scanState');

                break;
            }
            case "CAN.StopScan": {

                SDL.RadioModel.toggleProperty('scanState');

                break;
            }
            default: {
                // statements_def
                break;
            }
        }

        this._super();
    },

    /**
     * handle RPC requests here
     *
     * @type {Object} request
     */
    onRPCRequest: function(request) {

        Em.Logger.log("FFW.CAN.onRPCRequest");
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
    sendCANResult: function(resultCode, id, method) {

        Em.Logger.log("FFW." + method + "Response");

        if (resultCode === SDL.SDLModel.resultCode["SUCCESS"]) {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": id,
                "result": {
                    "code": resultCode, // type (enum) from SDL protocol
                    "method": method
                }
            };
            this.client.send(JSONMessage);
        }
    },

    /**
     * Send notification to CAN to tune radio
     *
     */
    TuneUp: function() {

        Em.Logger.log("FFW.CAN.TuneUp");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.TuneUp"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Send notification to CAN to tune radio
     *
     */
    TuneDown: function() {

        Em.Logger.log("FFW.CAN.TuneDown");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.TuneDown"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Send notification to CAN to Start Scan logic
     *
     */
    StartScan: function() {

        Em.Logger.log("FFW.CAN.StartScan");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.StartScan"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Send notification to CAN to Start Scan logic
     *
     */
    StopScan: function() {

        Em.Logger.log("FFW.CAN.StopScan");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.StopScan"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Notification about changed on HMI screen radio presets send to SDL
     *
     * @param {Object}
     */
    OnPresetsChanged: function(presets) {

        Em.Logger.log("FFW.CAN.OnPresetsChanged Notification");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.OnPresetsChanged",
            "params": {
                "customPresets": presets
            }
        };
        this.client.send(JSONMessage);

    },

    /**
     * Send response for request GetRadioDetails
     *
     * @param {Object}
     */
    GetRadioDetails: function(request) {

        Em.Logger.log("FFW.CAN.GetRadioDetails Response");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "id": id,
            "method": "CAN.GetRadioDetails",
            "result": {
                "code": resultCode,
                "method": method
            }
        };

        for (var key in SDL.RadioModel.radioDetails) {
            JSONMessage.result[key] = SDL.RadioModel.radioDetails[key];
        }
        this.client.send(JSONMessage);
    },

    /**
     * Notification When any of current radio tuner details are changed
     *
     * @param {Object}
     */
    OnRadioDetails: function(data) {

        Em.Logger.log("FFW.CAN.OnRadioDetails Notification");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.OnRadioDetails",
            "params": {
            }
        };

        for (var key in data) {
            JSONMessage.params[key] = data[key];
        }
        this.client.send(JSONMessage);

    },

    /**
     * Notification when have action in player
     *
     * @param {Object}
     */
    OnPlayerDetails: function (data) {

        Em.Logger.log("FFW.CAN.OnPlayerDetails Notification");

        // send response
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.OnPlayerDetails",
            "params": {
            }
        };

        for (var key in data) {
            JSONMessage.params[key] = data[key];
        }

        this.client.send(JSONMessage);

    },

    /**
     * Notification about changed on HMI screen radio presets send to SDL
     *
     * @param {Object}
     */
    OnPresetsChanged: function(presets) {

        Em.Logger.log("FFW.CAN.OnPresetsChanged Notification");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "CAN.OnPresetsChanged",
            "params": {
                "customPresets": presets
            }
        };
        this.client.send(JSONMessage);

    },

    sendPlayerDetails : function(){
        var player = SDL.MediaController.get('currentSelectedPlayer');

        if(player){
            var media = player.data.get('selectedItem'),
                params = {
                    "songInfo":{
                        "name": media.name,
                        "artist": media.artist,
                        "genre": media.genre,
                        "album": media.album,
                        "year": media.year,
                        "duration": media.duration,
                        "currentTime" : player.get('currentTime')
                    },
                    "model" : player.name
                };

            FFW.CAN.OnPlayerDetails(params);
        }
    }
});
