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
        'buttonsArea'
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

    /**
     * Extend deactivate method send SUCCESS response on deactivate with current
     * slider value
     */
    deactivate: function () {

        this._super();
        this.set('target', null);
    },

    inputChanges: function (element) {

//        TO DO
//        var keyboardEvent = document.createEvent("KeyboardEvent");
//        var initMethod = typeof keyboardEvent.initKeyboardEvent !== 'undefined' ? "initKeyboardEvent" : "initKeyEvent";
//
//
//        keyboardEvent[initMethod](
//            "keydown", // event type : keydown, keyup, keypress
//            true, // bubbles
//            true, // cancelable
//            window, // viewArg: should be window
//            false, // ctrlKeyArg
//            false, // altKeyArg
//            false, // shiftKeyArg
//            false, // metaKeyArg
//            element.text.charCodeAt(0), // keyCodeArg : unsigned long the virtual key code, else 0
//            0 // charCodeArgs : unsigned long the Unicode character associated with the depressed key, else 0
//        );

        switch (element.text) {
            case "Space": {
                this.searchBar.input.set('value', this.searchBar.input.value + " ");
                this.target.set('value', this.searchBar.input.value + " ");
                break;
            }
            case "Search": {
                this.target.search();
                this.deactivate();
                break;
            }
            default:{
                this.searchBar.input.set('value', this.searchBar.input.value + element.text);
                this.target.set('value', this.searchBar.input.value + " ");
            }

        }
    },

    clearBtn: function (element) {
        this.searchBar.input.set('value', this.searchBar.input.value.slice(0, -1))
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
            elementId: "tf1",
            classNames: "tf1"
            //tagName: 'input',
            //type: 'text',
            //attribute: ['type:text'],
            //valueBinding: 'SDL.SDLModel.keyboardInputValue',
            //attributeBindings: ['disabled'],
            //disabled: false
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
            //target: 'SDL.SDLController',
            //action: 'numericBtn'
        }),

        symbolBtn: SDL.Button.extend({
            classNames: 'symbolBtn controll',
            text: '!@#'
            //target: 'SDL.SDLController',
            //action: 'symbolBtn'
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
            //target: 'SDL.SDLController',
            //action: 'caseSwitchBtn'
        }),

        localisationBtn: SDL.Button.extend({
            classNames: 'localisationBtn controll',
            icon: 'images/info/info_leftMenu_apps_ico.png'
            //target: 'SDL.SDLController',
            //action: 'localisationBtn'
        }),

        searchBtn: SDL.Button.extend(SDL.PresetEvents, {
            classNames: 'searchBtn controll',
            text: 'Search',
            click: function(){
                this.get('parentView.parentView').inputChanges(this);
            },
            presetName: 'Search'
        })
    }),

    buttonsArea: SDL.SDLModel.keyboardLayout['QWERTY']

});
