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

SDL.AlertPopUp = Em.ContainerView.create({

    elementId: 'AlertPopUp',

    classNames: 'AlertPopUp',

    classNameBindings:
        [
            'active:AlertActive'
        ],

    childViews:
        [
            'applicationName',
            'image',
            'message1',
            'message2',
            'message3',
            'softbuttons',
            'progressIndicatorView'
        ],

    /**
     * Id of current request
     * 
     * @type {Number}
     */
    alertRequestId: null,

    content1: '',

    content2: '',

    content3: '',

    active: false,

    timer: null,

    timeout: null,

    progressIndicator: false,

    /**
     * Wagning image on Alert PopUp
     */
    image: Em.View.extend({
        elementId: 'alertPopUpImage',

        classNames: 'alertPopUpImage'
    }),

    /**
     * Wagning image on Alert PopUp
     */
    progressIndicatorView: Em.View.extend({
        elementId: 'progressIndicator',

        classNameBindings: 'this.parentView.progressIndicator:progressIndicator'
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
    deactivate: function(reason) {
        this.set('active', false);
        clearTimeout(this.timer);

        this.set('content1', '');
        this.set('content2', '');
        this.set('content3', '');

        if (reason == 'timeout' && this.softbuttons.buttons._childViews.length > 0 && reason === 'ABORTED') {
            SDL.SDLController.alertResponse(SDL.SDLModel.resultCode['ABORTED'], this.alertRequestId);
        } else {
            SDL.SDLController.alertResponse(SDL.SDLModel.resultCode['SUCCESS'], this.alertRequestId);
        }

        SDL.SDLController.onSystemContextChange();
    },

    /**
     * Container for softbuttons
     */
    softbuttons: Em.ContainerView.extend({

        childViews:
            [
                'buttons'
            ],

        buttons: Em.ContainerView.extend({
            elementId: 'alertSoftButtons',

            classNames: 'alertSoftButtons'
        })
    }),

    /**
     * @desc Function creates Soft Buttons on AlertPoUp
     * @param {Object} params
     */
    addSoftButtons: function(params, appID) {

        this.softbuttons.buttons.removeAllChildren();
        this.softbuttons.buttons.rerender();

        if(params){

            var softButtonsClass;
            switch(params.length){
                case 1:
                    softButtonsClass = 'one';
                    break;
                case 2:
                    softButtonsClass = 'two';
                    break;
                case 3:
                    softButtonsClass = 'three';
                    break;
                case 4:
                    softButtonsClass = 'four';
                    break;
            }

            for(var i = 0; i < params.length; i++){
                this.get('softbuttons.buttons.childViews')
                    .pushObject(SDL.Button.create(SDL.PresetEventsCustom, {
                    systemAction: params[i].systemAction,
                    groupName: "AlertPopUp",
                    classNameBindings: ['isHighlighted:isHighlighted'],
                    isHighlighted: params[i].isHighlighted ? true : false,
                    softButtonID: params[i].softButtonID,
                    icon: params[i].image ? params[i].image.value : "",
                    text: params[i].text,
                    classNames: 'list-item softButton ' + softButtonsClass,
                    elementId: 'softButton' + i,
                    templateName: params[i].image ? 'rightText' : 'text',
                    appID: appID
                }));
            }
        }
    },

    AlertActive: function(message, alertRequestId) {
        var self = this;

        this.set('alertRequestId', alertRequestId);

        this.addSoftButtons(message.softButtons, message.appID);

        this.set('progressIndicator', message.progressIndicator);

        this.set('appName', SDL.SDLController.getApplicationModel(message.appID).appName);

        for (var i = 0; i < message.alertStrings.length; i++) {
            switch (message.alertStrings[i].fieldName) {
                case 'alertText1': {
                    this.set('content1', message.alertStrings[i].fieldText);
                    break;
                }
                case 'alertText2': {
                    this.set('content2', message.alertStrings[i].fieldText);
                    break;
                }
                case 'alertText3': {
                    this.set('content3', message.alertStrings[i].fieldText);
                    break;
                }
            }
        }
        
        this.set('active', true);
        this.set('timeout', message.duration ? message.duration : 30000); //default timeout defined for Alert popUp

        clearTimeout(this.timer);
        this.timer = setTimeout(function() {
            self.deactivate('timeout');
        }, this.timeout);
    }
});