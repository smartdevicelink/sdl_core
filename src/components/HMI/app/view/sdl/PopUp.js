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
 * @name SDL.PopUp
 * @desc PopUp module visual representation
 * @category View
 * @filesource app/view/sdl/PopUp.js
 * @version 1.0
 */

SDL.PopUp = Em.ContainerView.extend({

    classNames: 'PopUp',

    childViews:
    [
        'message',
        'buttonOk',
        'buttonCancel',
        'backButton'
    ],

    classNameBindings: [
        'this.active:active_state:inactive_state'
    ],

    popUpId: 0,

    /**
     * Callback function to return result of made action by user
     */
    callback: null,

    content: 'Title',

    active: false,

    timer: null,

    backButton: SDL.Button.extend( {
        classNames: 'button backButton',
        text: 'X',
        click: function () {
            this._parentView.deactivate();
        },
        buttonAction: true,
        onDown: false,
        disabledBinding: 'parentView.buttons'
    }),

    buttons: true,

    buttonOk: SDL.Button.extend( {
        classNames: 'button vehicleHealthReport OkButton',
        text: 'Ok',
        action: 'deactivate',
        target: 'parentView',
        buttonAction: true,
        onDown: false,
        disabledBinding: 'parentView.buttons'
    }),

    buttonCancel: SDL.Button.extend( {
        classNames: 'button vehicleHealthReport CancelButton',
        text: 'Cancel',
        action: 'deactivate',
        target: 'parentView',
        buttonAction: false,
        onDown: false,
        disabledBinding: 'parentView.buttons'
    }),

    message: SDL.Label.extend({

        elementId: 'text',

        classNames: 'text',

        contentBinding: 'parentView.content'
    }),

    /**
     * Deactivate PopUp
     */
    deactivate: function(event) {
        this.set('active', false);

        if (this.callback && event) {
            this.callback(event.buttonAction);
        }

        this.set('callback', null);
        this.remove();
    },

    popupActivate: function(message, callback) {
        this.set('active', true);

        clearTimeout(this.timer);
        this.timer = null;

        if (callback) {
            this.set('callback', callback);
            this.set('buttons', false);
        } else {
            this.set('buttons', true);
            var self = this;
            this.timer = setTimeout(function(){
                self.deactivate();
            },
            5000);
        }

        this.set('content', message);

        return ++this.popUpId;
    }
});