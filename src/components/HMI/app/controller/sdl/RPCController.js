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
 * @name SDL.RPCController
 * @desc RPC Controller
 * @category Controller
 * @filesource app/controller/sdl/RPCController.js
 * @version 1.0
 */

SDL.RPCController = Em.Object
    .create( {

        capabilityCheckResult: null,

        /**
         * Start register RPC components on controller init
         */
        ConnectToSDL: function() {

            if (FLAGS.Buttons) {
                FFW.Buttons.connect();
            }

            if (FLAGS.TTS) {
                FFW.TTS.connect();
            }

            if (FLAGS.VR) {
                FFW.VR.connect();
            }

            if (FLAGS.BC) {
                FFW.BasicCommunication.connect();
            }

            if (FLAGS.UI) {
                FFW.UI.connect();
            }

            if (FLAGS.VI) {
                FFW.VehicleInfo.connect();
            }

            if (FLAGS.Navi) {
                FFW.Navigation.connect();
            }
        },

        capabilitiesCheck: function(key, value) {
            if (key == "imageType" && value == "STATIC") {
                SDL.RPCController.capabilityCheckResult = 'UNSUPPORTED_RESOURCE';
            }
            if (key == "type" && value == "PRE_RECORDED") {
                SDL.RPCController.capabilityCheckResult = 'UNSUPPORTED_RESOURCE';
            }
            if (key == "type" && value == "SAPI_PHONEMES") {
                SDL.RPCController.capabilityCheckResult = 'UNSUPPORTED_RESOURCE';
            }
            if (key == "type" && value == "LHPLUS_PHONEMES") {
                SDL.RPCController.capabilityCheckResult = 'UNSUPPORTED_RESOURCE';
            }
            if (key == "type" && value == "SILENCE") {
                SDL.RPCController.capabilityCheckResult = 'UNSUPPORTED_RESOURCE';
            }
        },

        /**
         * Method to check supported image type in request
         * 
         * @param {Object}
         *            array
         */
        checkImagesArray: function(array) {

            var error = false;
            if (array instanceof Array) {
                for ( var i = 0; i < array.length; i++) {
                    if (array[i].image && 
                        ((array[i].image.imageType !== "DYNAMIC") && 
                         (array[i].image.imageType !== "STATIC"))) {

                    }
                }
            }
            return error;
        },

        /**
         * Object that contains check methods that returns true if mandatory
         * fields is successful and returns false if not
         */
        BasicCommunication: Em.Object
            .create( {

                resultStruct: {},

                /**
                 * Validate method for request UpdateAppList
                 *
                 * @param {Object}
                 *            params
                 */
                UpdateAppList: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request UpdateDeviceList
                 * 
                 * @param {Object}
                 *            params
                 */
                UpdateDeviceList: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.deviceList == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'deviceList' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.deviceList.length < 0) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'deviceList'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },


                /**
                 * Validate method for request PolicyUpdate
                 *
                 * @param {Object}
                 *            params
                 */
                PolicyUpdate: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.file == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'file' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.file != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'file'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.timeout == null) {

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
                    if (params.retry == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'retry' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.retry.length < 1) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'retry'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

	 	        /**
                 * Validate method for request SystemRequest
                 *
                 * @param {Object}
                 *            params
                 */
                SystemRequest: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.fileName == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'fileName' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.fileName != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'fileName'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.requestType == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'requestType' does not exists!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ActivateApp
                 *
                 * @param {Object}
                 *            params
                 */
                ActivateApp: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request AllowSDLFunctionality
                 *
                 * @param {Object}
                 *            params
                 */
                AllowSDLFunctionality: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.device == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'device' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.device != 'object') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'device'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request AllowDeviceToConnect
                 * 
                 * @param {Object}
                 *            params
                 */
                AllowDeviceToConnect: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.device == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'device' does not exists!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetSystemInfo
                 *
                 * @param {Object}
                 *            params
                 */
                GetSystemInfo: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request MixingAudioSupported
                 * 
                 * @param {Object}
                 *            params
                 */
                MixingAudioSupported: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request AllowAllApps
                 * 
                 * @param {Object}
                 *            params
                 */
                AllowAllApps: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request AllowApp
                 * 
                 * @param {Object}
                 *            params
                 */
                AllowApp: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appPermissions == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'app' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appPermissions.length < 1) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appPermissions'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
         * Object that contains check methods that returns true if mandatory
         * fields is successful and returns false if not
         */
        TTS: Em.Object
            .create( {

                resultStruct: {},

                /**
                 * Validate method for request IsReady
                 * 
                 * @param {Object}
                 *            params
                 */
                IsReady: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetLanguage
                 * 
                 * @param {Object}
                 *            params
                 */
                GetLanguage: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request SetGlobalProperties
                 * 
                 * @param {Object}
                 *            params
                 */
                SetGlobalProperties: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetSupportedLanguages
                 * 
                 * @param {Object}
                 *            params
                 */
                GetSupportedLanguages: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request StopSpeaking
                 * 
                 * @param {Object}
                 *            params
                 */
                StopSpeaking: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request Speak
                 * 
                 * @param {Object}
                 *            params
                 */
                Speak: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.ttsChunks == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'ttsChunks' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.ttsChunks.length < 1) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'ttsChunks'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ChangeRegistration
                 * 
                 * @param {Object}
                 *            params
                 */
                ChangeRegistration: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.language == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'language' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.language != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'language'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request OnLanguageChange
                 * 
                 * @param {Object}
                 *            params
                 */
                OnLanguageChange: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.language == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'language' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.language != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'language'!"
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
         * Object that contains check methods that returns true if mandatory
         * fields is successful and returns false if not
         */
        Navigation: Em.Object
            .create( {

                resultStruct: {},

                /**
                 * Validate method for request IsReady
                 * 
                 * @param {Object}
                 *            params
                 */
                IsReady: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request Navigation.SendLocation
                 *
                 * @param {Object}
                 *            params
                 */
                SendLocation: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request Navigation.AlertManeuver
                 *
                 * @param {Object}
                 *            params
                 */
                AlertManeuver: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request StartStream
                 *
                 * @param {Object}
                 *            params
                 */
                StartStream: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.url == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'url' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.url != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'url'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request StopStream
                 *
                 * @param {Object}
                 *            params
                 */
                StopStream: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request StartAudioStream
                 *
                 * @param {Object}
                 *            params
                 */
                StartAudioStream: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.url == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'url' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.url != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'url'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request StopAudioStream
                 *
                 * @param {Object}
                 *            params
                 */
                StopAudioStream: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request Navigation.ShowConstantTBT
                 * 
                 * @param {Object}
                 *            params
                 */
                ShowConstantTBT: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("softButtons" in params) {
                        if (SDL.RPCController
                            .checkImagesArray(params.softButtons)) {
                            this.resultStruct = {
                                "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                "resultMessage": "Unsupported image type!"
                            };

                            return this.resultStruct;
                        }
                    }
                    if ("turnIcon" in params
                        && ((params.turnIcon.imageType !== "DYNAMIC") && (params.turnIcon.imageType !== "STATIC"))) {
                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Unsupported image type!"
                        };

                        return this.resultStruct;
                    }
                    if ("nextTurnIcon" in params
                        && ((params.nextTurnIcon.imageType !== "DYNAMIC") && (params.nextTurnIcon.imageType !== "STATIC"))) {
                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Unsupported image type!"
                        };

                        return this.resultStruct;
                    }
                    if (params.navigationTexts == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
                 * @param {Object}
                 *            params
                 */
                UpdateTurnList: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("softButtons" in params) {
                        if (SDL.RPCController
                            .checkImagesArray(params.softButtons)) {
                            this.resultStruct = {
                                "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                "resultMessage": "Unsupported image type!"
                            };

                            return this.resultStruct;
                        }
                    }
                    if ("turnList" in params) {

                        for ( var i = 0; i < params.turnList.length; i++) {
                            if (params.turnList[i].image
                                && ((params.turnIcon.imageType !== "DYNAMIC")
                                &&  (params.turnIcon.imageType !== "STATIC"))) {

                                this.resultStruct = {
                                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                    "resultMessage": "Unsupported image type!"
                                };

                                return this.resultStruct;
                                e;
                            }
                        }
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                }

            }),

        /**
         * Object that contains check methods that returns true if mandatory
         * fields is successful and returns false if not
         */
        UI: Em.Object
            .create( {

                resultStruct: {},

                /**
                 * Validate method for request SetDisplayLayout
                 * 
                 * @param {Object}
                 *            params
                 */
                SetDisplayLayout: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request IsReady
                 *
                 * @param {Object}
                 *            params
                 */
                IsReady: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request UI.Alert
                 * 
                 * @param {Object}
                 *            params
                 */
                Alert: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("softButtons" in params) {
                        if (SDL.RPCController
                            .checkImagesArray(params.softButtons)) {
                            this.resultStruct = {
                                "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                "resultMessage": "Unsupported image type!"
                            };

                            return this.resultStruct;
                        }
                    }
                    if (params.alertStrings == null) {

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
                    if ('duration' in params && typeof params.duration != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'duration'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
                 * @param {Object}
                 *            params
                 */
                ShowNotification: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("icon" in params &&
                        ((params.turnIcon.imageType !== "DYNAMIC") && (params.turnIcon.imageType !== "STATIC"))) {
                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Unsupported image type!"
                        };

                        return this.resultStruct;
                    }
                    if (params.text == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'text' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.text.length < 0) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'text'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.timeout == null) {

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
                },

                /**
                 * Validate method for request Show
                 * 
                 * @param {Object}
                 *            params
                 */
                Show: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("softButtons" in params) {
                        if (SDL.RPCController
                            .checkImagesArray(params.softButtons)) {
                            this.resultStruct = {
                                "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                "resultMessage": "Unsupported image type!"
                            };

                            return this.resultStruct;
                        }
                    }
                    if ("graphic" in params
                        && ((params.graphic.imageType !== "DYNAMIC") && (params.graphic.imageType !== "STATIC"))) {
                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Unsupported image type!"
                        };

                        return this.resultStruct;
                    }
                    if (params.showStrings == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
                 * @param {Object}
                 *            params
                 */
                AddCommand: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("cmdIcon" in params
                        && (params.cmdIcon.imageType !== "DYNAMIC" && params.cmdIcon.imageType !== "STATIC")) {
                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Unsupported image type!"
                        };

                        return this.resultStruct;
                    }
                    if (params.cmdID == null) {

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
                    if (params.menuParams == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'menuParams' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.menuParams != 'object') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'menuParams'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.menuParams.menuName == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'menuName' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.menuParams.menuName != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'menuName'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
                 * @param {Object}
                 *            params
                 */
                DeleteCommand: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.cmdID == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
                 * @param {Object}
                 *            params
                 */
                AddSubMenu: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.menuID == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
                 * @param {Object}
                 *            params
                 */
                DeleteSubMenu: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.menuID == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }
                    // don't delete if current submenu active
                    if (SDL.SDLController.getApplicationModel(params.appID)
                        .get('currentSubMenuId') == params.menuID) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["IN_USE"],
                            "resultMessage": "SubMenu is already in use!"
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
                 * @param {Object}
                 *            params
                 */
                PerformInteraction: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.timeout == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request SetMediaClockTimer
                 * 
                 * @param {Object}
                 *            params
                 */
                SetMediaClockTimer: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.updateMode == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'updateMode' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.updateMode != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'updateMode'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request SetGlobalProperties
                 * 
                 * @param {Object}
                 *            params
                 */
                SetGlobalProperties: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("vrHelp" in params) {
                        if (SDL.RPCController.checkImagesArray(params.vrHelp)) {
                            this.resultStruct = {
                                "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                "resultMessage": "Unsupported image type!"
                            };

                            return this.resultStruct;
                        }
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetCapabilities
                 * 
                 * @param {Object}
                 *            params
                 */
                GetCapabilities: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ChangeRegistration
                 * 
                 * @param {Object}
                 *            params
                 */
                ChangeRegistration: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.language == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'language' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.language != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'language'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetSupportedLanguages
                 * 
                 * @param {Object}
                 *            params
                 */
                GetSupportedLanguages: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetLanguage
                 * 
                 * @param {Object}
                 *            params
                 */
                GetLanguage: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request SetAppIcon
                 * 
                 * @param {Object}
                 *            params
                 */
                SetAppIcon: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.syncFileName == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'syncFileName' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.syncFileName != 'object') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'syncFileName'!"
                        };

                        return this.resultStruct;
                    }
                    if ("syncFileName" in params
                        && ((params.syncFileName.imageType !== "DYNAMIC") && (params.syncFileName.imageType !== "STATIC"))) {
                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Unsupported image type!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request Slider
                 * 
                 * @param {Object}
                 *            params
                 */
                Slider: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.numTicks == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'numTicks' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.numTicks != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'numTicks'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.position == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'position' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.position != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'position'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.sliderHeader == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'sliderHeader' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.sliderHeader != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'sliderHeader'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.timeout == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'timeout' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.timeout != 'number' || params.timeout < 0) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'timeout'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ScrollableMessage
                 * 
                 * @param {Object}
                 *            params
                 */
                ScrollableMessage: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if ("softButtons" in params) {
                        if (SDL.RPCController
                            .checkImagesArray(params.softButtons)) {
                            this.resultStruct = {
                                "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                "resultMessage": "Unsupported image type!"
                            };

                            return this.resultStruct;
                        }
                    }
                    if (params.messageText == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'messageText' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.messageText.length < 0) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'messageText'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.timeout == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request PerformAudioPassThru
                 * 
                 * @param {Object}
                 *            params
                 */
                PerformAudioPassThru: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.audioPassThruDisplayTexts == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'audioPassThruDisplayTexts' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.audioPassThruDisplayTexts.length < 0
                        || params.audioPassThruDisplayTexts.length > 2) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'audioPassThruDisplayTexts'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request EndAudioPassThru
                 * 
                 * @param {Object}
                 *            params
                 */
                EndAudioPassThru: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ClosePopUp
                 * 
                 * @param {Object}
                 *            params
                 */
                ClosePopUp: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ShowVrHelp
                 * 
                 * @param {Object}
                 *            params
                 */
                ShowVrHelp: function(params) {

                    if (params) {
                        if ("vrHelp" in params) {
                            if (SDL.RPCController
                                .checkImagesArray(params.vrHelp)) {
                                this.resultStruct = {
                                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                    "resultMessage": "Unsupported image type!"
                                };

                                return this.resultStruct;
                            }
                        }
                        if ("appID" in params) {

                            if (typeof params.appID != 'number') {

                                this.resultStruct = {
                                    "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                    "resultMessage": "Wrong type of parameter 'appID'!"
                                };

                                return this.resultStruct;
                            }
                        }
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                }
            }),

        /**
         * Object that contains check methods that returns true if mandatory
         * fields is successful and returns false if not
         */
        VehicleInfo: Em.Object
            .create( {

                resultStruct: {},

                /**
                 * Validate method for request IsReady
                 * 
                 * @param {Object}
                 *            params
                 */
                IsReady: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request DiagnosticMessage
                 *
                 * @param {Object}
                 *            params
                 */
                DiagnosticMessage: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetVehicleType
                 * 
                 * @param {Object}
                 *            params
                 */
                GetVehicleType: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ReadDID
                 * 
                 * @param {Object}
                 *            params
                 */
                ReadDID: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.ecuName == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'ecuName' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.ecuName != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'ecuName'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.didLocation == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'didLocation' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.didLocation.length < 0) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'didLocation'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetDTCs
                 * 
                 * @param {Object}
                 *            params
                 */
                GetDTCs: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.ecuName == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'ecuName' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.ecuName != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'ecuName'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetVehicleData
                 *
                 * @param {Object}
                 *            params
                 */
                GetVehicleData: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    /*if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }*/

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request SubscribeVehicleData
                 *
                 * @param {Object}
                 *            params
                 */
                SubscribeVehicleData: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request UnsubscribeVehicleData
                 *
                 * @param {Object}
                 *            params
                 */
                UnsubscribeVehicleData: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
         * Object that contains check methods that returns true if mandatory
         * fields is successful and returns false if not
         */
        VR: Em.Object
            .create( {

                resultStruct: {},

                /**
                 * Validate method for request IsReady
                 * 
                 * @param {Object}
                 *            params
                 */
                IsReady: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request PerformInteraction
                 *
                 * @param {Object}
                 *            params
                 */
                PerformInteraction: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.timeout == null) {

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

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetCapabilities
                 *
                 * @param {Object}
                 *            params
                 */
                GetCapabilities: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetLanguage
                 * 
                 * @param {Object}
                 *            params
                 */
                GetLanguage: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request GetSupportedLanguages
                 * 
                 * @param {Object}
                 *            params
                 */
                GetSupportedLanguages: function(params) {

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request AddCommand
                 * 
                 * @param {Object}
                 *            params
                 */
                AddCommand: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.cmdID == null) {

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
                    if (params.vrCommands == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'vrCommands' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.vrCommands.length < 0) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'vrCommands'!"
                        };

                        return this.resultStruct;
                    }
                    if ("appID" in params) {
                        if (typeof params.appID != 'number') {

                            this.resultStruct = {
                                "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                                "resultMessage": "Wrong type of parameter 'appID'!"
                            };

                            return this.resultStruct;
                        }
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request DeleteCommand
                 * 
                 * @param {Object}
                 *            params
                 */
                DeleteCommand: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.cmdID == null) {

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
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
                        };

                        return this.resultStruct;
                    }

                    this.resultStruct = {
                        "resultCode": SDL.SDLModel.resultCode["SUCCESS"]
                    };

                    return this.resultStruct;
                },

                /**
                 * Validate method for request ChangeRegistration
                 * 
                 * @param {Object}
                 *            params
                 */
                ChangeRegistration: function(params) {

                    if (params == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'params' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (params.language == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'language' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.language != 'string') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'language'!"
                        };

                        return this.resultStruct;
                    }
                    if (params.appID == null) {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Parameter 'appID' does not exists!"
                        };

                        return this.resultStruct;
                    }
                    if (typeof params.appID != 'number') {

                        this.resultStruct = {
                            "resultCode": SDL.SDLModel.resultCode["INVALID_DATA"],
                            "resultMessage": "Wrong type of parameter 'appID'!"
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
