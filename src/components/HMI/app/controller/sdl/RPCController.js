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
/**
 * @name SDL.ValidateMessage
 * @desc RPC Controller
 * @category Controller
 * @filesource app/controller/sdl/RPCController.js
 * @version 1.0
 */

SDL.ValidateMessage = Em.Object.create({

    
    /**
     * Object that contains check methods that
     * returns true if mandatory fields is successful
     * and returns false if not
     */
    Navigation: Em.Object.create({

        resultStruct: {},
        
        /**
         * Validate method for request Navigation.ShowConstantTBT
         * 
         * @param {Object} params
         */
        ShowConstantTBT: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.navigationTexts) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'navigationTexts' does not exists!"
                };

                return this.resultStruct;
            }
            if (params.navigationTexts.length < 0) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'navigationTexts'!"
                };

                return this.resultStruct;
            }
            if (!params.turnIcon) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'turnIcon' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.turnIcon != 'string') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'turnIcon'!"
                };
                
                return this.resultStruct;
            }
            if (!params.distanceToManeuver) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'distanceToManeuver' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.distanceToManeuver != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'distanceToManeuver'!"
                };
                
                return this.resultStruct;
            }
            if (!params.distanceToManeuverScale) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'distanceToManeuverScale' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.distanceToManeuverScale != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'distanceToManeuverScale'!"
                };
                
                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }
            
            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };
            
            return this.resultStruct;
        },
        
        /**
         * Validate method for request Navigation.UpdateTurnList
         * 
         * @param {Object} params
         */
        UpdateTurnList: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.turnList) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'navigationTexts' does not exists!"
                };

                return this.resultStruct;
            }
            if (params.turnList.length < 1) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'navigationTexts'!"
                };

                return this.resultStruct;
            }
            
            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };
            
            return this.resultStruct;
        }

    }),
    
    /**
     * Object that contains check methods that
     * returns true if mandatory fields is successful
     * and returns false if not
     */
    UI: Em.Object.create({

        resultStruct: {},
        
        /**
         * Validate method for request UI.Alert
         * 
         * @param {Object} params
         */
        Alert: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.alertStrings) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'alertStrings' does not exists!"
                };

                return this.resultStruct;
            }
            if (params.alertStrings.length < 0) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'alertStrings'!"
                };

                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request Show
         * 
         * @param {Object} params
         */
        Show: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.showStrings) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'showStrings' does not exists!"
                };

                return this.resultStruct;
            }
            if (params.showStrings.length < 0) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'showStrings'!"
                };

                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request AddCommand
         * 
         * @param {Object} params
         */
        AddCommand: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.cmdID) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'cmdID' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.cmdID != 'number') {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'cmdID'!"
                };

                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request DeleteCommand
         * 
         * @param {Object} params
         */
        DeleteCommand: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.cmdID) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'cmdID' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.cmdID != 'number') {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'cmdID'!"
                };

                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request AddSubMenu
         * 
         * @param {Object} params
         */
        AddSubMenu: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.menuID) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'menuID' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.menuID != 'number') {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'menuID'!"
                };

                return this.resultStruct;
            }
            if (!params.menuParams) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'menuParams' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request DeleteSubMenu
         * 
         * @param {Object} params
         */
        DeleteSubMenu: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.menuID) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'menuID' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.menuID != 'number') {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'menuID'!"
                };

                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request CreateInteractionChoiceSet
         * 
         * @param {Object} params
         */
        CreateInteractionChoiceSet: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.interactionChoiceSetID) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'interactionChoiceSetID' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.interactionChoiceSetID != 'number') {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'interactionChoiceSetID'!"
                };

                return this.resultStruct;
            }
            if (!params.choiceSet) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'choiceSet' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.choiceSet.length < 1) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'choiceSet'!"
                };

                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request DeleteInteractionChoiceSet
         * 
         * @param {Object} params
         */
        DeleteInteractionChoiceSet: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.interactionChoiceSetID) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'interactionChoiceSetID' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.interactionChoiceSetID != 'number') {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'interactionChoiceSetID'!"
                };

                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        },
        
        /**
         * Validate method for request PerformInteraction
         * 
         * @param {Object} params
         */
        PerformInteraction: function(params) {

            if (!params) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'params' does not exists!"
                };

                return this.resultStruct;
            }
            if (!params.initialText) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'initialText' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.initialText != 'object') {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'initialText'!"
                };

                return this.resultStruct;
            }
            if (!params.interactionChoiceSetIDList) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'interactionChoiceSetIDList' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.interactionChoiceSetIDList.length < 0 ) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'interactionChoiceSetIDList'!"
                };

                return this.resultStruct;
            }
            if (!params.timeout) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'timeout' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.timeout != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'timeout'!"
                };
                
                return this.resultStruct;
            }
            if (!params.appId) {

                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Parameter 'appId' does not exists!"
                };

                return this.resultStruct;
            }
            if (typeof params.appId != 'number') {
                
                this.resultStruct = {
                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                    "resultMessage": "Wrong type of parameter 'appId'!"
                };
                
                return this.resultStruct;
            }

            this.resultStruct = {
                "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
            };

            return this.resultStruct;
        }
    })
});
