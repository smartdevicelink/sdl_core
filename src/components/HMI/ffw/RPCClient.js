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
 * Base class for RPC client
 *
 * Class provides access to basic functionality of RPC components Message Broker
 * as: registerComponent unregisterComponent subscription to notifications logic
 * to calculate request id
 */

FFW.RPCClient = Em.Object
    .extend( {

        /*
         * transport layer for messages exchange
         */
        socket: null, // instance of websocket

        /*
         * url for message broker
         */
        url: FLAGS.WEBSOCKET_URL,

        /*
         * Component name in RPC system It is unique.
         */
        componentName: null,

        /*
         * observer of RPC states
         */
        observer: null,

        /*
         * these variables are used to have unique request ids for different
         * components in RPC bus idStart is received as a response for
         * registerRPCComponent messages. space for ids for specific RPC
         * component is allocated by message broker
         */
        idStart: -1,
        idRange: 1000,
        requestId: -1,

        registerRequestId: -1,
        unregisterRequestId: -1,

        /*
         * Open WebSocket and initialize handlers
         */
        connect: function(observer, startId) {

            this.observer = observer;
            this.idStart = startId;

            this.socket = new WebSocket(this.url);

            var self = this;

            this.socket.onopen = function(evt) {

                self.onWSOpen(evt)
            };
            this.socket.onclose = function(evt) {

                self.onWSClose(evt)
            };
            this.socket.onmessage = function(evt) {

                self.onWSMessage(evt)
            };
            this.socket.onerror = function(evt) {

                self.onWSError(evt)
            };

        },

        /*
         * Close WebSocket connection Please make sure that component was
         * unregistered in advance
         */
        disconnect: function() {

            this.unregisterRPCComponent();
            SDL.SDLController.unregisterComponentStatus(this.observer.client.componentName);
        },

        /*
         * WebSocket connection is ready Now RPC component can be registered in
         * message broker
         */
        onWSOpen: function(evt) {

            Em.Logger.log("RPCCLient.onWSOpen");

            this.registerRPCComponent();
        },

        /*
         * when result is received from RPC component this function is called It
         * is the propriate place to check results of reuqest execution Please
         * use previously store reuqestID to determine to which request repsonse
         * belongs to
         */
        onWSMessage: function(evt) {

            Em.Logger.log("Message received: " + evt.data);

            var jsonObj = JSON.parse(evt.data);

            JSON.parse(evt.data, SDL.RPCController.capabilitiesCheck);

            if (jsonObj.method == 'UI.Show' && SDL.RPCController.capabilityCheckResult == 'UNSUPPORTED_RESOURCE' && Object.size(jsonObj.params) != 3 && jsonObj.params.showStrings.length != 0) {

                this.observer.errorResponsePull[jsonObj.id] = SDL.SDLModel.resultCode["WARNINGS"];

                Em.Logger.error('Image of STATIC type is not supported on HMI. Other information was successfully displayed');

                SDL.RPCController.capabilityCheckResult = null;
            } else {

                switch (SDL.RPCController.capabilityCheckResult) {
                    case 'UNSUPPORTED_RESOURCE': {

                        this.observer.errorResponsePull[jsonObj.id] = SDL.SDLModel.resultCode["UNSUPPORTED_RESOURCE"];

                        Em.Logger.error('Unsupported incoming resource! In method ' + jsonObj.method);

                        SDL.RPCController.capabilityCheckResult = null;

                        break;
                    }
                }
            }


            // handle component registration
            if (jsonObj.id == this.registerRequestId && jsonObj.method == null && typeof jsonObj.result == 'number') {
                if (jsonObj.error == null) {
                    this.requestId = this.idStart = jsonObj.result;
                    this.observer.onRPCRegistered();
                }
                // handle component unregistration
            } else if (jsonObj.id == this.unregisterRequestId) {
                if (jsonObj.error == null) {
                    this.socket.close();
                    this.observer.onRPCUnregistered();
                }
                // handle result, error, notification, requests
            } else {
                if (jsonObj.id == null) {
                    this.observer.onRPCNotification(jsonObj);
                } else {
                    if (jsonObj.result != null)
                        this.observer.onRPCResult(jsonObj);
                    else if (jsonObj.error != null)
                        this.observer.onRPCError(jsonObj);
                    else
                        this.observer.onRPCRequest(jsonObj);
                }
            }
        },

        /*
         * WebSocket connection is closed Please make sure that RPCComponent was
         * dunregistered in advance
         */
        onWSClose: function(evt) {

            Em.Logger.log("RPCClient: Connection is closed");

            SDL.SDLController.unregisterComponentStatus(this.observer.client.componentName);

            var self = this;
            setTimeout(function(){
                    self.connect(self.observer, self.idStart);
                }, 5000);
            this.observer.onRPCDisconnected();
        },

        /*
         * WebSocket connection errors handling
         */
        onWSError: function(evt) {

            // Em.Logger.log("ERROR: " + evt.data);
            Em.Logger.log("ERROR: ");
        },

        /*
         * register component is RPC bus
         */
        registerRPCComponent: function() {

            this.registerRequestId = this.idStart;

            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": this.registerRequestId,
                "method": "MB.registerComponent",
                "params": {
                    "componentName": this.componentName
                }
            };
            this.send(JSONMessage);
        },

        /*
         * unregister component is RPC bus
         */
        unregisterRPCComponent: function() {

            this.unregisterRequestId = this.generateId();

            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": this.unregisterRequestId,
                "method": "MB.unregisterComponent",
                "params": {
                    "componentName": this.componentName
                }
            };
            this.send(JSONMessage);
        },

        /*
         * Subscribes to notification. Returns the request's id.
         */
        subscribeToNotification: function(notification) {

            var msgId = this.generateId();
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": msgId,
                "method": "MB.subscribeTo",
                "params": {
                    "propertyName": notification
                }
            };
            this.send(JSONMessage);
            return msgId;
        },

        /*
         * Unsubscribes from notification. Returns the request's id.
         */
        unsubscribeFromNotification: function(notification) {

            var msgId = this.generateId();
            var JSONMessage = {
                "jsonrpc": "2.0",
                "id": msgId,
                "method": "MB.unsubscribeFrom",
                "params": {
                    "propertyName": notification
                }
            };
            this.send(JSONMessage);
            return msgId;
        },

        /*
         * stringify object and send via socket connection
         */
        send: function(obj) {

            if (this.socket.readyState == this.socket.OPEN) {

                if (this.observer.errorResponsePull[obj.id] && this.observer.errorResponsePull[obj.id] !== 0 && obj.result) {
                    var method = obj.result.method;

                    delete obj.result;

                    obj.error = {
                        "code": this.observer.errorResponsePull[obj.id],
                        "message": this.observer.errorResponsePull[obj.id] == 21 ? "Image of STATIC type is not supported on HMI. Other information was successfully displayed" : "Unsupported incoming resource!",
                        "data": {
                            "method": method
                        }
                    }

                    delete this.observer.errorResponsePull[obj.id];
                }

                var strJson = JSON.stringify(obj);
                Em.Logger.log(strJson);

                var logTime = new Date();
                console.log(logTime.getHours() + ":" + logTime.getMinutes() + ":" + logTime.getSeconds() + ":" + logTime.getMilliseconds());

                this.socket.send(strJson);
            } else {
                Em.Logger
                    .error("RPCClient: Can't send message since socket is not ready");
            }
        },

        /*
         * Generate id for new request to RPC component Function has to be used
         * as private
         */
        generateId: function() {

            this.requestId++;
            if (this.requestId >= this.idStart + this.idRange)
                this.requestId = this.idStart;
            return this.requestId;
        }

    })
