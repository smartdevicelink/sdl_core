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

        onRPCRequest: function(request, error) {

            // parse JSON string and send back necessary data
        },

        /**
         * Check for unsupported image type
         * Return false if unsupported image type was found and delete unsupported resource
         * @param params
         * @returns {boolean}
         */
        checkImage: function(params){
            if ("graphic" in params) {
                if (params.graphic.imageType === "STATIC") {
                    delete params.graphic;
                    return false;
                }
            }
            if ("secondaryGraphic" in params) {
                if (params.secondaryGraphic.imageType === "STATIC") {
                    delete params.secondaryGraphic;
                    return false;
                }
            }
            if ("image" in params) {
                if (params.image.imageType === "STATIC") {
                    delete params.image;
                    return false;
                }
            }
            if ("secondaryImage" in params) {
                if (params.image.imageType === "STATIC") {
                    delete params.image;
                    return false;
                }
            }
            if ("turnIcon" in params) {
                if (params.turnIcon.imageType === "STATIC") {
                    delete params.turnIcon;
                    return false;
                }
            }
            if ("nextTurnIcon" in params) {
                if (params.nextTurnIcon.imageType === "STATIC") {
                    delete params.nextTurnIcon;
                    return false;
                }
            }
            if ("cmdIcon" in params) {
                if (params.cmdIcon.imageType === "STATIC") {
                    delete params.cmdIcon;
                    return false;
                }
            }
            if ("menuIcon" in params) {
                if (params.menuIcon.imageType === "STATIC") {
                    delete params.menuIcon;
                    return false;
                }
            }
            if ("syncFileName" in params) {
                if (params.syncFileName.imageType === "STATIC") {
                    delete params.syncFileName;
                    return false;
                }
            }
            if ("locationImage" in params) {
                if (params.locationImage.imageType === "STATIC") {
                    delete params.locationImage;
                    return false;
                }
            }
            return true;
        },

        /**
         * Check for unsupported image type
         * If unsupported image type was found delete unsupported resource
         * @param params
         */
        checkSoftButtons: function(params){

            if ("softButtons" in params) {
                for (var i = params.softButtons.length-1; i >= 0 ; i--) {
                    if (!this.checkImage(params.softButtons[i])) {
                        if (!("text" in params.softButtons[i])) {

                            params.softButtons.splice(i, 1);
                        }
                    }
                }

                if (params.softButtons.length == 0) {
                    delete params.softButtons;
                }
            }
        },

        /**
         * Check for unsupported image type
         * If unsupported image type was found delete unsupported resource
         * @param params
         */
        checkTurnList: function(params){

            if ("turnList" in params) {
                for (var i = params.turnList.length-1; i >= 0 ; i--) {
                    if (!this.checkImage(params.turnList[i])) {
                        if (!("navigationText" in params.turnList[i])) {

                            params.turnList.splice(i, 1);
                        }
                    }
                }

                if (params.turnList.length == 0) {
                    delete params.turnList;
                }
            }
        },

        /**
         * Check for unsupported tts type
         * If unsupported tts type was found delete unsupported resource
         * @param params
         */
        checkChunk: function(params){

            if ("ttsName" in params) {
                for (var i = params.ttsName.length-1; i >= 0 ; i--) {
                    if (params.ttsName[i].type != "TEXT") {

                        params.ttsName.splice(i, 1);
                    }
                }

                if (params.ttsName.length == 0) {
                    delete params.ttsName;
                }
            }
            if ("helpPrompt" in params) {
                for (var i = params.helpPrompt.length-1; i >= 0 ; i--) {
                    if (params.helpPrompt[i].type != "TEXT") {

                        params.helpPrompt.splice(i, 1);
                    }
                }

                if (params.helpPrompt.length == 0) {
                    delete params.helpPrompt;
                }
            }
            if ("initialPrompt" in params) {
                for (var i = params.initialPrompt.length-1; i >= 0 ; i--) {
                    if (params.initialPrompt[i].type != "TEXT") {

                        params.initialPrompt.splice(i, 1);
                    }
                }

                if (params.initialPrompt.length == 0) {
                    delete params.initialPrompt;
                }
            }
            if ("timeoutPrompt" in params) {
                for (var i = params.timeoutPrompt.length-1; i >= 0 ; i--) {
                    if (params.timeoutPrompt[i].type != "TEXT") {

                        params.timeoutPrompt.splice(i, 1);
                    }
                }

                if (params.timeoutPrompt.length == 0) {
                    delete params.timeoutPrompt;
                }
            }
            if ("ttsChunks" in params) {
                for (var i = params.ttsChunks.length-1; i >= 0 ; i--) {
                    if (params.ttsChunks[i].type != "TEXT") {

                        params.ttsChunks.splice(i, 1);
                    }
                }

                if (params.ttsChunks.length == 0) {
                    delete params.ttsChunks;
                }
            }
        },

        /**
         * Check for unsupported choiceSet type
         * If unsupported choiceSet type was found delete unsupported resource
         * @param params
         */
        checkChoice: function(params){

            if ("choiceSet" in params) {
                for (var i = params.choiceSet.length-1; i >= 0 ; i--) {
                    if (this.checkImage(params.choiceSet[i])) {
                        if (!("menuName" in params.choiceSet[i])
                            || !("secondaryText" in params.choiceSet[i])
                            || !("tertiaryText" in params.choiceSet[i])) {

                            params.choiceSet.splice(i, 1);
                        }
                    }
                }

                if (params.choiceSet.length == 0) {
                    delete params.choiceSet;
                }
            }
        },

        /**
         * Check for unsupported vrHelp type
         * If unsupported vrHelp type was found delete unsupported resource
         * @param params
         */
        checkHelpItems: function(params){

            if ("vrHelp" in params) {
                for (var i = params.vrHelp.length-1; i >= 0 ; i--) {
                    if (this.checkImage(params.vrHelp[i])) {
                        if (!("text" in params.vrHelp[i])) {

                            params.vrHelp.splice(i, 1);
                        }
                    }
                }

                if (params.vrHelp.length == 0) {
                    delete params.vrHelp;
                }
            }
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
