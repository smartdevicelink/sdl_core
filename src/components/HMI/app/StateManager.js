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
 * @name SDL.States
 * @desc State manager definition
 * @category State Manager
 * @filesource app/StateManager.js
 * @version 1.0
 */

// Extend base Em.State object
Em.State.reopen( {

    // used for determine display status
    active: false,

    enter: function() {
        this.set( 'active', true );
    },

    exit: function() {
        if( SDL.States ){
            if( this.name == "sdlmedia" || this.name == "nonMedia" ){
                SDL.SDLAppController.deactivateApp();
            }
        }
        this.set( 'active', false );
    }
} );

// State Manager class
var StateManager = Em.StateManager.extend( {

    // default state
    initialState: 'home',
    
    /**
     *  Name of the next state to which at the moment is a transition
     */
    nextState: 'home',
    
    /**
     * Method used for determine previous currentState and
     * make 
     */
    goToStates: function( stateName ){
        this.set( 'nextState', stateName );
        this.transitionTo( stateName );
    },

    /** Go to parent state */
    back: function() {
        if( this.currentState.parentState.hasOwnProperty( 'name' ) ){
            this.goToStates( this.currentState.parentState.get( 'path' ) );
        }else{
            this.goToStates( 'home' );
        }

        SDL.StateVisitor.visit( this.currentState );
    },

    /** Home state */
    home: Em.State.create( {

    } ),

    /** Climate state */
    climate: Em.State.create( {

    } ),

    /** info state */
    info: Em.State.create( {

        exit: function() {
            SDL.InfoController.set( 'activeState', SDL.States.currentState.get( 'path' ) );
            this._super();
        },

        services: Em.State.create( {

        } ),

        travelLink: Em.State.create( {

        } ),

        alerts: Em.State.create( {

        } ),

        calendar: Em.State.create( {

        } ),

        apps: Em.State.create( {

            /**
             * Calls function from BasicCommunicationRPC to get new list of applications 
             */
            enter: function() {
                this._super();
                FFW.BasicCommunication.getAppList();
            }
        } ),

        devicelist: Em.State.create( {

        } ),

        nonMedia: Em.State.create( {
            enter: function() {
                this._super();

                SDL.NonMediaController.restoreCurrentApp();
            }
        } )
    } ),

    /** settings state */
    settings: Em.State.create( {

    } ),

    /** Media state */
    media: Em.State.create( {

        exit: function() {
            SDL.MediaController.set( 'activeState', SDL.States.currentState.get( 'path' ) );
            this._super();
        },

        player: Em.State.create( {} ),

        sdlmedia: Em.State.create( {

            enter: function() {
                this._super();

                SDL.SDLMediaController.restoreCurrentApp();
            }

        } )
    } ),

    /** Navigation state */
    navigation: Em.State.create( {

    } ),

    /** Phone state */
    phone: Em.State.create( {
/*
        enter: function() {
            SDL.PhoneController.set( 'hideMenu', false );

            this._super();
        },

        exit: function() {
            this._super();

            var path = SDL.States.currentState.get( 'path' );

            if( path.split( '.' ).length > 2 ){
                SDL.PhoneController.set( 'activeState', path.substring( 0, path.indexOf( '.', 6 ) ) );
            }else{
                SDL.PhoneController.set( 'activeState', path );
            }
        },*/

        dialpad: Em.State.create( {

        } )
    } )
} );
