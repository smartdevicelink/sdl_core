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
            'SDL.SDLModel.VRActive:active',
            'SDL.SDLModel.VRHelpListActivated:move'
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

    VRActiveBinding: 'SDL.SDLModel.VRActive',

    popUp: Em.View.extend( {

        elementId: 'popUp',

        classNames: 'popUp'
    } ),

    AddCommand: function(cmdID, vrCommands, appID, type, grammarID) {

        if (type == "Application") {
            for( var i = 0; i < vrCommands.length; i++ ){
                this.get( 'listOfCommands.list.childViews' ).pushObject( SDL.Button.create( {
                    action: 'onActivateSDLApp',
                    target: 'SDL.SDLController',
                    appID: appID,
                    text: vrCommands[i],
                    classNames: 'list-item',
                    templateName: 'text'
                } ) );
            }
        } else {
            for( var j = 0; j < vrCommands.length; j++ ){

                this.get( 'listOfCommands.list.childViews' ).pushObject( SDL.Button.create( {
                    action: type == 'Command' ? 'onVRCommand' : 'VRPerformAction',
                    target: 'SDL.SDLAppController',
                    appID: appID,
                    grammarID: grammarID,
                    commandID: cmdID,
                    text: vrCommands[j],
                    type: type,
                    hideButtons: function() {
                        if (this.type == "Command" && SDL.SDLModel.performInteractionSession.length == 0) {
                            return false;
                        } else if (SDL.SDLModel.performInteractionSession && SDL.SDLModel.performInteractionSession.indexOf(this.grammarID) >= 0) {
                            return false;
                        } else {
                            return true;
                        }
                    }.property('SDL.SDLModel.performInteractionSession'),
                    classNameBindings: ['this.hideButtons:hide'],
                    classNames: 'list-item',
                    templateName: 'text'
                } ) );
            }
        }
    },

    updateVR: function(){

        this.listOfCommands.list.removeAllChildren();
        this.listOfCommands.list.refresh();

        var len = SDL.SDLModel.VRCommands.length;
        for (var i = 0; i < len; i++) {
            this.AddCommand(SDL.SDLModel.VRCommands[i].cmdID, SDL.SDLModel.VRCommands[i].vrCommands, SDL.SDLModel.VRCommands[i].appID, SDL.SDLModel.VRCommands[i].type);
        }

        if (SDL.SDLAppController.model) {

            len = SDL.SDLAppController.model.VRCommands.length;
            for (var i = 0; i < len; i++) {
                this.AddCommand(SDL.SDLAppController.model.VRCommands[i].cmdID,
                    SDL.SDLAppController.model.VRCommands[i].vrCommands,
                    SDL.SDLAppController.model.VRCommands[i].appID,
                    SDL.SDLAppController.model.VRCommands[i].type,
                    SDL.SDLAppController.model.VRCommands[i].grammarID);
            }
        }

        if (SDL.SDLModel.stateLimited) {
            len = SDL.SDLController.getApplicationModel(SDL.SDLModel.stateLimited).VRCommands.length;
            for (var i = 0; i < len; i++) {
                this.AddCommand(SDL.SDLController.getApplicationModel(SDL.SDLModel.stateLimited).VRCommands[i].cmdID,
                    SDL.SDLController.getApplicationModel(SDL.SDLModel.stateLimited).VRCommands[i].vrCommands,
                    SDL.SDLController.getApplicationModel(SDL.SDLModel.stateLimited).VRCommands[i].appID,
                    SDL.SDLController.getApplicationModel(SDL.SDLModel.stateLimited).VRCommands[i].type,
                    SDL.SDLController.getApplicationModel(SDL.SDLModel.stateLimited).VRCommands[i].grammarID);
            }
        }

    }.observes('SDL.SDLAppController.model'),

    DeleteCommand: function(commandID, appID) {

        if (commandID != 0) {
            var t = this.get('listOfCommands.list.childViews').filterProperty('commandID', commandID);
            for (var i = 0; i < t.length; i++) {
                t[i].remove();
                t[i].destroy();
            }
        } else {
            var t = this.get('listOfCommands.list.childViews').filterProperty('appID', appID);
            for (var i = 0; i < t.length; i++) {
                t[i].remove();
                t[i].destroy();
            }
        }
    },

    DeleteActivateApp: function(appID) {

        var t = this.get('listOfCommands.list.childViews').filterProperty('appID', appID);
        for (var i = 0; i < t.length; i++) {
            t[i].remove();
            t[i].destroy();
        }
    },

    /**
     * List for option on SDLOptionsView screen
     */
    listOfCommands: SDL.List.extend( {

        elementId: 'VR_list',

        itemsOnPage: 5,

        /** Items array */
        items: [
            {
                type: SDL.Button,
                params: {
                    //templateName: template,
                    text: 'Help',
                    target: 'SDL.SDLController',
                    action: 'vrHelpAction',
                    onDown: false
                }
            }
        ]
    } ),

    // deactivate VR on change application state
    onStateChange: function() {
        if (this.VRActive) {
    		FFW.VR.Started();
    		this.set( 'VRActive', false );
    	} else {
    		FFW.VR.Stopped();
    	}
    }.observes('SDL.TransitionIterator.ready'),

    onActivate: function() {
        SDL.SDLController.VRMove();
    	if (this.VRActive) {

    		FFW.VR.Started();
            SDL.SDLController.onSystemContextChange();
    	} else {

    		FFW.VR.Stopped();
            SDL.SDLController.onSystemContextChange();
    	}
    }.observes('this.VRActive'),
    
    /**
     * This event triggered when component is placed to
     * document DOM structure
     */
    didInsertElement: function() {
        this._super();
    }
} );