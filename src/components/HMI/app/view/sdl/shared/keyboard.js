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
 * @name SDL.Keyboard
 * @desc Slider visual representation
 * @category View
 * @filesource app/view/sdl/shared/keyboard.js
 * @version 1.0
 */

SDL.Keyboard = SDL.SDLAbstractView.create({

    elementId: 'keyboard_view',

    childViews: [
        'backButton',
        'microphone',
        'searchBar',
        'controlls',
        'buttonsAreaQWERTY',
        'buttonsAreaQWERTZ',
        'buttonsAreaAZERTY'
    ],

    /**
     * Activate keyboard method
     *
     * @param {Object}
     */
    activate: function( element ) {
        if( element.tagName === 'input' ){

            this.set( 'active', true );
            this.set('target', element);
        }
    },

    backButton: SDL.Button.extend( {
        classNames:
            [
                'back-button'
            ],
        action: function() {

            if (SDL.SDLAppController.model && SDL.SDLAppController.model.activeRequests.uiPerformInteraction && !SDL.InteractionChoicesView.active) {
                FFW.UI.OnKeyboardInput("", "ENTRY_CANCELLED");
                SDL.InteractionChoicesView.deactivate("ABORTED");
            }
            SDL.Keyboard.deactivate();
        },
        icon: 'images/media/ico_back.png',
        onDown: false
    } ),

    /**
     * Extend deactivate method send SUCCESS response on deactivate with current
     * slider value
     */
    deactivate: function () {

        this._super();
        this.searchBar.input.set('value', null);
        this.set('target', null);
    },

    inputChanges: function (element) {

        if (SDL.SDLAppController.model && SDL.SDLAppController.model.activeRequests.uiPerformInteraction) {
            SDL.SDLController.onResetTimeout(SDL.SDLAppController.model.appID, "UI.PerformInteraction");
        }

        if (this.searchBar.input.value == null) {
            this.searchBar.input.set('value', "");
        }

        switch (element.text) {
            case "Space": {
                this.searchBar.input.set('value', this.searchBar.input.value + " ");
                this.target.set('value', this.searchBar.input.value);
                break;
            }
            case "Search": {
                if (this.target.value == null) {
                    this.target.set('value', "");
                }
                this.target.search();
                this.deactivate();
                break;
            }
            default:{
                this.searchBar.input.set('value', this.searchBar.input.value + element.text);
                this.target.set('value', this.searchBar.input.value);
            }

        }
    },

    clearBtn: function (element) {
        this.searchBar.input.set('value', this.searchBar.input.value.slice(0, -1));
        this.target.set('value', this.searchBar.input.value);
        if (this.searchBar.input.value == "") {
            FFW.UI.OnKeyboardInput("", "ENTRY_CANCELLED");
        }
        SDL.SDLController.onResetTimeout(SDL.SDLAppController.model.appID, "UI.PerformInteraction");
    },

    /**
     * Tearget element that initiated keyboard
     */
    target: null,

    microphone: SDL.Button.extend({
        classNames: 'microphone',
        text: 'mic'
    }),

    searchBar: Em.ContainerView.extend({

        classNames: 'searchBar',

        childViews: [
            'input',
            'clearBtn',
            'serchIcon'
        ],

        clearBtn: SDL.Button.extend({
            classNames: 'clearBtn',
            text: 'X',
            action: 'clearBtn',
            target: 'parentView.parentView'
        }),

        serchIcon: Em.View.extend({
            classNames: 'serchIcon'
        }),

        input: Ember.TextField.extend({
            elementId: "keyboardInput",
            classNames: "keyboardInput",
            valueBinding: 'SDL.SDLModel.keyboardInputValue'
        })

    }),

    controlls: Em.ContainerView.extend({

        classNames: 'controlls',

        childViews: [
            'numericBtn',
            'symbolBtn',
            'spaceBtn',
            'caseSwitchBtn',
            'localisationBtn',
            'searchBtn'
        ],

        numericBtn: SDL.Button.extend({
            classNames: 'numericBtn controll',
            text: '123'
        }),

        symbolBtn: SDL.Button.extend({
            classNames: 'symbolBtn controll',
            text: '!@#'
        }),

        spaceBtn: SDL.Button.extend({
            classNames: 'spaceBtn controll',
            text: 'Space',
            target    : 'parentView.parentView',
            action    : 'inputChanges'
        }),

        caseSwitchBtn: SDL.Button.extend({
            classNames: 'caseSwitchBtn controll',
            text: 'ABC'
        }),

        localisationBtn: SDL.Button.extend({
            classNames: 'localisationBtn controll',
            icon: 'images/info/info_leftMenu_apps_ico.png'
        }),

        searchBtn: SDL.Button.extend(SDL.PresetEvents, {
            classNames: 'searchBtn controll',
            text: 'Search',
            click: function(){
                this.get('parentView.parentView').inputChanges(this);
            },
            presetName: 'SEARCH'
        })
    }),

    disableButtons: function(){

        if (SDL.SDLAppController.model) {
            if (!SDL.SDLAppController.model.globalProperties.keyboardProperties) {
                return;
            }
            var list = SDL.SDLAppController.model.globalProperties.keyboardProperties.limitedCharacterList ? SDL.SDLAppController.model.globalProperties.keyboardProperties.limitedCharacterList : [];

            for (var i = 0; i < list.length; i++) {
                list[i] = list[i].toLowerCase();
            }

            if (SDL.SDLAppController.model && list.length){

                for (var i = 0; i < this.buttonsAreaQWERTY._childViews.length; i++) {

                    if (list.indexOf(this.buttonsAreaQWERTY._childViews[i].text) < 0) {
                        this.buttonsAreaQWERTY._childViews[i].set('disabled', true);
                        this.buttonsAreaQWERTZ._childViews[i].set('disabled', true);
                        this.buttonsAreaAZERTY._childViews[i].set('disabled', true);
                    } else {
                        this.buttonsAreaQWERTY._childViews[i].set('disabled', false);
                        this.buttonsAreaQWERTZ._childViews[i].set('disabled', false);
                        this.buttonsAreaAZERTY._childViews[i].set('disabled', false);
                    }
                }
            } else if (SDL.SDLAppController.model && !list.length) {
                for (var i = 0; i < this.buttonsAreaQWERTY._childViews.length; i++) {
                    this.buttonsAreaQWERTY._childViews[i].set('disabled', false);
                }
            }
        }

    }.observes('SDL.SDLAppController.model.globalProperties.keyboardProperties.limitedCharacterList.@each'),

    buttonsAreaQWERTY: SDL.QWERTYLayout.create({

        classNameBindings: 'this.pQWERTY::hide',

        pQWERTY: function(){
            if (SDL.SDLAppController.model && SDL.SDLAppController.model.globalProperties.keyboardProperties.keyboardLayout == "QWERTY") {
                return true;
            } else {
                return false;
            }
        }.property('SDL.SDLAppController.model.globalProperties.keyboardProperties.keyboardLayout')

    }),

    buttonsAreaQWERTZ: SDL.QWERTZLayout.create({

        classNameBindings: 'this.pQWERTZ::hide',

        pQWERTZ: function(){
            if (SDL.SDLAppController.model && SDL.SDLAppController.model.globalProperties.keyboardProperties.keyboardLayout == "QWERTZ") {
                return true;
            } else {
                return false;
            }
        }.property('SDL.SDLAppController.model.globalProperties.keyboardProperties.keyboardLayout')

    }),

    buttonsAreaAZERTY: SDL.AZERTYLayout.create({

        classNameBindings: 'this.pAZERTY::hide',

        pAZERTY: function(){
            if (SDL.SDLAppController.model && SDL.SDLAppController.model.globalProperties.keyboardProperties.keyboardLayout == "AZERTY") {
                return true;
            } else {
                return false;
            }
        }.property('SDL.SDLAppController.model.globalProperties.keyboardProperties.keyboardLayout')

    })

});
