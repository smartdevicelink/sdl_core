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
 * @name SDL.SDLAppController
 * @desc SDL abstract application controller
 * @category Controller
 * @filesource app/controller/sdl/AppController.js
 * @version 1.0
 */

SDL.SDLAppController = Em.Object.create( {
    
    init: function() {
        this._super();

        FFW.UI.set( 'isReady', true );
        FFW.Navigation.set( 'isReady', true );
    },
    
    /**
     * Active application model binding type {SDLAppModel}
     */
    model: null,

    /**
     * Handeler for command button press
     * 
     * @param element: SDL.Button
     */
    onCommand: function( element ) {

        // if submenu
        if( element.menuID ){

            // activate driver destruction if necessary
            if( SDL.SDLModel.driverDistractionState ){
                SDL.DriverDistraction.activate();
            }else{
                this.onSubMenu( element.menuID );
            }

            return;
        }

        FFW.UI.onCommand( element.commandID, this.model.appID );
    },

    /**
     * Open commands submenu
     * 
     * @param {Number}
     */
    onSubMenu: function( id ) {
        this.model.set( 'currentSubMenuId', id );
    },

    /**
     * Handeler for command button press
     * 
     * @param element: SDL.Button
     */
    onVRCommand: function( element ) {
        FFW.VR.onCommand( element.commandID, element.appID );
    },

    /**
     * Handeler for preform interaction choice send response to device and
     * deactivate interactions window
     * 
     * @param element: SDL.Button
     */
    onChoiceInteraction: function( element ) {

        FFW.UI.interactionResponse( SDL.SDLModel.resultCode["SUCCESS"], element.performInteractionRequestID, element.choiceID );

        SDL.InteractionChoicesView.deactivate( false );
    },

    /**
     * Open commands list
     */
    openCommandsList: function() {
        SDL.OptionsView.activate();
    },

    /**
     * Notification of deactivation of current application model initiated in
     * StateManager
     */
    deactivateApp: function() {

        if( this.model ){
            SDL.SDLModel.onDeactivateApp( SDL.States.nextState, this.model.appID );
        }

    },

    /**
     * Method clears all applications data and unregister models
     */
    onSDLDisconected: function() {
        var i = 0, apps = SDL.SDLModel.registeredApps;

        for( i = 0; i < apps.length; i++ ){
            SDL.SDLModel.onAppUnregistered( {
                "appID": apps[i].appID
            } );
        }
    }
} );
