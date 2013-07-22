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
 * @name SDL.SDLAppModel
 * @desc Abstarct model for SDL applications
 * @category Model
 * @filesource app/model/sdl/AppModel.js
 * @version 1.0
 */

SDL.SDLAppModel = Em.Object.extend( {

    /**
     * Application Id
     * 
     * @type {Number}
     */
    appID: null,

    /**
     * Application name
     * 
     * @type {String}
     */
    appName: '',

    /**
     * Chosen device name
     * 
     * @type {String}
     */
    deviceName: '',

    /**
     * Statusbar text
     * 
     * @type {String}
     */
    statusText: '',

    /**
     * Info data
     * 
     * @type: {Em.Object}
     */
    appInfo: null,

    /**
     * Current language of applications UI component
     * 
     * @type {String}
     */
    UILanguage: 'EN-US',

    /**
     * Current language of applications TTS and applications VR component
     * 
     * @type {String}
     */
    TTSVRLanguage: 'EN-US',

    /**
     * Array of Soft Buttons
     * 
     * @type {Array}
     */
    softButtons: [],

    /**
     * Array of Objects for TBTTurnList
     * 
     * @type {Array}
     */
    turnList: [],

    /**
     * URL to application Icon
     * 
     * @type {String}
     */
    appIcon: 'images/info/info_leftMenu_apps_ico.png',

    /**
     * Application commands list
     * 
     * @type {Array}
     */
    commandsList: [],

    /**
     * Return current menu commands
     * 
     * @return {Array}
     */
    currentCommandsList: function() {
        return this.get( 'commandsList' ).filterProperty( 'parent', this.get( 'currentSubMenuId' ) );
    }.property( 'this.commandsList.@each', 'this.currentSubMenuId' ),

    /**
     * Current command submenu identificator
     * 
     * @type {Number}
     */
    currentSubMenuId: 0,

    /**
     * Return current submenu name
     * 
     * @return {String}
     */
    currentSubMenuLabel: function() {
        var submenu = this.get( 'commandsList' ).filterProperty( 'menuID', this.get( 'currentSubMenuId' ) );
        return submenu.length ? submenu[0].name : 'Options';
    }.property( 'this.currentSubMenuId' ),

    /**
     * Interaction chooses data
     * 
     * @type {Object}
     */
    interactionChoices: {},

    /**
     * Update Soft Buttons will handle on command Show
     * 
     * @param {Array}
     */
    updateSoftButtons: function( buttons ) {
        // delete existing buttons from array
        this.softButtons.splice( 0 );

        // push new buttons to array
        this.get( 'softButtons' ).pushObjects( buttons );
    },

    /**
     * Add command to list
     * 
     * @param {Object}
     */
    addCommand: function( params ) {

        if (params.menuParams) {
            this.get( 'commandsList' ).pushObject( {
                commandID: params.cmdID,
                name: params.menuParams.menuName ? params.menuParams.menuName : "",
                parent: params.menuParams.parentID ? params.menuParams.parentID : 0,
                position: params.menuParams.position ? params.menuParams.position : 0,
                icon: params.cmdIcon ? params.cmdIcon.value : null
            } );
        } else {
            this.get( 'commandsList' ).pushObject( {
                commandID: params.cmdID,
                name: '',
                parent: 0,
                position: 0,
                icon: params.cmdIcon ? params.cmdIcon.value : null
            } );
        }
    },

    /**
     * Delete command from list
     * 
     * @param {Number}
     */
    deleteCommand: function( commandID ) {

        this.get( 'commandsList' ).removeObjects( this.get( 'commandsList' ).filterProperty( 'commandID', commandID ) );
    },

    /**
     * Add submenu to commands list
     * 
     * @param {Object}
     */
    addSubMenu: function( params ) {

        this.get( 'commandsList' ).pushObject( {
            menuID: params.menuID,
            name: params.menuParams.menuName ? params.menuParams.menuName : '',
            parent: 0,
            position: params.menuParams.position ? params.menuParams.position : 0
        } );
    },

    /**
     * Delete submenu and related commands from list
     * 
     * @param {Number}
     */
    deleteSubMenu: function( menuID ) {

        // don't delete if current submenu active
        if( this.get( 'currentSubMenuId' ) == menuID ){
            return 'IN_USE';
            // SDL.SDLAppController.onSubMenu(0);
        }

        // remove submenu
        this.get( 'commandsList' ).removeObjects( this.get( 'commandsList' ).filterProperty( 'menuID', menuID ) );

        // remove commands from deleted submenu
        this.get( 'commandsList' ).removeObjects( this.get( 'commandsList' ).filterProperty( 'parent', menuID ) );

        return SDL.SDLModel.resultCode['SUCCESS'];
    },

    /**
     * SDL UI PreformInteraction response handeler open Perform Interaction
     * screen and show choices
     * 
     * @param {Object} message
     * @param {Number} performInteractionRequestId
     */
    onPreformInteraction: function( message, performInteractionRequestId ) {

        SDL.InteractionChoicesView.clean();
        
        SDL.InteractionChoicesView.preformChoices( message.choiceSet, performInteractionRequestId, message.timeout );

        SDL.InteractionChoicesView.activate( message.initialText.fieldText );
    },

    /**
     * SDL UI CreateInteraction response handeler push set of commands to voice
     * recognition list
     * 
     * @param {Object}
     */
    onCreateInteraction: function( message ) {

        this.interactionChoices[message.interactionChoiceSetID] = message.choiceSet;

    },

    /**
     * SDL UI DeleteInteraction response handeler close current interaction set
     * window (if opened) and delete current set commands from voice recognition
     * list
     * 
     * @param {Object}
     */
    onDeleteInteraction: function( message ) {
        delete this.interactionChoices[message.interactionChoiceSetID];
    },

    /**
     * SDL UI Slider response handeler open Slider window with received
     * parameters
     * 
     * @param {Object}
     */
    onSlider: function( message ) {

        SDL.SliderView.loadData( message );

        SDL.SliderView.activate( this.appName );

        setTimeout( function() {
            if( SDL.SliderView.active ){
                SDL.SliderView.deactivate();
            }
        }, message.params.timeout );
    }
} );
