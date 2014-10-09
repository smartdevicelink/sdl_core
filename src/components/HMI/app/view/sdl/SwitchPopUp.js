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
 * @name SDL.AlertPopUp
 * @desc AlertPopUp module visual representation
 * @category View
 * @filesource app/view/sdl/AlertPopUp.js
 * @version 1.0
 */

SDL.SwitchPopUp = Em.ContainerView.create({

    elementId: 'SwitchPopUp',

    classNames: 'SwitchPopUp',

    classNameBindings:
        [
            'active:active'
        ],

    childViews:
        [
//            'applicationName',
            //'image',
            'message1',
            'message2',
//            'message3',
            'buttonOk',
            'buttonCancel'
        ],

    /**
     * Id of current request
     * 
     * @type {Number}
     */
    alertRequestId: null,

    content1: 'Mobile Device is requesting access to take control of the onboard HD Radio system.',

    content2: 'Do you wish to grant this request?',

    active: false,

    timer: null,

    /**
     * Wagning image on Alert PopUp
     */
    image: Em.View.extend({
        elementId: 'alertPopUpImage',

        classNames: 'alertPopUpImage'
    }),

    applicationName: SDL.Label.extend({

        elementId: 'applicationName',

        classNames: 'applicationName',

        contentBinding: 'parentView.appName'
    }),

    message1: SDL.Label.extend({

        elementId: 'message1',

        classNames: 'message1',

        contentBinding: 'parentView.content1'
    }),

    message2: SDL.Label.extend({

        elementId: 'message2',

        classNames: 'message2',

        contentBinding: 'parentView.content2'
    }),

    message3: SDL.Label.extend({

        elementId: 'message3',

        classNames: 'message3',

        contentBinding: 'parentView.content3'
    }),

    /**
     * Deactivate PopUp
     */
    deactivate: function() {
        this.set('active', false);

        SDL.SDLController.onSystemContextChange();
    },

    /**
     * Button Ok
     */
    buttonOk: SDL.Button.extend({
        classNames: 'buttonOk softButton',
        actionUp: function () {
            SDL.SDLController.SwitchPopUpAction(true);
            return false;
        },
        value: true,
        templateName: 'text',
        text: 'Grant'
    }),

    /**
     * Button Cancel
     */
    buttonCancel: SDL.Button.extend({
        classNames: 'buttonCancel softButton',
        actionUp: function () {
            SDL.SDLController.SwitchPopUpAction(false);
        },
        templateName: 'text',
        text: 'Deny'
    }),

    activate: function(params) {

        this.set('appName', SDL.SDLController.getApplicationModel(params.appID).appName);
        if (this.appName) {
            this.set('content1', 'Mobile Device "'+ this.appName +'" is requesting access to take control of the onboard HD Radio system.');
        }
        
        this.set('active', true);
        SDL.SDLController.onSystemContextChange();
    }
});