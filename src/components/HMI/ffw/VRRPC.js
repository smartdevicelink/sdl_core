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
 * Reference implementation of VR component.
 * 
 * Interface to get or set some essential information sent from SDLCore. VR is
 * responsible for receiving voice commands choosed by user and sending
 * notification of chosen commands to SDLCore.
 * 
 */

FFW.VR = FFW.RPCObserver.create( {

    /**
     * If true then VR is present and ready to communicate with SDL.
     * 
     * @type {Boolean}
     */
    isReady: true,

    /**
     * Contains response codes for request that should be processed but there were some kind of errors
     * Error codes will be injected into response.
     */
    errorResponsePull: {},

    /*
     * access to basic RPC functionality
     */
    client: FFW.RPCClient.create( {
        componentName: "VR"
    }),

    /*
     * connect to RPC bus
     */
    connect: function() {

        this.client.connect(this, 500); // Magic number is unique identifier for
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

        Em.Logger.log("FFW.VR.onRPCRegistered");
        this._super();
    },

    /*
     * Client is unregistered - no more requests
     */
    onRPCUnregistered: function() {

        Em.Logger.log("FFW.VR.onRPCUnregistered");
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

        Em.Logger.log("FFW.VR.onRPCResult");
        this._super();
    },

    /*
     * handle RPC erros here
     */
    onRPCError: function(error) {

        Em.Logger.log("FFW.VR.onRPCError");
        this._super();
    },

    /*
     * handle RPC notifications here
     */
    onRPCNotification: function(notification) {

        Em.Logger.log("FFW.VR.onRPCNotification");
        this._super();
    },

    /*
     * send notification when command was triggered
     */
    onCommand: function(element) {

        Em.Logger.log("FFW.VR.onCommand");

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnCommand",
            "params": {
                "commandID": element.commandID
            }
        };
        this.client.send(JSONMessage);

        SDL.VRPopUp.set('VRActive', false);
    },

    /*
     * handle RPC requests here
     */
    onRPCRequest: function(request) {

        Em.Logger.log("FFW.VR.onRPCRequest");
        if (this.validationCheck(request)) {

            switch (request.method) {
            case "VR.AddCommand": {

                SDL.SDLModel.addCommandVR(request.params);

                this.sendVRResult(SDL.SDLModel.resultCode["SUCCESS"],
                    request.id,
                    request.method);

                break;
            }
            case "VR.DeleteCommand": {

                SDL.SDLModel.deleteCommandVR(request);

                break;
            }
            case "VR.GetSupportedLanguages": {

                Em.Logger.log("FFW." + request.method + "Response");

                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type
                        // (enum)
                        // from SDL
                        "method": "VR.GetSupportedLanguages",
                        "languages": SDL.SDLModel.sdlLanguagesList
                    }
                };
                this.client.send(JSONMessage);

                break;
            }
            case "VR.GetLanguage": {

                Em.Logger.log("FFW." + request.method + "Response");

                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type
                        // (enum)
                        // from SDL
                        "method": "VR.GetLanguage",
                        "language": SDL.SDLModel.hmiTTSVRLanguage
                    }
                };
                this.client.send(JSONMessage);

                break;
            }
            case "VR.ChangeRegistration": {

                SDL.SDLModel.changeRegistrationTTSVR(request.params.language, request.params.appID);

                if (request.params.vrSynonyms) {
                    SDL.VRPopUp.DeleteCommand(0, request.params.appID);
                    SDL.VRPopUp.AddCommand(0, request.params.vrSynonyms, request.params.appID, "Application");
                }

                this.sendVRResult(SDL.SDLModel.resultCode["SUCCESS"],
                    request.id,
                    request.method);

                break;
            }
            case "VR.IsReady": {

                Em.Logger.log("FFW." + request.method + "Response");

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "available": this.get('isReady'),
                        "code": 0,
                        "method": "VR.IsReady"
                    }
                };

                this.client.send(JSONMessage);

                break;
            }
            case "VR.PerformInteraction":
            {

                // Werify if there is an ansupported data in request
                if (this.errorResponsePull[request.id] != null) {

                    //Check if there is any available data to  process the request
                    if ("helpPrompt" in request.params
                        || "initialPrompt" in request.params
                        || "timeoutPrompt" in request.params
                        || "grammarID" in request.params) {

                        this.errorResponsePull[request.id].code = SDL.SDLModel.resultCode["WARNINGS"];
                    } else {
                        //If no available data sent error response and stop process current request

                        this.sendError(this.errorResponsePull[request.id].code, request.id, request.method,
                                "Unsupported " + this.errorResponsePull[request.id].type + " type. Request was not processed.");
                        this.errorResponsePull[request.id] = null;

                        return;
                    }
                }

                SDL.SDLModel.vrPerformInteraction(request);

                break;
            }
            case "VR.GetCapabilities": {

                Em.Logger.log("FFW." + request.method + "Response");

                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"],
                        "method": "VR.GetCapabilities",
                        "vrCapabilities": ["TEXT"]
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
     * send notification when command was triggered
     *
     * @param {Number} requestID
     * @param {Number} resultCode
     * @param {Number} commandID
     */
    interactionResponse: function (requestID, resultCode, commandID) {

        Em.Logger.log("FFW.VR.PerformInteractionResponse");

        if (this.errorResponsePull[requestID]) {

            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": requestID,
                "error": {
                    "code": this.errorResponsePull[requestID].code,
                    "message": "Unsupported " + this.errorResponsePull[requestID].type + " type. Available data in request was processed.",
                    "data": {
                        "method": "VR.PerformInteraction"
                    }
                }
            };

            if (commandID) {
                JSONMessage.error.data.choiceID = commandID;
            }

            this.client.send(JSONMessage);
            this.errorResponsePull[requestID] = null;
            return;
        }

        if (resultCode === SDL.SDLModel.resultCode["SUCCESS"]) {
            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": requestID,
                "result": {
                    "code": resultCode,
                    "method": "VR.PerformInteraction"
                }
            };

            if (commandID) {
                JSONMessage.result.choiceID = commandID;
            }
        } else {
            // send repsonse
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": requestID,
                "error": {
                    "code": resultCode, // type (enum) from SDL protocol
                    "message": "Perform Interaction error response.",
                    "data": {
                        "method": "VR.PerformInteraction"
                    }
                }
            };
        }

        SDL.SDLModel.set('performInteractionSession', []);

        this.client.send(JSONMessage);
    },

    /**
     * send response from onRPCRequest
     * 
     * @param {Number}
     *            resultCode
     * @param {Number}
     *            id
     * @param {String}
     *            method
     */
    sendVRResult: function(resultCode, id, method) {

        if (this.errorResponsePull[id]) {

            this.sendError(this.errorResponsePull[id].code, id, method,
                    "Unsupported " + this.errorResponsePull[id].type + " type. Available data in request was processed.");
            this.errorResponsePull[id] = null;
            return;
        }

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

    /*
     * send notification when command was triggered from VR
     */
    onChoise: function(commandID) {

        Em.Logger.log("FFW.VR.PerformInteraction");

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnChoise",
            "params": {
                "choiceID": commandID
            }
        };

        this.client.send(JSONMessage);
    },

    /**
     * Initiated by VR module to let SDL know that VR session has started.
     */
    Started: function() {

        Em.Logger.log("FFW.VR.Started");

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.Started"
        };
        this.client.send(JSONMessage);
    },

    /**
     * Initiated by VR module to let SDL know that VR session has stopped.
     */
    Stopped: function() {

        Em.Logger.log("FFW.VR.Stopped");

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.Stopped"
        };
        this.client.send(JSONMessage);
    },

    /**
     * send notification when command was triggered
     */
    onCommand: function(commandID, appID) {

        Em.Logger.log("FFW.VR.onCommand");

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnCommand",
            "params": {
                "cmdID": commandID,
                "appID": appID
            }
        };
        this.client.send(JSONMessage);
    },

    /**
     * Notifies if sdl VR components language was changed
     */
    OnLanguageChange: function(lang) {

        Em.Logger.log("FFW.VR.OnLanguageChange");

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnLanguageChange",
            "params": {
                "language": lang
            }
        };
        this.client.send(JSONMessage);
    }
})