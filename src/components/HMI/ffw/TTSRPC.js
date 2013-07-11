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
 * Reference implementation of TTS component.
 * 
 * TTS is responsible for playing sound data sent from SDLCore to notify user
 * about some events happened.
 */

FFW.TTS = FFW.RPCObserver.create( {
    
    /**
     * If true then TTS is present and ready to communicate with SDL.
     *
     * @type {Boolean}
     */
    isReady: false,
    
    /*
     * access to basic RPC functionality
     */
    client: FFW.RPCClient.create( {
        componentName: "TTS"
    } ),

    /*
     * connect to RPC bus
     */
    connect: function() {
        this.client.connect( this, 300 );
    },

    /*
     * disconnect from RPC bus
     */
    disconnect: function() {
        this.client.disconnect();
    },

    /*
     * Client is registered - we can send request starting from this point of
     * time
     */
    onRPCRegistered: function() {
        Em.Logger.log( "FFW.TTS.onRPCRegistered" );
        this._super();
    },

    /*
     * Client is unregistered - no more requests
     */
    onRPCUnregistered: function() {
        Em.Logger.log( "FFW.TTS.onRPCUnregistered" );
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
    onRPCResult: function( response ) {
        Em.Logger.log( "FFW.TTS.onRPCResult" );
        this._super();
    },

    /*
     * handle RPC erros here
     */
    onRPCError: function( error ) {
        Em.Logger.log( "FFW.TTS.onRPCError" );
        this._super();
    },

    /*
     * handle RPC notifications here
     */
    onRPCNotification: function( notification ) {
        Em.Logger.log( "FFW.TTS.onRPCNotification" );
        this._super();
    },

    /*
     * handle RPC requests here
     */
    onRPCRequest: function( request ) {
        Em.Logger.log( "FFW.TTS.onRPCRequest" );
        this._super();

        switch( request.method ){
            case "TTS.Speak": {
                SDL.SDLModel.onPrompt( request.params.ttsChunks.splice( 0, 1 ) );

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type (enum) from SDL
                        "method": "TTS.Speak"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "TTS.SetGlobalProperties": {

                SDL.SDLModel.setProperties( request.params );

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type (enum) from SDL
                        "method": "TTS.SetGlobalProperties"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "TTS.StopSpeaking": {

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type (enum) from SDL
                        "method": "TTS.StopSpeaking"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "TTS.GetCapabilities": {

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "capabilities":
                            [
                                "TEXT"
                            ],

                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type (enum) from SDL
                        // protocol
                        "method": "TTS.GetCapabilities"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "TTS.GetSupportedLanguages": {

                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type (enum) from SDL
                        "method": "TTS.GetSupportedLanguages",
                        "languages": SDL.SDLModel.sdlLanguagesList
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "TTS.GetLanguage": {

                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"],  // type (enum) from SDL
                        "method": "TTS.GetLanguage",
                        "language": SDL.SDLModel.hmiTTSVRLanguage
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "TTS.ChangeRegistration": {

                SDL.SDLModel.changeRegistrationTTSVR( request.params.language );

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "code": SDL.SDLModel.resultCode["SUCCESS"], // type (enum) from SDL
                        // protocol
                        "method": "TTS.ChangeRegistration"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            
            case "TTS.IsReady": {
                
                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "available": this.get('isReady'),
                        "code": SDL.SDLModel.resultCode["SUCCESS"],
                        "method" : "TTS.IsReady"
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

    /*
     * Notifies if sdl TTS components language was changed
     */
    OnLanguageChange: function( lang ) {
        Em.Logger.log( "FFW.TTS.OnLanguageChange" );

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "TTS.OnLanguageChange",
            "params": {
                "language": lang
            }
        };
        this.client.send( JSONMessage );
    }
} )