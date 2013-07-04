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
 * @name SDL.SDLNonMediaModel
 * @desc Concrate model for SDL NonMedia application
 * @category Model
 * @filesource app/model/sdl/NonMediaModel.js
 * @version 1.0
 */

SDL.SDLNonMediaModel = SDL.SDLAppModel.extend( {

    init: function() {

        this._super();

        // init properties here
        this.set( 'appInfo', Em.Object.create( {
            field1: '<field1>',
            field2: '<field2>',
            field3: '<field3>',
            field4: '<field4>',
            mainImage: 'images/sdl/audio_icon.jpg',
            image: '',
            customPresets: []
        } ) );

        // this.set('appIcon', 'images/info/info_leftMenu_apps_ico.png'),

        this.set( 'commandsList', [] );
        this.set( 'softButtons', [] );
    },

    /**
     * Method hides sdl activation button and sdl application
     * 
     * @param {Number}
     */
    onDeleteApplication: function( appId ) {
        SDL.NonMediaController.onDeleteApplication( appId );
    },

    /**
     * Activate current application model
     */
    turnOnSDL: function() {
        SDL.NonMediaController.activateApp( this );
    },

    /**
     * Method to clear App OverLay
     */
    clearAppOverLay: function() {

        clearInterval( this.timer );
        this.appInfo.set( 'field1', '' );
        this.appInfo.set( 'field2', '' );
        this.appInfo.set( 'field3', '' );
        this.appInfo.set( 'field4', '' );
        this.appInfo.set( 'alignment', '' );
        this.appInfo.set( 'mainImage', 'images/sdl/audio_icon.jpg' );
        this.updateSoftButtons();
        for( i = 0; i < 6; i++ ){
            this.appInfo.set( 'customPresets.' + i, '' );
        }

    },

    /**
     * Applin UI Show handler
     * 
     * @param {Object}
     */
    onSDLUIShow: function( params ) {
        
        for (var i = 0; i < params.alertStrings.length; i++) {
            switch (params.alertStrings[key]) {
                case 'mainField1': {
                    this.appInfo.set('field1', params.alertStrings[key].fieldText);
                    break;
                }
                case 'mainField2': {
                    this.appInfo.set('field2', params.alertStrings[key].fieldText);
                    break;
                }
                case 'mainField3': {
                    this.appInfo.set('field3', params.alertStrings[key].fieldText);
                    break;
                }
                case 'mainField4': {
                    this.appInfo.set('field4', params.alertStrings[key].fieldText);
                    break;
                }
                case 'statusBar': {
                    this.appInfo.set('statusText', params.alertStrings[key].fieldText);
                    break;
                }
                case 'mediaClock': {
                    this.appInfo.set('mediaClock', params.alertStrings[key].fieldText);
                    break;
                }
                case 'mediaTrack': {
                    this.appInfo.set('mediaTrack', params.alertStrings[key].fieldText);
                    break;
                }
            }
        }

        if( params.graphic ){
            this.appInfo.set( 'mainImage', params.graphic );
        }else{
            this.appInfo.set( 'mainImage', 'images/sdl/audio_icon.jpg' );
        }

        if( params.customPresets ){
            this.appInfo.set( 'customPresets', params.customPresets );
        }

        // Get soft buttons if exists
        if( params.softButtons ){
            this.updateSoftButtons( params.softButtons );
        }
    },

    sdlSetMediaClockTimer: function() {
        return;
    }
} );