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
/**
 * @desc Observer of RPCClient
 */

FFW.RPCObserver = Em.Object
    .extend( {

        onRPCRegistered: function() {

            // request necessary parameters from Backend
            SDL.SDLController.registeredComponentStatus(this.client.componentName);
        },

        onRPCUnregistered: function() {

            Em.Logger.log("FFW.RPCObserver.onUnregistered");
        },

        onRPCDisconnected: function() {

            Em.Logger.log("FFW.RPCObserver.onRPCDisconnected");
        },

        /*
         * when result is received from RPC component this function is called It
         * is the propriate place to check results of reuqest execution Please
         * use previously store reuqestID to determine to which request repsonse
         * belongs to
         */
        onRPCResult: function(result) {

            // parse JSON string and set necessary properties
        },

        /*
         */
        onRPCError: function(error) {

            // parse JSON string and set necessary properties
        },

        /*
         */
        onRPCNotification: function(notification) {

            // parse JSON string and set necessary properties
        },

        onRPCRequest: function(request) {

            // parse JSON string and send back necessary data
        },

        validationCheck: function(request) {

            if (request && request.method) {
                var parsedMethod = request.method.split(/[.]/), validateFunc, result;

                //if (request.params && "appID" in request.params && SDL.SDLModel.registeredApps.filterProperty('appID', request.params.appID).length > 0) {

                if (request.params && "appID" in request.params && typeof(request.params.appID) == "number" && SDL.SDLModel.registeredApps.filterProperty('appID', request.params.appID).length <= 0) {
                    Em.Logger.error('No application registered with current appID!');
                    return false;
                }

                if (SDL.RPCController[parsedMethod[0]][parsedMethod[1]]) {
                    validateFunc = SDL.RPCController[parsedMethod[0]][parsedMethod[1]];
                    result = validateFunc(request.params);

                    if (result.resultCode != SDL.SDLModel.resultCode["SUCCESS"]) {
                        this.sendError(result.resultCode,
                            request.id,
                            request.method,
                            result.resultMessage);
                        return false;
                    } else {
                        return true;
                    }
                } else {
                    this.sendError(SDL.SDLModel.resultCode["UNSUPPORTED_REQUEST"],
                            request.id,
                            request.method,
                            'Unsupported incoming request! ' + parsedMethod[0]
                                + '.' + parsedMethod[1]);
                    Em.Logger.error('No method "' + parsedMethod[0] + '.'
                        + parsedMethod[1] + '"');
                    return false;
                }
            }
        }
    });
