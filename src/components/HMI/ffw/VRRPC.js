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
    isReady: false,
    
    /*
     * access to basic RPC functionality
     */
    client: FFW.RPCClient.create( {
        componentName: "VR"
    } ),

    /*
     * connect to RPC bus
     */
    connect: function() {
        this.client.connect( this, 500 );
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
        Em.Logger.log( "FFW.VR.onRPCRegistered" );
        this._super();
    },

    /*
     * Client is unregistered - no more requests
     */
    onRPCUnregistered: function() {
        Em.Logger.log( "FFW.VR.onRPCUnregistered" );
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
        Em.Logger.log( "FFW.VR.onRPCResult" );
        this._super();
    },

    /*
     * handle RPC erros here
     */
    onRPCError: function( error ) {
        Em.Logger.log( "FFW.VR.onRPCError" );
        this._super();
    },

    /*
     * handle RPC notifications here
     */
    onRPCNotification: function( notification ) {
        Em.Logger.log( "FFW.VR.onRPCNotification" );
        this._super();
    },

    /*
     * send notification when command was triggered
     */
    onCommand: function( element ) {
        Em.Logger.log( "FFW.VR.onCommand" );

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnCommand",
            "params": {
                "commandId": element.commandId
            }
        };
        this.client.send( JSONMessage );

        SDL.VRPopUp.set( 'VRActive', false );
    },

    /*
     * handle RPC requests here
     */
    onRPCRequest: function( request ) {
        Em.Logger.log( "FFW.VR.onRPCRequest" );
        this._super();

        switch( request.method ){
            case "VR.AddCommand": {

                SDL.SDLModel.addCommandVR( request.params );

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "resultCode": "SUCCESS", // type (enum) from SDL
                                                    // protocol
                        "method": request.method + "Response"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "VR.DeleteCommand": {

                SDL.SDLModel.deleteCommandVR( request.params.cmdId );

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "resultCode": "SUCCESS", // type (enum) from SDL
                                                    // protocol
                        "method": request.method + "Response"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "VR.GetSupportedLanguages": {

                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "resultCode": "SUCCESS", // type (enum) from SDL
                                                    // protocol
                        "method": "VR.GetSupportedLanguagesResponse",
                        "languages": SDL.SDLModel.sdlLanguagesList
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "VR.GetLanguage": {

                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "resultCode": "SUCCESS", // type (enum) from SDL
                                                    // protocol
                        "method": "VR.GetLanguageResponse",
                        "language": SDL.SDLModel.hmiTTSVRLanguage
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "VR.GetCapabilities": {

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "capabilities":
                            [
                                "TEXT"
                            ],
                        "method": "VR.GetCapabilitiesResponse",
                        "resultCode": "SUCCESS" // type (enum) from SDL protocol
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "VR.ChangeRegistration": {

                SDL.SDLModel.changeRegistrationTTSVR( request.params.language );

                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "resultCode": "SUCCESS", // type (enum) from SDL
                                                    // protocol
                        "method": request.method + "Response"
                    }
                };
                this.client.send( JSONMessage );

                break;
            }
            case "VR.IsReady": {
                
                // send repsonse
                var JSONMessage = {
                    "jsonrpc": "2.0",
                    "id": request.id,
                    "result": {
                        "method" : "VR.IsReady",
                        "available": this.get('isReady')
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
     * send notification when command was triggered from VR
     */
    onChoise: function( commandId ) {
        Em.Logger.log( "FFW.VR.PerformInteractionResponse" );

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnChoise",
            "params": {
                "choiceID": commandId
            }
        };

        this.client.send( JSONMessage );
    },

    /*
     * send notification when command was triggered
     */
    onCommand: function( commandId, appId ) {
        Em.Logger.log( "FFW.VR.onCommand" );

        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnCommand",
            "params": {
                "cmdID": commandId,
                "appId": appId
            }
        };
        this.client.send( JSONMessage );
    },

    /*
     * Notifies if sdl VR components language was changed
     */
    OnLanguageChange: function( lang ) {
        Em.Logger.log( "FFW.VR.OnLanguageChange" );

        // send repsonse
        var JSONMessage = {
            "jsonrpc": "2.0",
            "method": "VR.OnLanguageChange",
            "params": {
                "language": lang
            }
        };
        this.client.send( JSONMessage );
    }
} )