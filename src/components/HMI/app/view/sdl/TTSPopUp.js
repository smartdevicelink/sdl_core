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
 * @name SDL.TTSPopUp
 * @desc TTSPopUp module visual representation
 * @category View
 * @filesource app/view/sdl/TTSPopUp.js
 * @version 1.0
 */

SDL.TTSPopUp = Em.ContainerView.create( {

    elementId: 'TTSPopUp',

    classNames: 'TTSPopUp',

    classNameBindings: [
        'active'
    ],

    childViews: [
        'popUp',
        'message',
        'okButton',
        'timerText',
        'checkBoxLabel',
        'checkBox'
    ],

    requestId: null,

    content: 'Messaage',

    active: false,

    timer: null,

    appID: null,

    timerSeconds: 5,

    popUp: Ember.TextArea.extend( {

        elementId: 'popUp',

        classNames: 'popUp',

        valueBinding: 'parentView.content'
    }),

    message: SDL.Label.extend( {

        elementId: 'message',

        classNames: 'message'
    }),

    okButton: SDL.Button.extend( {
        classNames: 'button okButton',
        text: 'Reset Timeout',
        action: 'resetTimeout',
        target: 'parentView',
        buttonAction: true,
        onDown: false,
        disabledBinding: 'parentView.buttons'
    }),

    checkBoxLabel: SDL.Label.extend({

        elementId: 'checkBoxLabel',

        classNames: 'checkBoxLabel',

        content: 'Send response'
    }),

    checkBox: Em.Checkbox.extend( {

        elementId: 'checkBoxTTS',

        classNames: 'checkBoxTTS',

        checked: true

    }),

    timerText: SDL.Label.extend({

        elementId: 'timerText',

        classNames: 'timerText',

        contentBinding: 'parentView.timerSeconds'
    }),



    resetTimeout: function () {
        this.set('timerSeconds', 10);
        FFW.TTS.OnResetTimeout(this.appID, "TTS.Speak");
    },

    ActivateTTS: function(msg, appID) {

        var self = this;

        this.set('appID', appID);
        this.set('content', msg);
        this.set('active', true);

        clearInterval(this.timer);
        this.timer = setInterval(function() {

            self.set('timerSeconds', self.timerSeconds - 1);
        }, 1000); // timeout for TTS popUp timer interval in milliseconds
        FFW.TTS.Started();
    },

    timerHandler: function () {
        if (this.timerSeconds === 0) {
            this.DeactivateTTS();
        }
    }.observes('this.timerSeconds'),

    DeactivateTTS: function() {
        clearInterval(this.timer);
        this.set('active', false);
        this.appID = null;
        this.set('timerSeconds', 5);
        this.checkBox.set('checked', true);

        if (this.checkBox.checked) {
            SDL.SDLController.TTSResponseHandler();
            FFW.TTS.Stopped();
        }
    },

    /**
     * This event triggered when component is placed to document DOM structure
     */
    didInsertElement: function() {

        this._super();
    }
});
