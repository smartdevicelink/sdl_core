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
 * @name SDL.VRPopUp
 * @desc VRPopUp module visual representation
 * @category View
 * @filesource app/view/sdl/VRPopUp.js
 * @version 1.0
 */

SDL.VRPopUp = Em.ContainerView.create( {

    elementId: 'VRPopUp',

    classNames: 'VRPopUp',

    classNameBindings:
        [
            'VRActive:active'
        ],

    childViews:
        [
            'popUp',
            'VRLabel',
            'VRImage',
            'listOfCommands'
        ],

    VRImage: Em.View.extend( {

        elementId: 'VRImage',

        classNames: 'VRImage'
    } ),

    VRLabel: SDL.Label.extend( {

        elementId: 'VRLabel',

        classNames: 'VRLabel',

        content: 'Speak the command'
    } ),

    VRActive: false,

    popUp: Em.View.extend( {

        elementId: 'popUp',

        classNames: 'popUp'
    } ),

    AddActivateApp: function( appId, appName ) {

        this.get( 'listOfCommands.list.childViews' ).pushObject( SDL.Button.create( {
            action: 'onActivateSDLApp',
            target: 'SDL.SDLController',
            text: appName,
            appName: appName,
            appId: appId,
            classNames: 'list-item',
            templateName: 'text'
        } ) );

    },

    DeleteActivateApp: function( appId ) {

        this.get( 'listOfCommands.list.childViews' ).removeObjects( this.get( 'listOfCommands.list.childViews' ).filterProperty( 'activeAppId', appId ) );

    },

    AddCommand: function( commandId, vrCommands, appId ) {

        for( var j = 0; j < vrCommands.length; j++ ){
            this.get( 'listOfCommands.list.childViews' ).pushObject( SDL.Button.create( {
                action: 'onVRCommand',
                target: 'SDL.SDLAppController',
                appId: appId,
                commandId: commandId,
                text: vrCommands[j],
                classNames: 'list-item',
                templateName: 'text'
            } ) );
        }

    },

    DeleteCommand: function( commandId ) {

        this.get( 'listOfCommands.list.childViews' ).removeObjects( this.get( 'listOfCommands.list.childViews' ).filterProperty( 'commandId', commandId ) );

    },

    CreateInteractionChoise: function( params, performInteractionRequestId ) {

        if( !params ){
            return;
        }

        for( var i = 0; i < params.length; i++ ){
            for( var j = 0; j < params[i].vrCommands.length; j++ ){
                this.get( 'listOfCommands.list.childViews' ).pushObject( SDL.Button.create( {
                    action: 'onChoiceInteraction',
                    target: 'SDL.SDLAppController',
                    choiceId: params[i].choiceID,
                    btnType: 'interactionChoice',
                    text: params[i].vrCommands[j],
                    performInteractionRequestId: performInteractionRequestId,
                    classNames: 'list-item',
                    templateName: 'text'
                } ) );
            }
        }
    },

    DeleteInteractionChoise: function() {

        if( !SDL.InteractionChoicesView.active ){
            this.get( 'listOfCommands.list.childViews' ).removeObjects( this.get( 'listOfCommands.list.childViews' ).filterProperty( 'btnType', 'interactionChoice' ) );
        }

    }.observes( 'SDL.InteractionChoicesView.active' ),

    activateVRPopUp: function() {
        var self = this;

        if( this.VRActive ){
            this.set( 'VRActive', false );
        }else{
            // play audio alert
            SDL.Audio.play( 'audio/say.wav' );

            this.set( 'VRActive', true );
        }

        SDL.SDLController.onSystemContextChange();
    },

    /**
     * List for option on SDLOptionsView screen
     */
    listOfCommands: SDL.List.extend( {

        elementId: 'VR_list',

        itemsOnPage: 5,

        /** Items array */
        items: new Array()
    } ),

    // deactivate VR on change application state
    onStateChange: function() {
        if( this.VRActive ){
            this.set( 'VRActive', false );
        }
    }.observes( 'SDL.TransitionIterator.ready' )
} );