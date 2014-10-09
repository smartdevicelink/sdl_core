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
 * @name SDL.MediaController
 * @desc Media module logic
 * @category Controller
 * @filesource app/controller/MediaController.js
 * @version 1.0
 */
SDL.MediaController = Em.Object.create({

    /**
     * Initial substate
     */
    activeState: 'media.player.radio',

    /** Current selected players module Data reference*/
    currentPlayerModuleData: SDL.CDModel,

    /** Current selected player object  reference*/
    currentSelectedPlayer: null,

    /**
     * Turn on CD
     */
    turnOnCD: function () {
        //this.onPlayerEnter(SDL.CDModel, 'cd');
	
        if (!SDL.States.media.player.cd.active) {
            SDL.States.goToStates('media.player.cd');
        }

        SDL.CDModel.set('active', true);

        this.onPlayerEnter(SDL.CDModel, 'cd');
    },

    /**
     * Turn on USB
     */
    turnOnUSB: function() {

        if (!SDL.States.media.player.usb.active) {
            SDL.States.goToStates('media.player.usb');
        }

        this.onPlayerEnter(SDL.USBModel, 'usb');
    },
    /**
     * Turn on Radio
     */
    turnOnRadio: function() {


        if (!SDL.States.media.player.radio.active) {
            SDL.States.goToStates('media.player.radio');
        }

        SDL.RadioModel.set('active', true);
    },

    /**
     * Switching on Application
     */
    turnOnSDL: function () {

        SDL.CDModel.set('active', false);
        /**
         * Set SDL Data active, flag for status bar
         */
        if (SDL.SDLAppController.model) {
            SDL.SDLAppController.model.set('active', true);
        }
        /**
         * Go to SDL state
         */
        if (SDL.SDLAppController.model.appType) {
            for (var i = 0; i < SDL.SDLAppController.model.appType.length; i++) {
                if (SDL.SDLAppController.model.appType[i] == "NAVIGATION") {
                    SDL.BaseNavigationView.update();
                    SDL.States.goToStates('navigationApp.baseNavigation');
                    return;
                }
            }
        }

        SDL.States.goToStates('media.sdlmedia');

    },

    /**
     * Switching off CD
     */
    deactivateCD: function() {

        SDL.CDModel.set('active', false);
    },

    /**
     * Switching off USB
     */
    deactivateUSB: function() {
        SDL.USBModel.set('active', false);
    },

    /**
     * Switching off Radio
     */
    deactivateRadio: function() {

        SDL.RadioModel.set('active', false);
    },

    /**  On player module enter event */
    onPlayerEnter: function(data, state){
        if(this.currentSelectedPlayer)
            this.currentSelectedPlayer.pause();

        data.set('active', true);

        SDL.States.goToState('media.player.'+state);

        this.set('currentPlayerModuleData',data.PlayList);
        this.set('currentSelectedPlayer',data.player);
    },

    /**
     * Player Prev track event
     */
    prevTrack: function(){
        this.currentSelectedPlayer.prevTrackPress();
    },

    /**
     * Player Play track event
     */
    playTrack: function(){
        this.currentSelectedPlayer.playTrackPress();
    },

    /**
     * Player Next track event
     */
    nextTrack: function(){
        this.currentSelectedPlayer.nextTrackPress();
    },

    /**
     * turn on shuffle help video event
     */
    turnOnShuffle : function(){

    },

    /**
     * turn on scan event
     */
    turnOnScan : function(){

    },

    /**
     * turn on more info event
     */
    turnOnMoreInfo : function(){

    },

    /**
     * turn on options event
     */
    turnOnOptions:function(){

    },

    /**
     * turn on browse event
     */
    turnOnBrowse: function(){

    }
});