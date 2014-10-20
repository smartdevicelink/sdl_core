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
 * @name SDL.AudioPassThruPopUp
 * @desc AudioPassThruPopUp module visual representation
 * @category View
 * @filesource app/view/sdl/AudioPassThruPopUp.js
 * @version 1.0
 */

SDL.AudioPassThruPopUp = Em.ContainerView.create( {

    elementId: 'AudioPassThruPopUp',

    classNames: 'AudioPassThruPopUp',

    classNameBindings:
        [
            'activate:AudioPassThruActive'
        ],

    childViews:
        [
            'applicationName',
            'image',
            'message1',
            'message2',
            'message3',
            'buttonRetry',
            'buttonDone',
            'buttonCancel'
        ],

    content1: 'Title',

    content2: 'Text',

    activateBinding: 'SDL.SDLModel.AudioPassThruState',

    timer: null,

    applicationName: SDL.Label.extend( {

        elementId: 'AudioPassThruPopUpApplicationName',

        classNames: 'applicationName',

        contentBinding: 'parentView.appName'
    } ),

    /**
     * Wagning image on Alert PopUp
     */
    image: Em.View.extend( {
        elementId: 'audioPassThruImage',

        classNames: 'audioPassThruImage'
    } ),

    message1: SDL.Label.extend( {

        elementId: 'AudioPassThruPopUpMessage1',

        classNames: 'message1',

        contentBinding: 'parentView.content1'
    } ),

    message2: SDL.Label.extend( {

        elementId: 'AudioPassThruPopUpMessage2',

        classNames: 'message2',

        contentBinding: 'parentView.content2'
    } ),

    message3: SDL.Label.extend( {

        elementId: 'AudioPassThruPopUpMessage3',

        classNames: 'message3',

        contentBinding: 'parentView.content3'
    } ),

    buttonRetry: SDL.Button.create( {
        elementId: 'AudioPassThruPopUpButtonRetry',
        classNames: 'buttonRetry softButton',
        text: 'Retry',
        responseResult: SDL.SDLModel.resultCode['RETRY'],
        actionUp: function() {
            SDL.SDLController.callPerformAudioPassThruPopUpErrorResponse( this );
        }
    } ),

    buttonDone: SDL.Button.create( {
        elementId: 'AudioPassThruPopUpButtonDone',
        classNames: 'buttonDone softButton',
        text: 'Done',
        responseResult: SDL.SDLModel.resultCode['SUCCESS'],
        actionUp: function() {
            SDL.SDLController.callPerformAudioPassThruPopUpResponse( this );
        }
    } ),

    buttonCancel: SDL.Button.create( {
        elementId: 'AudioPassThruPopUpButtonCancel',
        classNames: 'buttonCancel softButton',
        text: 'Cancel',
        responseResult: SDL.SDLModel.resultCode['ABORTED'],
        actionUp: function() {
            SDL.SDLController.callPerformAudioPassThruPopUpErrorResponse( this );
        }
    } ),

    /**
     * Method to start AudioPassThru timer to deactivate popUp and send response to SDL
     */
    StartAudioPassThruTimer: function() {
        clearTimeout( this.timer );
        this.timer = setTimeout( function() {
            SDL.SDLController.performAudioPassThruResponse( SDL.SDLModel.resultCode["SUCCESS"] );
        }, SDL.SDLModel.AudioPassThruData.maxDuration );
    },

    /**
     * Method clears PopUp's timer when activity flag become false, and show
     * PopUp with data come from SDLCorel when activity flag become true
     */
    AudioPassThruActivity: function() {

        if( this.activate ){

            var self = this, data = SDL.SDLModel.AudioPassThruData;

            this.set( 'appName', SDL.SDLController.getApplicationModel( data.appID ).appName );

            for (var i = 0; i < data.audioPassThruDisplayTexts.length; i++) {
                switch (data.audioPassThruDisplayTexts[i].fieldName) {
                    case 'audioPassThruDisplayText1': {
                        this.set('content1', data.audioPassThruDisplayTexts[i].fieldText);
                        break;
                    }
                    case 'audioPassThruDisplayText2': {
                        this.set('content2', data.audioPassThruDisplayTexts[i].fieldText);
                        break;
                    }
                }
            }
        }else{
            if( this.timer ){
                clearTimeout( this.timer );
                this.timer = null;
            }

            SDL.SDLController.onSystemContextChange();
        }
    }.observes( 'activate' )
} );